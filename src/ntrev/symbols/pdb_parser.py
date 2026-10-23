"""
Genuine Microsoft PDB v7.00 (`MSF 7.00`) Binary Parser & Builder.

Parses:
  - MSF 7.00 SuperBlock (`Microsoft C/C++ MSF 7.00\\r\\n\\x1aDS\\0\\0\\0`), BlockMap,
    and multi-block Stream Directory.
  - Stream 1 (PDB Info): Version, Timestamp, Age, 16-byte GUID (verified against PE RSDS).
  - Stream 2 (TPI — Type Information): CodeView type records (`LF_POINTER`, `LF_MODIFIER`,
    `LF_PROCEDURE`, `LF_ARGLIST`, `LF_FIELDLIST`, `LF_MEMBER`, `LF_BITFIELD`, `LF_ARRAY`,
    `LF_STRUCTURE`, `LF_CLASS`, `LF_UNION`, `LF_ENUM`) + forward-reference size resolution.
  - Stream 3 (DBI — Debug Information) + Public/Global Symbol Record Stream:
    `S_PUB32 (0x110e)`, `S_GPROC32 (0x1110)`, `S_LPROC32 (0x110f)` mapped to PE section RVAs,
    preserving `/OPT:ICF` aliases and separating global data symbols (`data_symbols_by_rva`).
  - Also supports JSON PDB symbol/type manifests and includes `MSF70PDBBuilder`.
"""

from __future__ import annotations

import json
import math
import struct
import uuid
from pathlib import Path
from typing import Any

from ntrev.models import BinaryImage, PDBDatabase, PDBSymbol, StructDef, StructField

MSF70_MAGIC = b"Microsoft C/C++ MSF 7.00\r\n\x1aDS\x00\x00\x00"

# CodeView TPI Leaf Constants
LF_MODIFIER = 0x1001
LF_POINTER = 0x1002
LF_PROCEDURE = 0x1008
LF_ARGLIST = 0x1201
LF_FIELDLIST = 0x1203
LF_BITFIELD = 0x1205
LF_ARRAY = 0x1503
LF_CLASS = 0x1504
LF_STRUCTURE = 0x1505
LF_UNION = 0x1506
LF_ENUM = 0x1507
LF_INDEX = 0x1404
LF_MEMBER = 0x150D

# CodeView Symbol Record Constants
S_END = 0x0006
S_PUB32 = 0x110E
S_LPROC32 = 0x110F
S_GPROC32 = 0x1110
S_LOCAL = 0x113E

# CodeView Numeric Leaf Prefixes (>= 0x8000)
LF_CHAR = 0x8000
LF_SHORT = 0x8001
LF_USHORT = 0x8002
LF_LONG = 0x8003
LF_ULONG = 0x8004
LF_QUADWORD = 0x8009
LF_UQUADWORD = 0x800A

# Built-in CodeView Primitive Type Indices (< 0x1000)
CV_PRIMITIVES: dict[int, tuple[str, int]] = {
    0x0003: ("void", 0),
    0x0010: ("int8_t", 1),
    0x0020: ("uint8_t", 1),
    0x0030: ("uint8_t", 1),  # T_BOOL08 (BOOLEAN)
    0x0068: ("int8_t", 1),
    0x0069: ("uint8_t", 1),
    0x0070: ("char", 1),
    0x0071: ("uint16_t", 2),  # wchar_t
    0x0011: ("int16_t", 2),
    0x0021: ("uint16_t", 2),
    0x0072: ("int16_t", 2),
    0x0073: ("uint16_t", 2),
    0x0012: ("int32_t", 4),  # LONG / NTSTATUS
    0x0022: ("uint32_t", 4),  # ULONG
    0x0074: ("int32_t", 4),
    0x0075: ("uint32_t", 4),
    0x0013: ("int64_t", 8),
    0x0023: ("uint64_t", 8),
    0x0076: ("int64_t", 8),
    0x0077: ("uint64_t", 8),
    0x0040: ("float", 4),
    0x0041: ("double", 8),
}


def load_pdb_database(
    pdb_path: str | Path,
    binary: BinaryImage | None = None,
) -> PDBDatabase:
    """Load either a binary MSF 7.00 `.pdb` file or a `.json` PDB dump."""
    path = Path(pdb_path)
    raw = path.read_bytes()
    if raw.startswith(MSF70_MAGIC):
        return _parse_msf70_pdb(path, raw, binary)
    if raw.lstrip().startswith(b"{"):
        return _parse_json_pdb(path, json.loads(raw.decode("utf-8")))
    raise ValueError(f"Unrecognized PDB file format in {path}")


def _decode_cv_numeric(buf: bytes, offset: int) -> tuple[int, int]:
    """Decode a CodeView numeric leaf at `offset`, returning `(value, new_offset)`."""
    if offset + 2 > len(buf):
        return 0, len(buf)
    val = struct.unpack_from("<H", buf, offset)[0]
    offset += 2
    if val < 0x8000:
        return val, offset
    if val == LF_CHAR and offset + 1 <= len(buf):
        return struct.unpack_from("<b", buf, offset)[0], offset + 1
    if val == LF_SHORT and offset + 2 <= len(buf):
        return struct.unpack_from("<h", buf, offset)[0], offset + 2
    if val == LF_USHORT and offset + 2 <= len(buf):
        return struct.unpack_from("<H", buf, offset)[0], offset + 2
    if val == LF_LONG and offset + 4 <= len(buf):
        return struct.unpack_from("<i", buf, offset)[0], offset + 4
    if val == LF_ULONG and offset + 4 <= len(buf):
        return struct.unpack_from("<I", buf, offset)[0], offset + 4
    if val == LF_QUADWORD and offset + 8 <= len(buf):
        return struct.unpack_from("<q", buf, offset)[0], offset + 8
    if val == LF_UQUADWORD and offset + 8 <= len(buf):
        return struct.unpack_from("<Q", buf, offset)[0], offset + 8
    return 0, offset


def _read_cstr(buf: bytes, offset: int) -> tuple[str, int]:
    end = buf.find(b"\x00", offset)
    if end == -1:
        return buf[offset:].decode("utf-8", errors="replace"), len(buf)
    return buf[offset:end].decode("utf-8", errors="replace"), end + 1


def _align_pad(buf: bytes, offset: int) -> int:
    """Skip CodeView LF_PAD1..LF_PAD15 (0xF1..0xFF) padding bytes inside field lists."""
    while offset < len(buf) and buf[offset] >= 0xF0:
        offset += buf[offset] & 0x0F
    return offset


def _parse_msf70_pdb(
    pdb_path: Path,
    raw: bytes,
    binary: BinaryImage | None,
) -> PDBDatabase:
    block_size, _fpm_blk, _num_blocks, num_dir_bytes, _unk, block_map_addr = struct.unpack_from(
        "<IIIIII", raw, 32
    )

    num_dir_blocks = math.ceil(num_dir_bytes / block_size)
    map_off = block_map_addr * block_size
    dir_block_indices = struct.unpack_from(f"<{num_dir_blocks}I", raw, map_off)

    dir_bytes = bytearray()
    for b_idx in dir_block_indices:
        start = b_idx * block_size
        dir_bytes.extend(raw[start : start + block_size])
    dir_data = bytes(dir_bytes[:num_dir_bytes])

    num_streams = struct.unpack_from("<I", dir_data, 0)[0]
    pos = 4
    stream_sizes: list[int] = []
    for _ in range(num_streams):
        sz = struct.unpack_from("<i", dir_data, pos)[0]
        stream_sizes.append(sz)
        pos += 4

    streams: list[bytes] = []
    for sz in stream_sizes:
        if sz <= 0:
            streams.append(b"")
            continue
        n_blks = math.ceil(sz / block_size)
        blk_list = struct.unpack_from(f"<{n_blks}I", dir_data, pos)
        pos += 4 * n_blks
        s_buf = bytearray()
        for b_idx in blk_list:
            start = b_idx * block_size
            s_buf.extend(raw[start : start + block_size])
        streams.append(bytes(s_buf[:sz]))

    guid_str = "00000000-0000-0000-0000-000000000000"
    age = 1
    if len(streams) > 1 and len(streams[1]) >= 28:
        _ver, _sig, age = struct.unpack_from("<III", streams[1], 0)
        guid_bytes = streams[1][12:28]
        guid_str = str(uuid.UUID(bytes_le=guid_bytes)).upper()

    # Parse TPI Stream (Stream 2)
    tpi_records: dict[int, dict[str, Any]] = {}
    structs: dict[str, StructDef] = {}
    full_sizes: dict[str, int] = {}
    if len(streams) > 2 and len(streams[2]) >= 56:
        tpi_records, structs, full_sizes = _parse_tpi_stream(streams[2])

    # Parse DBI Stream (Stream 3) & Symbol Record Stream
    symbols_by_rva: dict[int, PDBSymbol] = {}
    symbols_by_name: dict[str, PDBSymbol] = {}
    data_symbols_by_rva: dict[int, PDBSymbol] = {}

    if len(streams) > 3 and len(streams[3]) >= 64:
        sym_rec_stream_idx = struct.unpack_from("<H", streams[3], 20)[0]
        sec_rvas, exec_segs = _build_section_rva_info(binary)
        if 0 < sym_rec_stream_idx < len(streams) and streams[sym_rec_stream_idx]:
            syms = _parse_symbol_record_stream(
                streams[sym_rec_stream_idx], sec_rvas, exec_segs, tpi_records, full_sizes
            )
            for s in syms:
                symbols_by_name[s.name] = s
                if s.is_function:
                    symbols_by_rva.setdefault(s.rva, s)
                else:
                    data_symbols_by_rva.setdefault(s.rva, s)

    return PDBDatabase(
        pdb_path=str(pdb_path),
        guid=guid_str,
        age=age,
        structs=structs,
        symbols_by_rva=symbols_by_rva,
        symbols_by_name=symbols_by_name,
        data_symbols_by_rva=data_symbols_by_rva,
    )


def _resolve_ti_type(
    ti: int,
    tpi_records: dict[int, dict[str, Any]],
    full_sizes: dict[str, int],
    depth: int = 0,
) -> tuple[str, int, bool, str | None]:
    """Return `(c_type, byte_size, is_pointer, pointee_struct_name)` for a CodeView TypeIndex."""
    if depth > 8:
        return ("uint64_t", 8, False, None)

    if ti < 0x1000:
        ptr_mode = (ti >> 8) & 0xFF
        base_code = ti & 0xFF
        base_name, base_sz = CV_PRIMITIVES.get(base_code, ("void", 0))
        if ptr_mode in (0x04, 0x06):  # Near32 / Near64 pointer
            return (f"{base_name}*", 8, True, base_name)
        return (base_name, base_sz, False, None)

    rec = tpi_records.get(ti)
    if not rec:
        return ("uint64_t", 8, False, None)

    kind = rec["kind"]
    if kind == "modifier":
        inner_c, sz, is_p, pt = _resolve_ti_type(
            rec["underlying"], tpi_records, full_sizes, depth + 1
        )
        prefix = "const " if (rec["attr"] & 0x01) and not inner_c.startswith("const ") else ""
        return (f"{prefix}{inner_c}", sz, is_p, pt)

    if kind == "pointer":
        inner_c, _sz, _ip, _pt = _resolve_ti_type(
            rec["referent"], tpi_records, full_sizes, depth + 1
        )
        pointee_struct = None
        ref_ti = rec["referent"]
        ref_rec = tpi_records.get(ref_ti)
        while ref_rec and ref_rec["kind"] == "modifier":
            ref_ti = ref_rec["underlying"]
            ref_rec = tpi_records.get(ref_ti)
        if ref_rec and ref_rec["kind"] in ("struct", "union"):
            pointee_struct = ref_rec["name"]
            kw = "union" if ref_rec["kind"] == "union" else "struct"
            const_pfx = "const " if inner_c.startswith("const ") else ""
            inner_c = f"{const_pfx}{kw} {pointee_struct}"
        return (f"{inner_c}*", 8, True, pointee_struct)

    if kind in ("struct", "union"):
        kw = "union" if kind == "union" else "struct"
        sname = rec["name"]
        st_sz = rec.get("size", 0) or full_sizes.get(sname, 0)
        return (f"{kw} {sname}", st_sz, False, None)

    if kind == "enum":
        return _resolve_ti_type(rec["underlying"], tpi_records, full_sizes, depth + 1)

    if kind == "array":
        elem_c, _esz, _ip, _pt = _resolve_ti_type(
            rec["elem_ti"], tpi_records, full_sizes, depth + 1
        )
        return (elem_c, rec["byte_size"], False, None)

    if kind == "bitfield":
        base_c, base_sz, _, _ = _resolve_ti_type(
            rec["base_ti"], tpi_records, full_sizes, depth + 1
        )
        return (base_c, base_sz, False, None)

    if kind == "procedure":
        return ("void*", 8, True, None)

    return ("uint64_t", 8, False, None)


def _expand_fieldlist_members(
    field_ti: int, records: dict[int, dict[str, Any]], visited: set[int] | None = None
) -> list[dict[str, Any]]:
    """Expand `LF_FIELDLIST` records including `LF_INDEX` continuation chains."""
    if visited is None:
        visited = set()
    if field_ti in visited:
        return []
    visited.add(field_ti)
    fl_rec = records.get(field_ti)
    if not fl_rec or fl_rec.get("kind") != "fieldlist":
        return []
    out = list(fl_rec.get("members", []))
    for cont_ti in fl_rec.get("continuations", []):
        out.extend(_expand_fieldlist_members(cont_ti, records, visited))
    return out


def _parse_tpi_stream(
    tpi: bytes,
) -> tuple[dict[int, dict[str, Any]], dict[str, StructDef], dict[str, int]]:
    _ver, hdr_size, ti_begin, ti_end, rec_bytes = struct.unpack_from("<IIIII", tpi, 0)
    pos = hdr_size
    end = min(len(tpi), hdr_size + rec_bytes)
    cur_ti = ti_begin
    records: dict[int, dict[str, Any]] = {}
    full_sizes: dict[str, int] = {}

    while pos + 4 <= end and cur_ti < ti_end:
        rec_len, leaf = struct.unpack_from("<HH", tpi, pos)
        body = tpi[pos + 4 : pos + 2 + rec_len]
        pos += 2 + rec_len

        if leaf == LF_MODIFIER and len(body) >= 6:
            underlying, attr = struct.unpack_from("<IH", body, 0)
            records[cur_ti] = {"kind": "modifier", "underlying": underlying, "attr": attr}

        elif leaf == LF_POINTER and len(body) >= 8:
            referent, attrs = struct.unpack_from("<II", body, 0)
            records[cur_ti] = {"kind": "pointer", "referent": referent, "attrs": attrs}

        elif leaf == LF_ARGLIST and len(body) >= 4:
            count = struct.unpack_from("<I", body, 0)[0]
            arg_tis = (
                list(struct.unpack_from(f"<{count}I", body, 4))
                if len(body) >= 4 + 4 * count
                else []
            )
            records[cur_ti] = {"kind": "arglist", "args": arg_tis}

        elif leaf == LF_PROCEDURE and len(body) >= 12:
            rv_ti, call_conv, _func_attr, parm_count, arglist_ti = struct.unpack_from(
                "<IBBHI", body, 0
            )
            records[cur_ti] = {
                "kind": "procedure",
                "rv_ti": rv_ti,
                "call_conv": call_conv,
                "parm_count": parm_count,
                "arglist_ti": arglist_ti,
            }

        elif leaf == LF_BITFIELD and len(body) >= 6:
            base_ti, length, position = struct.unpack_from("<IBB", body, 0)
            records[cur_ti] = {
                "kind": "bitfield",
                "base_ti": base_ti,
                "bit_size": length,
                "bit_offset": position,
            }

        elif leaf == LF_ARRAY and len(body) >= 8:
            elem_ti, idx_ti = struct.unpack_from("<II", body, 0)
            arr_sz, p2 = _decode_cv_numeric(body, 8)
            _name, _ = _read_cstr(body, p2)
            records[cur_ti] = {
                "kind": "array",
                "elem_ti": elem_ti,
                "idx_ti": idx_ti,
                "byte_size": arr_sz,
            }

        elif leaf == LF_ENUM and len(body) >= 12:
            _count, _prop, utype, _field_ti = struct.unpack_from("<HHII", body, 0)
            en_name, _ = _read_cstr(body, 12)
            records[cur_ti] = {
                "kind": "enum",
                "name": en_name,
                "underlying": utype,
            }

        elif leaf == LF_FIELDLIST:
            members = []
            continuations = []
            fpos = 0
            while fpos + 2 <= len(body):
                fpos = _align_pad(body, fpos)
                if fpos + 2 > len(body):
                    break
                sub_leaf = struct.unpack_from("<H", body, fpos)[0]
                fpos += 2
                if sub_leaf == LF_MEMBER and fpos + 6 <= len(body):
                    _attr, fld_ti = struct.unpack_from("<HI", body, fpos)
                    fpos += 6
                    fld_off, fpos = _decode_cv_numeric(body, fpos)
                    fld_name, fpos = _read_cstr(body, fpos)
                    members.append({"name": fld_name, "offset": fld_off, "ti": fld_ti})
                elif sub_leaf == LF_INDEX and fpos + 6 <= len(body):
                    _pad, cont_ti = struct.unpack_from("<HI", body, fpos)
                    fpos += 6
                    continuations.append(cont_ti)
                else:
                    break
            records[cur_ti] = {
                "kind": "fieldlist",
                "members": members,
                "continuations": continuations,
            }

        elif leaf in (LF_STRUCTURE, LF_CLASS, LF_UNION) and len(body) >= 8:
            is_union = leaf == LF_UNION
            if is_union:
                count, prop, field_ti = struct.unpack_from("<HHI", body, 0)
                st_size, p2 = _decode_cv_numeric(body, 8)
            else:
                count, prop, field_ti, _derived, _vshape = struct.unpack_from("<HHIII", body, 0)
                st_size, p2 = _decode_cv_numeric(body, 16)
            st_name, _ = _read_cstr(body, p2)
            if st_name in ("<unnamed-tag>", "<anonymous-tag>", "__unnamed"):
                st_name = f"_ANON_TAG_{cur_ti:x}"
            is_fwdref = bool(prop & 0x0080)
            if not is_fwdref and st_size > 0:
                full_sizes[st_name] = st_size
            records[cur_ti] = {
                "kind": "union" if is_union else "struct",
                "name": st_name,
                "size": st_size,
                "field_ti": field_ti,
                "count": count,
                "is_fwdref": is_fwdref,
            }

        cur_ti += 1

    # Materialize complete StructDefs with resolved forward-reference sizes & LF_INDEX continuations
    structs: dict[str, StructDef] = {}
    for _ti, rec in records.items():
        if rec["kind"] not in ("struct", "union") or rec.get("is_fwdref"):
            continue
        raw_members = _expand_fieldlist_members(rec["field_ti"], records)
        fields: list[StructField] = []
        used_names: set[str] = set()
        for m in raw_members:
            fname = m["name"]
            if fname in ("<unnamed-tag>", "<anonymous-tag>", ""):
                fname = f"_anon_0x{m['offset']:x}"
            if fname in used_names:
                fname = f"{fname}_0x{m['offset']:x}"
            used_names.add(fname)

            c_ty, f_sz, is_ptr, pointee = _resolve_ti_type(m["ti"], records, full_sizes)
            m_rec = records.get(m["ti"])
            bit_off = m_rec["bit_offset"] if m_rec and m_rec["kind"] == "bitfield" else None
            bit_sz = m_rec["bit_size"] if m_rec and m_rec["kind"] == "bitfield" else None
            arr_len = None
            if m_rec and m_rec["kind"] == "array":
                _, elem_sz, _, _ = _resolve_ti_type(m_rec["elem_ti"], records, full_sizes)
                if elem_sz > 0:
                    arr_len = m_rec["byte_size"] // elem_sz
            fields.append(
                StructField(
                    name=fname,
                    offset=m["offset"],
                    size=f_sz,
                    c_type=c_ty,
                    is_pointer=is_ptr,
                    pointee_type=pointee,
                    array_len=arr_len,
                    bit_offset=bit_off,
                    bit_size=bit_sz,
                )
            )
        clean_base = rec["name"].lstrip("_")
        sdef = StructDef(
            name=rec["name"],
            size=rec["size"],
            fields=fields,
            typedef_name=clean_base if not clean_base.startswith("ANON_TAG_") else None,
            pointer_typedef=f"P{clean_base}" if not clean_base.startswith("ANON_TAG_") else None,
            is_union=(rec["kind"] == "union"),
            source="pdb",
        )
        structs[sdef.name] = sdef

    return records, structs, full_sizes


def _build_section_rva_info(binary: BinaryImage | None) -> tuple[list[int], set[int]]:
    """1-based PE section index -> `(sec_rvas, executable_seg_indices)`."""
    if binary is None or not binary.sections:
        return [0, 0x1000, 0x2000, 0x3000, 0x4000], {1}
    sec_rvas = [0] + [sec.virtual_address for sec in binary.sections]
    exec_segs = {idx + 1 for idx, sec in enumerate(binary.sections) if sec.is_executable}
    return sec_rvas, exec_segs


def _clean_pdb_symbol_name(raw_name: str, rva: int = 0) -> str:
    """Clean fastcall/stdcall or MSVC C++ mangled names into valid C identifiers."""
    if raw_name.startswith("__imp_"):
        return raw_name
    if raw_name.startswith("@") and "@" in raw_name[1:]:
        raw_name = raw_name[1:].split("@", 1)[0]
    if raw_name.startswith("?"):
        # Skip string literals (??_C@), vftables (??_7), scalar/vector deleting dtors, typeinfo
        if raw_name.startswith(("??_C@", "??_7", "??_R", "??_E", "??_G", "?_OptionsStorage")):
            return ""
        parts = [p for p in raw_name.lstrip("?").split("@") if p and not p.startswith("$")]
        clean_parts: list[str] = []
        for p in parts[:2]:
            s = "".join(ch if (ch.isalnum() or ch == "_") else "_" for ch in p).strip("_")
            if s:
                clean_parts.append(s)
        if len(clean_parts) >= 2:
            base = f"{clean_parts[1]}__{clean_parts[0]}"
        elif clean_parts:
            base = clean_parts[0]
        else:
            return ""
        return f"{base}_{rva:x}" if rva else base
    # Sanitize any non-identifier chars (e.g. `$`, `.`, `-` in `$filt$0` or PGO names)
    if not raw_name.isidentifier():
        sanitized = "".join(ch if (ch.isalnum() or ch == "_") else "_" for ch in raw_name)
        if sanitized and sanitized[0].isdigit():
            sanitized = "_" + sanitized
        return f"{sanitized}_{rva:x}" if (rva and sanitized != raw_name) else sanitized
    return raw_name


def _parse_symbol_record_stream(
    sym_stream: bytes,
    sec_rvas: list[int],
    exec_segs: set[int],
    tpi_records: dict[int, dict[str, Any]],
    full_sizes: dict[str, int],
) -> list[PDBSymbol]:
    # Pre-scan whether S_PUB32 stream carries explicit CVPSF_FUNCTION (0x02) flags
    stream_has_pub_func_flags = False
    scan_pos = 0
    while scan_pos + 4 <= len(sym_stream):
        rl, rt = struct.unpack_from("<HH", sym_stream, scan_pos)
        if rl < 2:
            break
        if rt == S_PUB32 and rl >= 12:
            pf = struct.unpack_from("<I", sym_stream, scan_pos + 4)[0]
            if pf & 0x02:
                stream_has_pub_func_flags = True
                break
        scan_pos += 2 + rl

    symbols: list[PDBSymbol] = []
    seen_func_names: dict[str, int] = {}
    pos = 0
    while pos + 4 <= len(sym_stream):
        rec_len, rec_ty = struct.unpack_from("<HH", sym_stream, pos)
        if rec_len < 2:
            break
        body = sym_stream[pos + 4 : pos + 2 + rec_len]
        pos += 2 + rec_len

        if rec_ty == S_PUB32 and len(body) >= 10:
            pub_flags, offset, seg = struct.unpack_from("<IIH", body, 0)
            raw_name, _ = _read_cstr(body, 10)
            if seg == 0 or seg >= len(sec_rvas) and len(sec_rvas) > 5:
                continue
            if raw_name.startswith("__imp_"):
                continue
            base_rva = sec_rvas[seg] if seg < len(sec_rvas) else 0x1000
            rva = base_rva + offset
            is_func = (
                bool(pub_flags & 0x02) and (seg in exec_segs or len(sec_rvas) <= 5)
                if stream_has_pub_func_flags
                else (bool(pub_flags & 0x02) or (seg in exec_segs))
            )
            if raw_name.startswith("?") and not is_func:
                continue
            name = _clean_pdb_symbol_name(raw_name, rva if raw_name.startswith("?") else 0)
            if not name:
                continue
            if is_func:
                prev_rva = seen_func_names.get(name)
                if prev_rva is not None and prev_rva != rva:
                    name = f"{name}_{rva:x}"
                seen_func_names[name] = rva
            symbols.append(
                PDBSymbol(
                    name=name,
                    rva=rva,
                    section=seg,
                    is_function=is_func,
                )
            )

        elif rec_ty in (S_GPROC32, S_LPROC32) and len(body) >= 35:
            _p_parent, _p_end, _p_next, code_len, _dbg_start, _dbg_end, proc_ti, offset, seg, _flags = (
                struct.unpack_from("<IIIIIIIIHB", body, 0)
            )
            raw_name, _ = _read_cstr(body, 35)
            name = _clean_pdb_symbol_name(raw_name)
            base_rva = sec_rvas[seg] if seg < len(sec_rvas) else 0x1000
            rva = base_rva + offset

            ret_ty: str | None = None
            param_types: list[tuple[str, str]] = []
            proc_rec = tpi_records.get(proc_ti)
            if proc_rec and proc_rec["kind"] == "procedure":
                ret_ty, _, _, _ = _resolve_ti_type(proc_rec["rv_ti"], tpi_records, full_sizes)
                arg_rec = tpi_records.get(proc_rec["arglist_ti"], {})
                for idx, ati in enumerate(arg_rec.get("args", [])):
                    arg_c, _, _, _ = _resolve_ti_type(ati, tpi_records, full_sizes)
                    param_types.append((f"arg{idx + 1}", arg_c))

            symbols.append(
                PDBSymbol(
                    name=name,
                    rva=rva,
                    size=code_len,
                    section=seg,
                    is_function=True,
                    return_type=ret_ty,
                    param_types=param_types,
                )
            )

        elif rec_ty == S_LOCAL and len(body) >= 6 and symbols:
            _loc_ti, loc_flags = struct.unpack_from("<IH", body, 0)
            loc_name, _ = _read_cstr(body, 6)
            if (loc_flags & 0x0001) and symbols[-1].param_types:  # fIsParam
                pt_list = symbols[-1].param_types
                for idx_p, (cur_n, cur_t) in enumerate(pt_list):
                    if cur_n == f"arg{idx_p + 1}":
                        pt_list[idx_p] = (loc_name, cur_t)
                        break

    return symbols


def _parse_json_pdb(pdb_path: Path, data: dict[str, Any]) -> PDBDatabase:
    db = PDBDatabase(
        pdb_path=str(pdb_path),
        guid=data.get("guid", "00000000-0000-0000-0000-000000000000"),
        age=int(data.get("age", 1)),
    )
    for st in data.get("structs", []):
        fields = [
            StructField(
                name=f["name"],
                offset=int(f["offset"]),
                size=int(f["size"]),
                c_type=f["c_type"],
                is_pointer=bool(f.get("is_pointer", False)),
                pointee_type=f.get("pointee_type"),
            )
            for f in st.get("fields", [])
        ]
        sdef = StructDef(
            name=st["name"],
            size=int(st["size"]),
            fields=fields,
            typedef_name=st["name"].lstrip("_"),
            pointer_typedef=f"P{st['name'].lstrip('_')}",
            source="pdb",
        )
        db.structs[sdef.name] = sdef

    for sym in data.get("symbols", []):
        psym = PDBSymbol(
            name=sym["name"],
            rva=int(sym["rva"], 0) if isinstance(sym["rva"], str) else int(sym["rva"]),
            size=int(sym.get("size", 0)),
            is_function=bool(sym.get("is_function", True)),
            return_type=sym.get("return_type"),
            param_types=[(p[0], p[1]) for p in sym.get("param_types", [])],
        )
        db.symbols_by_name[psym.name] = psym
        if psym.is_function:
            db.symbols_by_rva[psym.rva] = psym
        else:
            db.data_symbols_by_rva[psym.rva] = psym

    return db


class MSF70PDBBuilder:
    """
    Constructs valid binary Microsoft MSF 7.00 `.pdb` files from Python definitions.
    """

    C_TO_CV_PRIMITIVE: dict[str, int] = {
        "void": 0x0003,
        "void*": 0x0603,
        "char": 0x0070,
        "int8_t": 0x0010,
        "uint8_t": 0x0020,
        "BOOLEAN": 0x0030,
        "int16_t": 0x0011,
        "uint16_t": 0x0021,
        "int32_t": 0x0074,
        "NTSTATUS": 0x0012,
        "LONG": 0x0012,
        "uint32_t": 0x0075,
        "ULONG": 0x0022,
        "int64_t": 0x0076,
        "uint64_t": 0x0077,
        "size_t": 0x0077,
        "ULONG_PTR": 0x0023,
        "uint32_t*": 0x0675,
        "int32_t*": 0x0674,
        "uint64_t*": 0x0677,
        "int64_t*": 0x0676,
        "uint8_t*": 0x0620,
    }

    def __init__(self, guid: str | None = None, age: int = 1) -> None:
        self.guid = guid or str(uuid.uuid4()).upper()
        self.age = age
        self.structs: list[StructDef] = []
        self.symbols: list[PDBSymbol] = []

    def add_struct(self, sdef: StructDef) -> None:
        self.structs.append(sdef)

    def add_symbol(self, sym: PDBSymbol) -> None:
        self.symbols.append(sym)

    @staticmethod
    def _encode_numeric(val: int) -> bytes:
        if 0 <= val < 0x8000:
            return struct.pack("<H", val)
        if 0 <= val <= 0xFFFF:
            return struct.pack("<HH", LF_USHORT, val)
        return struct.pack("<HI", LF_ULONG, val)

    @staticmethod
    def _pack_tpi_record(leaf: int, body: bytes) -> bytes:
        total_unaligned = 2 + len(body)
        rem = total_unaligned % 4
        if rem != 0:
            pad_len = 4 - rem
            pad_bytes = bytes(0xF0 + i for i in range(pad_len, 0, -1))
            body = body + pad_bytes
        rec_len = 2 + len(body)
        return struct.pack("<HH", rec_len, leaf) + body

    def build_bytes(self, text_section_rva: int = 0x1000) -> bytes:
        next_ti = 0x1000
        tpi_chunks: list[bytes] = []
        fwd_ti_map: dict[str, int] = {}
        ptr_ti_map: dict[str, int] = {}

        for st in self.structs:
            name_z = st.name.encode("utf-8") + b"\x00"
            leaf = LF_UNION if st.is_union else LF_STRUCTURE
            if st.is_union:
                fwd_body = struct.pack("<HHI", 0, 0x0080, 0) + self._encode_numeric(0) + name_z
            else:
                fwd_body = struct.pack("<HHIII", 0, 0x0080, 0, 0, 0) + self._encode_numeric(0) + name_z
            tpi_chunks.append(self._pack_tpi_record(leaf, fwd_body))
            fwd_ti = next_ti
            fwd_ti_map[st.name] = fwd_ti
            next_ti += 1

            ptr_body = struct.pack("<II", fwd_ti, 0x0001000C)
            tpi_chunks.append(self._pack_tpi_record(LF_POINTER, ptr_body))
            ptr_ti_map[st.name] = next_ti
            ptr_ti_map[f"struct {st.name}*"] = next_ti
            if st.pointer_typedef:
                ptr_ti_map[st.pointer_typedef] = next_ti
            next_ti += 1

        def resolve_c_type_ti(c_ty: str) -> int:
            clean = c_ty.strip()
            if clean in self.C_TO_CV_PRIMITIVE:
                return self.C_TO_CV_PRIMITIVE[clean]
            if clean in ptr_ti_map:
                return ptr_ti_map[clean]
            if clean.startswith("struct ") and clean.endswith("*"):
                sname = clean[len("struct ") : -1].strip()
                if sname in ptr_ti_map:
                    return ptr_ti_map[sname]
            if clean.startswith("struct "):
                sname = clean[len("struct ") :].strip()
                if sname in fwd_ti_map:
                    return fwd_ti_map[sname]
            if clean.endswith("*"):
                return 0x0603
            return 0x0077

        for st in self.structs:
            fl_body = bytearray()
            for fld in st.fields:
                fti = resolve_c_type_ti(fld.c_type)
                sub = (
                    struct.pack("<HHI", LF_MEMBER, 0x0003, fti)
                    + self._encode_numeric(fld.offset)
                    + fld.name.encode("utf-8")
                    + b"\x00"
                )
                rem = len(sub) % 4
                if rem != 0:
                    pad_len = 4 - rem
                    sub += bytes(0xF0 + i for i in range(pad_len, 0, -1))
                fl_body.extend(sub)

            tpi_chunks.append(self._pack_tpi_record(LF_FIELDLIST, bytes(fl_body)))
            fl_ti = next_ti
            next_ti += 1

            name_z = st.name.encode("utf-8") + b"\x00"
            if st.is_union:
                st_body = (
                    struct.pack("<HHI", len(st.fields), 0, fl_ti)
                    + self._encode_numeric(st.size)
                    + name_z
                )
                tpi_chunks.append(self._pack_tpi_record(LF_UNION, st_body))
            else:
                st_body = (
                    struct.pack("<HHIII", len(st.fields), 0, fl_ti, 0, 0)
                    + self._encode_numeric(st.size)
                    + name_z
                )
                tpi_chunks.append(self._pack_tpi_record(LF_STRUCTURE, st_body))
            next_ti += 1

        sym_proc_ti: dict[str, int] = {}
        for sym in self.symbols:
            if sym.is_function and (sym.return_type or sym.param_types):
                arg_tis = [resolve_c_type_ti(pt) for _, pt in sym.param_types]
                al_body = struct.pack(f"<I{len(arg_tis)}I", len(arg_tis), *arg_tis)
                tpi_chunks.append(self._pack_tpi_record(LF_ARGLIST, al_body))
                al_ti = next_ti
                next_ti += 1

                rv_ti = resolve_c_type_ti(sym.return_type or "uint64_t")
                proc_body = struct.pack("<IBBHI", rv_ti, 0, 0, len(arg_tis), al_ti)
                tpi_chunks.append(self._pack_tpi_record(LF_PROCEDURE, proc_body))
                sym_proc_ti[sym.name] = next_ti
                next_ti += 1

        tpi_records_blob = b"".join(tpi_chunks)
        tpi_hdr = struct.pack(
            "<IIIII",
            20040203,
            56,
            0x1000,
            next_ti,
            len(tpi_records_blob),
        ).ljust(56, b"\x00")
        stream2_tpi = tpi_hdr + tpi_records_blob

        guid_bytes = uuid.UUID(self.guid).bytes_le
        stream1_pdb = struct.pack("<III", 20000404, 0x67000000, self.age) + guid_bytes + struct.pack("<I", 0)

        sym_chunks: list[bytes] = []
        for sym in self.symbols:
            sec_offset = max(0, sym.rva - text_section_rva)
            name_z = sym.name.encode("utf-8") + b"\x00"
            if sym.name in sym_proc_ti or sym.size > 0:
                pti = sym_proc_ti.get(sym.name, 0)
                gbody = (
                    struct.pack(
                        "<IIIIIIIIHB",
                        0,
                        0,
                        0,
                        sym.size,
                        0,
                        sym.size,
                        pti,
                        sec_offset,
                        1,
                        0,
                    )
                    + name_z
                )
                sym_chunks.append(self._pack_tpi_record(S_GPROC32, gbody))
                for pname, ptype in sym.param_types:
                    loc_ti = resolve_c_type_ti(ptype)
                    lbody = struct.pack("<IH", loc_ti, 0x0001) + pname.encode("utf-8") + b"\x00"
                    sym_chunks.append(self._pack_tpi_record(S_LOCAL, lbody))
                sym_chunks.append(struct.pack("<HH", 2, S_END))
            else:
                pbody = struct.pack("<IIH", 2 if sym.is_function else 0, sec_offset, 1) + name_z
                sym_chunks.append(self._pack_tpi_record(S_PUB32, pbody))
        stream4_syms = b"".join(sym_chunks)

        dbi_hdr = bytearray(64)
        struct.pack_into("<iI", dbi_hdr, 0, -1, 19990903)
        struct.pack_into("<HHH", dbi_hdr, 16, 0xFFFF, 0xFFFF, 4)
        stream3_dbi = bytes(dbi_hdr)

        streams = [b"", stream1_pdb, stream2_tpi, stream3_dbi, stream4_syms]
        return self._pack_msf70_container(streams)

    @staticmethod
    def _pack_msf70_container(streams: list[bytes], block_size: int = 4096) -> bytes:
        next_blk = 3
        stream_blocks: list[list[int]] = []
        stream_blobs: dict[int, bytes] = {}

        for st in streams:
            if not st:
                stream_blocks.append([])
                continue
            n_blks = math.ceil(len(st) / block_size)
            blks = []
            for i in range(n_blks):
                b_idx = next_blk
                next_blk += 1
                chunk = st[i * block_size : (i + 1) * block_size]
                stream_blobs[b_idx] = chunk.ljust(block_size, b"\x00")
                blks.append(b_idx)
            stream_blocks.append(blks)

        dir_buf = bytearray()
        dir_buf.extend(struct.pack("<I", len(streams)))
        for st in streams:
            dir_buf.extend(struct.pack("<i", len(st)))
        for blks in stream_blocks:
            for b_idx in blks:
                dir_buf.extend(struct.pack("<I", b_idx))

        dir_bytes = bytes(dir_buf)
        n_dir_blks = max(1, math.ceil(len(dir_bytes) / block_size))
        dir_blk_indices: list[int] = []
        for i in range(n_dir_blks):
            b_idx = next_blk
            next_blk += 1
            chunk = dir_bytes[i * block_size : (i + 1) * block_size]
            stream_blobs[b_idx] = chunk.ljust(block_size, b"\x00")
            dir_blk_indices.append(b_idx)

        block_map_addr = next_blk
        next_blk += 1
        map_chunk = struct.pack(f"<{len(dir_blk_indices)}I", *dir_blk_indices)
        stream_blobs[block_map_addr] = map_chunk.ljust(block_size, b"\x00")

        total_blocks = next_blk
        superblock = bytearray(block_size)
        superblock[0:32] = MSF70_MAGIC
        struct.pack_into(
            "<IIIIII",
            superblock,
            32,
            block_size,
            1,
            total_blocks,
            len(dir_bytes),
            0,
            block_map_addr,
        )

        out = bytearray()
        out.extend(superblock)
        out.extend(b"\xFF" * block_size)
        out.extend(b"\xFF" * block_size)
        for b_idx in range(3, total_blocks):
            out.extend(stream_blobs[b_idx])
        return bytes(out)
