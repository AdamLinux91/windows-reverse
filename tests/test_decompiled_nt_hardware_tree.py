"""End-to-end verification of the decompiled real Windows NT kernel replica & hardware tree."""

from __future__ import annotations

import json
import struct
import subprocess
import sys
import uuid
import zlib
from pathlib import Path

import pefile

REPO_ROOT = Path(__file__).resolve().parent.parent
HW_DIR = REPO_ROOT / "decompiled_nt_kernel"

sys.path.insert(0, str(HW_DIR / "scripts"))
from pe_elf_linker import compute_pe_checksum  # noqa: E402


ALL_16_SUBSYSTEMS = (
    "ke",
    "ps",
    "mm",
    "ob",
    "io",
    "ex",
    "rtl",
    "se",
    "cm",
    "cc",
    "fsrtl",
    "po",
    "alpc",
    "tm",
    "etw_vf",
    "zw_nt",
)


def _assert_valid_native_ntoskrnl_pe(pe_path: Path) -> pefile.PE:
    assert pe_path.is_file(), f"Missing PE image: {pe_path}"
    raw = pe_path.read_bytes()
    pe = pefile.PE(str(pe_path))

    assert pe.FILE_HEADER.Machine == 0x8664
    assert pe.OPTIONAL_HEADER.Magic == 0x20B
    assert pe.OPTIONAL_HEADER.Subsystem == 1  # IMAGE_SUBSYSTEM_NATIVE (winload.efi)
    assert (pe.OPTIONAL_HEADER.DllCharacteristics & 0x0080) == 0  # FORCE_INTEGRITY cleared
    assert pe.OPTIONAL_HEADER.DATA_DIRECTORY[4].VirtualAddress == 0  # SECURITY cleared
    assert pe.OPTIONAL_HEADER.CheckSum == compute_pe_checksum(raw)

    # Full retail 10.0.16299.15 export & import surface
    assert len(pe.DIRECTORY_ENTRY_EXPORT.symbols) == 2848
    assert len(pe.DIRECTORY_ENTRY_IMPORT) == 18
    assert hasattr(pe, "DIRECTORY_ENTRY_BASERELOC")
    assert len(pe.DIRECTORY_ENTRY_BASERELOC) >= 1

    # Verify .pdata RUNTIME_FUNCTION table is strictly sorted & non-overlapping
    pdd = pe.OPTIONAL_HEADER.DATA_DIRECTORY[3]
    assert pdd.VirtualAddress > 0 and pdd.Size >= 12
    poff = pe.get_offset_from_rva(pdd.VirtualAddress)
    prev_end = 0
    for i in range(0, pdd.Size, 12):
        b_rva, e_rva, _u_rva = struct.unpack_from("<III", raw, poff + i)
        assert b_rva < e_rva
        assert b_rva >= prev_end
        prev_end = e_rva

    # Verify LOAD_CONFIG + GFIDS Control Flow Guard 5-byte <IB sorted table
    lc_dd = pe.OPTIONAL_HEADER.DATA_DIRECTORY[10]
    assert lc_dd.VirtualAddress > 0 and lc_dd.Size >= 0x94
    lc_off = pe.get_offset_from_rva(lc_dd.VirtualAddress)
    gf_va, gf_cnt = struct.unpack_from("<QQ", raw, lc_off + 0x80)
    gf_flags = struct.unpack_from("<I", raw, lc_off + 0x90)[0]
    assert ((gf_flags >> 28) & 0xF) == 1  # 5-byte <IB entries
    gf_off = pe.get_offset_from_rva(gf_va - pe.OPTIONAL_HEADER.ImageBase)
    gf_rvas = [struct.unpack_from("<IB", raw, gf_off + i * 5)[0] for i in range(gf_cnt)]
    assert len(gf_rvas) >= 500
    assert gf_rvas == sorted(set(gf_rvas))
    return pe


def test_real_nt_z3_proof_manifest_complete() -> None:
    manifest_path = HW_DIR / "proofs" / "z3_equivalence_proofs.json"
    assert manifest_path.is_file()
    manifest = json.loads(manifest_path.read_text(encoding="utf-8"))

    assert manifest["total_functions"] >= 500
    assert manifest["verified_functions"] == manifest["total_functions"]
    assert "10.0.16299.15" in manifest["binary"]
    assert "83DB4240-4EFD-4AB6-AFB6-FA864B700CB3" in manifest["pdb"]

    subsystems = manifest["subsystems"]
    for key in ALL_16_SUBSYSTEMS:
        assert subsystems[key] > 0, f"Subsystem {key} has 0 functions"
    assert sum(subsystems.values()) == manifest["total_functions"]

    by_name = {fn["name"]: fn for fn in manifest["functions"]}
    # Raw-assembly preserved routines across kernel entry, IRQL, SList, IAT thunks, and Zw* SSDT stubs
    for asm_fn in (
        "KiSystemStartup",
        "KeGetCurrentIrql",
        "KzRaiseIrql",
        "KiFlushCurrentTbWorker",
        "KiSystemCall64",
        "ExpInterlockedPushEntrySList",
        "ExpInterlockedPopEntrySList",
        "RtlCompareMemory",
        "TmCurrentTransaction",
        "ZwClose",
    ):
        assert by_name[asm_fn]["is_raw_asm"] is True

    # De-optimized unrolled loops & magic-multiplier divisions
    assert by_name["KeCheckForZeroPage"]["rerolled_loops"]
    assert by_name["KeSetCoalescableTimer"]["recovered_divisions"]
    assert by_name["PiUEventHashStringIntoBucket"]["recovered_divisions"]

    # Every single function has VERIFIED_EQUIVALENT or PRESERVED_RAW_ASM proof status
    for fn in manifest["functions"]:
        assert fn["proof"]["status"] in ("VERIFIED_EQUIVALENT", "PRESERVED_RAW_ASM")

    # Verify standalone specs & Windows BCD installer scripts exist
    assert (HW_DIR / "specs" / "ntoskrnl_16299_exports.json").is_file()
    assert (HW_DIR / "specs" / "ntoskrnl_16299_imports.json").is_file()
    assert (HW_DIR / "specs" / "ntoskrnl_16299_rva_map.json").is_file()
    assert (HW_DIR / "scripts" / "Install-DecompiledKernel.ps1").is_file()
    assert (HW_DIR / "scripts" / "install_to_windows.bat").is_file()


def test_hardware_and_windows_dropin_targets_build_and_execute_on_silicon() -> None:
    subprocess.run(["make", "clean"], cwd=HW_DIR, check=True)

    # 1. Build standalone Windows ntoskrnl.exe, UEFI BOOTX64.EFI, Multiboot2 ELF64, and USB image
    subprocess.run(
        ["make", f"PYTHON={sys.executable}", "all"],
        cwd=HW_DIR,
        capture_output=True,
        text=True,
        check=True,
    )

    # 2. Validate standalone from-source Windows PE32+ `ntoskrnl.exe` (`IMAGE_SUBSYSTEM_NATIVE`)
    win_pe_path = HW_DIR / "build" / "ntoskrnl.exe"
    pe_standalone = _assert_valid_native_ntoskrnl_pe(win_pe_path)
    exp_names = {
        e.name.decode() for e in pe_standalone.DIRECTORY_ENTRY_EXPORT.symbols if e.name
    }
    for req_sym in (
        "PsGetProcessId",
        "KeInitializeDpc",
        "ZwClose",
        "NtBuildNumber",
        "PsInitialSystemProcess",
        "HalDispatchTable",
    ):
        assert req_sym in exp_names

    # 3. Validate binary-grafted drop-in `ntoskrnl_dropin.exe` builder (`windows-dropin`)
    #    Use retail 10.0.16299.15 host if cached in /tmp/real_nt, else test cross-build mode on win_pe_path
    retail_host = Path("/tmp/real_nt/ntoskrnl_16299_15.exe")
    host_arg = retail_host if retail_host.is_file() else win_pe_path
    subprocess.run(
        [
            "make",
            f"PYTHON={sys.executable}",
            f"NTOSKRNL={host_arg}",
            "windows-dropin",
        ],
        cwd=HW_DIR,
        capture_output=True,
        text=True,
        check=True,
    )
    dropin_pe_path = HW_DIR / "build" / "ntoskrnl_dropin.exe"
    pe_dropin = _assert_valid_native_ntoskrnl_pe(dropin_pe_path)
    sec_names = {s.Name.rstrip(b"\x00").decode() for s in pe_dropin.sections}
    assert ".ntrev" in sec_names
    assert ".ntrevd" in sec_names
    assert ".reloc2" in sec_names

    # 4. Validate BOOTX64.EFI PE32+ UEFI Application + .reloc directory
    efi_path = HW_DIR / "build" / "BOOTX64.EFI"
    assert efi_path.is_file()
    pe = pefile.PE(str(efi_path))
    assert pe.FILE_HEADER.Machine == 0x8664
    assert pe.OPTIONAL_HEADER.Magic == 0x20B
    assert pe.OPTIONAL_HEADER.Subsystem == 10  # IMAGE_SUBSYSTEM_EFI_APPLICATION
    assert hasattr(pe, "DIRECTORY_ENTRY_BASERELOC")
    assert len(pe.DIRECTORY_ENTRY_BASERELOC) >= 1

    # 5. Validate Multiboot2 + Multiboot1 bare-metal Ring-0 ELF64 image
    elf_path = HW_DIR / "build" / "nt_hybrid_kernel.elf"
    elf_bytes = elf_path.read_bytes()
    assert elf_bytes[:4] == b"\x7fELF"
    mb2_off = elf_bytes.find(struct.pack("<I", 0xE85250D6))
    mb1_off = elf_bytes.find(struct.pack("<I", 0x1BADB002))
    assert 0 <= mb2_off < 32768
    assert 0 <= mb1_off < 8192
    magic, arch, length, csum = struct.unpack_from("<IIII", elf_bytes, mb2_off)
    assert ((magic + arch + length + csum) & 0xFFFFFFFF) == 0

    # 6. Validate GPT + genuine FAT32 EFI System Partition USB image
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

    # 7. Execute all 16 decompiled NT subsystems natively on live x86-64 CPU silicon
    res_silicon = subprocess.run(
        ["make", "test-silicon"],
        cwd=HW_DIR,
        capture_output=True,
        text=True,
        check=True,
    )
    assert "19 PASSED, 0 FAILED" in res_silicon.stdout

    # Clean up generated build/ artifacts so working tree stays clean
    subprocess.run(["make", "clean"], cwd=HW_DIR, check=True)
