#!/usr/bin/env python3
"""
ELF64-to-PE32+ Ring-0 Kernel Linker Primitives (`pe_elf_linker.py`)

Pure-Python (zero external compiler/linker dependencies beyond GNU `gcc -mabi=ms`)
engine for linking compiled `decompiled_nt_kernel/` ELF64 `.o` objects into:
  1. Standalone `IMAGE_SUBSYSTEM_NATIVE` PE32+ `ntoskrnl.exe` (`link_ntoskrnl_pe.py`)
  2. Binary-Grafted Boot-to-Desktop `ntoskrnl_dropin.exe` (`build_windows_dropin_ntoskrnl.py`)

Implements:
  - ELF64 relocatable object (`ET_REL`, `EM_X86_64`) section, symbol (`STB_GLOBAL`/`STB_WEAK`/
    `SHN_COMMON`), and `.rela.*` (`R_X86_64_64`, `R_X86_64_PC32`, `R_X86_64_PLT32`,
    `R_X86_64_32`, `R_X86_64_32S`) linking
  - Microsoft x64 `.pdata` (`RUNTIME_FUNCTION` + `UNWIND_INFO` v1) prologue synthesis
  - Windows Control Flow Guard (`GFIDS` 5-byte `<IB` stride=1 sorted table) construction
  - `IMAGE_BASE_RELOCATION` (`IMAGE_REL_BASED_DIR64`) parsing & packing for KASLR
  - Bit-exact `CheckSumMappedFile` PE header checksum computation
"""
from __future__ import annotations

import struct
from dataclasses import dataclass, field
from pathlib import Path

# ELF64 constants
SHT_PROGBITS = 1
SHT_SYMTAB = 2
SHT_STRTAB = 3
SHT_RELA = 4
SHT_NOBITS = 8

SHF_WRITE = 0x1
SHF_ALLOC = 0x2
SHF_EXECINSTR = 0x4

STB_LOCAL = 0
STB_GLOBAL = 1
STB_WEAK = 2

STT_FUNC = 2

SHN_UNDEF = 0
SHN_ABS = 0xFFF1
SHN_COMMON = 0xFFF2

R_X86_64_NONE = 0
R_X86_64_64 = 1
R_X86_64_PC32 = 2
R_X86_64_PLT32 = 4
R_X86_64_32 = 10
R_X86_64_32S = 11

# Windows PE / x64 SEH constants
IMAGE_REL_BASED_ABSOLUTE = 0
IMAGE_REL_BASED_DIR64 = 10

UWOP_PUSH_NONVOL = 0
UWOP_ALLOC_LARGE = 1
UWOP_ALLOC_SMALL = 2


def align_up(val: int, align: int) -> int:
    if align <= 1:
        return val
    return (val + align - 1) & ~(align - 1)


@dataclass
class ElfSection:
    index: int
    name: str
    sh_type: int
    sh_flags: int
    sh_size: int
    sh_addralign: int
    data: bytearray
    kind: str = ""  # "code" | "data" | ""
    merged_offset: int = 0


@dataclass
class ElfSymbol:
    name: str
    bind: int
    sym_type: int
    shndx: int
    value: int
    size: int


@dataclass
class ElfRela:
    target_sec_idx: int
    r_offset: int
    sym_idx: int
    r_type: int
    r_addend: int


@dataclass
class ParsedObject:
    path: Path
    sections: dict[int, ElfSection] = field(default_factory=dict)
    symbols: list[ElfSymbol] = field(default_factory=list)
    relocs: list[ElfRela] = field(default_factory=list)


def parse_elf64_object(path: str | Path) -> ParsedObject:
    raw = Path(path).read_bytes()
    if raw[:4] != b"\x7fELF" or raw[4] != 2 or raw[5] != 1:
        raise ValueError(f"{path}: not a 64-bit little-endian ELF object")

    (
        _e_type,
        e_machine,
        _e_version,
        _e_entry,
        _e_phoff,
        e_shoff,
        _e_flags,
        _e_ehsize,
        _e_phentsize,
        _e_phnum,
        e_shentsize,
        e_shnum,
        e_shstrndx,
    ) = struct.unpack_from("<HHIQQQIHHHHHH", raw, 16)
    if e_machine != 62:
        raise ValueError(f"{path}: expected EM_X86_64 (62), got {e_machine}")

    shdrs: list[tuple[int, int, int, int, int, int, int, int, int, int]] = []
    for i in range(e_shnum):
        off = e_shoff + i * e_shentsize
        shdrs.append(struct.unpack_from("<IIQQQQIIQQ", raw, off))

    shstr_off = shdrs[e_shstrndx][4]
    shstr_sz = shdrs[e_shstrndx][5]
    shstr = raw[shstr_off : shstr_off + shstr_sz]

    def read_cstr(buf: bytes, pos: int) -> str:
        end = buf.find(b"\x00", pos)
        return buf[pos:end].decode("utf-8", errors="replace") if end >= 0 else ""

    obj = ParsedObject(path=Path(path))

    # 1. Load allocatable sections
    for idx, sh in enumerate(shdrs):
        sh_name, sh_type, sh_flags, _addr, sh_offset, sh_size, sh_link, sh_info, sh_align, sh_entsize = sh
        sname = read_cstr(shstr, sh_name)
        if (sh_flags & SHF_ALLOC) and sh_type in (SHT_PROGBITS, SHT_NOBITS):
            if sname.startswith(".note") or sname.startswith(".eh_frame"):
                continue
            data = (
                bytearray(sh_size)
                if sh_type == SHT_NOBITS
                else bytearray(raw[sh_offset : sh_offset + sh_size])
            )
            kind = "code" if (sh_flags & SHF_EXECINSTR) else "data"
            obj.sections[idx] = ElfSection(
                index=idx,
                name=sname,
                sh_type=sh_type,
                sh_flags=sh_flags,
                sh_size=sh_size,
                sh_addralign=max(int(sh_align), 1),
                data=data,
                kind=kind,
            )

    # 2. Load symbol table
    for idx, sh in enumerate(shdrs):
        sh_name, sh_type, _flags, _addr, sh_offset, sh_size, sh_link, _info, _align, sh_entsize = sh
        if sh_type == SHT_SYMTAB:
            str_sh = shdrs[sh_link]
            strtab = raw[str_sh[4] : str_sh[4] + str_sh[5]]
            ent_sz = sh_entsize or 24
            for pos in range(sh_offset, sh_offset + sh_size, ent_sz):
                st_name, st_info, _other, st_shndx, st_value, st_size = struct.unpack_from(
                    "<IBBHQQ", raw, pos
                )
                obj.symbols.append(
                    ElfSymbol(
                        name=read_cstr(strtab, st_name),
                        bind=st_info >> 4,
                        sym_type=st_info & 0xF,
                        shndx=st_shndx,
                        value=st_value,
                        size=st_size,
                    )
                )
            break

    # 3. Load RELA relocations targeting allocatable sections
    for idx, sh in enumerate(shdrs):
        _name, sh_type, _flags, _addr, sh_offset, sh_size, _link, sh_info, _align, sh_entsize = sh
        if sh_type == SHT_RELA and sh_info in obj.sections:
            ent_sz = sh_entsize or 24
            for pos in range(sh_offset, sh_offset + sh_size, ent_sz):
                r_offset, r_info, r_addend = struct.unpack_from("<QQq", raw, pos)
                obj.relocs.append(
                    ElfRela(
                        target_sec_idx=sh_info,
                        r_offset=r_offset,
                        sym_idx=r_info >> 32,
                        r_type=r_info & 0xFFFFFFFF,
                        r_addend=r_addend,
                    )
                )

    return obj


@dataclass
class LinkedKernelBlob:
    code_bytes: bytearray
    data_bytes: bytearray
    code_rva: int
    data_rva: int
    global_sym_rva: dict[str, int]
    func_bounds: dict[str, tuple[int, int]]  # name -> (start_rva, end_rva)
    dir64_reloc_rvas: list[int]


def link_elf_objects(
    obj_paths: list[str | Path],
    code_rva: int,
    data_rva_start: int | None = None,
    external_rva_resolver: dict[str, int] | None = None,
    image_base: int = 0x140000000,
    section_align: int = 0x1000,
) -> LinkedKernelBlob:
    """
    Merge all `.text` and `.rodata`/`.data`/`.bss` sections across `obj_paths`,
    resolve global/weak/common symbols + `external_rva_resolver`, and apply all
    x86-64 relocations relative to `code_rva` and `data_rva`.
    """
    objs = [parse_elf64_object(p) for p in obj_paths]
    ext_map = external_rva_resolver or {}

    code_buf = bytearray()
    data_buf = bytearray()

    # Lay out code sections (16-byte aligned between objects) and data sections
    for obj in objs:
        for sec in obj.sections.values():
            if sec.kind == "code":
                align = max(sec.sh_addralign, 16)
                pad = align_up(len(code_buf), align) - len(code_buf)
                if pad:
                    code_buf.extend(b"\xCC" * pad)
                sec.merged_offset = len(code_buf)
                code_buf.extend(sec.data)
            elif sec.kind == "data":
                align = min(max(sec.sh_addralign, 8), 4096)
                pad = align_up(len(data_buf), align) - len(data_buf)
                if pad:
                    data_buf.extend(b"\x00" * pad)
                sec.merged_offset = len(data_buf)
                data_buf.extend(sec.data)

    # Allocate SHN_COMMON symbols in data_buf
    common_offsets: dict[str, tuple[int, int]] = {}  # name -> (offset_in_data, size)
    for obj in objs:
        for sym in obj.symbols:
            if sym.shndx == SHN_COMMON and sym.name:
                prev = common_offsets.get(sym.name)
                if prev is None or sym.size > prev[1]:
                    align = min(max(int(sym.value), 8), 4096)
                    pad = align_up(len(data_buf), align) - len(data_buf)
                    if pad:
                        data_buf.extend(b"\x00" * pad)
                    common_offsets[sym.name] = (len(data_buf), max(sym.size, 8))
                    data_buf.extend(b"\x00" * max(sym.size, 8))

    if len(code_buf) == 0:
        code_buf.extend(b"\xC3")
    if len(data_buf) == 0:
        data_buf.extend(b"\x00" * 16)

    data_rva = (
        data_rva_start
        if data_rva_start is not None
        else align_up(code_rva + len(code_buf), section_align)
    )

    # Resolve global & weak symbol RVAs (STB_GLOBAL overrides STB_WEAK; STB_WEAK overrides SHN_COMMON)
    sym_rva: dict[str, int] = {}
    sym_strength: dict[str, int] = {}  # 1=common, 2=weak, 3=strong
    func_bounds: dict[str, tuple[int, int]] = {}

    for cname, (coff, _csz) in common_offsets.items():
        sym_rva[cname] = data_rva + coff
        sym_strength[cname] = 1

    for obj in objs:
        for sym in obj.symbols:
            if sym.bind not in (STB_GLOBAL, STB_WEAK) or not sym.name:
                continue
            if sym.shndx == SHN_UNDEF or sym.shndx not in obj.sections:
                continue
            sec = obj.sections[sym.shndx]
            base_rva = code_rva if sec.kind == "code" else data_rva
            rva = base_rva + sec.merged_offset + sym.value
            strength = 3 if sym.bind == STB_GLOBAL else 2
            if strength >= sym_strength.get(sym.name, 0):
                # Prefer first strong definition if multiple exist, override weak
                if strength > sym_strength.get(sym.name, 0):
                    sym_rva[sym.name] = rva
                    sym_strength[sym.name] = strength
                    if sec.kind == "code":
                        fsz = sym.size if sym.size > 0 else 16
                        func_bounds[sym.name] = (rva, rva + fsz)

    # Apply relocations across all objects
    dir64_relocs: list[int] = []
    unresolved_syms: list[str] = []

    for obj in objs:
        for rel in obj.relocs:
            if rel.r_type == R_X86_64_NONE:
                continue
            tsec = obj.sections[rel.target_sec_idx]
            target_buf = code_buf if tsec.kind == "code" else data_buf
            sec_base_rva = code_rva if tsec.kind == "code" else data_rva
            buf_pos = tsec.merged_offset + rel.r_offset
            site_rva = sec_base_rva + buf_pos

            sym = obj.symbols[rel.sym_idx]
            if sym.bind == STB_LOCAL:
                if sym.shndx in obj.sections:
                    ssec = obj.sections[sym.shndx]
                    sbase = code_rva if ssec.kind == "code" else data_rva
                    target_sym_rva = sbase + ssec.merged_offset + sym.value
                elif sym.shndx == SHN_ABS:
                    target_sym_rva = sym.value
                else:
                    raise ValueError(f"{obj.path}: unresolved local symbol in {tsec.name}")
            else:
                if sym.name in sym_rva:
                    target_sym_rva = sym_rva[sym.name]
                elif sym.name in ext_map:
                    target_sym_rva = ext_map[sym.name]
                else:
                    unresolved_syms.append(sym.name)
                    continue

            if rel.r_type in (R_X86_64_PC32, R_X86_64_PLT32):
                disp = (target_sym_rva + rel.r_addend) - site_rva
                if not (-0x80000000 <= disp <= 0x7FFFFFFF):
                    raise OverflowError(
                        f"PC32 overflow for {sym.name}: disp=0x{disp:x} at RVA 0x{site_rva:x}"
                    )
                struct.pack_into("<i", target_buf, buf_pos, disp)
            elif rel.r_type == R_X86_64_64:
                abs_va = (image_base + target_sym_rva + rel.r_addend) & 0xFFFFFFFFFFFFFFFF
                struct.pack_into("<Q", target_buf, buf_pos, abs_va)
                dir64_relocs.append(site_rva)
            elif rel.r_type == R_X86_64_32:
                val32 = (image_base + target_sym_rva + rel.r_addend) & 0xFFFFFFFF
                struct.pack_into("<I", target_buf, buf_pos, val32)
            elif rel.r_type == R_X86_64_32S:
                val32s = target_sym_rva + rel.r_addend
                struct.pack_into("<i", target_buf, buf_pos, val32s)
            else:
                raise ValueError(f"Unsupported ELF64 relocation type {rel.r_type} for {sym.name}")

    if unresolved_syms:
        uniq = sorted(set(unresolved_syms))
        raise RuntimeError(f"Unresolved kernel symbols ({len(uniq)}): {uniq[:20]}")

    return LinkedKernelBlob(
        code_bytes=code_buf,
        data_bytes=data_buf,
        code_rva=code_rva,
        data_rva=data_rva,
        global_sym_rva=sym_rva,
        func_bounds=func_bounds,
        dir64_reloc_rvas=sorted(set(dir64_relocs)),
    )


def synthesize_win64_unwind_tables(
    code_bytes: bytes | bytearray,
    code_rva: int,
    func_bounds: dict[str, tuple[int, int]],
    raw_asm_names: set[str] | None = None,
    unwind_info_base_rva: int = 0,
) -> tuple[bytes, list[tuple[int, int, int]]]:
    """
    Decode x86-64 prologues of compiled `-mabi=ms` functions and emit:
      1. Packed 4-byte-aligned `UNWIND_INFO` v1 structures (starting at `unwind_info_base_rva`)
      2. Sorted list of `(BeginRva, EndRva, UnwindInfoRva)` `RUNTIME_FUNCTION` tuples
    Leaf functions (zero stack/non-volatile modifications) require no `.pdata` entry per Win64 ABI.
    """
    skip = raw_asm_names or set()
    unwind_blob = bytearray()
    runtime_fns: list[tuple[int, int, int]] = []

    for fname, (start_rva, end_rva) in sorted(func_bounds.items(), key=lambda kv: kv[1][0]):
        if fname in skip or end_rva <= start_rva:
            continue
        fn_off = start_rva - code_rva
        fn_len = min(end_rva - start_rva, 32)
        if fn_off < 0 or fn_off + fn_len > len(code_bytes):
            continue

        buf = code_bytes[fn_off : fn_off + fn_len]
        pos = 0
        # Each op is a list of 16-bit USHORTs in prologue execution order; reversed at the end
        ops_in_order: list[list[int]] = []
        prolog_end = 0

        while pos < len(buf):
            b0 = buf[pos]
            # 1-byte `push rax..rdi` (0x50..0x57)
            if 0x50 <= b0 <= 0x57:
                reg = b0 - 0x50
                pos += 1
                prolog_end = pos
                code_ushort = pos | (UWOP_PUSH_NONVOL << 8) | (reg << 12)
                ops_in_order.append([code_ushort])
                continue
            # 2-byte `push r8..r15` (0x41 0x50..0x57)
            if b0 == 0x41 and pos + 1 < len(buf) and 0x50 <= buf[pos + 1] <= 0x57:
                reg = 8 + (buf[pos + 1] - 0x50)
                pos += 2
                prolog_end = pos
                code_ushort = pos | (UWOP_PUSH_NONVOL << 8) | (reg << 12)
                ops_in_order.append([code_ushort])
                continue
            # 4-byte `sub rsp, imm8` (0x48 0x83 0xEC imm8)
            if pos + 4 <= len(buf) and buf[pos : pos + 3] == b"\x48\x83\xEC":
                imm8 = buf[pos + 3]
                if 8 <= imm8 <= 128 and (imm8 % 8) == 0:
                    pos += 4
                    prolog_end = pos
                    op_info = (imm8 - 8) // 8
                    code_ushort = pos | (UWOP_ALLOC_SMALL << 8) | (op_info << 12)
                    ops_in_order.append([code_ushort])
                break
            # 7-byte `sub rsp, imm32` (0x48 0x81 0xEC imm32)
            if pos + 7 <= len(buf) and buf[pos : pos + 3] == b"\x48\x81\xEC":
                imm32 = struct.unpack_from("<I", buf, pos + 3)[0]
                if imm32 >= 8 and (imm32 % 8) == 0 and imm32 <= 0x7FFF8:
                    pos += 7
                    prolog_end = pos
                    code_ushort = pos | (UWOP_ALLOC_LARGE << 8) | (0 << 12)
                    ops_in_order.append([code_ushort, imm32 // 8])
                break
            # Allow `mov [rsp+disp8], reg` home-space saves before/between pushes without aborting
            if (
                pos + 5 <= len(buf)
                and buf[pos] in (0x48, 0x4C)
                and buf[pos + 1] == 0x89
                and (buf[pos + 2] & 0xC7) == 0x44
                and buf[pos + 3] == 0x24
            ):
                pos += 5
                continue
            break

        if not ops_in_order:
            continue

        flat_ushorts: list[int] = []
        for grp in reversed(ops_in_order):
            flat_ushorts.extend(grp)

        pad = align_up(len(unwind_blob), 4) - len(unwind_blob)
        if pad:
            unwind_blob.extend(b"\x00" * pad)
        ui_rva = unwind_info_base_rva + len(unwind_blob)

        # UNWIND_INFO header: Version=1 (bits 0..2), Flags=0, SizeOfProlog, CountOfCodes, FrameReg=0
        unwind_blob.extend(struct.pack("<BBBB", 0x01, prolog_end, len(flat_ushorts), 0x00))
        for u16 in flat_ushorts:
            unwind_blob.extend(struct.pack("<H", u16))
        if len(flat_ushorts) & 1:
            unwind_blob.extend(b"\x00\x00")

        runtime_fns.append((start_rva, end_rva, ui_rva))

    return bytes(unwind_blob), runtime_fns


def parse_pe_relocs(reloc_bytes: bytes) -> list[int]:
    """Parse existing `IMAGE_BASE_RELOCATION` blocks into sorted `DIR64` RVA list."""
    out: list[int] = []
    pos = 0
    n = len(reloc_bytes)
    while pos + 8 <= n:
        page_rva, blk_sz = struct.unpack_from("<II", reloc_bytes, pos)
        if blk_sz < 8 or pos + blk_sz > n:
            break
        if page_rva == 0 and blk_sz == 8:
            pos += 8
            continue
        for epos in range(pos + 8, pos + blk_sz, 2):
            entry = struct.unpack_from("<H", reloc_bytes, epos)[0]
            rtype = entry >> 12
            roff = entry & 0xFFF
            if rtype == IMAGE_REL_BASED_DIR64:
                out.append(page_rva + roff)
        pos += blk_sz
    return sorted(set(out))


def build_pe_relocs(dir64_rvas: list[int]) -> bytes:
    """Pack sorted `DIR64` RVAs into 4 KiB `IMAGE_BASE_RELOCATION` blocks."""
    if not dir64_rvas:
        # Emit a single dummy page block if empty so PE loaders expecting non-zero .reloc succeed
        return struct.pack("<IIHH", 0x1000, 12, 0, 0)

    by_page: dict[int, list[int]] = {}
    for rva in sorted(set(dir64_rvas)):
        by_page.setdefault(rva & ~0xFFF, []).append(rva & 0xFFF)

    blob = bytearray()
    for page_rva in sorted(by_page.keys()):
        offs = by_page[page_rva]
        entries = [(IMAGE_REL_BASED_DIR64 << 12) | off for off in offs]
        if len(entries) & 1:
            entries.append(IMAGE_REL_BASED_ABSOLUTE)
        blk_sz = 8 + 2 * len(entries)
        blob.extend(struct.pack("<II", page_rva, blk_sz))
        for ent in entries:
            blob.extend(struct.pack("<H", ent))
    return bytes(blob)


def compute_pe_checksum(pe_bytes: bytes | bytearray) -> int:
    """
    Exact Microsoft `CheckSumMappedFile` algorithm:
    16-bit ones'-complement folding sum (skipping `OptionalHeader.CheckSum` at `e_lfanew + 0x58`)
    plus total file byte length.
    """
    e_lfanew = struct.unpack_from("<I", pe_bytes, 0x3C)[0]
    csum_off = e_lfanew + 0x58

    buf = bytearray(pe_bytes)
    buf[csum_off : csum_off + 4] = b"\x00\x00\x00\x00"
    if len(buf) & 1:
        buf.append(0)

    # Sum 16-bit words in chunks for speed in pure Python
    mv = memoryview(buf).cast("H")
    total = sum(mv)
    while total >> 16:
        total = (total & 0xFFFF) + (total >> 16)
    return (total + len(pe_bytes)) & 0xFFFFFFFF
