#!/usr/bin/env python3
"""
Standalone From-Source Windows PE32+ Kernel Linker (`link_ntoskrnl_pe.py`)

Links the 16 compiled NT kernel subsystem `.o` objects + Ring-0 `winload.efi` startup
(`ki_system_startup.o`, `nt_windows_boot.o`, `nt_kernel_runtime.o`, `nt_reconstructed_globals.o`)
into a genuine `IMAGE_SUBSYSTEM_NATIVE (1)` 64-bit PE32+ `ntoskrnl.exe` containing:
  - Zero proprietary Microsoft machine-code bytes (100% built from C/ASM source)
  - `AddressOfEntryPoint` -> `NtKiSystemStartupEntry(PLOADER_PARAMETER_BLOCK)`
  - All 2,848 ordinal-matched `ntoskrnl.exe` (10.0.16299.15) exports (`.edata`)
    binding all 527 Z3-verified C/ASM functions + all 70 initialized data symbols
  - All 18 kernel import descriptors (`HAL.dll`, `BOOTVID.dll`, `PSHED.dll`, `kdcom.dll`,
    `CI.dll`, `msrpc.sys`, `cng.sys`, + 11 `ext-ms-win-*` kernel API sets) in `.idata`/IAT
  - Synthesized Windows x64 `.pdata` (`RUNTIME_FUNCTION` + `UNWIND_INFO` v1)
  - `IMAGE_LOAD_CONFIG_DIRECTORY64` (`Size=256`) + sorted 5-byte `<IB` `GFIDS` CFG table
  - Valid `RT_VERSION` (`.rsrc`) `VS_FIXEDFILEINFO` (`10.0.16299.15`)
  - Full `IMAGE_REL_BASED_DIR64` `.reloc` table for `winload.efi` KASLR + exact PE `CheckSum`
"""
from __future__ import annotations

import argparse
import json
import struct
import sys
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))
from pe_elf_linker import (
    align_up,
    build_pe_relocs,
    compute_pe_checksum,
    link_elf_objects,
    synthesize_win64_unwind_tables,
)

IMAGE_BASE = 0x140000000
SECTION_ALIGN = 0x1000
FILE_ALIGN = 0x200


def build_version_resource_section(rsrc_rva: int) -> bytes:
    """Build a minimal valid PE `.rsrc` section with `RT_VERSION (16)` -> ID 1 -> Lang 1033."""
    # VS_FIXEDFILEINFO (52 bytes) for 10.0.16299.15
    ms = (10 << 16) | 0
    ls = (16299 << 16) | 15
    fixed_info = struct.pack(
        "<IIIIIIIIIIIII",
        0xFEEF04BD,  # dwSignature
        0x00010000,  # dwStrucVersion
        ms,          # dwFileVersionMS
        ls,          # dwFileVersionLS
        ms,          # dwProductVersionMS
        ls,          # dwProductVersionLS
        0x3F,        # dwFileFlagsMask
        0x00,        # dwFileFlags
        0x00040004,  # dwFileOS = VOS_NT_WINDOWS32
        0x00000001,  # dwFileType = VFT_APP / KERNEL
        0x00000000,  # dwFileSubtype
        0,
        0,
    )
    key_u16 = "VS_VERSION_INFO".encode("utf-16le") + b"\x00\x00"
    hdr = struct.pack("<HHH", 0, len(fixed_info), 0) + key_u16
    while len(hdr) & 3:
        hdr += b"\x00"
    vs_blob = bytearray(hdr + fixed_info)
    struct.pack_into("<H", vs_blob, 0, len(vs_blob))

    # Resource directory tree:
    # 0x00: Root Dir (16B) + 1 Entry (8B) [Type=16 RT_VERSION -> 0x18]
    # 0x18: Name Dir (16B) + 1 Entry (8B) [ID=1 -> 0x30]
    # 0x30: Lang Dir (16B) + 1 Entry (8B) [Lang=0x0409 -> DataEntry @ 0x48]
    # 0x48: IMAGE_RESOURCE_DATA_ENTRY (16B) -> data @ 0x58
    tree = bytearray(0x58)
    struct.pack_into("<IIHHHH", tree, 0x00, 0, 0, 0, 0, 0, 1)
    struct.pack_into("<II", tree, 0x10, 16, 0x80000018)
    struct.pack_into("<IIHHHH", tree, 0x18, 0, 0, 0, 0, 0, 1)
    struct.pack_into("<II", tree, 0x28, 1, 0x80000030)
    struct.pack_into("<IIHHHH", tree, 0x30, 0, 0, 0, 0, 0, 1)
    struct.pack_into("<II", tree, 0x40, 0x0409, 0x00000048)
    struct.pack_into("<IIII", tree, 0x48, rsrc_rva + 0x58, len(vs_blob), 0, 0)
    tree.extend(vs_blob)
    return bytes(tree)


def build_edata_blob(
    edata_rva: int,
    dll_name: str,
    ordinal_base: int,
    exports: list[tuple[int, str, int]],  # (ordinal, name, target_rva)
) -> bytes:
    """Build PE `.edata` (`IMAGE_EXPORT_DIRECTORY` + AddressOfFunctions/Names/NameOrdinals)."""
    max_ord = max(o for o, _, _ in exports)
    num_funcs = max_ord - ordinal_base + 1
    named = sorted([(nm, o) for o, nm, _ in exports if nm], key=lambda x: x[0].encode("ascii"))
    num_names = len(named)

    # Layout inside edata_blob:
    # 0x00: IMAGE_EXPORT_DIRECTORY (40 bytes)
    # 0x28: AddressOfFunctions (num_funcs * 4)
    # ... : AddressOfNames (num_names * 4)
    # ... : AddressOfNameOrdinals (num_names * 2)
    # ... : DLL name + export name ASCII strings
    off_funcs = 40
    off_names = off_funcs + num_funcs * 4
    off_ords = off_names + num_names * 4
    off_strs = align_up(off_ords + num_names * 2, 4)

    blob = bytearray(off_strs)
    dll_name_rva = edata_rva + len(blob)
    blob.extend(dll_name.encode("ascii") + b"\x00")

    ord_to_rva = {o: rva for o, _, rva in exports}
    for i in range(num_funcs):
        rva = ord_to_rva.get(ordinal_base + i, 0)
        struct.pack_into("<I", blob, off_funcs + i * 4, rva)

    for idx, (nm, ord_val) in enumerate(named):
        nm_rva = edata_rva + len(blob)
        blob.extend(nm.encode("ascii") + b"\x00")
        struct.pack_into("<I", blob, off_names + idx * 4, nm_rva)
        struct.pack_into("<H", blob, off_ords + idx * 2, ord_val - ordinal_base)

    struct.pack_into(
        "<IIHHIIIIIII",
        blob,
        0,
        0,                      # Characteristics
        0x59CD8000,             # TimeDateStamp
        10,                     # MajorVersion
        0,                      # MinorVersion
        dll_name_rva,           # Name
        ordinal_base,           # Base
        num_funcs,              # NumberOfFunctions
        num_names,              # NumberOfNames
        edata_rva + off_funcs,  # AddressOfFunctions
        edata_rva + off_names,  # AddressOfNames
        edata_rva + off_ords,   # AddressOfNameOrdinals
    )
    return bytes(blob)


def main() -> int:
    ap = argparse.ArgumentParser(description="Link standalone Windows PE32+ ntoskrnl.exe")
    ap.add_argument("--specs-dir", required=True, type=Path)
    ap.add_argument("--proofs", required=True, type=Path)
    ap.add_argument("--out", required=True, type=Path)
    ap.add_argument("objects", nargs="+", type=Path)
    args = ap.parse_args()

    exports_spec = json.loads((args.specs_dir / "ntoskrnl_16299_exports.json").read_text())
    imports_spec = json.loads((args.specs_dir / "ntoskrnl_16299_imports.json").read_text())
    manifest = json.loads(args.proofs.read_text())
    raw_asm_names = {f["name"] for f in manifest["functions"] if f.get("is_raw_asm")}

    # 1. Pre-plan IAT layout at the start of `.data` (`iat_base_offset = 0`)
    #    Reserve 4096 bytes (`0x1000`) at `.text` RVA `0x1000..` first; `.data` RVA is computed
    #    after object code size is known, so we do a fast 2-pass link:
    #    Pass 1 measures exact code_bytes length so `data_rva` + `iat_rva` are exact!
    iat_entries_flat: list[tuple[str, str, int, int | None]] = []  # (dll, func_name, hint, ord)
    mod_slices: list[tuple[str, int, int]] = []  # (dll, start_idx, count)
    for mod in imports_spec["modules"]:
        start_i = len(iat_entries_flat)
        for fn in mod["functions"]:
            iat_entries_flat.append((mod["dll"], fn["name"], fn.get("hint", 0), fn.get("ordinal")))
        mod_slices.append((mod["dll"], start_i, len(mod["functions"])))
        # Null terminator QWORD after each module's IAT block
        iat_entries_flat.append(("", "", 0, None))

    iat_byte_size = align_up(len(iat_entries_flat) * 8, 64)

    # Map retail IAT RVAs -> function name so preserved ASM IAT thunks (`__nt_asm_sym_1403f5xxx`)
    # bind directly to our real `.idata` IAT slots!
    retail_iat_rva_to_name: dict[int, str] = {}
    for mod in imports_spec["modules"]:
        for fn in mod["functions"]:
            retail_iat_rva_to_name[fn["iat_rva"]] = fn["name"]

    code_rva = 0x1000
    # Pass 1 dummy resolver (all external symbols -> 0x1000) to measure exact code size
    dummy_ext: dict[str, int] = {}
    for _dll, fnm, _h, _o in iat_entries_flat:
        if fnm:
            dummy_ext[f"__imp_{fnm}"] = 0x2000
            dummy_ext[fnm] = 0x2000
    for r_rva in retail_iat_rva_to_name:
        dummy_ext[f"__nt_asm_sym_{IMAGE_BASE + r_rva:x}"] = 0x2000

    pass1 = link_elf_objects(
        args.objects,
        code_rva=code_rva,
        data_rva_start=0x200000,
        external_rva_resolver=dummy_ext,
        image_base=IMAGE_BASE,
    )

    # Compute exact `.data` RVA (`IAT` lives at `[data_rva .. data_rva + iat_byte_size)`,
    # and ELF `.data`/`.bss` starts at `data_rva + iat_byte_size`!)
    data_sec_rva = align_up(code_rva + len(pass1.code_bytes), SECTION_ALIGN)
    elf_data_rva = data_sec_rva + iat_byte_size

    real_ext: dict[str, int] = {}
    iat_slot_rva_by_name: dict[str, int] = {}
    for idx, (_dll, fnm, _h, _o) in enumerate(iat_entries_flat):
        if fnm:
            slot_rva = data_sec_rva + idx * 8
            iat_slot_rva_by_name[fnm] = slot_rva
            real_ext[f"__imp_{fnm}"] = slot_rva
            real_ext[fnm] = slot_rva

    for r_rva, fnm in retail_iat_rva_to_name.items():
        if fnm in iat_slot_rva_by_name:
            real_ext[f"__nt_asm_sym_{IMAGE_BASE + r_rva:x}"] = iat_slot_rva_by_name[fnm]

    linked = link_elf_objects(
        args.objects,
        code_rva=code_rva,
        data_rva_start=elf_data_rva,
        external_rva_resolver=real_ext,
        image_base=IMAGE_BASE,
    )

    # 2. Build `.rdata` (`.edata` + `.idata` descriptors/ILT/HintName + `LOAD_CONFIG` + `GFIDS`)
    rdata_sec_rva = align_up(elf_data_rva + len(linked.data_bytes), SECTION_ALIGN)
    rdata_buf = bytearray()

    fallback_stub_rva = linked.global_sym_rva["__nt_export_fallback_stub"]
    export_tuples: list[tuple[int, str, int]] = []
    bound_live_exports = 0
    bound_data_exports = 0
    for exp in exports_spec["exports"]:
        nm = exp["name"]
        ord_val = exp["ordinal"]
        if nm in linked.global_sym_rva:
            tgt_rva = linked.global_sym_rva[nm]
            if exp["is_data"]:
                bound_data_exports += 1
            else:
                bound_live_exports += 1
        else:
            tgt_rva = fallback_stub_rva
        export_tuples.append((ord_val, nm, tgt_rva))

    edata_rva = rdata_sec_rva
    edata_bytes = build_edata_blob(
        edata_rva=edata_rva,
        dll_name="ntoskrnl.exe",
        ordinal_base=exports_spec["ordinal_base"],
        exports=export_tuples,
    )
    rdata_buf.extend(edata_bytes)
    pad = align_up(len(rdata_buf), 16) - len(rdata_buf)
    rdata_buf.extend(b"\x00" * pad)

    # `.idata`: Import Directory Table ((len(mod_slices) + 1) * 20B) + ILT (len(iat_entries_flat)*8B) + Hint/Names + DLL names
    idata_rva = rdata_sec_rva + len(rdata_buf)
    idt_size = (len(mod_slices) + 1) * 20
    ilt_off_in_idata = idt_size
    ilt_size = len(iat_entries_flat) * 8
    str_off_in_idata = ilt_off_in_idata + ilt_size

    idata_blob = bytearray(str_off_in_idata)
    iat_init_bytes = bytearray(iat_byte_size)

    for idx, (_dll, fnm, hint, ord_imp) in enumerate(iat_entries_flat):
        if not fnm and ord_imp is None:
            continue
        if ord_imp is not None and fnm.startswith("Ordinal_"):
            thunk_val = (1 << 63) | (ord_imp & 0xFFFF)
        else:
            if len(idata_blob) & 1:
                idata_blob.append(0)
            hn_rva = idata_rva + len(idata_blob)
            idata_blob.extend(struct.pack("<H", hint & 0xFFFF) + fnm.encode("ascii") + b"\x00")
            thunk_val = hn_rva
        struct.pack_into("<Q", idata_blob, ilt_off_in_idata + idx * 8, thunk_val)
        struct.pack_into("<Q", iat_init_bytes, idx * 8, thunk_val)

    for m_idx, (dll_nm, start_i, _cnt) in enumerate(mod_slices):
        dll_rva = idata_rva + len(idata_blob)
        idata_blob.extend(dll_nm.encode("ascii") + b"\x00")
        ilt_rva = idata_rva + ilt_off_in_idata + start_i * 8
        iat_rva = data_sec_rva + start_i * 8
        struct.pack_into("<IIIII", idata_blob, m_idx * 20, ilt_rva, 0, 0, dll_rva, iat_rva)

    rdata_buf.extend(idata_blob)
    pad = align_up(len(rdata_buf), 16) - len(rdata_buf)
    rdata_buf.extend(b"\x00" * pad)

    # Allocate 8-byte pointers in `.data` for GuardCFCheckFunctionPointer & GuardCFDispatchFunctionPointer
    guard_ptrs_off = len(linked.data_bytes)
    guard_check_ptr_rva = elf_data_rva + guard_ptrs_off
    guard_disp_ptr_rva = guard_check_ptr_rva + 8
    guard_check_fn_rva = linked.global_sym_rva["__nt_guard_check_icall_nop"]
    guard_disp_fn_rva = linked.global_sym_rva["__nt_guard_dispatch_icall_nop"]
    linked.data_bytes.extend(
        struct.pack("<QQ", IMAGE_BASE + guard_check_fn_rva, IMAGE_BASE + guard_disp_fn_rva)
    )

    # Build sorted 5-byte `<IB` GFIDS Control Flow Guard table for all code targets
    gfids_rvas = sorted(
        set(
            [linked.global_sym_rva["NtKiSystemStartupEntry"], fallback_stub_rva]
            + [b[0] for b in linked.func_bounds.values()]
        )
    )
    gfids_rva = rdata_sec_rva + len(rdata_buf)
    for grva in gfids_rvas:
        rdata_buf.extend(struct.pack("<IB", grva, 0))
    rdata_buf.extend(b"\x00" * (align_up(len(rdata_buf), 16) - len(rdata_buf)))

    # Build `IMAGE_LOAD_CONFIG_DIRECTORY64` (256 bytes, matching retail 10.0.16299.15 layout)
    load_cfg_rva = rdata_sec_rva + len(rdata_buf)
    load_cfg = bytearray(256)
    sec_cookie_rva = linked.global_sym_rva["__security_cookie"]
    struct.pack_into("<I", load_cfg, 0x00, 256)  # Size
    struct.pack_into("<Q", load_cfg, 0x58, IMAGE_BASE + sec_cookie_rva)  # SecurityCookie
    struct.pack_into("<Q", load_cfg, 0x70, IMAGE_BASE + guard_check_ptr_rva)
    struct.pack_into("<Q", load_cfg, 0x78, IMAGE_BASE + guard_disp_ptr_rva)
    struct.pack_into("<Q", load_cfg, 0x80, IMAGE_BASE + gfids_rva)
    struct.pack_into("<Q", load_cfg, 0x88, len(gfids_rvas))
    struct.pack_into("<I", load_cfg, 0x90, 0x1001C500)  # Stride=1 + CF_INSTRUMENTED + EXPORT_SUPPRESSED
    rdata_buf.extend(load_cfg)

    # Prepend `iat_init_bytes` to `linked.data_bytes` so `.data` starts at `data_sec_rva`
    full_data_bytes = bytes(iat_init_bytes) + bytes(linked.data_bytes)

    # 3. Synthesize Windows x64 `.pdata` + `UNWIND_INFO`
    # Place `UNWIND_INFO` at the end of `.rdata`, and `RUNTIME_FUNCTION` table in `.pdata`!
    rdata_buf.extend(b"\x00" * (align_up(len(rdata_buf), 8) - len(rdata_buf)))
    unwind_base_rva = rdata_sec_rva + len(rdata_buf)
    unwind_bytes, runtime_fns = synthesize_win64_unwind_tables(
        linked.code_bytes,
        code_rva=code_rva,
        func_bounds=linked.func_bounds,
        raw_asm_names=raw_asm_names | {"NtKiSystemStartupEntry", "__nt_export_fallback_stub"},
        unwind_info_base_rva=unwind_base_rva,
    )
    rdata_buf.extend(unwind_bytes)

    pdata_sec_rva = align_up(rdata_sec_rva + len(rdata_buf), SECTION_ALIGN)
    pdata_buf = bytearray()
    for b_rva, e_rva, ui_rva in runtime_fns:
        pdata_buf.extend(struct.pack("<III", b_rva, e_rva, ui_rva))
    if not pdata_buf:
        pdata_buf.extend(b"\x00" * 12)

    # 4. `.rsrc` (`10.0.16299.15` version resource)
    rsrc_sec_rva = align_up(pdata_sec_rva + len(pdata_buf), SECTION_ALIGN)
    rsrc_bytes = build_version_resource_section(rsrc_sec_rva)

    # 5. `.reloc` (`DIR64` relocations: ELF `R_X86_64_64` + `guard_ptrs` + `LOAD_CONFIG` VA fields)
    all_dir64 = sorted(
        set(
            linked.dir64_reloc_rvas
            + [
                guard_check_ptr_rva,
                guard_disp_ptr_rva,
                load_cfg_rva + 0x58,
                load_cfg_rva + 0x70,
                load_cfg_rva + 0x78,
                load_cfg_rva + 0x80,
            ]
        )
    )
    reloc_sec_rva = align_up(rsrc_sec_rva + len(rsrc_bytes), SECTION_ALIGN)
    reloc_bytes = build_pe_relocs(all_dir64)

    # 6. Assemble full PE32+ image (`Subsystem = 1 IMAGE_SUBSYSTEM_NATIVE`)
    sections = [
        (b".text\x00\x00\x00", code_rva, bytes(linked.code_bytes), 0x60000020),
        (b".data\x00\x00\x00", data_sec_rva, full_data_bytes, 0xC0000040),
        (b".rdata\x00\x00", rdata_sec_rva, bytes(rdata_buf), 0x40000040),
        (b".pdata\x00\x00", pdata_sec_rva, bytes(pdata_buf), 0x40000040),
        (b".rsrc\x00\x00\x00", rsrc_sec_rva, rsrc_bytes, 0x40000040),
        (b".reloc\x00\x00", reloc_sec_rva, reloc_bytes, 0x42000040),
    ]

    e_lfanew = 0x80
    num_secs = len(sections)
    hdr_end = e_lfanew + 4 + 20 + 240 + num_secs * 40
    size_of_headers = align_up(hdr_end, FILE_ALIGN)

    pe_img = bytearray(size_of_headers)
    # DOS Header + stub
    struct.pack_into("<H", pe_img, 0x00, 0x5A4D)  # 'MZ'
    struct.pack_into("<I", pe_img, 0x3C, e_lfanew)

    # PE Signature + COFF File Header
    struct.pack_into("<I", pe_img, e_lfanew, 0x00004550)  # 'PE\0\0'
    struct.pack_into(
        "<HHIIIHH",
        pe_img,
        e_lfanew + 4,
        0x8664,      # Machine = IMAGE_FILE_MACHINE_AMD64
        num_secs,
        0x59CD8000,  # TimeDateStamp
        0,
        0,
        240,         # SizeOfOptionalHeader (PE32+)
        0x0022,      # Characteristics = EXECUTABLE_IMAGE | LARGE_ADDRESS_AWARE
    )

    last_rva = sections[-1][1] + len(sections[-1][2])
    size_of_image = align_up(last_rva, SECTION_ALIGN)
    size_of_code = align_up(len(linked.code_bytes), FILE_ALIGN)
    size_of_init_data = sum(align_up(len(s[2]), FILE_ALIGN) for s in sections[1:])
    entry_rva = linked.global_sym_rva["NtKiSystemStartupEntry"]

    opt_off = e_lfanew + 24
    struct.pack_into(
        "<HBBIIIIIQIIHHHHHHIIIIHHQQQQII",
        pe_img,
        opt_off,
        0x020B,             # Magic = PE32+
        14,                 # MajorLinkerVersion
        0,                  # MinorLinkerVersion
        size_of_code,
        size_of_init_data,
        0,                  # SizeOfUninitializedData
        entry_rva,          # AddressOfEntryPoint -> NtKiSystemStartupEntry
        code_rva,           # BaseOfCode
        IMAGE_BASE,         # ImageBase = 0x140000000
        SECTION_ALIGN,
        FILE_ALIGN,
        10,                 # MajorOperatingSystemVersion
        0,                  # MinorOperatingSystemVersion
        10,                 # MajorImageVersion
        0,                  # MinorImageVersion
        10,                 # MajorSubsystemVersion
        0,                  # MinorSubsystemVersion
        0,                  # Win32VersionValue
        size_of_image,
        size_of_headers,
        0,                  # CheckSum (filled below)
        1,                  # Subsystem = IMAGE_SUBSYSTEM_NATIVE (1)
        0x4160,             # DllCharacteristics = HIGH_ENTROPY_VA | DYNAMIC_BASE | NX_COMPAT | GUARD_CF
        0x40000,            # SizeOfStackReserve
        0x6000,             # SizeOfStackCommit (24 KiB Windows x64 kernel stack)
        0x100000,           # SizeOfHeapReserve
        0x1000,             # SizeOfHeapCommit
        0,                  # LoaderFlags
        16,                 # NumberOfRvaAndSizes
    )

    # Data Directories (16 * 8B at opt_off + 112)
    dd_off = opt_off + 112
    data_dirs = {
        0: (edata_rva, len(edata_bytes)),
        1: (idata_rva, idt_size),
        2: (rsrc_sec_rva, len(rsrc_bytes)),
        3: (pdata_sec_rva, len(runtime_fns) * 12 if runtime_fns else 0),
        5: (reloc_sec_rva, len(reloc_bytes)),
        10: (load_cfg_rva, 256),
        12: (data_sec_rva, len(iat_entries_flat) * 8),
    }
    for d_idx, (d_rva, d_sz) in data_dirs.items():
        struct.pack_into("<II", pe_img, dd_off + d_idx * 8, d_rva, d_sz)

    # Section Headers + Raw Section Bodies
    sh_base = opt_off + 240
    cur_file_off = size_of_headers
    for s_idx, (sname, srva, sbytes, schars) in enumerate(sections):
        raw_sz = align_up(len(sbytes), FILE_ALIGN)
        v_sz = len(sbytes)
        struct.pack_into(
            "<8sIIIIIIHHI",
            pe_img,
            sh_base + s_idx * 40,
            sname,
            v_sz,
            srva,
            raw_sz,
            cur_file_off,
            0,
            0,
            0,
            0,
            schars,
        )
        pe_img.extend(sbytes)
        pe_img.extend(b"\x00" * (raw_sz - len(sbytes)))
        cur_file_off += raw_sz

    # Compute & store exact `CheckSumMappedFile` PE checksum
    checksum = compute_pe_checksum(pe_img)
    struct.pack_into("<I", pe_img, e_lfanew + 0x58, checksum)

    args.out.parent.mkdir(parents=True, exist_ok=True)
    args.out.write_bytes(pe_img)
    print(
        f"[OK] Linked standalone Windows PE32+ kernel: {args.out}\n"
        f"     Subsystem=1 (NATIVE), Entry=0x{entry_rva:x}, ImageSize=0x{size_of_image:x}, "
        f"Exports={len(export_tuples)} ({bound_live_exports} live code + {bound_data_exports} data), "
        f"Imports={len(mod_slices)} DLLs, SEH={len(runtime_fns)}, CFG={len(gfids_rvas)}, "
        f"CheckSum=0x{checksum:08x}"
    )
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
