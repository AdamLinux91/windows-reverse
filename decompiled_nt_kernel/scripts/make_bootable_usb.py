#!/usr/bin/env python3
"""
Zero-Dependency Bootable GPT + FAT32 EFI System Partition USB Image Builder.

Creates `build/nt_hardware_usb.img` (~35 MiB) using ONLY the Python 3 standard
library (no root/sudo, no loop-mount, no mtools/dosfstools required):
  - LBA 0          : Protective MBR (type 0xEE)
  - LBA 1..33      : Primary GPT Header + Partition Array (EFI System Partition)
  - LBA 2048..70047: Genuine FAT32 filesystem (>= 65,525 clusters per fatgen103):
                       \\EFI\\BOOT\\BOOTX64.EFI
                       \\KERNEL\\NTHYBRID.ELF
                       \\README.TXT
  - Last 33 LBAs   : Backup GPT Partition Array + Backup GPT Header (IEEE CRC32)

Flash directly onto a physical USB drive on Linux / macOS:
    sudo dd if=build/nt_hardware_usb.img of=/dev/sdX bs=4M status=progress oflag=sync
"""

from __future__ import annotations

import struct
import sys
import uuid
import zlib
from pathlib import Path

SECTOR_SIZE = 512
ESP_START_LBA = 2048
ESP_SECTORS = 68096  # 1 sec/cluster -> >66,900 data clusters (true FAT32 per fatgen103!)
TOTAL_LBAS = ESP_START_LBA + ESP_SECTORS + 64

# UEFI Specification Section 5.3.3 — EFI System Partition GUID: C12A7328-F81F-11D2-BA4B-00A0C93EC93B
ESP_TYPE_GUID_LE = uuid.UUID("C12A7328-F81F-11D2-BA4B-00A0C93EC93B").bytes_le
DISK_GUID_LE = uuid.UUID("9A221E89-4EFD-4AB6-AFB6-FA864B700CB3").bytes_le
PART_GUID_LE = uuid.UUID("83DB4240-4EFD-4AB6-AFB6-FA864B700CB3").bytes_le


def _crc32(data: bytes) -> int:
    return zlib.crc32(data) & 0xFFFFFFFF


def _make_protective_mbr(total_lbas: int) -> bytes:
    mbr = bytearray(SECTOR_SIZE)
    span = min(total_lbas - 1, 0xFFFFFFFF)
    # Partition entry 1 at +446: Protective GPT (0xEE) starting at LBA 1
    struct.pack_into(
        "<BBBBBBBBII",
        mbr,
        446,
        0x00,
        0x00,
        0x02,
        0x00,
        0xEE,
        0xFF,
        0xFF,
        0xFF,
        1,
        span,
    )
    mbr[510] = 0x55
    mbr[511] = 0xAA
    return bytes(mbr)


def _make_gpt_entries(first_lba: int, last_lba: int) -> bytes:
    entries = bytearray(128 * 128)
    name_utf16 = "NT Hybrid Kernel ESP".encode("utf-16le")[:72]
    entry0 = bytearray(128)
    entry0[0:16] = ESP_TYPE_GUID_LE
    entry0[16:32] = PART_GUID_LE
    struct.pack_into("<QQQ", entry0, 32, first_lba, last_lba, 0)
    entry0[56 : 56 + len(name_utf16)] = name_utf16
    entries[0:128] = entry0
    return bytes(entries)


def _make_gpt_header(
    my_lba: int,
    alt_lba: int,
    first_usable: int,
    last_usable: int,
    part_entries_lba: int,
    entries_crc: int,
) -> bytes:
    hdr = bytearray(92)
    struct.pack_into(
        "<8sIIIIQQQQ16sQIII",
        hdr,
        0,
        b"EFI PART",
        0x00010000,  # Revision 1.0
        92,          # HeaderSize
        0,           # HeaderCRC32 placeholder
        0,           # Reserved
        my_lba,
        alt_lba,
        first_usable,
        last_usable,
        DISK_GUID_LE,
        part_entries_lba,
        128,
        128,
        entries_crc,
    )
    crc = _crc32(bytes(hdr))
    struct.pack_into("<I", hdr, 16, crc)
    sec = bytearray(SECTOR_SIZE)
    sec[:92] = hdr
    return bytes(sec)


def _make_dir_entry(name83: bytes, attr: int, first_cluster: int, size: int) -> bytes:
    assert len(name83) == 11
    hi = (first_cluster >> 16) & 0xFFFF
    lo = first_cluster & 0xFFFF
    # Fixed timestamp 2026-10-23 12:00:00 (date=(46<<9)|(10<<5)|23 = 0x5D57, time=0x6000)
    return struct.pack(
        "<11sBBBHHHHHHHI",
        name83,
        attr,
        0,
        0,
        0x6000,
        0x5D57,
        0x5D57,
        hi,
        0x6000,
        0x5D57,
        lo,
        size,
    )


def _build_fat32_esp(efi_bytes: bytes, elf_bytes: bytes, readme_bytes: bytes) -> bytearray:
    vol = bytearray(ESP_SECTORS * SECTOR_SIZE)
    reserved_secs = 32
    num_fats = 2
    # Each cluster = 1 sector (512B). Need ~68096 FAT entries * 4B = 272384B = 532 sectors/FAT
    fat_sz_32 = 536
    data_start_sec = reserved_secs + num_fats * fat_sz_32
    total_clusters = ESP_SECTORS - data_start_sec
    assert total_clusters >= 65525, f"FAT32 requires >=65525 clusters, got {total_clusters}"

    # 1. FAT32 Boot Sector (BPB) at sector 0
    bpb = bytearray(SECTOR_SIZE)
    bpb[0:3] = b"\xEB\x58\x90"
    bpb[3:11] = b"MSWIN4.1"
    struct.pack_into("<HBHBHHBHHHII", bpb, 11,
        SECTOR_SIZE,    # BPB_BytsPerSec
        1,              # BPB_SecPerClus
        reserved_secs,  # BPB_RsvdSecCnt
        num_fats,       # BPB_NumFATs
        0,              # BPB_RootEntCnt (0 for FAT32)
        0,              # BPB_TotSec16
        0xF8,           # BPB_Media (fixed disk)
        0,              # BPB_FATSz16
        63,             # BPB_SecPerTrk
        255,            # BPB_NumHeads
        ESP_START_LBA,  # BPB_HiddSec
        ESP_SECTORS,    # BPB_TotSec32
    )
    struct.pack_into("<IHHIHH12sBBBI11s8s", bpb, 36,
        fat_sz_32,      # BPB_FATSz32
        0,              # BPB_ExtFlags
        0,              # BPB_FSVer
        2,              # BPB_RootClus = Cluster 2
        1,              # BPB_FSInfo = Sector 1
        6,              # BPB_BkBootSec = Sector 6
        b"\x00" * 12,
        0x80,           # BS_DrvNum
        0,
        0x29,           # BS_BootSig
        0x9A221E89,     # BS_VolID
        b"NT_HYBRID  ", # BS_VolLab (11B)
        b"FAT32   ",    # BS_FilSysType (8B)
    )
    bpb[510] = 0x55
    bpb[511] = 0xAA
    vol[0:SECTOR_SIZE] = bpb
    vol[6 * SECTOR_SIZE : 7 * SECTOR_SIZE] = bpb  # Backup boot sector at sector 6

    # 2. FSInfo at sector 1 (and backup at sector 7)
    fsinfo = bytearray(SECTOR_SIZE)
    struct.pack_into("<I", fsinfo, 0, 0x41615252)
    struct.pack_into("<I", fsinfo, 484, 0x61417272)
    struct.pack_into("<II", fsinfo, 488, 0xFFFFFFFF, 0xFFFFFFFF)
    fsinfo[510] = 0x55
    fsinfo[511] = 0xAA
    vol[SECTOR_SIZE : 2 * SECTOR_SIZE] = fsinfo
    vol[7 * SECTOR_SIZE : 8 * SECTOR_SIZE] = fsinfo

    # Helper to allocate contiguous cluster chains in FAT
    fat = [0] * (total_clusters + 2)
    fat[0] = 0x0FFFFFF8
    fat[1] = 0x0FFFFFFF
    next_clus = 6  # Reserve clusters: 2=Root, 3=EFI, 4=EFI\BOOT, 5=KERNEL
    for dclus in (2, 3, 4, 5):
        fat[dclus] = 0x0FFFFFFF

    def cluster_offset(c: int) -> int:
        return (data_start_sec + (c - 2)) * SECTOR_SIZE

    def write_file_chain(blob: bytes) -> int:
        nonlocal next_clus
        if not blob:
            return 0
        n_clus = (len(blob) + SECTOR_SIZE - 1) // SECTOR_SIZE
        start = next_clus
        for i in range(n_clus):
            c = start + i
            fat[c] = (c + 1) if (i + 1 < n_clus) else 0x0FFFFFFF
            chunk = blob[i * SECTOR_SIZE : (i + 1) * SECTOR_SIZE]
            off = cluster_offset(c)
            vol[off : off + len(chunk)] = chunk
        next_clus += n_clus
        return start

    efi_clus = write_file_chain(efi_bytes)
    elf_clus = write_file_chain(elf_bytes)
    txt_clus = write_file_chain(readme_bytes)

    # Populate directories:
    # Cluster 2: Root directory (\EFI, \KERNEL, \README.TXT, Volume Label)
    root_off = cluster_offset(2)
    vol[root_off + 0 : root_off + 32] = _make_dir_entry(b"NT_HYBRID  ", 0x08, 0, 0)
    vol[root_off + 32 : root_off + 64] = _make_dir_entry(b"EFI        ", 0x10, 3, 0)
    vol[root_off + 64 : root_off + 96] = _make_dir_entry(b"KERNEL     ", 0x10, 5, 0)
    vol[root_off + 96 : root_off + 128] = _make_dir_entry(b"README  TXT", 0x20, txt_clus, len(readme_bytes))

    # Cluster 3: \EFI directory (., .., BOOT)
    efi_dir_off = cluster_offset(3)
    vol[efi_dir_off + 0 : efi_dir_off + 32] = _make_dir_entry(b".          ", 0x10, 3, 0)
    vol[efi_dir_off + 32 : efi_dir_off + 64] = _make_dir_entry(b"..         ", 0x10, 0, 0)
    vol[efi_dir_off + 64 : efi_dir_off + 96] = _make_dir_entry(b"BOOT       ", 0x10, 4, 0)

    # Cluster 4: \EFI\BOOT directory (., .., BOOTX64.EFI)
    boot_dir_off = cluster_offset(4)
    vol[boot_dir_off + 0 : boot_dir_off + 32] = _make_dir_entry(b".          ", 0x10, 4, 0)
    vol[boot_dir_off + 32 : boot_dir_off + 64] = _make_dir_entry(b"..         ", 0x10, 3, 0)
    vol[boot_dir_off + 64 : boot_dir_off + 96] = _make_dir_entry(b"BOOTX64 EFI", 0x20, efi_clus, len(efi_bytes))

    # Cluster 5: \KERNEL directory (., .., NTHYBRID.ELF)
    kern_dir_off = cluster_offset(5)
    vol[kern_dir_off + 0 : kern_dir_off + 32] = _make_dir_entry(b".          ", 0x10, 5, 0)
    vol[kern_dir_off + 32 : kern_dir_off + 64] = _make_dir_entry(b"..         ", 0x10, 0, 0)
    vol[kern_dir_off + 64 : kern_dir_off + 96] = _make_dir_entry(b"NTHYBRIDELF", 0x20, elf_clus, len(elf_bytes))

    # Serialize FAT #1 and FAT #2
    fat_bytes = bytearray(fat_sz_32 * SECTOR_SIZE)
    for idx, val in enumerate(fat):
        if idx * 4 + 4 <= len(fat_bytes):
            struct.pack_into("<I", fat_bytes, idx * 4, val)
    fat1_off = reserved_secs * SECTOR_SIZE
    fat2_off = (reserved_secs + fat_sz_32) * SECTOR_SIZE
    vol[fat1_off : fat1_off + len(fat_bytes)] = fat_bytes
    vol[fat2_off : fat2_off + len(fat_bytes)] = fat_bytes

    return vol


def build_usb_image(efi_path: Path, elf_path: Path, out_img: Path) -> None:
    efi_bytes = efi_path.read_bytes()
    elf_bytes = elf_path.read_bytes()
    readme_bytes = (
        b"Decompiled Windows NT 10.0.16299.15 Hybrid Kernel Boot Media\r\n"
        b"UEFI Payload      : \\EFI\\BOOT\\BOOTX64.EFI\r\n"
        b"Multiboot2 Kernel : \\KERNEL\\NTHYBRID.ELF\r\n"
        b"Serial Telemetry  : COM1 (0x3F8) @ 115200 8N1\r\n"
    )

    disk = bytearray(TOTAL_LBAS * SECTOR_SIZE)
    disk[0:SECTOR_SIZE] = _make_protective_mbr(TOTAL_LBAS)

    esp_end_lba = ESP_START_LBA + ESP_SECTORS - 1
    first_usable_lba = 34
    last_usable_lba = TOTAL_LBAS - 34

    entries = _make_gpt_entries(ESP_START_LBA, esp_end_lba)
    entries_crc = _crc32(entries)

    # Primary GPT (LBA 1 header, LBAs 2..33 entries)
    disk[SECTOR_SIZE : 2 * SECTOR_SIZE] = _make_gpt_header(
        my_lba=1,
        alt_lba=TOTAL_LBAS - 1,
        first_usable=first_usable_lba,
        last_usable=last_usable_lba,
        part_entries_lba=2,
        entries_crc=entries_crc,
    )
    disk[2 * SECTOR_SIZE : 34 * SECTOR_SIZE] = entries

    # FAT32 EFI System Partition at LBA 2048
    esp_vol = _build_fat32_esp(efi_bytes, elf_bytes, readme_bytes)
    disk[ESP_START_LBA * SECTOR_SIZE : (ESP_START_LBA + ESP_SECTORS) * SECTOR_SIZE] = esp_vol

    # Backup GPT (LBAs TOTAL_LBAS-33..TOTAL_LBAS-2 entries, LBA TOTAL_LBAS-1 header)
    bk_entries_lba = TOTAL_LBAS - 33
    disk[bk_entries_lba * SECTOR_SIZE : (TOTAL_LBAS - 1) * SECTOR_SIZE] = entries
    disk[(TOTAL_LBAS - 1) * SECTOR_SIZE : TOTAL_LBAS * SECTOR_SIZE] = _make_gpt_header(
        my_lba=TOTAL_LBAS - 1,
        alt_lba=1,
        first_usable=first_usable_lba,
        last_usable=last_usable_lba,
        part_entries_lba=bk_entries_lba,
        entries_crc=entries_crc,
    )

    out_img.parent.mkdir(parents=True, exist_ok=True)
    out_img.write_bytes(disk)
    print(
        f"[USB] Built GPT+FAT32 bootable hardware image: {out_img} "
        f"({len(disk) / (1024 * 1024):.1f} MiB, BOOTX64.EFI={len(efi_bytes):,}B, NTHYBRID.ELF={len(elf_bytes):,}B)"
    )


if __name__ == "__main__":
    if len(sys.argv) != 4:
        print(f"Usage: {sys.argv[0]} <BOOTX64.EFI> <nt_hybrid_kernel.elf> <output.img>")
        raise SystemExit(1)
    build_usb_image(Path(sys.argv[1]), Path(sys.argv[2]), Path(sys.argv[3]))
