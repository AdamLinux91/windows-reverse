"""Tests for PE32+ binary loading, binary MSF 7.00 PDB parsing, and Ghidra JSON import."""

from __future__ import annotations

import json
from pathlib import Path

from ntrev.frontend import GhidraHeadlessBridge, load_binary_image
from ntrev.models import PDBSymbol, StructDef, StructField
from ntrev.symbols import MSF70PDBBuilder, load_pdb_database


def test_pe32_plus_loader_extracts_sections_pdata_and_rsds(
    kernel_fixture_paths: tuple[Path, Path],
) -> None:
    pe_path, _pdb_path = kernel_fixture_paths
    img = load_binary_image(pe_path)

    assert img.format == "PE32+"
    assert img.machine == "AMD64"
    assert img.image_base == 0x140000000

    sec_names = {s.name for s in img.sections}
    assert {".text", ".rdata", ".pdata"}.issubset(sec_names)

    # 5 C/ABI functions have RUNTIME_FUNCTION unwind entries; KiSystemCall64Entry does not
    assert len(img.pdata_functions) == 5
    for rf in img.pdata_functions:
        assert rf.end_rva > rf.begin_rva >= 0x1000

    assert img.rsds is not None
    assert img.rsds.pdb_filename == "ntoskrnl_mini.pdb"
    assert img.rsds.age == 1
    assert "https://msdl.microsoft.com/download/symbols/ntoskrnl_mini.pdb/" in img.rsds.symbol_server_url


def test_msf70_pdb_roundtrip_structs_and_typed_symbols(tmp_path: Path) -> None:
    guid = "11223344-5566-7788-99AA-BBCCDDEEFF00"
    builder = MSF70PDBBuilder(guid=guid, age=2)

    builder.add_struct(
        StructDef(
            name="_KPROCESS_TEST",
            typedef_name="KPROCESS_TEST",
            pointer_typedef="PKPROCESS_TEST",
            size=0x40,
            fields=[
                StructField("DirectoryTableBase", 0x18, 8, "uint64_t"),
                StructField("ActiveProcessors", 0x20, 8, "uint64_t"),
                StructField("Flags", 0x28, 4, "uint32_t"),
            ],
        )
    )
    builder.add_symbol(
        PDBSymbol(
            name="KeQueryProcessCr3",
            rva=0x1040,
            size=16,
            is_function=True,
            return_type="uint64_t",
            param_types=[("Process", "struct _KPROCESS_TEST*")],
        )
    )

    pdb_file = tmp_path / "test_kernel.pdb"
    pdb_file.write_bytes(builder.build_bytes(text_section_rva=0x1000))

    parsed = load_pdb_database(pdb_file)
    assert parsed.guid == guid
    assert parsed.age == 2
    assert "_KPROCESS_TEST" in parsed.structs

    st = parsed.structs["_KPROCESS_TEST"]
    assert st.size == 0x40
    fld = st.field_at_offset(0x18)
    assert fld is not None and fld.name == "DirectoryTableBase"

    sym = parsed.symbols_by_name["KeQueryProcessCr3"]
    assert sym.rva == 0x1040
    assert sym.size == 16
    assert sym.return_type == "uint64_t"
    assert sym.param_types == [("Process", "struct _KPROCESS_TEST*")]


def test_ghidra_headless_json_importer(tmp_path: Path) -> None:
    ghidra_payload = {
        "program_name": "ntoskrnl.exe",
        "image_base": "0x140000000",
        "program_guid": "GHIDRA-TEST-GUID",
        "structures": [
            {
                "name": "_DEVICE_OBJECT",
                "size": 336,
                "is_union": False,
                "fields": [
                    {"name": "Type", "offset": 0, "size": 2, "c_type": "int16_t", "is_pointer": False},
                    {"name": "DriverObject", "offset": 8, "size": 8, "c_type": "void*", "is_pointer": True},
                ],
            }
        ],
        "functions": [
            {
                "name": "IoGetAttachedDevice",
                "entry_va": "0x140002100",
                "size": 42,
                "return_type": "struct _DEVICE_OBJECT*",
                "parameters": [{"name": "DeviceObject", "c_type": "struct _DEVICE_OBJECT*"}],
            }
        ],
    }
    json_path = tmp_path / "ghidra_out.json"
    json_path.write_text(json.dumps(ghidra_payload), encoding="utf-8")

    db = GhidraHeadlessBridge.load_ghidra_export(json_path)
    assert "_DEVICE_OBJECT" in db.structs
    assert db.structs["_DEVICE_OBJECT"].source == "ghidra"
    assert "IoGetAttachedDevice" in db.symbols_by_name
    assert db.symbols_by_name["IoGetAttachedDevice"].rva == 0x2100
