"""
Canonical Windows NT Executive / Kernel x64 structure definitions & WDK API catalog.

Provides:
  - Baseline x64 NT kernel structure layouts (`_LIST_ENTRY`, `_UNICODE_STRING`, `_STRING`,
    `_RTL_BITMAP`, `_DISPATCHER_HEADER`, `_KEVENT`, `_KDPC`, `_MDL`, `_SLIST_HEADER`,
    `_EX_RUNDOWN_REF`, `_PROCESSOR_NUMBER`, `_KPCR`, `_KPRCB`, `_KTHREAD`, `_EPROCESS`,
    `_DEVICE_OBJECT`, `_IRP`, `_OBJECT_HEADER`) used when no PDB is supplied and overridden
    by exact TPI records when a Microsoft `.pdb` is loaded.
  - `KPCR_GS_OFFSETS`: `gs:[0x188]` (`KeGetCurrentThread()`) and `gs:[0x20]` (`KeGetCurrentPrcb()`).
  - `KNOWN_WDK_PROTOTYPES`: Documented Microsoft WDK / NTIFS kernel API signatures used when
    public stripped PDBs omit private `S_GPROC32` parameter records (analogous to Ghidra `ntddk_64.gdt`).
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
            name="_SINGLE_LIST_ENTRY",
            typedef_name="SINGLE_LIST_ENTRY",
            pointer_typedef="PSINGLE_LIST_ENTRY",
            size=8,
            source="nt_builtin",
            fields=[
                StructField("Next", 0x00, 8, "struct _SINGLE_LIST_ENTRY*", True, "_SINGLE_LIST_ENTRY"),
            ],
        ),
        StructDef(
            name="_SLIST_HEADER",
            typedef_name="SLIST_HEADER",
            pointer_typedef="PSLIST_HEADER",
            size=16,
            source="nt_builtin",
            fields=[
                StructField("Alignment", 0x00, 8, "uint64_t"),
                StructField("Region", 0x08, 8, "uint64_t"),
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
            name="_STRING",
            typedef_name="ANSI_STRING",
            pointer_typedef="PANSI_STRING",
            size=16,
            source="nt_builtin",
            fields=[
                StructField("Length", 0x00, 2, "uint16_t"),
                StructField("MaximumLength", 0x02, 2, "uint16_t"),
                StructField("Buffer", 0x08, 8, "char*", True, "char"),
            ],
        ),
        StructDef(
            name="_RTL_BITMAP",
            typedef_name="RTL_BITMAP",
            pointer_typedef="PRTL_BITMAP",
            size=16,
            source="nt_builtin",
            fields=[
                StructField("SizeOfBitMap", 0x00, 4, "uint32_t"),
                StructField("Buffer", 0x08, 8, "uint32_t*", True, "uint32_t"),
            ],
        ),
        StructDef(
            name="_LARGE_INTEGER",
            typedef_name="LARGE_INTEGER",
            pointer_typedef="PLARGE_INTEGER",
            size=8,
            source="nt_builtin",
            fields=[
                StructField("QuadPart", 0x00, 8, "int64_t"),
            ],
        ),
        StructDef(
            name="_LUID",
            typedef_name="LUID",
            pointer_typedef="PLUID",
            size=8,
            source="nt_builtin",
            fields=[
                StructField("LowPart", 0x00, 4, "uint32_t"),
                StructField("HighPart", 0x04, 4, "int32_t"),
            ],
        ),
        StructDef(
            name="_PROCESSOR_NUMBER",
            typedef_name="PROCESSOR_NUMBER",
            pointer_typedef="PPROCESSOR_NUMBER",
            size=4,
            source="nt_builtin",
            fields=[
                StructField("Group", 0x00, 2, "uint16_t"),
                StructField("Number", 0x02, 1, "uint8_t"),
                StructField("Reserved", 0x03, 1, "uint8_t"),
            ],
        ),
        StructDef(
            name="_EX_RUNDOWN_REF",
            typedef_name="EX_RUNDOWN_REF",
            pointer_typedef="PEX_RUNDOWN_REF",
            size=8,
            source="nt_builtin",
            fields=[
                StructField("Count", 0x00, 8, "uint64_t"),
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
            name="_KEVENT",
            typedef_name="KEVENT",
            pointer_typedef="PKEVENT",
            size=24,
            source="nt_builtin",
            fields=[
                StructField("Header", 0x00, 24, "DISPATCHER_HEADER"),
            ],
        ),
        StructDef(
            name="_KDPC",
            typedef_name="KDPC",
            pointer_typedef="PKDPC",
            size=64,
            source="nt_builtin",
            fields=[
                StructField("TargetInfoAsUlong", 0x00, 4, "uint32_t"),
                StructField("DpcListEntry", 0x08, 8, "SINGLE_LIST_ENTRY"),
                StructField("ProcessorHistory", 0x10, 8, "uint64_t"),
                StructField("DeferredRoutine", 0x18, 8, "void*", True, "void"),
                StructField("DeferredContext", 0x20, 8, "void*", True, "void"),
                StructField("SystemArgument1", 0x28, 8, "void*", True, "void"),
                StructField("SystemArgument2", 0x30, 8, "void*", True, "void"),
                StructField("DpcData", 0x38, 8, "void*", True, "void"),
            ],
        ),
        StructDef(
            name="_MDL",
            typedef_name="MDL",
            pointer_typedef="PMDL",
            size=48,
            source="nt_builtin",
            fields=[
                StructField("Next", 0x00, 8, "struct _MDL*", True, "_MDL"),
                StructField("Size", 0x08, 2, "int16_t"),
                StructField("MdlFlags", 0x0A, 2, "int16_t"),
                StructField("Process", 0x10, 8, "struct _EPROCESS*", True, "_EPROCESS"),
                StructField("MappedSystemVa", 0x18, 8, "void*", True, "void"),
                StructField("StartVa", 0x20, 8, "void*", True, "void"),
                StructField("ByteCount", 0x28, 4, "uint32_t"),
                StructField("ByteOffset", 0x2C, 4, "uint32_t"),
            ],
        ),
        StructDef(
            name="_KPRCB",
            typedef_name="KPRCB",
            pointer_typedef="PKPRCB",
            size=0x200,
            source="nt_builtin",
            fields=[
                StructField("CurrentThread", 0x08, 8, "struct _KTHREAD*", True, "_KTHREAD"),
                StructField("NextThread", 0x10, 8, "struct _KTHREAD*", True, "_KTHREAD"),
                StructField("IdleThread", 0x18, 8, "struct _KTHREAD*", True, "_KTHREAD"),
                StructField("Number", 0x24, 4, "uint32_t"),
                StructField("RspBase", 0x28, 8, "uint64_t"),
                StructField("Group", 0xD0, 1, "uint8_t"),
                StructField("GroupIndex", 0xD1, 1, "uint8_t"),
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
            name="_DEVICE_OBJECT",
            typedef_name="DEVICE_OBJECT",
            pointer_typedef="PDEVICE_OBJECT",
            size=0x150,
            source="nt_builtin",
            fields=[
                StructField("Type", 0x00, 2, "int16_t"),
                StructField("Size", 0x02, 2, "uint16_t"),
                StructField("ReferenceCount", 0x04, 4, "int32_t"),
                StructField("DriverObject", 0x08, 8, "void*", True, "void"),
                StructField("NextDevice", 0x10, 8, "struct _DEVICE_OBJECT*", True, "_DEVICE_OBJECT"),
                StructField("AttachedDevice", 0x18, 8, "struct _DEVICE_OBJECT*", True, "_DEVICE_OBJECT"),
                StructField("CurrentIrp", 0x20, 8, "struct _IRP*", True, "_IRP"),
                StructField("Flags", 0x30, 4, "uint32_t"),
                StructField("StackSize", 0x4C, 1, "int8_t"),
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
    0x20: ("KeGetCurrentPrcb()", "struct _KPRCB*", "_KPRCB"),
    0x180: ("KeGetPcr()->Prcb", "void*", None),
    0x188: ("KeGetCurrentThread()", "struct _KTHREAD*", "_KTHREAD"),
    0x1A8: ("KeGetCurrentPrcb()->RspBase", "uint64_t", None),
}


# Documented Microsoft WDK / NTIFS x64 API Signatures (fallback when stripped PDB lacks S_GPROC32)
KNOWN_WDK_PROTOTYPES: dict[str, tuple[str, list[tuple[str, str]]]] = {
    # Ke / Ki — Kernel Dispatcher, IRQL, SpinLocks, Events, DPCs
    "KeGetCurrentIrql": ("KIRQL", []),
    "KzRaiseIrql": ("KIRQL", [("NewIrql", "KIRQL")]),
    "KzLowerIrql": ("void", [("NewIrql", "KIRQL")]),
    "KeRaiseIrqlToDpcLevel": ("KIRQL", []),
    "KeInitializeSpinLock": ("void", [("SpinLock", "uint64_t*")]),
    "KeTestSpinLock": ("BOOLEAN", [("SpinLock", "uint64_t*")]),
    "KeInitializeEvent": (
        "void",
        [("Event", "struct _KEVENT*"), ("Type", "uint32_t"), ("State", "BOOLEAN")],
    ),
    "KeReadStateEvent": ("LONG", [("Event", "struct _KEVENT*")]),
    "KeReadStateSemaphore": ("LONG", [("Semaphore", "struct _KSEMAPHORE*")]),
    "KeReadStateMutant": ("LONG", [("Mutant", "struct _KMUTANT*")]),
    "KeReadStateTimer": ("BOOLEAN", [("Timer", "struct _KTIMER*")]),
    "KeInitializeDpc": (
        "void",
        [
            ("Dpc", "struct _KDPC*"),
            ("DeferredRoutine", "void*"),
            ("DeferredContext", "void*"),
        ],
    ),
    "KeSetImportanceDpc": ("void", [("Dpc", "struct _KDPC*"), ("Importance", "uint32_t")]),
    "KeSetTargetProcessorDpc": ("void", [("Dpc", "struct _KDPC*"), ("Number", "int8_t")]),
    "KeReadStateEvent": ("LONG", [("Event", "struct _DISPATCHER_HEADER*")]),
    "KeReadStateMutex": ("LONG", [("Mutex", "struct _DISPATCHER_HEADER*")]),
    "KeReadStateSemaphore": ("LONG", [("Semaphore", "struct _DISPATCHER_HEADER*")]),
    "KeReadStateTimer": ("BOOLEAN", [("Timer", "struct _DISPATCHER_HEADER*")]),
    "KeSetCoalescableTimer": (
        "BOOLEAN",
        [
            ("Timer", "void*"),
            ("DueTime", "int64_t"),
            ("Period", "ULONG"),
            ("TolerableDelay", "ULONG"),
        ],
    ),
    "KiSetTimerEx": (
        "BOOLEAN",
        [
            ("Timer", "void*"),
            ("DueTime", "int64_t"),
            ("Period", "ULONG"),
            ("Hand", "uint64_t"),
        ],
    ),
    "KeQueryPriorityThread": ("int32_t", [("Thread", "struct _KTHREAD*")]),
    "KeGetCurrentProcessorNumberEx": (
        "ULONG",
        [("ProcNumber", "struct _PROCESSOR_NUMBER*")],
    ),
    "KeGetCurrentThread": ("struct _KTHREAD*", []),
    "KeCheckForZeroPage": ("uint64_t", [("PageBase", "uint64_t*")]),
    # Ps — Process & Thread Manager
    "PsGetCurrentProcess": ("struct _EPROCESS*", []),
    "PsGetCurrentThread": ("struct _ETHREAD*", []),
    "PsGetProcessId": ("void*", [("Process", "struct _EPROCESS*")]),
    "PsGetProcessInheritedFromUniqueProcessId": (
        "void*",
        [("Process", "struct _EPROCESS*")],
    ),
    "PsGetProcessImageFileName": ("uint8_t*", [("Process", "struct _EPROCESS*")]),
    "PsGetProcessExitStatus": ("NTSTATUS", [("Process", "struct _EPROCESS*")]),
    "PsGetProcessPeb": ("void*", [("Process", "struct _EPROCESS*")]),
    "PsGetProcessSectionBaseAddress": ("void*", [("Process", "struct _EPROCESS*")]),
    "PsIsSystemProcess": ("BOOLEAN", [("Process", "struct _EPROCESS*")]),
    "PsIsProtectedProcess": ("BOOLEAN", [("Process", "struct _EPROCESS*")]),
    "PsIsProtectedProcessLight": ("BOOLEAN", [("Process", "struct _EPROCESS*")]),
    "PsGetThreadId": ("void*", [("Thread", "struct _ETHREAD*")]),
    "PsGetThreadProcess": ("struct _EPROCESS*", [("Thread", "struct _KTHREAD*")]),
    "PsGetThreadProcessId": ("void*", [("Thread", "struct _ETHREAD*")]),
    "PsGetThreadTeb": ("void*", [("Thread", "struct _KTHREAD*")]),
    "PsIsSystemThread": ("BOOLEAN", [("Thread", "struct _KTHREAD*")]),
    # Mm — Memory Manager
    "MmSizeOfMdl": ("size_t", [("Base", "void*"), ("Length", "size_t")]),
    "MmGetMaximumFileSectionSize": ("uint64_t", []),
    "MmIsAddressValid": ("BOOLEAN", [("VirtualAddress", "void*")]),
    "MiIsAddressValid": ("BOOLEAN", [("VirtualAddress", "void*")]),
    "MmFreePagesFromMdl": ("void", [("MemoryDescriptorList", "struct _MDL*")]),
    # Ob — Object Manager
    "ObGetObjectType": ("struct _OBJECT_TYPE*", [("Object", "void*")]),
    "ObGetFilterVersion": ("uint16_t", []),
    "ObQueryNameInfo": ("void*", [("Object", "void*")]),
    "ObDereferenceObject": ("int64_t", [("Object", "void*")]),
    "ObfDereferenceObjectWithTag": ("int64_t", [("Object", "void*"), ("Tag", "ULONG")]),
    "ObIsKernelHandle": ("BOOLEAN", [("Handle", "void*")]),
    "ObpIsKernelHandle": ("BOOLEAN", [("Handle", "void*"), ("PreviousMode", "BOOLEAN")]),
    # Io — I/O & PnP Manager
    "IoGetAttachedDevice": (
        "struct _DEVICE_OBJECT*",
        [("DeviceObject", "struct _DEVICE_OBJECT*")],
    ),
    "IoGetIoPriorityHint": ("uint32_t", [("Irp", "struct _IRP*")]),
    "IoGetCurrentProcess": ("struct _EPROCESS*", []),
    "IoThreadToProcess": ("struct _EPROCESS*", [("Thread", "struct _ETHREAD*")]),
    "IoIs32bitProcess": ("BOOLEAN", [("Irp", "struct _IRP*")]),
    "PiUEventHashStringIntoBucket": ("ULONG", [("SourceString", "const uint16_t*")]),
    # Ex — Executive
    "ExInitializeRundownProtection": (
        "void",
        [("RunRef", "struct _EX_RUNDOWN_REF*")],
    ),
    "ExReInitializeRundownProtection": (
        "void",
        [("RunRef", "struct _EX_RUNDOWN_REF*")],
    ),
    "ExQueryDepthSList": ("uint16_t", [("SListHead", "union _SLIST_HEADER*")]),
    "ExpInterlockedPushEntrySList": (
        "struct _SINGLE_LIST_ENTRY*",
        [
            ("ListHead", "union _SLIST_HEADER*"),
            ("ListEntry", "struct _SINGLE_LIST_ENTRY*"),
        ],
    ),
    "ExpInterlockedPopEntrySList": (
        "struct _SINGLE_LIST_ENTRY*",
        [("ListHead", "union _SLIST_HEADER*")],
    ),
    # Rtl — Run-Time Library
    "RtlInitUnicodeString": (
        "void",
        [
            ("DestinationString", "struct _UNICODE_STRING*"),
            ("SourceString", "const uint16_t*"),
        ],
    ),
    "RtlInitAnsiString": (
        "void",
        [("DestinationString", "struct _STRING*"), ("SourceString", "const char*")],
    ),
    "RtlInitializeBitMap": (
        "void",
        [
            ("BitMapHeader", "struct _RTL_BITMAP*"),
            ("BitMapBuffer", "uint32_t*"),
            ("SizeOfBitMap", "ULONG"),
        ],
    ),
    "RtlTestBit": (
        "BOOLEAN",
        [("BitMapHeader", "struct _RTL_BITMAP*"), ("BitNumber", "ULONG")],
    ),
    "RtlSetBit": (
        "void",
        [("BitMapHeader", "struct _RTL_BITMAP*"), ("BitNumber", "ULONG")],
    ),
    "RtlClearBit": (
        "void",
        [("BitMapHeader", "struct _RTL_BITMAP*"), ("BitNumber", "ULONG")],
    ),
    "RtlCompareMemory": (
        "size_t",
        [("Source1", "const void*"), ("Source2", "const void*"), ("Length", "size_t")],
    ),
    "RtlExtendedMagicDivide": (
        "int64_t",
        [("Dividend", "int64_t"), ("MagicDivisor", "int64_t"), ("ShiftCount", "int8_t")],
    ),
    "RtlSecondsSince1970ToTime": (
        "void",
        [("ElapsedSeconds", "ULONG"), ("Time", "union _LARGE_INTEGER*")],
    ),
    "RtlSecondsSince1980ToTime": (
        "void",
        [("ElapsedSeconds", "ULONG"), ("Time", "union _LARGE_INTEGER*")],
    ),
    "RtlTimeToSecondsSince1970": (
        "BOOLEAN",
        [("Time", "union _LARGE_INTEGER*"), ("ElapsedSeconds", "ULONG*")],
    ),
    "RtlHashUnicodeString": (
        "NTSTATUS",
        [
            ("String", "const struct _UNICODE_STRING*"),
            ("CaseInSensitive", "BOOLEAN"),
            ("HashAlgorithm", "ULONG"),
            ("HashValue", "ULONG*"),
        ],
    ),
    # Se / Kd
    "SeTokenIsRestricted": ("BOOLEAN", [("Token", "struct _TOKEN*")]),
    "SeTokenIsAdmin": ("BOOLEAN", [("Token", "struct _TOKEN*")]),
    "SepSidInToken": (
        "BOOLEAN",
        [
            ("Token", "void*"),
            ("PrincipalSelfSid", "void*"),
            ("Sid", "void*"),
            ("DenyOnly", "BOOLEAN"),
            ("Restricted", "BOOLEAN"),
            ("SingleSid", "BOOLEAN"),
        ],
    ),
    "SeQueryAuthenticationIdToken": (
        "NTSTATUS",
        [("Token", "struct _TOKEN*"), ("AuthenticationId", "struct _LUID*")],
    ),
    "KdRefreshDebuggerNotPresent": ("BOOLEAN", []),
    "KdEnterDebugger": ("BOOLEAN", [("TrapFrame", "void*")]),
    "KdpPrintString": ("BOOLEAN", [("Output", "struct _STRING*")]),
    "KdExitDebugger": ("void", [("Enable", "BOOLEAN")]),
}
