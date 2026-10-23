#!/usr/bin/env python3
"""Link a standalone PE from the generated whole-kernel object tree.

This is intentionally different from ``build_windows_dropin_ntoskrnl.py``:

* it requires the complete ``ntrev`` whole-kernel manifest;
* it does not accept an allowlist or a host-code fallback;
* it makes the generated ``KiSystemStartup`` the PE entry point;
* it redirects generated exports to generated functions; and
* it overwrites the original executable section bytes with INT3 guards so an
  accidental call into an unconverted Microsoft code address fails immediately.

The input PE is used only as a compatibility container for its image identity, import
contracts, non-executable initialized data, resources, and loader metadata. It is not
used as an executable fallback. Raw assembly functions in the generated tree remain
part of the standalone result by design; those are the explicit non-C boundary of the
whole-kernel lifter.

This linker is a structural build step, not a Windows boot proof. The output must pass
an actual Windows VM boot/application gate before it can be called a functioning NT
kernel.
"""

from __future__ import annotations

import argparse
import json
import struct
import sys
from pathlib import Path

SCRIPT_DIR = Path(__file__).resolve().parent
sys.path.insert(0, str(SCRIPT_DIR))

from build_windows_dropin_ntoskrnl import (  # noqa: E402
    parse_host_exports,
    parse_host_imports,
    parse_host_pe_metadata,
    parse_pe_relocs,
    rva_to_file_offset,
)
from pe_elf_linker import (  # noqa: E402
    align_up,
    build_pe_relocs,
    compute_pe_checksum,
    link_elf_objects,
    parse_elf64_object,
    synthesize_win64_unwind_tables,
)

IMAGE_SCN_MEM_EXECUTE = 0x20000000


def _read_u64_at_rva(buf: bytes | bytearray, sections: list[dict], rva: int) -> int | None:
    off = rva_to_file_offset(sections, rva)
    if off is None or off + 8 > len(buf):
        return None
    return struct.unpack_from("<Q", buf, off)[0]


def _write_u64_at_rva(buf: bytearray, sections: list[dict], rva: int, value: int) -> bool:
    off = rva_to_file_offset(sections, rva)
    if off is None or off + 8 > len(buf):
        return False
    struct.pack_into("<Q", buf, off, value & 0xFFFFFFFFFFFFFFFF)
    return True


def _append_section(
    buf: bytearray,
    meta: dict,
    name: bytes,
    rva: int,
    payload: bytes,
    characteristics: int,
    section_index: int,
) -> None:
    raw_ptr = len(buf)
    raw_size = align_up(len(payload), meta["file_align"])
    buf.extend(payload)
    buf.extend(b"\x00" * (raw_size - len(payload)))
    sh_pos = meta["sh_off"] + (meta["num_secs"] + section_index) * 40
    struct.pack_into(
        "<8sIIIIIIHHI",
        buf,
        sh_pos,
        name,
        len(payload),
        rva,
        raw_size,
        raw_ptr,
        0,
        0,
        0,
        0,
        characteristics,
    )


def _make_symbol_maps(manifest: dict, image_base: int, object_paths: list[Path]) -> tuple[dict, dict, set[str]]:
    """Return candidate/original maps plus symbols that must bind to original PE addresses."""
    functions = manifest.get("functions", {})
    candidate_to_original: dict[str, str] = {}
    original_to_candidate: dict[str, str] = {}
    for candidate, entry in functions.items():
        original = str(entry.get("orig_name", candidate))
        candidate_to_original[candidate] = original
        original_to_candidate.setdefault(original, candidate)
        for alias in entry.get("export_aliases", []):
            original_to_candidate.setdefault(str(alias), candidate)

    external: dict[str, int] = {"__ImageBase": 0}
    preferred: set[str] = {"__ImageBase"}

    # The generated manifest stores initialized data symbols as C names -> original RVA.
    for symbol, rva in manifest.get("data_symbols", {}).items():
        external[str(symbol)] = int(rva)
        preferred.add(str(symbol))

    for symbol, section in manifest.get("sections", {}).items():
        if isinstance(section, dict) and "rva" in section:
            external[str(symbol)] = int(section["rva"])
            preferred.add(str(symbol))

    # Inline literal labels encode their original VA in the symbol name. They must not
    # silently bind to the generated duplicate data blob.
    for obj_path in object_paths:
        parsed = parse_elf64_object(obj_path)
        for symbol in parsed.symbols:
            name = symbol.name
            if name.startswith("__nt_asm_sym_"):
                try:
                    target_va = int(name.rsplit("_", 1)[1], 16)
                except ValueError:
                    continue
                if image_base <= target_va < image_base + 0x100000000:
                    external[name] = target_va - image_base
                    preferred.add(name)

    return {
        "candidate_to_original": candidate_to_original,
        "original_to_candidate": original_to_candidate,
    }, external, preferred


def _reject_referenced_weak_functions(object_paths: list[Path]) -> None:
    """Reject no-op weak function shims before they can enter a standalone image."""
    weak_functions: dict[str, Path] = {}
    parsed_objects = []
    for path in object_paths:
        parsed = parse_elf64_object(path)
        parsed_objects.append(parsed)
        for symbol in parsed.symbols:
            # ELF STB_WEAK = 2, STT_FUNC = 2. Weak data definitions are handled by
            # the manifest's original-data binding; weak function definitions are not
            # acceptable because they hide missing generated implementation paths.
            if symbol.bind == 2 and symbol.sym_type == 2 and symbol.shndx not in (0, 0xFFF2):
                weak_functions.setdefault(symbol.name, parsed.path)

    referenced: dict[str, Path] = {}
    for parsed in parsed_objects:
        for relocation in parsed.relocs:
            symbol = parsed.symbols[relocation.sym_idx]
            if symbol.name in weak_functions:
                referenced.setdefault(symbol.name, parsed.path)

    if referenced:
        details = ", ".join(
            f"{name} (defined by {weak_functions[name].name}, referenced by {path.name})"
            for name, path in sorted(referenced.items())
        )
        raise RuntimeError(
            "Standalone link refuses referenced weak function fallbacks; generate the "
            f"missing raw/generated implementation instead: {details}"
        )


def _rva_is_executable(meta: dict, rva: int) -> bool:
    for section in meta["sections"]:
        if not (section["Characteristics"] & IMAGE_SCN_MEM_EXECUTE):
            continue
        start = section["VirtualAddress"]
        end = start + max(section["VirtualSize"], section["SizeOfRawData"])
        if start <= rva < end:
            return True
    return False


def _function_target_map(manifest: dict, linked_symbols: dict[str, int]) -> dict[int, int]:
    """Map original function-start RVAs to generated RVAs."""
    result: dict[int, int] = {}
    missing: list[str] = []
    for candidate, entry in manifest.get("functions", {}).items():
        generated = linked_symbols.get(candidate)
        if generated is None:
            missing.append(candidate)
            continue
        result[int(entry["rva"])] = generated
    if missing:
        raise RuntimeError(
            f"Standalone link is incomplete: {len(missing)} generated functions are absent; "
            f"first missing symbols: {missing[:12]}"
        )
    return result


def _patch_cfg_table(
    buf: bytearray,
    meta: dict,
    data_buf: bytearray,
    data_rva: int,
    generated_function_rvas: list[int],
) -> tuple[int | None, int]:
    """Append a generated-only GFIDS table and update LOAD_CONFIG if present."""
    if len(meta["data_dirs"]) <= 10 or not meta["data_dirs"][10][0]:
        return None, 0
    lc_rva, lc_size = meta["data_dirs"][10]
    if lc_size < 0x94:
        return None, 0
    lc_off = rva_to_file_offset(meta["sections"], lc_rva)
    if lc_off is None or lc_off + 0x98 > len(buf):
        return None, 0
    gf_flags = struct.unpack_from("<I", buf, lc_off + 0x90)[0]
    stride = (gf_flags >> 28) & 0xF
    if stride > 0xF:
        return None, 0
    data_buf.extend(b"\x00" * (align_up(len(data_buf), 16) - len(data_buf)))
    gf_rva = data_rva + len(data_buf)
    zero_meta = b"\x00" * stride
    for rva in sorted(set(generated_function_rvas)):
        data_buf.extend(struct.pack("<I", rva) + zero_meta)
    struct.pack_into("<QQ", buf, lc_off + 0x80, meta["image_base"] + gf_rva, len(set(generated_function_rvas)))
    return lc_rva + 0x80, len(set(generated_function_rvas))


def main() -> int:
    parser = argparse.ArgumentParser(
        description="Link a standalone generated whole-kernel Windows PE (no host-code fallback)"
    )
    parser.add_argument("--input-ntoskrnl", required=True, type=Path, help="Matching PE/PDB source image used for loader contracts")
    parser.add_argument("--whole-manifest", required=True, type=Path, help="specs/nt_whole_kernel_rvas.json")
    parser.add_argument("--out", required=True, type=Path)
    parser.add_argument("--entry-symbol", default="KiSystemStartup", help="Generated original symbol used as PE entry point")
    parser.add_argument("objects", nargs="+", type=Path)
    args = parser.parse_args()

    raw_input = args.input_ntoskrnl.read_bytes()
    meta = parse_host_pe_metadata(raw_input)
    if meta["subsystem"] != 1:
        raise ValueError(f"Input image is not IMAGE_SUBSYSTEM_NATIVE (got {meta['subsystem']})")
    manifest = json.loads(args.whole_manifest.read_text(encoding="utf-8"))
    if not manifest.get("functions"):
        raise ValueError("Whole-kernel manifest contains no functions")
    if int(manifest.get("image_base", meta["image_base"])) != meta["image_base"]:
        raise ValueError("Whole-kernel manifest and input PE have different image bases")

    host_exports, _ = parse_host_exports(raw_input, meta)
    host_imports = parse_host_imports(raw_input, meta)
    object_paths = [Path(p) for p in args.objects]
    _reject_referenced_weak_functions(object_paths)
    maps, data_external, preferred_external = _make_symbol_maps(manifest, meta["image_base"], object_paths)

    # Only imported IAT slots and manifest-declared data are loader contracts. Never
    # place host export implementations in this map: doing so would make an absent
    # generated function silently call the stock kernel, which is forbidden for this
    # standalone image. A missing generated function now fails the link explicitly.
    # Only the indirection slots are valid generated-side references. A direct
    # imported function symbol is deliberately not resolved here: the generated
    # emitter must preserve the `__imp_*` loader contract instead of turning an
    # accidental direct call into an implicit host-code dependency.
    external = {name: rva for name, rva in host_imports.items() if name.startswith("__imp_")}
    external.update(data_external)

    # First link fixes the generated code size. The final link places generated data
    # immediately after that code, while all initialized kernel globals bind to their
    # original PE RVAs through `external_precedence`.
    code_rva = align_up(meta["size_of_image"], meta["sec_align"])
    provisional_data_rva = align_up(code_rva + 0x100000, meta["sec_align"])
    provisional = link_elf_objects(
        object_paths,
        code_rva=code_rva,
        data_rva_start=provisional_data_rva,
        external_rva_resolver=external,
        image_base=meta["image_base"],
        section_align=meta["sec_align"],
        external_precedence=preferred_external,
    )
    data_rva = align_up(code_rva + len(provisional.code_bytes), meta["sec_align"])
    linked = link_elf_objects(
        object_paths,
        code_rva=code_rva,
        data_rva_start=data_rva,
        external_rva_resolver=external,
        image_base=meta["image_base"],
        section_align=meta["sec_align"],
        external_precedence=preferred_external,
    )

    function_targets = _function_target_map(manifest, linked.global_sym_rva)
    entry_candidate = maps["original_to_candidate"].get(args.entry_symbol, args.entry_symbol)
    if entry_candidate not in linked.global_sym_rva:
        raise RuntimeError(
            f"Generated entry symbol {args.entry_symbol!r} was not linked. "
            "The whole-kernel manifest must contain the real KiSystemStartup path."
        )
    entry_rva = linked.global_sym_rva[entry_candidate]

    raw_asm_names = {
        candidate for candidate, entry in manifest["functions"].items() if entry.get("is_raw_asm")
    }
    data_buf = bytearray(linked.data_bytes)
    data_buf.extend(b"\x00" * (align_up(len(data_buf), 8) - len(data_buf)))
    unwind_base = data_rva + len(data_buf)
    unwind_bytes, runtime_functions = synthesize_win64_unwind_tables(
        linked.code_bytes,
        code_rva=code_rva,
        func_bounds=linked.func_bounds,
        raw_asm_names=raw_asm_names,
        unwind_info_base_rva=unwind_base,
    )
    data_buf.extend(unwind_bytes)
    # A fixture or a leaf-only image can legitimately have no unwind records. Real
    # ntoskrnl builds normally emit many; the Windows loader still accepts a valid
    # native image with an empty generated exception directory.

    cfg_reloc_rva, cfg_count = _patch_cfg_table(
        bytearray(raw_input),  # patched again below against the output buffer
        meta,
        data_buf,
        data_rva,
        [start for start, _end in linked.func_bounds.values()],
    )

    # Truncate any Authenticode overlay and erase every original executable byte. The
    # original PE remains a container for data/loader metadata, never executable code.
    max_mapped_end = max(
        section["PointerToRawData"] + section["SizeOfRawData"]
        for section in meta["sections"]
        if section["PointerToRawData"] > 0
    )
    output = bytearray(raw_input[: align_up(max_mapped_end, meta["file_align"])])
    for section in meta["sections"]:
        if not (section["Characteristics"] & IMAGE_SCN_MEM_EXECUTE):
            continue
        start = section["PointerToRawData"]
        end = min(start + section["SizeOfRawData"], len(output))
        if end > start:
            output[start:end] = b"\xCC" * (end - start)

    # Redirect every exported generated function. Data exports remain backed by the
    # original initialized non-executable sections.
    redirected_exports = 0
    unredirected_executable_exports: list[str] = []
    for export_name, (old_rva, slot_offset) in host_exports.items():
        candidate = maps["original_to_candidate"].get(export_name)
        if candidate is None or candidate not in linked.global_sym_rva:
            if _rva_is_executable(meta, old_rva):
                unredirected_executable_exports.append(export_name)
            continue
        struct.pack_into("<I", output, slot_offset, linked.global_sym_rva[candidate])
        redirected_exports += 1
    if unredirected_executable_exports:
        raise RuntimeError(
            "Standalone link is incomplete: executable exports have no generated target; "
            f"first missing exports: {unredirected_executable_exports[:20]}"
        )

    # Patch original DIR64 function pointers (SSDT/callback tables) to generated starts.
    # Interior/mid-function pointers are not guessed; they cause a hard failure rather
    # than silently entering the wrong recompiled instruction offset.
    existing_reloc_rvas: list[int] = []
    if len(meta["data_dirs"]) > 5 and meta["data_dirs"][5][0] and meta["data_dirs"][5][1]:
        rel_rva, rel_size = meta["data_dirs"][5]
        rel_off = rva_to_file_offset(meta["sections"], rel_rva)
        if rel_off is not None:
            existing_reloc_rvas = parse_pe_relocs(bytes(output[rel_off : rel_off + rel_size]))
    relocated_function_pointers = 0
    for slot_rva in existing_reloc_rvas:
        value = _read_u64_at_rva(output, meta["sections"], slot_rva)
        if value is None or value < meta["image_base"]:
            continue
        target_rva = value - meta["image_base"]
        new_target = function_targets.get(target_rva)
        if new_target is not None:
            if not _write_u64_at_rva(output, meta["sections"], slot_rva, meta["image_base"] + new_target):
                raise RuntimeError(f"Cannot patch function pointer relocation at RVA 0x{slot_rva:x}")
            relocated_function_pointers += 1

    # Replace the exception directory with generated functions only.
    data_buf.extend(b"\x00" * (align_up(len(data_buf), 16) - len(data_buf)))
    generated_pdata_rva = data_rva + len(data_buf)
    for begin_rva, end_rva, unwind_rva in runtime_functions:
        data_buf.extend(struct.pack("<III", begin_rva, end_rva, unwind_rva))
    struct.pack_into("<II", output, meta["dd_off"] + 3 * 8, generated_pdata_rva, len(runtime_functions) * 12)

    # Rebuild the base-relocation directory for original plus generated absolute sites.
    if cfg_reloc_rva is not None:
        # `_patch_cfg_table` used a temporary buffer to calculate the location; apply it
        # to the real output at the same offset and preserve the relocation for the pointer.
        lc_rva, _lc_size = meta["data_dirs"][10]
        lc_off = rva_to_file_offset(meta["sections"], lc_rva)
        if lc_off is not None:
            # The temporary call's address is deterministic from `data_buf` layout.
            # Recompute and write the generated GFIDS pointer directly.
            gf_rva = data_rva + len(linked.data_bytes)
            gf_rva = data_rva + (align_up(len(linked.data_bytes), 8) - len(linked.data_bytes)) + len(unwind_bytes)
            gf_rva = align_up(gf_rva, 16)
            # `_patch_cfg_table` appended the table after unwind bytes at this exact
            # aligned location; its final pointer/count are written below.
            struct.pack_into("<QQ", output, lc_off + 0x80, meta["image_base"] + gf_rva, cfg_count)
            existing_reloc_rvas.append(lc_rva + 0x80)

    reloc_rva = align_up(data_rva + len(data_buf), meta["sec_align"])
    reloc_bytes = build_pe_relocs(sorted(set(existing_reloc_rvas) | set(linked.dir64_reloc_rvas)))

    first_raw = min(section["PointerToRawData"] for section in meta["sections"] if section["PointerToRawData"] > 0)
    if meta["sh_off"] + (meta["num_secs"] + 3) * 40 > first_raw:
        raise RuntimeError("Input PE has no room for generated standalone section headers")

    _append_section(output, meta, b".ntrev\x00\x00", code_rva, bytes(linked.code_bytes), 0x60000020, 0)
    _append_section(output, meta, b".ntrevd\x00", data_rva, bytes(data_buf), 0xC0000040, 1)
    _append_section(output, meta, b".reloc2\x00", reloc_rva, reloc_bytes, 0x42000040, 2)

    struct.pack_into("<H", output, meta["e_lfanew"] + 6, meta["num_secs"] + 3)
    struct.pack_into("<I", output, meta["opt_off"] + 16, entry_rva)
    struct.pack_into("<I", output, meta["opt_off"] + 56, align_up(reloc_rva + len(reloc_bytes), meta["sec_align"]))
    struct.pack_into("<II", output, meta["dd_off"] + 4 * 8, 0, 0)  # no stale Authenticode signature
    struct.pack_into("<H", output, meta["opt_off"] + 70, meta["dll_chars"] & ~0x80)
    struct.pack_into("<II", output, meta["dd_off"] + 5 * 8, reloc_rva, len(reloc_bytes))
    struct.pack_into("<I", output, meta["e_lfanew"] + 0x58, compute_pe_checksum(output))

    args.out.parent.mkdir(parents=True, exist_ok=True)
    args.out.write_bytes(output)
    report = {
        "mode": "standalone-generated-whole-kernel",
        "input_image": str(args.input_ntoskrnl),
        "output_image": str(args.out),
        "entry_symbol": args.entry_symbol,
        "entry_rva": entry_rva,
        "generated_functions": len(manifest["functions"]),
        "linked_functions": len(linked.func_bounds),
        "redirected_exports": redirected_exports,
        "relocated_function_pointers": relocated_function_pointers,
        "generated_unwind_functions": len(runtime_functions),
        "cfg_targets": cfg_count,
        "host_fallback": False,
        "original_executable_bytes_are_guards": True,
    }
    args.out.with_suffix(args.out.suffix + ".standalone.json").write_text(
        json.dumps(report, indent=2) + "\n", encoding="utf-8"
    )
    print(json.dumps(report, indent=2))
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
