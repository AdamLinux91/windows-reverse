"""
Windows PE32+ (ntoskrnl.exe / .sys / .dll) and ELF64 loader.

Extracts:
  - Executable & data sections (.text, PAGE, INIT, KVASCODE, .rdata, .data)
  - .pdata x64 SEH RUNTIME_FUNCTION + UNWIND_INFO exception unwind boundaries
    (including MSVC PGO UNW_FLAG_CHAININFO chained cold chunks)
  - Export & Import tables (preserving /OPT:ICF multi-symbol export aliases)
  - IMAGE_DEBUG_DIRECTORY CodeView RSDS (PDB70 GUID, Age, PDB path, MS Symbol Server URL)
"""

from __future__ import annotations

import struct
import uuid
from pathlib import Path

import pefile

from ntrev.models import (
    BinaryImage,
    CodeViewRSDS,
    PESectionInfo,
    RuntimeFunctionEntry,
)

IMAGE_SCN_MEM_EXECUTE = 0x20000000
IMAGE_SCN_MEM_WRITE = 0x80000000
IMAGE_DEBUG_TYPE_CODEVIEW = 2
UNW_FLAG_CHAININFO = 0x04


def load_binary_image(path: str | Path) -> BinaryImage:
    """Load a PE32+ (ntoskrnl.exe, .sys, .dll, .exe) or ELF64 x86-64 binary."""
    file_path = Path(path)
    raw = file_path.read_bytes()
    if raw[:2] == b"MZ":
        return _load_pe_image(file_path, raw)
    if raw[:4] == b"\x7fELF":
        return _load_elf64_image(file_path, raw)
    raise ValueError(f"Unsupported binary format in {file_path} (expected PE32+ MZ or ELF64)")


def _load_pe_image(file_path: Path, raw: bytes) -> BinaryImage:
    pe = pefile.PE(data=raw, fast_load=False)
    image_base = int(pe.OPTIONAL_HEADER.ImageBase)
    entry_rva = int(pe.OPTIONAL_HEADER.AddressOfEntryPoint)
    machine_val = int(pe.FILE_HEADER.Machine)
    machine_str = "AMD64" if machine_val == 0x8664 else f"0x{machine_val:04x}"

    sections: list[PESectionInfo] = []
    for sec in pe.sections:
        name = sec.Name.rstrip(b"\x00").decode("ascii", errors="replace")
        rva = int(sec.VirtualAddress)
        vsize = int(sec.Misc_VirtualSize)
        data = sec.get_data()
        chars = int(sec.Characteristics)
        is_exec = bool(chars & IMAGE_SCN_MEM_EXECUTE)
        is_write = bool(chars & IMAGE_SCN_MEM_WRITE)
        sections.append(
            PESectionInfo(
                name=name,
                virtual_address=rva,
                virtual_size=max(vsize, len(data)),
                raw_data=data,
                is_executable=is_exec,
                is_writable=is_write,
            )
        )

    exports: dict[int, str] = {}
    export_by_name: dict[str, int] = {}
    if hasattr(pe, "DIRECTORY_ENTRY_EXPORT"):
        for sym in pe.DIRECTORY_ENTRY_EXPORT.symbols:
            if sym.address is not None and not getattr(sym, "forwarder", None):
                sym_name = (
                    sym.name.decode("ascii", errors="replace")
                    if sym.name
                    else f"Ordinal_{sym.ordinal}"
                )
                rva_val = int(sym.address)
                exports.setdefault(rva_val, sym_name)
                export_by_name[sym_name] = rva_val

    imports: dict[int, str] = {}
    if hasattr(pe, "DIRECTORY_ENTRY_IMPORT"):
        for entry in pe.DIRECTORY_ENTRY_IMPORT:
            dll_name = entry.dll.decode("ascii", errors="replace") if entry.dll else "unknown.dll"
            for imp in entry.imports:
                if imp.address is not None:
                    fn_name = (
                        imp.name.decode("ascii", errors="replace")
                        if imp.name
                        else f"ord_{imp.ordinal}"
                    )
                    imports[int(imp.address)] = f"{dll_name}!{fn_name}"

    pdata_functions = _extract_pdata_functions(pe)
    rsds = _extract_rsds_codeview(pe)

    return BinaryImage(
        path=str(file_path),
        format="PE32+",
        image_base=image_base,
        entry_point_rva=entry_rva,
        machine=machine_str,
        sections=sections,
        exports=exports,
        export_by_name=export_by_name,
        imports=imports,
        pdata_functions=pdata_functions,
        rsds=rsds,
    )


def _extract_pdata_functions(pe: pefile.PE) -> list[RuntimeFunctionEntry]:
    """Parse IMAGE_DIRECTORY_ENTRY_EXCEPTION (index 3) RUNTIME_FUNCTION table."""
    entries: list[RuntimeFunctionEntry] = []
    if len(pe.OPTIONAL_HEADER.DATA_DIRECTORY) <= 3:
        return entries

    exc_dir = pe.OPTIONAL_HEADER.DATA_DIRECTORY[3]
    if not exc_dir.VirtualAddress or not exc_dir.Size:
        return entries

    pdata_bytes = pe.get_data(exc_dir.VirtualAddress, exc_dir.Size)
    count = len(pdata_bytes) // 12
    for i in range(count):
        begin_rva, end_rva, unwind_rva = struct.unpack_from("<III", pdata_bytes, i * 12)
        if begin_rva == 0 and end_rva == 0:
            continue
        if end_rva <= begin_rva:
            continue
        prolog_size = 0
        unwind_flags = 0
        chained_parent_rva: int | None = None
        if unwind_rva:
            try:
                # Bit 0 of unwind_rva indicates an indirectRUNTIME_FUNCTION pointer
                if unwind_rva & 1:
                    parent_rf = pe.get_data(unwind_rva & ~1, 12)
                    if len(parent_rf) == 12:
                        chained_parent_rva = struct.unpack_from("<I", parent_rf, 0)[0]
                else:
                    hdr = pe.get_data(unwind_rva, 4)
                    if len(hdr) == 4:
                        ver_flags, prolog_size, code_cnt = hdr[0], hdr[1], hdr[2]
                        unwind_flags = (ver_flags >> 3) & 0x1F
                        if unwind_flags & UNW_FLAG_CHAININFO:
                            # Chained RUNTIME_FUNCTION sits immediately after aligned UNWIND_CODE array
                            aligned_codes = code_cnt + (code_cnt & 1)
                            chain_off = 4 + aligned_codes * 2
                            chain_blob = pe.get_data(unwind_rva + chain_off, 12)
                            if len(chain_blob) == 12:
                                chained_parent_rva = struct.unpack_from("<I", chain_blob, 0)[0]
            except Exception:
                pass
        entries.append(
            RuntimeFunctionEntry(
                begin_rva=begin_rva,
                end_rva=end_rva,
                unwind_info_rva=unwind_rva,
                prolog_size=prolog_size,
                unwind_flags=unwind_flags,
                chained_parent_rva=chained_parent_rva,
            )
        )
    return entries


def _extract_rsds_codeview(pe: pefile.PE) -> CodeViewRSDS | None:
    """Extract RSDS PDB70 debug signature from IMAGE_DIRECTORY_ENTRY_DEBUG."""
    if not hasattr(pe, "DIRECTORY_ENTRY_DEBUG"):
        return None
    for dbg in pe.DIRECTORY_ENTRY_DEBUG:
        if dbg.struct.Type == IMAGE_DEBUG_TYPE_CODEVIEW:
            rva = dbg.struct.AddressOfRawData
            size = dbg.struct.SizeOfData
            if rva and size >= 24:
                blob = pe.get_data(rva, size)
            elif dbg.struct.PointerToRawData and size >= 24:
                raw = pe.__data__
                off = dbg.struct.PointerToRawData
                blob = bytes(raw[off : off + size])
            else:
                continue
            if blob[:4] == b"RSDS":
                guid_bytes = blob[4:20]
                age = struct.unpack_from("<I", blob, 20)[0]
                pdb_name = blob[24:].split(b"\x00", 1)[0].decode("utf-8", errors="replace")
                guid_obj = uuid.UUID(bytes_le=guid_bytes)
                return CodeViewRSDS(
                    guid=str(guid_obj).upper(),
                    age=age,
                    pdb_filename=pdb_name,
                )
    return None


def _load_elf64_image(file_path: Path, raw: bytes) -> BinaryImage:
    """Minimal ELF64 x86-64 loader for direct object/kernel testing."""
    e_type, e_machine = struct.unpack_from("<HH", raw, 16)
    e_entry, _e_phoff, e_shoff = struct.unpack_from("<QQQ", raw, 24)
    _e_flags, _e_ehsize, _e_phentsize, _e_phnum, e_shentsize, e_shnum, e_shstrndx = (
        struct.unpack_from("<IHHHHHH", raw, 48)
    )
    if e_machine != 62:  # EM_X86_64
        raise ValueError(f"Unsupported ELF machine {e_machine} (expected EM_X86_64=62)")

    shdrs = []
    for i in range(e_shnum):
        off = e_shoff + i * e_shentsize
        shdrs.append(struct.unpack_from("<IIQQQQIIQQ", raw, off))

    shstr_off = shdrs[e_shstrndx][4]
    shstr_size = shdrs[e_shstrndx][5]
    shstrtab = raw[shstr_off : shstr_off + shstr_size]

    def read_cstr(buf: bytes, idx: int) -> str:
        end = buf.find(b"\x00", idx)
        return buf[idx:end].decode("ascii", errors="replace") if end != -1 else ""

    image_base = 0x140000000 if e_type == 1 else 0  # Relocatable .o gets synthetic base
    sections: list[PESectionInfo] = []
    exports: dict[int, str] = {}
    export_by_name: dict[str, int] = {}
    sec_rva_map: dict[int, int] = {}
    next_rva = 0x1000

    for idx, sh in enumerate(shdrs):
        sh_name, sh_type, sh_flags, sh_addr, sh_offset, sh_size, sh_link, *_ = sh
        if sh_type == 0 or sh_size == 0:
            continue
        name = read_cstr(shstrtab, sh_name)
        is_exec = bool(sh_flags & 0x4)  # SHF_EXECINSTR
        is_alloc = bool(sh_flags & 0x2)  # SHF_ALLOC
        if not is_alloc:
            continue
        rva = int(sh_addr - image_base) if sh_addr >= image_base and e_type != 1 else next_rva
        next_rva = ((rva + int(sh_size) + 0xFFF) // 0x1000) * 0x1000
        sec_rva_map[idx] = rva
        data = raw[sh_offset : sh_offset + sh_size] if sh_type != 8 else b"\x00" * sh_size
        sections.append(
            PESectionInfo(
                name=name,
                virtual_address=rva,
                virtual_size=int(sh_size),
                raw_data=data,
                is_executable=is_exec,
                is_writable=bool(sh_flags & 0x1),
            )
        )

    # Extract ELF symbol table functions
    for sh in shdrs:
        sh_name, sh_type, _sh_flags, _sh_addr, sh_offset, sh_size, sh_link, _info, _align, entsize = sh
        if sh_type in (2, 11) and entsize >= 24:  # SHT_SYMTAB / SHT_DYNSYM
            str_sh = shdrs[sh_link]
            strtab = raw[str_sh[4] : str_sh[4] + str_sh[5]]
            for pos in range(sh_offset, sh_offset + sh_size, entsize):
                st_name, st_info, _st_other, st_shndx, st_value, _st_size = struct.unpack_from(
                    "<IBBHQQ", raw, pos
                )
                sym_type = st_info & 0xF
                sym_name = read_cstr(strtab, st_name)
                if not sym_name or st_shndx not in sec_rva_map:
                    continue
                if sym_type in (2, 0) and not sym_name.startswith("."):
                    rva = sec_rva_map[st_shndx] + (int(st_value) if e_type == 1 else 0)
                    if e_type != 1:
                        rva = int(st_value - image_base)
                    exports.setdefault(rva, sym_name)
                    export_by_name[sym_name] = rva

    return BinaryImage(
        path=str(file_path),
        format="ELF64",
        image_base=image_base,
        entry_point_rva=int(e_entry - image_base) if e_entry >= image_base else 0x1000,
        machine="AMD64",
        sections=sections,
        exports=exports,
        export_by_name=export_by_name,
    )
