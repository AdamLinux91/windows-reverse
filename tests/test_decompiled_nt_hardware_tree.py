"""End-to-end verification of the decompiled real Windows NT kernel hardware tree."""

from __future__ import annotations

import json
import subprocess
import struct
import sys
import uuid
import zlib
from pathlib import Path

import pefile

REPO_ROOT = Path(__file__).resolve().parent.parent
HW_DIR = REPO_ROOT / "decompiled_nt_kernel"


def test_real_nt_z3_proof_manifest_complete() -> None:
    manifest_path = HW_DIR / "proofs" / "z3_equivalence_proofs.json"
    assert manifest_path.is_file()
    manifest = json.loads(manifest_path.read_text(encoding="utf-8"))

    assert manifest["total_functions"] == 69
    assert manifest["verified_functions"] == 69
    assert "10.0.16299.15" in manifest["binary"]
    assert "83DB4240-4EFD-4AB6-AFB6-FA864B700CB3" in manifest["pdb"]

    subsystems = manifest["subsystems"]
    for key in ("ke", "ps", "mm", "ob", "io", "ex", "rtl", "se"):
        assert subsystems[key] > 0, f"Subsystem {key} has 0 functions"
    assert sum(subsystems.values()) == 69

    by_name = {fn["name"]: fn for fn in manifest["functions"]}
    # Raw-assembly preserved routines
    for asm_fn in (
        "KeGetCurrentIrql",
        "KzRaiseIrql",
        "KiFlushCurrentTbWorker",
        "KiSystemCall64",
        "ExpInterlockedPushEntrySList",
        "ExpInterlockedPopEntrySList",
        "RtlCompareMemory",
    ):
        assert by_name[asm_fn]["is_raw_asm"] is True

    # De-optimized unrolled loops & magic-multiplier divisions
    assert by_name["KeCheckForZeroPage"]["rerolled_loops"]
    assert by_name["KeSetCoalescableTimer"]["recovered_divisions"]
    assert by_name["PiUEventHashStringIntoBucket"]["recovered_divisions"]

    # Every single function has VERIFIED_EQUIVALENT or PRESERVED_RAW_ASM proof status
    for fn in manifest["functions"]:
        assert fn["proof"]["status"] in ("VERIFIED_EQUIVALENT", "PRESERVED_RAW_ASM")


def test_hardware_targets_build_and_execute_on_silicon() -> None:
    # 1. Clean build of UEFI PE32+, Multiboot2 ELF64, and GPT+FAT32 USB image
    res_build = subprocess.run(
        ["make", "clean"],
        cwd=HW_DIR,
        capture_output=True,
        text=True,
        check=True,
    )
    assert res_build.returncode == 0

    res_all = subprocess.run(
        ["make", f"PYTHON={sys.executable}", "all"],
        cwd=HW_DIR,
        capture_output=True,
        text=True,
        check=True,
    )
    assert res_all.returncode == 0

    # 2. Validate BOOTX64.EFI PE32+ UEFI Application + .reloc directory
    efi_path = HW_DIR / "build" / "BOOTX64.EFI"
    assert efi_path.is_file()
    pe = pefile.PE(str(efi_path))
    assert pe.FILE_HEADER.Machine == 0x8664
    assert pe.OPTIONAL_HEADER.Magic == 0x20B
    assert pe.OPTIONAL_HEADER.Subsystem == 10  # IMAGE_SUBSYSTEM_EFI_APPLICATION
    assert hasattr(pe, "DIRECTORY_ENTRY_BASERELOC")
    assert len(pe.DIRECTORY_ENTRY_BASERELOC) >= 1

    # 3. Validate Multiboot2 + Multiboot1 bare-metal Ring-0 ELF64 image
    elf_path = HW_DIR / "build" / "nt_hybrid_kernel.elf"
    elf_bytes = elf_path.read_bytes()
    assert elf_bytes[:4] == b"\x7fELF"
    mb2_off = elf_bytes.find(struct.pack("<I", 0xE85250D6))
    mb1_off = elf_bytes.find(struct.pack("<I", 0x1BADB002))
    assert 0 <= mb2_off < 32768
    assert 0 <= mb1_off < 8192
    magic, arch, length, csum = struct.unpack_from("<IIII", elf_bytes, mb2_off)
    assert ((magic + arch + length + csum) & 0xFFFFFFFF) == 0

    # 4. Validate GPT + genuine FAT32 EFI System Partition USB image
    usb_path = HW_DIR / "build" / "nt_hardware_usb.img"
    img = usb_path.read_bytes()
    assert img[510:512] == b"\x55\xaa"
    assert img[446 + 4] == 0xEE  # Protective MBR

    gpt_hdr = bytearray(img[512 : 512 + 92])
    assert gpt_hdr[:8] == b"EFI PART"
    stored_crc = struct.unpack_from("<I", gpt_hdr, 16)[0]
    struct.pack_into("<I", gpt_hdr, 16, 0)
    assert (zlib.crc32(gpt_hdr) & 0xFFFFFFFF) == stored_crc

    esp_guid = uuid.UUID(bytes_le=img[1024:1040])
    assert str(esp_guid).upper() == "C12A7328-F81F-11D2-BA4B-00A0C93EC93B"

    esp_off = 2048 * 512
    assert img[esp_off + 82 : esp_off + 90] == b"FAT32   "
    assert img[esp_off + 510 : esp_off + 512] == b"\x55\xaa"

    # 5. Execute all 8 decompiled NT subsystems natively on live x86-64 CPU silicon
    res_silicon = subprocess.run(
        ["make", "test-silicon"],
        cwd=HW_DIR,
        capture_output=True,
        text=True,
        check=True,
    )
    assert "12 PASSED, 0 FAILED" in res_silicon.stdout

    # Clean up generated build/ artifacts so working tree stays clean
    subprocess.run(["make", "clean"], cwd=HW_DIR, check=True)
