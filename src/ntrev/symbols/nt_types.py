"""
Canonical Windows NT Executive / Kernel x64 structure definitions.

Provides baseline x64 layouts (`_LIST_ENTRY`, `_UNICODE_STRING`, `_DISPATCHER_HEADER`,
`_KPCR`, `_KPRCB`, `_KTHREAD`, `_EPROCESS`, `_IRP`) used alongside PDB TPI records,
including `gs:[0x188]` (`KPCR->Prcb.CurrentThread`) resolution.
"""

from __future__ import annotations

from ntrev.models import StructDef, StructField


def get_builtin_nt_structs() -> dict[str, StructDef]:
    """Return built-in Windows x64 NT kernel structure definitions."""
    structs: list[StructDef] = [
        StructDef(
            name="_LIST_ENTRY",
            typedef_name="LIST_ENTRY",
            pointer_typedef="PLIST_ENTRY",
            size=16,
            source="nt_builtin",
            fields=[
                StructField("Flink", 0x00, 8, "struct _LIST_ENTRY*", True, "_LIST_ENTRY"),
                StructField("Blink", 0x08, 8, "struct _LIST_ENTRY*", True, "_LIST_ENTRY"),
            ],
        ),
        StructDef(
            name="_UNICODE_STRING",
            typedef_name="UNICODE_STRING",
            pointer_typedef="PUNICODE_STRING",
            size=16,
            source="nt_builtin",
            fields=[
                StructField("Length", 0x00, 2, "uint16_t"),
                StructField("MaximumLength", 0x02, 2, "uint16_t"),
                StructField("Buffer", 0x08, 8, "uint16_t*", True, "uint16_t"),
            ],
        ),
        StructDef(
            name="_DISPATCHER_HEADER",
            typedef_name="DISPATCHER_HEADER",
            pointer_typedef="PDISPATCHER_HEADER",
            size=24,
            source="nt_builtin",
            fields=[
                StructField("Lock", 0x00, 4, "int32_t"),
                StructField("SignalState", 0x04, 4, "int32_t"),
                StructField("WaitListHead", 0x08, 16, "LIST_ENTRY"),
            ],
        ),
        StructDef(
            name="_KTHREAD",
            typedef_name="KTHREAD",
            pointer_typedef="PKTHREAD",
            size=0x200,
            source="nt_builtin",
            fields=[
                StructField("Header", 0x00, 24, "DISPATCHER_HEADER"),
                StructField("InitialStack", 0x28, 8, "void*", True, "void"),
                StructField("StackLimit", 0x30, 8, "void*", True, "void"),
                StructField("KernelStack", 0x58, 8, "void*", True, "void"),
                StructField("WaitIrql", 0x68, 1, "uint8_t"),
                StructField("Priority", 0x7B, 1, "int8_t"),
                StructField("ApcStateProcess", 0x98, 8, "struct _EPROCESS*", True, "_EPROCESS"),
                StructField("Teb", 0xF0, 8, "void*", True, "void"),
                StructField("State", 0x184, 1, "uint8_t"),
                StructField("PreviousMode", 0x1F6, 1, "int8_t"),
            ],
        ),
        StructDef(
            name="_EPROCESS",
            typedef_name="EPROCESS",
            pointer_typedef="PEPROCESS",
            size=0x480,
            source="nt_builtin",
            fields=[
                StructField("PcbHeader", 0x00, 24, "DISPATCHER_HEADER"),
                StructField("DirectoryTableBase", 0x28, 8, "uint64_t"),
                StructField("UniqueProcessId", 0x2E0, 8, "uint64_t"),
                StructField("ActiveProcessLinks", 0x2E8, 16, "LIST_ENTRY"),
                StructField("QuotaUsagePool", 0x300, 8, "uint64_t"),
                StructField("QuotaPeakPool", 0x308, 8, "uint64_t"),
                StructField("Flags", 0x310, 4, "uint32_t"),
                StructField("ExitStatus", 0x314, 4, "NTSTATUS"),
                StructField("Token", 0x358, 8, "void*", True, "void"),
                StructField("InheritedFromUniqueProcessId", 0x3E0, 8, "uint64_t"),
            ],
        ),
        StructDef(
            name="_IRP",
            typedef_name="IRP",
            pointer_typedef="PIRP",
            size=0xD0,
            source="nt_builtin",
            fields=[
                StructField("Type", 0x00, 2, "int16_t"),
                StructField("Size", 0x02, 2, "uint16_t"),
                StructField("MdlAddress", 0x08, 8, "void*", True, "void"),
                StructField("Flags", 0x10, 4, "uint32_t"),
                StructField("AssociatedIrpSystemBuffer", 0x18, 8, "void*", True, "void"),
                StructField("IoStatusStatus", 0x30, 4, "NTSTATUS"),
                StructField("IoStatusInformation", 0x38, 8, "uint64_t"),
                StructField("RequestorMode", 0x40, 1, "int8_t"),
                StructField("StackCount", 0x42, 1, "int8_t"),
                StructField("CurrentLocation", 0x43, 1, "int8_t"),
                StructField("UserBuffer", 0x70, 8, "void*", True, "void"),
            ],
        ),
    ]
    return {s.name: s for s in structs}


KPCR_GS_OFFSETS: dict[int, tuple[str, str, str | None]] = {
    # gs_offset -> (c_expression, c_type, pointee_struct_name)
    0x18: ("KeGetPcr()->NtTib.Self", "void*", None),
    0x20: ("KeGetCurrentPrcb()", "void*", None),
    0x180: ("KeGetPcr()->Prcb", "void*", None),
    0x188: ("KeGetCurrentThread()", "PKTHREAD", "_KTHREAD"),
    0x1A8: ("KeGetCurrentPrcb()->RspBase", "uint64_t", None),
}
