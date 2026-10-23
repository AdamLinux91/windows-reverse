"""
Synthesizes a genuine Windows x64 (`ms_abi`) PE32+ Kernel Binary (`ntoskrnl_mini.exe`)
and matching binary MSF 7.00 Symbol Database (`ntoskrnl_mini.pdb`).

Compiles real C & ring-0 x86-64 assembly routines with `gcc -O2/-O3 -mabi=ms` and wraps
their machine code into a spec-compliant PE32+ (`Subsystem = IMAGE_SUBSYSTEM_NATIVE`,
`ImageBase = 0x140000000`) complete with:
  - `.text` executable kernel code section
  - `.rdata` `IMAGE_DEBUG_DIRECTORY` + `RSDS` CodeView PDB70 signature
  - `.pdata` `RUNTIME_FUNCTION` + `UNWIND_INFO` x64 SEH exception table
  - Binary `MSF 7.00` `.pdb` containing `_EPROCESS` / `_KTHREAD` TPI structs & `S_GPROC32` symbols
"""

from __future__ import annotations

import struct
import subprocess
import tempfile
import uuid
from pathlib import Path

from ntrev.models import PDBSymbol, StructDef, StructField
from ntrev.symbols.pdb_parser import MSF70PDBBuilder

KERNEL_C_SOURCE = r"""
#include <stdint.h>

struct __attribute__((packed)) _EPROCESS_MINI {
    uint8_t  _pad0[0x28];
    uint64_t DirectoryTableBase;       /* +0x028 */
    uint8_t  _pad1[0x2B0];
    uint64_t UniqueProcessId;          /* +0x2E0 */
    uint8_t  ActiveProcessLinks[0x10]; /* +0x2E8 */
    uint8_t  _pad2[0x08];
    uint64_t QuotaUsagePool;           /* +0x300 */
    uint64_t QuotaPeakPool;            /* +0x308 */
    uint32_t Flags;                    /* +0x310 */
    int32_t  ExitStatus;               /* +0x314 */
};

struct __attribute__((packed)) _KTHREAD_MINI {
    uint8_t  _pad0[0x98];
    struct _EPROCESS_MINI* ApcStateProcess; /* +0x98 */
};

/* 1. PDB struct field access + conditional diamond + NTSTATUS return */
__attribute__((ms_abi, noinline))
int32_t PsChargeProcessPoolQuota(struct _EPROCESS_MINI* Process, uint64_t Amount) {
    uint64_t next_usage = Process->QuotaUsagePool + Amount;
    if (next_usage > Process->QuotaPeakPool) {
        return (int32_t)0xC000009A; /* STATUS_INSUFFICIENT_RESOURCES */
    }
    Process->QuotaUsagePool = next_usage;
    return 0; /* STATUS_SUCCESS */
}

/* 2. Chained NT pointer traversal: Thread->ApcStateProcess->UniqueProcessId */
__attribute__((ms_abi, noinline))
uint64_t PsGetThreadProcessId(struct _KTHREAD_MINI* Thread) {
    struct _EPROCESS_MINI* proc = Thread->ApcStateProcess;
    return proc->UniqueProcessId;
}

/* 3. Magic-multiplier constant division strength-reduction (`Ticks / 10000ULL`) */
__attribute__((ms_abi, noinline))
uint64_t RtlConvertTicksToMilliseconds(uint64_t Ticks) {
    return Ticks / 10000ULL;
}
"""

UNROLLED_LINEAR_C_SOURCE = r"""
#include <stdint.h>

/* 4. Linearly unrolled 6-element strided reduction (`-O3 -funroll-loops`) */
__attribute__((ms_abi, noinline, optimize("O3", "unroll-loops")))
uint64_t KeSumProcessorTicksUnrolled(const uint32_t* Ticks) {
    uint64_t acc = 0;
    #pragma GCC unroll 6
    for (int i = 0; i < 6; ++i) {
        acc += Ticks[i];
    }
    return acc;
}
"""

MULTIBRANCH_ASM_SOURCE = r"""
.intel_syntax noprefix
.text

/* 5. Loop unrolled into 5 distinct branches by compiler optimization */
.globl KiDispatchPriorityBatch
.type KiDispatchPriorityBatch, @function
KiDispatchPriorityBatch:
    xor eax, eax
    test edx, edx
    je .L_done
    cmp edx, 1
    je .L_b1
    cmp edx, 2
    je .L_b2
    cmp edx, 3
    je .L_b3
    cmp edx, 4
    je .L_b4
.L_b5:
    mov eax, DWORD PTR [rcx]
    mov r8d, DWORD PTR [rcx + 4]
    add rax, r8
    mov r8d, DWORD PTR [rcx + 8]
    add rax, r8
    mov r8d, DWORD PTR [rcx + 12]
    add rax, r8
    mov r8d, DWORD PTR [rcx + 16]
    add rax, r8
    ret
.L_b4:
    mov eax, DWORD PTR [rcx]
    mov r8d, DWORD PTR [rcx + 4]
    add rax, r8
    mov r8d, DWORD PTR [rcx + 8]
    add rax, r8
    mov r8d, DWORD PTR [rcx + 12]
    add rax, r8
    ret
.L_b3:
    mov eax, DWORD PTR [rcx]
    mov r8d, DWORD PTR [rcx + 4]
    add rax, r8
    mov r8d, DWORD PTR [rcx + 8]
    add rax, r8
    ret
.L_b2:
    mov eax, DWORD PTR [rcx]
    mov r8d, DWORD PTR [rcx + 4]
    add rax, r8
    ret
.L_b1:
    mov eax, DWORD PTR [rcx]
.L_done:
    ret
.size KiDispatchPriorityBatch, .-KiDispatchPriorityBatch

/* 6. Non-C Ring-0 NT Kernel Syscall Entry Stub (preserved as raw inline ASM) */
.globl KiSystemCall64Entry
.type KiSystemCall64Entry, @function
KiSystemCall64Entry:
    swapgs
    mov QWORD PTR gs:[0x10], rsp
    mov rsp, QWORD PTR gs:[0x1a8]
    mov ecx, 0xc0000082
    rdmsr
    swapgs
    sysretq
.size KiSystemCall64Entry, .-KiSystemCall64Entry
"""


def _extract_elf_functions(obj_path: Path) -> list[tuple[str, bytes]]:
    """Extract `(func_name, machine_bytes)` in symbol order from an ELF64 `.o` file."""
    raw = obj_path.read_bytes()
    e_shoff = struct.unpack_from("<Q", raw, 40)[0]
    e_shentsize, e_shnum, e_shstrndx = struct.unpack_from("<HHH", raw, 58)

    shdrs = [
        struct.unpack_from("<IIQQQQIIQQ", raw, e_shoff + i * e_shentsize)
        for i in range(e_shnum)
    ]
    shstr = raw[shdrs[e_shstrndx][4] : shdrs[e_shstrndx][4] + shdrs[e_shstrndx][5]]

    def cstr(buf: bytes, off: int) -> str:
        end = buf.find(b"\x00", off)
        return buf[off:end].decode("ascii", errors="replace") if end != -1 else ""

    text_idx = -1
    text_bytes = b""
    for idx, sh in enumerate(shdrs):
        if cstr(shstr, sh[0]) == ".text":
            text_idx = idx
            text_bytes = raw[sh[4] : sh[4] + sh[5]]
            break

    funcs: list[tuple[int, str, bytes]] = []
    for sh in shdrs:
        if sh[1] == 2:  # SHT_SYMTAB
            str_sh = shdrs[sh[6]]
            strtab = raw[str_sh[4] : str_sh[4] + str_sh[5]]
            entsize = sh[9]
            for pos in range(sh[4], sh[4] + sh[5], entsize):
                st_name, st_info, _other, st_shndx, st_val, st_size = struct.unpack_from(
                    "<IBBHQQ", raw, pos
                )
                if (st_info & 0xF) == 2 and st_shndx == text_idx and st_size > 0:
                    fname = cstr(strtab, st_name)
                    funcs.append((int(st_val), fname, text_bytes[st_val : st_val + st_size]))

    funcs.sort(key=lambda t: t[0])
    return [(name, code) for _, name, code in funcs]


def _pack_pe32_plus(
    functions: list[tuple[str, bytes, bool]],
    guid_str: str,
    pdb_filename: str,
    image_base: int = 0x140000000,
) -> tuple[bytes, dict[str, tuple[int, int]]]:
    """
    Assemble a valid Windows x64 PE32+ executable image with `.text`, `.rdata` (RSDS),
    and `.pdata` (`RUNTIME_FUNCTION` + `UNWIND_INFO`).
    """
    file_align = 0x200
    sec_align = 0x1000

    def align_up(v: int, a: int) -> int:
        return ((v + a - 1) // a) * a

    # 1. Build `.text` at RVA 0x1000 with 16-byte INT3 function alignment
    text_rva = 0x1000
    text_buf = bytearray()
    fn_layout: dict[str, tuple[int, int]] = {}  # name -> (rva, size)
    pdata_specs: list[tuple[int, int]] = []

    for name, code, has_pdata in functions:
        while len(text_buf) % 16 != 0:
            text_buf.append(0xCC)
        start_rva = text_rva + len(text_buf)
        text_buf.extend(code)
        end_rva = text_rva + len(text_buf)
        fn_layout[name] = (start_rva, len(code))
        if has_pdata:
            pdata_specs.append((start_rva, end_rva))

    text_vsize = len(text_buf)
    text_raw = bytes(text_buf).ljust(align_up(max(text_vsize, 1), file_align), b"\xCC")

    # 2. Build `.rdata` at RVA 0x2000 containing IMAGE_DEBUG_DIRECTORY + RSDS + UNWIND_INFO
    rdata_rva = 0x2000
    rsds_blob = (
        b"RSDS"
        + uuid.UUID(guid_str).bytes_le
        + struct.pack("<I", 1)
        + pdb_filename.encode("utf-8")
        + b"\x00"
    )
    dbg_entry_size = 28
    rsds_rva = rdata_rva + dbg_entry_size
    unwind_rva = align_up(rsds_rva + len(rsds_blob), 4)
    unwind_info_blob = struct.pack("<BBBB", 0x01, 0x00, 0x00, 0x00)  # Version 1, 0 unwind codes

    # File offset computation: Headers = 0x400 bytes
    headers_raw_size = 0x400
    text_file_off = headers_raw_size
    rdata_file_off = text_file_off + len(text_raw)
    rsds_file_off = rdata_file_off + dbg_entry_size

    dbg_dir = struct.pack(
        "<IIHHIIII",
        0,                      # Characteristics
        0x67000000,             # TimeDateStamp
        0,                      # MajorVersion
        0,                      # MinorVersion
        2,                      # Type = IMAGE_DEBUG_TYPE_CODEVIEW
        len(rsds_blob),         # SizeOfData
        rsds_rva,               # AddressOfRawData
        rsds_file_off,          # PointerToRawData
    )
    rdata_buf = bytearray(dbg_dir + rsds_blob)
    while rdata_rva + len(rdata_buf) < unwind_rva:
        rdata_buf.append(0x00)
    rdata_buf.extend(unwind_info_blob)
    rdata_vsize = len(rdata_buf)
    rdata_raw = bytes(rdata_buf).ljust(align_up(rdata_vsize, file_align), b"\x00")

    # 3. Build `.pdata` at RVA 0x3000 (`RUNTIME_FUNCTION` array)
    pdata_rva = 0x3000
    pdata_buf = bytearray()
    for b_rva, e_rva in pdata_specs:
        pdata_buf.extend(struct.pack("<III", b_rva, e_rva, unwind_rva))
    pdata_vsize = len(pdata_buf)
    pdata_raw = bytes(pdata_buf).ljust(align_up(max(pdata_vsize, 12), file_align), b"\x00")
    pdata_file_off = rdata_file_off + len(rdata_raw)

    size_of_image = 0x4000

    # Assemble DOS + PE32+ Headers
    hdr = bytearray(headers_raw_size)
    hdr[0:2] = b"MZ"
    struct.pack_into("<I", hdr, 0x3C, 0x80)  # e_lfanew = 0x80

    pe_off = 0x80
    hdr[pe_off : pe_off + 4] = b"PE\x00\x00"

    # IMAGE_FILE_HEADER (20 bytes)
    num_sections = 3
    opt_hdr_size = 240  # Standard PE32+ Optional Header size
    characteristics = 0x0022  # EXECUTABLE_IMAGE | LARGE_ADDRESS_AWARE
    struct.pack_into(
        "<HHIIIHH",
        hdr,
        pe_off + 4,
        0x8664,          # Machine = AMD64
        num_sections,
        0x67000000,      # TimeDateStamp
        0,
        0,
        opt_hdr_size,
        characteristics,
    )

    # IMAGE_OPTIONAL_HEADER64 (240 bytes)
    opt_off = pe_off + 24
    struct.pack_into(
        "<HBBIIIIIQIIHHHHHHIIIIHHQQQQII",
        hdr,
        opt_off,
        0x020B,             # Magic = PE32+
        14,                 # MajorLinkerVersion
        0,                  # MinorLinkerVersion
        len(text_raw),      # SizeOfCode
        len(rdata_raw) + len(pdata_raw),  # SizeOfInitializedData
        0,                  # SizeOfUninitializedData
        text_rva,           # AddressOfEntryPoint
        text_rva,           # BaseOfCode
        image_base,         # ImageBase
        sec_align,          # SectionAlignment
        file_align,         # FileAlignment
        10, 0,              # OS Version (Windows 10/11 NT 10.0)
        10, 0,              # Image Version
        10, 0,              # Subsystem Version
        0,                  # Win32VersionValue
        size_of_image,      # SizeOfImage
        headers_raw_size,   # SizeOfHeaders
        0,                  # CheckSum
        1,                  # Subsystem = IMAGE_SUBSYSTEM_NATIVE (ntoskrnl.exe!)
        0x8160,             # DllCharacteristics (HIGH_ENTROPY_VA | DYNAMIC_BASE | NX_COMPAT)
        0x100000,           # SizeOfStackReserve
        0x10000,            # SizeOfStackCommit
        0x100000,           # SizeOfHeapReserve
        0x10000,            # SizeOfHeapCommit
        0,                  # LoaderFlags
        16,                 # NumberOfRvaAndSizes
    )

    # Data Directories (16 * 8 bytes at opt_off + 112)
    dd_off = opt_off + 112
    # Index 3: IMAGE_DIRECTORY_ENTRY_EXCEPTION (.pdata)
    struct.pack_into("<II", hdr, dd_off + 3 * 8, pdata_rva, pdata_vsize)
    # Index 6: IMAGE_DIRECTORY_ENTRY_DEBUG
    struct.pack_into("<II", hdr, dd_off + 6 * 8, rdata_rva, dbg_entry_size)

    # Section Headers (40 bytes each at opt_off + 240)
    sec_off = opt_off + opt_hdr_size
    sec_defs = [
        (b".text\x00\x00\x00", text_vsize, text_rva, len(text_raw), text_file_off, 0x60000020),
        (b".rdata\x00\x00", rdata_vsize, rdata_rva, len(rdata_raw), rdata_file_off, 0x40000040),
        (b".pdata\x00\x00", pdata_vsize, pdata_rva, len(pdata_raw), pdata_file_off, 0x40000040),
    ]
    for i, (sname, vsize, rva, raw_sz, f_off, chars) in enumerate(sec_defs):
        struct.pack_into(
            "<8sIIIIIIHHI",
            hdr,
            sec_off + i * 40,
            sname,
            vsize,
            rva,
            raw_sz,
            f_off,
            0,
            0,
            0,
            0,
            chars,
        )

    pe_bytes = bytes(hdr) + text_raw + rdata_raw + pdata_raw
    return pe_bytes, fn_layout


def build_ntoskrnl_mini_fixture(output_dir: str | Path) -> tuple[Path, Path]:
    """Compile C + ring-0 assembly into `ntoskrnl_mini.exe` + `ntoskrnl_mini.pdb`."""
    out = Path(output_dir)
    out.mkdir(parents=True, exist_ok=True)

    with tempfile.TemporaryDirectory(prefix="ntrev_fixture_") as tmp_s:
        tmp = Path(tmp_s)
        c1 = tmp / "kernel_c.c"
        c2 = tmp / "unrolled.c"
        s1 = tmp / "kernel_asm.s"
        o1 = tmp / "kernel_c.o"
        o2 = tmp / "unrolled.o"
        o3 = tmp / "kernel_asm.o"

        c1.write_text(KERNEL_C_SOURCE, encoding="utf-8")
        c2.write_text(UNROLLED_LINEAR_C_SOURCE, encoding="utf-8")
        s1.write_text(MULTIBRANCH_ASM_SOURCE, encoding="utf-8")

        common_cflags = [
            "gcc",
            "-O2",
            "-mabi=ms",
            "-fno-asynchronous-unwind-tables",
            "-fno-stack-protector",
            "-fno-pic",
            "-fcf-protection=none",
            "-c",
        ]
        subprocess.run([*common_cflags, str(c1), "-o", str(o1)], check=True)
        subprocess.run([*common_cflags, "-O3", "-funroll-loops", str(c2), "-o", str(o2)], check=True)
        subprocess.run(["gcc", "-c", str(s1), "-o", str(o3)], check=True)

        funcs_with_pdata: list[tuple[str, bytes, bool]] = []
        for fn_name, code in _extract_elf_functions(o1):
            funcs_with_pdata.append((fn_name, code, True))
        for fn_name, code in _extract_elf_functions(o2):
            funcs_with_pdata.append((fn_name, code, True))
        for fn_name, code in _extract_elf_functions(o3):
            is_c_abi = fn_name != "KiSystemCall64Entry"
            funcs_with_pdata.append((fn_name, code, is_c_abi))

    guid = "B4E2D19A-7F10-4C8E-9A31-55D011E8A420"
    pdb_name = "ntoskrnl_mini.pdb"
    pe_bytes, fn_layout = _pack_pe32_plus(funcs_with_pdata, guid, pdb_name)

    pe_path = out / "ntoskrnl_mini.exe"
    pdb_path = out / pdb_name
    pe_path.write_bytes(pe_bytes)

    # Build matching binary MSF 7.00 PDB with TPI structs + S_GPROC32 symbols
    pdb_builder = MSF70PDBBuilder(guid=guid, age=1)

    eproc = StructDef(
        name="_EPROCESS",
        typedef_name="EPROCESS",
        pointer_typedef="PEPROCESS",
        size=0x480,
        fields=[
            StructField("DirectoryTableBase", 0x28, 8, "uint64_t"),
            StructField("UniqueProcessId", 0x2E0, 8, "uint64_t"),
            StructField("QuotaUsagePool", 0x300, 8, "uint64_t"),
            StructField("QuotaPeakPool", 0x308, 8, "uint64_t"),
            StructField("Flags", 0x310, 4, "uint32_t"),
            StructField("ExitStatus", 0x314, 4, "NTSTATUS"),
        ],
    )
    kthread = StructDef(
        name="_KTHREAD",
        typedef_name="KTHREAD",
        pointer_typedef="PKTHREAD",
        size=0x200,
        fields=[
            StructField("ApcStateProcess", 0x98, 8, "struct _EPROCESS*", True, "_EPROCESS"),
        ],
    )
    pdb_builder.add_struct(eproc)
    pdb_builder.add_struct(kthread)

    prototypes: dict[str, tuple[str, list[tuple[str, str]]]] = {
        "PsChargeProcessPoolQuota": (
            "NTSTATUS",
            [("Process", "struct _EPROCESS*"), ("Amount", "uint64_t")],
        ),
        "PsGetThreadProcessId": (
            "uint64_t",
            [("Thread", "struct _KTHREAD*")],
        ),
        "RtlConvertTicksToMilliseconds": (
            "uint64_t",
            [("Ticks", "uint64_t")],
        ),
        "KeSumProcessorTicksUnrolled": (
            "uint64_t",
            [("Ticks", "uint32_t*")],
        ),
        "KiDispatchPriorityBatch": (
            "uint64_t",
            [("Weights", "uint32_t*"), ("Count", "uint32_t")],
        ),
        "KiSystemCall64Entry": ("void", []),
    }

    for fname, (rva, sz) in fn_layout.items():
        ret_t, params = prototypes.get(fname, ("uint64_t", []))
        pdb_builder.add_symbol(
            PDBSymbol(
                name=fname,
                rva=rva,
                size=sz,
                is_function=True,
                return_type=ret_t,
                param_types=params,
            )
        )

    pdb_path.write_bytes(pdb_builder.build_bytes(text_section_rva=0x1000))
    return pe_path, pdb_path


if __name__ == "__main__":
    pe_f, pdb_f = build_ntoskrnl_mini_fixture(Path("output/fixture"))
    print(f"Generated {pe_f} and {pdb_f}")
