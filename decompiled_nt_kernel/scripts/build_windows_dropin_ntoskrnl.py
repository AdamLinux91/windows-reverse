#!/usr/bin/env python3
"""
Boot-to-Desktop Binary-Grafted Drop-In `ntoskrnl.exe` Builder
(`build_windows_dropin_ntoskrnl.py`)

Links the 16 compiled `-mabi=ms` NT kernel subsystem `.o` object files directly onto a
retail Windows x64 `ntoskrnl.exe` host binary to produce `build/ntoskrnl_dropin.exe`,
a full-desktop bootable Windows NT kernel image that can be dropped straight into
`C:\\Windows\\System32\\ntoskrnl_decompiled.exe` and booted via `winload.efi`.

Operations performed:
  1. Appends `.ntrev` (non-paged `MEM_EXECUTE | MEM_READ` Ring-0 code) containing our
     compiled 16-subsystem C & preserved assembly functions, resolving all `R_X86_64_PC32`,
     `R_X86_64_PLT32`, and `R_X86_64_64` relocations directly against `.ntrev` peers and
     host `ntoskrnl.exe` RVAs (`ntoskrnl_16299_rva_map.json` on `10.0.16299.15`, or dynamic
     `.edata`/`.idata` binding on other Windows 10/11 x64 builds).
  2. Redirects `.edata` (`IMAGE_EXPORT_DIRECTORY.AddressOfFunctions`) RVAs for exported
     decompiled routines directly to `.ntrev` (all external drivers `hal.dll`, `ntfs.sys`,
     `ndis.sys`, `tcpip.sys`, `win32kfull.sys`, `dxgkrnl.sys` invoke our compiled code).
  3. Patches non-chained `size >= 5` primary function prologues at their original `.text`/
     `PAGE` RVAs with `E9 <rel32>` jumps to `.ntrev` (after verifying zero `.reloc` overlap
     inside the 5-byte prologue window so `winload.efi` KASLR relocation never corrupts `E9`).
  4. Synthesizes Microsoft x64 `.pdata` (`RUNTIME_FUNCTION` + `UNWIND_INFO` v1) entries for
     non-leaf `.ntrev` functions and extends Data Directory 3 (`.pdata`).
  5. Rebuilds the sorted Control Flow Guard `GFIDS` table (preserving exact `(GuardFlags>>28)&0xF`
     entry stride) with all `.ntrev` function RVAs and updates `LOAD_CONFIG`.
  6. Rebuilds `.reloc` (`IMAGE_REL_BASED_DIR64`), strips invalidated `SECURITY` DataDir[4] +
     `IMAGE_DLLCHARACTERISTICS_FORCE_INTEGRITY`, and writes an exact `CheckSumMappedFile`
     PE header checksum.
"""
from __future__ import annotations

import argparse
import json
import struct
import sys
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))
from pe_elf_linker import (
    STB_GLOBAL,
    STB_WEAK,
    SHN_UNDEF,
    align_up,
    build_pe_relocs,
    compute_pe_checksum,
    link_elf_objects,
    parse_elf64_object,
    parse_pe_relocs,
    synthesize_win64_unwind_tables,
)


def rva_to_file_offset(sections: list[dict], rva: int) -> int | None:
    for sec in sections:
        va = sec["VirtualAddress"]
        span = max(sec["VirtualSize"], sec["SizeOfRawData"])
        if va <= rva < va + span:
            off_in_sec = rva - va
            if off_in_sec < sec["SizeOfRawData"]:
                return sec["PointerToRawData"] + off_in_sec
    return None


def parse_host_pe_metadata(buf: bytes) -> dict:
    if buf[:2] != b"MZ":
        raise ValueError("Host binary is not a valid MZ/PE executable")
    e_lfanew = struct.unpack_from("<I", buf, 0x3C)[0]
    if buf[e_lfanew : e_lfanew + 4] != b"PE\x00\x00":
        raise ValueError("Host binary missing PE\\0\\0 signature")

    machine, num_secs, timestamp, _sym_ptr, _num_syms, opt_hdr_sz, chars = struct.unpack_from(
        "<HHIIIHH", buf, e_lfanew + 4
    )
    if machine != 0x8664:
        raise ValueError(f"Expected AMD64 (0x8664) ntoskrnl.exe, got 0x{machine:x}")

    opt_off = e_lfanew + 24
    magic = struct.unpack_from("<H", buf, opt_off)[0]
    if magic != 0x020B:
        raise ValueError(f"Expected PE32+ (0x20B), got 0x{magic:x}")

    entry_rva = struct.unpack_from("<I", buf, opt_off + 16)[0]
    image_base = struct.unpack_from("<Q", buf, opt_off + 24)[0]
    sec_align, file_align = struct.unpack_from("<II", buf, opt_off + 32)
    size_of_image, size_of_headers = struct.unpack_from("<II", buf, opt_off + 56)
    subsystem, dll_chars = struct.unpack_from("<HH", buf, opt_off + 68)
    num_dd = struct.unpack_from("<I", buf, opt_off + 108)[0]

    dd_off = opt_off + 112
    data_dirs: list[tuple[int, int]] = []
    for i in range(min(num_dd, 16)):
        data_dirs.append(struct.unpack_from("<II", buf, dd_off + i * 8))

    sh_off = opt_off + opt_hdr_sz
    sections: list[dict] = []
    for i in range(num_secs):
        pos = sh_off + i * 40
        nm, vsz, vrva, rsz, rptr, _p1, _p2, _n1, _n2, schars = struct.unpack_from(
            "<8sIIIIIIHHI", buf, pos
        )
        sections.append(
            {
                "index": i,
                "header_off": pos,
                "name": nm.rstrip(b"\x00").decode("ascii", errors="replace"),
                "VirtualSize": vsz,
                "VirtualAddress": vrva,
                "SizeOfRawData": rsz,
                "PointerToRawData": rptr,
                "Characteristics": schars,
            }
        )

    # Extract RSDS GUID if present in DEBUG directory (DataDir[6])
    rsds_guid = ""
    if len(data_dirs) > 6 and data_dirs[6][0] and data_dirs[6][1] >= 28:
        dbg_rva, dbg_sz = data_dirs[6]
        dbg_off = rva_to_file_offset(sections, dbg_rva)
        if dbg_off is not None:
            for dpos in range(dbg_off, dbg_off + dbg_sz, 28):
                if dpos + 28 > len(buf):
                    break
                _ch, _ts, _maj, _min, dtype, dsz, _addr_raw, ptr_raw = struct.unpack_from(
                    "<IIHHIIII", buf, dpos
                )
                if dtype == 2 and ptr_raw + 24 <= len(buf) and buf[ptr_raw : ptr_raw + 4] == b"RSDS":
                    d1, d2, d3 = struct.unpack_from("<IHH", buf, ptr_raw + 4)
                    d4 = buf[ptr_raw + 12 : ptr_raw + 20]
                    rsds_guid = (
                        f"{d1:08X}-{d2:04X}-{d3:04X}-"
                        f"{d4[:2].hex().upper()}-{d4[2:].hex().upper()}"
                    )
                    break

    return {
        "e_lfanew": e_lfanew,
        "opt_off": opt_off,
        "dd_off": dd_off,
        "sh_off": sh_off,
        "num_secs": num_secs,
        "opt_hdr_sz": opt_hdr_sz,
        "entry_rva": entry_rva,
        "image_base": image_base,
        "sec_align": sec_align,
        "file_align": file_align,
        "size_of_image": size_of_image,
        "size_of_headers": size_of_headers,
        "subsystem": subsystem,
        "dll_chars": dll_chars,
        "data_dirs": data_dirs,
        "sections": sections,
        "rsds_guid": rsds_guid,
    }


def parse_host_exports(buf: bytes, meta: dict) -> tuple[dict[str, tuple[int, int]], int]:
    """Return `(name -> (current_rva, func_table_entry_file_offset), addr_of_funcs_file_off)`."""
    edata_rva, edata_sz = meta["data_dirs"][0]
    if not edata_rva or not edata_sz:
        return {}, 0
    ed_off = rva_to_file_offset(meta["sections"], edata_rva)
    if ed_off is None:
        return {}, 0
    (
        _chars,
        _ts,
        _maj,
        _min,
        _name_rva,
        _base,
        num_funcs,
        num_names,
        addr_funcs_rva,
        addr_names_rva,
        addr_ords_rva,
    ) = struct.unpack_from("<IIHHIIIIIII", buf, ed_off)

    af_off = rva_to_file_offset(meta["sections"], addr_funcs_rva)
    an_off = rva_to_file_offset(meta["sections"], addr_names_rva)
    ao_off = rva_to_file_offset(meta["sections"], addr_ords_rva)
    if af_off is None or an_off is None or ao_off is None:
        return {}, 0

    out: dict[str, tuple[int, int]] = {}
    for i in range(num_names):
        nm_rva = struct.unpack_from("<I", buf, an_off + i * 4)[0]
        ord_idx = struct.unpack_from("<H", buf, ao_off + i * 2)[0]
        nm_off = rva_to_file_offset(meta["sections"], nm_rva)
        if nm_off is None or ord_idx >= num_funcs:
            continue
        end = buf.find(b"\x00", nm_off)
        nm = buf[nm_off:end].decode("ascii", errors="replace")
        slot_off = af_off + ord_idx * 4
        fn_rva = struct.unpack_from("<I", buf, slot_off)[0]
        out[nm] = (fn_rva, slot_off)
    return out, af_off


def parse_host_imports(buf: bytes, meta: dict) -> dict[str, int]:
    """Return `name / __imp_name -> iat_slot_rva` from host `.idata`."""
    if len(meta["data_dirs"]) <= 1 or not meta["data_dirs"][1][0]:
        return {}
    idata_rva, _idata_sz = meta["data_dirs"][1]
    id_off = rva_to_file_offset(meta["sections"], idata_rva)
    if id_off is None:
        return {}

    out: dict[str, int] = {}
    pos = id_off
    while pos + 20 <= len(buf):
        ilt_rva, _ts, _fwd, dll_rva, iat_rva = struct.unpack_from("<IIIII", buf, pos)
        if ilt_rva == 0 and dll_rva == 0 and iat_rva == 0:
            break
        thunk_rva = ilt_rva if ilt_rva else iat_rva
        thunk_off = rva_to_file_offset(meta["sections"], thunk_rva)
        idx = 0
        while thunk_off is not None and thunk_off + idx * 8 + 8 <= len(buf):
            val = struct.unpack_from("<Q", buf, thunk_off + idx * 8)[0]
            if val == 0:
                break
            if not (val & (1 << 63)):
                hn_off = rva_to_file_offset(meta["sections"], val & 0x7FFFFFFF)
                if hn_off is not None:
                    end = buf.find(b"\x00", hn_off + 2)
                    inm = buf[hn_off + 2 : end].decode("ascii", errors="replace")
                    slot_rva = iat_rva + idx * 8
                    out[inm] = slot_rva
                    out[f"__imp_{inm}"] = slot_rva
            idx += 1
        pos += 20
    return out


def main() -> int:
    ap = argparse.ArgumentParser(
        description="Build boot-to-desktop drop-in Windows ntoskrnl.exe by grafting compiled .ntrev objects"
    )
    ap.add_argument("--host-ntoskrnl", required=True, type=Path, help="Host Windows x64 ntoskrnl.exe")
    ap.add_argument("--specs-dir", required=True, type=Path)
    ap.add_argument("--proofs", type=Path, help="Legacy selected-tree proof manifest")
    ap.add_argument(
        "--whole-manifest",
        type=Path,
        help="ntrev whole-kernel RVA manifest; maps generated C names back to PDB/PE names",
    )
    ap.add_argument(
        "--patch-list",
        type=Path,
        help=(
            "Optional JSON allowlist of candidate entry points to redirect. With this option "
            "the host kernel remains authoritative for every non-listed function."
        ),
    )
    ap.add_argument("--out", required=True, type=Path)
    ap.add_argument("objects", nargs="+", type=Path, help="Compiled subsystem .o files")
    args = ap.parse_args()

    raw_in = args.host_ntoskrnl.read_bytes()
    meta = parse_host_pe_metadata(raw_in)
    if args.whole_manifest is not None:
        whole_manifest = json.loads(args.whole_manifest.read_text(encoding="utf-8"))
        rva_spec = {"decompiled_functions": {}}
        for candidate_name, entry in whole_manifest.get("functions", {}).items():
            rva_spec["decompiled_functions"][candidate_name] = {
                "rva": int(entry["rva"]),
                "patch_prologue": (not bool(entry.get("is_raw_asm")) and int(entry.get("size", 0)) >= 5),
            }
        manifest = {
            "functions": [
                {"name": candidate_name, "is_raw_asm": bool(entry.get("is_raw_asm"))}
                for candidate_name, entry in whole_manifest.get("functions", {}).items()
            ]
        }
    else:
        if args.proofs is None:
            raise ValueError("--proofs is required unless --whole-manifest is supplied")
        rva_spec = json.loads((args.specs_dir / "ntoskrnl_16299_rva_map.json").read_text())
        manifest = json.loads(args.proofs.read_text())
    raw_asm_names = {f["name"] for f in manifest["functions"] if f.get("is_raw_asm")}

    # A patch list turns this builder into a conservative, host-backed hybrid. The
    # original kernel remains in place for every non-listed entry point, which is the
    # only practical way to retain application compatibility while candidate functions
    # are being validated inside a real Windows guest. Without a list, the historical
    # all-functions graft behavior is retained for backwards compatibility.
    patch_host_names: set[str] | None = None
    patch_candidate_names: set[str] = set()
    candidate_to_host: dict[str, str] = {}
    if args.whole_manifest is not None:
        for candidate_name, entry in whole_manifest.get("functions", {}).items():
            candidate_to_host[candidate_name] = str(entry.get("orig_name", candidate_name))
    if args.patch_list is not None:
        patch_doc = json.loads(args.patch_list.read_text(encoding="utf-8"))
        patch_items = patch_doc.get("functions", patch_doc) if isinstance(patch_doc, dict) else patch_doc
        if not isinstance(patch_items, list) or not patch_items:
            raise ValueError("--patch-list must contain a non-empty JSON functions list")
        patch_host_names = set()
        for item in patch_items:
            if isinstance(item, str):
                host_name = candidate_name = item
            elif isinstance(item, dict):
                host_name = str(item.get("host_name", item.get("name", ""))).strip()
                candidate_name = str(item.get("candidate_name", item.get("name", host_name))).strip()
            else:
                raise ValueError("Each --patch-list functions entry must be a string or object")
            if not host_name or not candidate_name:
                raise ValueError("Each --patch-list entry needs a function name")
            patch_host_names.add(host_name)
            patch_candidate_names.add(candidate_name)
            candidate_to_host[candidate_name] = host_name

    host_exports, _af_off = parse_host_exports(raw_in, meta)
    host_imports = parse_host_imports(raw_in, meta)

    is_exact_16299 = meta["rsds_guid"] == "83DB4240-4EFD-4AB6-AFB6-FA864B700CB3"

    # Build external RVA resolver & select graftable objects
    ext_resolver: dict[str, int] = {}
    for k, rva in host_imports.items():
        ext_resolver[k] = rva
    for k, (rva, _slot) in host_exports.items():
        ext_resolver[k] = rva

    selected_objects: list[Path] = []
    if is_exact_16299:
        for sym_nm, info in rva_spec.get("external_symbols", {}).items():
            ext_resolver[sym_nm] = info["rva"]
        selected_objects = list(args.objects)
        mode_desc = "Full PDB-Verified 10.0.16299.15 Mode (All 16 Subsystems)"
    else:
        # Dynamic Cross-Build Mode: keep objects whose external symbols all resolve via host .edata/.idata
        defined_across_objs: set[str] = set()
        parsed_map = {p: parse_elf64_object(p) for p in args.objects}
        for p, pobj in parsed_map.items():
            for s in pobj.symbols:
                if s.bind in (STB_GLOBAL, STB_WEAK) and s.shndx != SHN_UNDEF and s.name:
                    defined_across_objs.add(s.name)
        for p, pobj in parsed_map.items():
            undefs = {
                s.name
                for s in pobj.symbols
                if s.shndx == SHN_UNDEF and s.bind == STB_GLOBAL and s.name
            }
            missing = undefs - defined_across_objs - set(ext_resolver.keys())
            if not missing:
                selected_objects.append(p)
        if not selected_objects:
            raise RuntimeError(
                f"Host ntoskrnl.exe (RSDS={meta['rsds_guid']}) did not satisfy dynamic symbol binding "
                "for whole subsystem units; supply a 10.0.16299.15 host or rebuild with matching PDB."
            )
        mode_desc = f"Dynamic Cross-Build Mode (RSDS={meta['rsds_guid']}, {len(selected_objects)} units)"

    # Strip unmapped trailing Authenticode SECURITY table so appended sections are contiguous on disk
    max_mapped_end = max(
        s["PointerToRawData"] + s["SizeOfRawData"] for s in meta["sections"]
    )
    buf = bytearray(raw_in[: align_up(max_mapped_end, meta["file_align"])])

    first_raw = min(s["PointerToRawData"] for s in meta["sections"] if s["PointerToRawData"] > 0)
    sh_table_end = meta["sh_off"] + meta["num_secs"] * 40
    if sh_table_end + 3 * 40 > first_raw:
        raise RuntimeError("Insufficient room in PE section header table for 3 appended sections")

    # Parse existing `.reloc` RVAs before modifying anything
    existing_reloc_rvas: list[int] = []
    if len(meta["data_dirs"]) > 5 and meta["data_dirs"][5][0] and meta["data_dirs"][5][1]:
        rel_rva, rel_sz = meta["data_dirs"][5]
        rel_off = rva_to_file_offset(meta["sections"], rel_rva)
        if rel_off is not None:
            existing_reloc_rvas = parse_pe_relocs(bytes(buf[rel_off : rel_off + rel_sz]))
    reloc_rva_set = set(existing_reloc_rvas)

    # Link selected `.o` objects into `.ntrev` (starting at aligned `SizeOfImage`)
    ntrev_code_rva = align_up(meta["size_of_image"], meta["sec_align"])
    linked = link_elf_objects(
        selected_objects,
        code_rva=ntrev_code_rva,
        external_rva_resolver=ext_resolver,
        image_base=meta["image_base"],
        section_align=meta["sec_align"],
    )
    ntrev_data_rva = linked.data_rva
    ntrevd_buf = bytearray(linked.data_bytes)

    # 1. Redirect `.edata` exports for grafted functions. In staged hybrid mode,
    #    only the explicit allowlist is redirected; all other exports remain host code.
    edata_redirected = 0
    patched_candidates: set[str] = set()
    for fn_name, new_rva in linked.global_sym_rva.items():
        if fn_name not in linked.func_bounds:
            continue
        host_name = candidate_to_host.get(fn_name, fn_name)
        if patch_host_names is not None and host_name not in patch_host_names and fn_name not in patch_candidate_names:
            continue
        if host_name in host_exports:
            _old_rva, slot_file_off = host_exports[host_name]
            struct.pack_into("<I", buf, slot_file_off, new_rva)
            edata_redirected += 1
            patched_candidates.add(fn_name)

    # 2. Patch non-chained primary function prologues (`E9 <rel32>`) where safe
    prologues_patched = 0
    decomp_meta = rva_spec.get("decompiled_functions", {})
    for fn_name, (new_rva, _end_rva) in linked.func_bounds.items():
        host_name = candidate_to_host.get(fn_name, fn_name)
        if patch_host_names is not None and host_name not in patch_host_names and fn_name not in patch_candidate_names:
            continue
        orig_rva: int | None = None
        can_patch = False
        if is_exact_16299 and fn_name in decomp_meta:
            orig_rva = decomp_meta[fn_name]["rva"]
            can_patch = bool(decomp_meta[fn_name]["patch_prologue"])
        elif host_name in host_exports and host_name != "KiSystemStartup":
            orig_rva = host_exports[host_name][0]
            can_patch = True

        if not can_patch or orig_rva is None:
            continue
        # Never overwrite a 5-byte prologue if any `.reloc` DIR64 fixup overlaps `[orig_rva - 7 .. orig_rva + 5)`!
        if any((orig_rva - 7) <= rr < (orig_rva + 5) for rr in range(orig_rva - 7, orig_rva + 5) if rr in reloc_rva_set):
            continue
        orig_file_off = rva_to_file_offset(meta["sections"], orig_rva)
        if orig_file_off is None or orig_file_off + 5 > len(buf):
            continue
        rel32 = new_rva - (orig_rva + 5)
        if -0x80000000 <= rel32 <= 0x7FFFFFFF:
            buf[orig_file_off] = 0xE9
            struct.pack_into("<i", buf, orig_file_off + 1, rel32)
            prologues_patched += 1
            patched_candidates.add(fn_name)

    if patch_host_names is not None:
        unresolved_patch_names = sorted(
            name for name in patch_host_names
            if name not in {candidate_to_host.get(candidate, candidate) for candidate in patched_candidates}
        )
        if unresolved_patch_names:
            raise RuntimeError(
                "The staged hybrid patch list was not fully applied: "
                + ", ".join(unresolved_patch_names)
            )

    # 3. Synthesize `.pdata` (`RUNTIME_FUNCTION` + `UNWIND_INFO`) and append to existing `.pdata`
    ntrevd_buf.extend(b"\x00" * (align_up(len(ntrevd_buf), 8) - len(ntrevd_buf)))
    ui_base_rva = ntrev_data_rva + len(ntrevd_buf)
    ui_bytes, new_rt_fns = synthesize_win64_unwind_tables(
        linked.code_bytes,
        code_rva=ntrev_code_rva,
        func_bounds=linked.func_bounds,
        raw_asm_names=raw_asm_names,
        unwind_info_base_rva=ui_base_rva,
    )
    ntrevd_buf.extend(ui_bytes)

    pdata_count_added = len(new_rt_fns)
    if new_rt_fns and len(meta["data_dirs"]) > 3 and meta["data_dirs"][3][0]:
        old_pdata_rva, old_pdata_sz = meta["data_dirs"][3]
        old_pdata_off = rva_to_file_offset(meta["sections"], old_pdata_rva)
        if old_pdata_off is not None:
            ntrevd_buf.extend(b"\x00" * (align_up(len(ntrevd_buf), 16) - len(ntrevd_buf)))
            merged_pdata_rva = ntrev_data_rva + len(ntrevd_buf)
            ntrevd_buf.extend(buf[old_pdata_off : old_pdata_off + old_pdata_sz])
            for b_rva, e_rva, u_rva in new_rt_fns:
                ntrevd_buf.extend(struct.pack("<III", b_rva, e_rva, u_rva))
            new_pdata_sz = old_pdata_sz + len(new_rt_fns) * 12
            struct.pack_into("<II", buf, meta["dd_off"] + 3 * 8, merged_pdata_rva, new_pdata_sz)

    # 4. Rebuild Control Flow Guard (`GFIDS`) table in `.ntrevd` so `_guard_dispatch_icall`
    #    validates every indirect call into `.ntrev`!
    cfg_total_entries = 0
    if len(meta["data_dirs"]) > 10 and meta["data_dirs"][10][0] and meta["data_dirs"][10][1] >= 0x94:
        lc_rva, _lc_sz = meta["data_dirs"][10]
        lc_off = rva_to_file_offset(meta["sections"], lc_rva)
        if lc_off is not None:
            gf_table_va, gf_count = struct.unpack_from("<QQ", buf, lc_off + 0x80)
            gf_flags = struct.unpack_from("<I", buf, lc_off + 0x90)[0]
            if gf_table_va and gf_count > 0:
                stride = (gf_flags >> 28) & 0xF
                entry_sz = 4 + stride
                gf_rva = gf_table_va - meta["image_base"]
                gf_off = rva_to_file_offset(meta["sections"], gf_rva)
                if gf_off is not None:
                    gf_map: dict[int, bytes] = {}
                    for i in range(gf_count):
                        epos = gf_off + i * entry_sz
                        erva = struct.unpack_from("<I", buf, epos)[0]
                        gf_map[erva] = bytes(buf[epos + 4 : epos + entry_sz])
                    zero_meta = b"\x00" * stride
                    for start_rva, _end_rva in linked.func_bounds.values():
                        gf_map.setdefault(start_rva, zero_meta)

                    ntrevd_buf.extend(b"\x00" * (align_up(len(ntrevd_buf), 16) - len(ntrevd_buf)))
                    new_gf_rva = ntrev_data_rva + len(ntrevd_buf)
                    for erva in sorted(gf_map.keys()):
                        ntrevd_buf.extend(struct.pack("<I", erva) + gf_map[erva])
                    cfg_total_entries = len(gf_map)
                    struct.pack_into(
                        "<QQ",
                        buf,
                        lc_off + 0x80,
                        meta["image_base"] + new_gf_rva,
                        cfg_total_entries,
                    )
                    existing_reloc_rvas.append(lc_rva + 0x80)

    # 5. Build updated `.reloc2` section (`existing_reloc_rvas + linked.dir64_reloc_rvas`)
    merged_dir64 = sorted(set(existing_reloc_rvas) | set(linked.dir64_reloc_rvas))
    reloc2_bytes = build_pe_relocs(merged_dir64)
    reloc2_rva = align_up(ntrev_data_rva + len(ntrevd_buf), meta["sec_align"])

    # 6. Append `.ntrev`, `.ntrevd`, and `.reloc2` sections to `buf` & update PE headers
    appended_secs = [
        (b".ntrev\x00\x00", ntrev_code_rva, bytes(linked.code_bytes), 0x60000020),
        (b".ntrevd\x00", ntrev_data_rva, bytes(ntrevd_buf), 0xC0000040),
        (b".reloc2\x00", reloc2_rva, reloc2_bytes, 0x42000040),
    ]

    for idx_add, (sname, srva, sbytes, schars) in enumerate(appended_secs):
        raw_ptr = len(buf)
        raw_sz = align_up(len(sbytes), meta["file_align"])
        buf.extend(sbytes)
        buf.extend(b"\x00" * (raw_sz - len(sbytes)))
        sh_pos = meta["sh_off"] + (meta["num_secs"] + idx_add) * 40
        struct.pack_into(
            "<8sIIIIIIHHI",
            buf,
            sh_pos,
            sname,
            len(sbytes),
            srva,
            raw_sz,
            raw_ptr,
            0,
            0,
            0,
            0,
            schars,
        )

    new_num_secs = meta["num_secs"] + len(appended_secs)
    new_size_of_image = align_up(reloc2_rva + len(reloc2_bytes), meta["sec_align"])

    # Update NumberOfSections & SizeOfImage
    struct.pack_into("<H", buf, meta["e_lfanew"] + 6, new_num_secs)
    struct.pack_into("<I", buf, meta["opt_off"] + 56, new_size_of_image)

    # Strip invalidated Authenticode SECURITY directory (DataDir[4]) & clear FORCE_INTEGRITY (0x0080)
    struct.pack_into("<II", buf, meta["dd_off"] + 4 * 8, 0, 0)
    new_dll_chars = meta["dll_chars"] & ~0x0080
    struct.pack_into("<H", buf, meta["opt_off"] + 70, new_dll_chars)

    # Point DataDir[5] (`.reloc`) to `.reloc2`
    struct.pack_into("<II", buf, meta["dd_off"] + 5 * 8, reloc2_rva, len(reloc2_bytes))

    # Recompute exact `CheckSumMappedFile` PE checksum
    new_checksum = compute_pe_checksum(buf)
    struct.pack_into("<I", buf, meta["e_lfanew"] + 0x58, new_checksum)

    args.out.parent.mkdir(parents=True, exist_ok=True)
    args.out.write_bytes(buf)
    print(
        f"[OK] Built boot-to-desktop drop-in Windows kernel: {args.out}\n"
        f"     Mode           : {mode_desc}\n"
        f"     Grafted Code   : {len(linked.func_bounds)} functions in .ntrev (0x{ntrev_code_rva:x}, {len(linked.code_bytes):,} bytes)\n"
        f"     Patch Policy   : {'allowlisted host-backed hybrid' if patch_host_names is not None else 'legacy all-grafted'}\n"
        f"     Exports Bound  : {edata_redirected} .edata RVAs redirected to .ntrev\n"
        f"     Prologues      : {prologues_patched} internal functions E9-trampolined to .ntrev\n"
        f"     SEH & CFG      : +{pdata_count_added} .pdata unwind entries | {cfg_total_entries:,} GFIDS CFG targets\n"
        f"     PE CheckSum    : 0x{new_checksum:08x} (SizeOfImage=0x{new_size_of_image:x})"
    )
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
