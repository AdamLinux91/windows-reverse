/*
 * Reconstructed Windows NT Kernel Subsystem: Process, Thread, Job & Silo Executive (Ps/Psp)
 * Source Binary : ntoskrnl.exe (10.0.16299.15 x64, RSDS 83DB4240-4EFD-4AB6-AFB6-FA864B700CB3)
 * Functions     : 73 Z3-verified / assembly-preserved routines
 */
#include "../../include/nt_reconstructed_types.h"

/*
 * Function : PsGetCurrentProcess @ 0x1400e9990 (17 bytes)
 * Z3 Proof : VERIFIED_EQUIVALENT (4.46 ms, 33 SMT assertions)
 * Verified : win64_callee_saved_abi, x86_64_to_llvm_ir_equivalence
 */
struct _EPROCESS* __attribute__((ms_abi)) PsGetCurrentProcess(void) {
    struct _KTHREAD* __attribute__((unused)) ssa_gs_val = KeGetCurrentThread();
    struct _KPROCESS* __attribute__((unused)) ssa_mem_ld = (struct _KPROCESS*)(uintptr_t)(ssa_gs_val->ApcState.Process);
    return (struct _EPROCESS*)(uintptr_t)(((uint64_t)(uintptr_t)ssa_mem_ld));
}

/*
 * Function : PsGetCurrentThread @ 0x14015f4e0 (10 bytes)
 * Z3 Proof : VERIFIED_EQUIVALENT (3.64 ms, 33 SMT assertions)
 * Verified : win64_callee_saved_abi, x86_64_to_llvm_ir_equivalence
 */
struct _ETHREAD* __attribute__((ms_abi)) PsGetCurrentThread(void) {
    struct _KTHREAD* __attribute__((unused)) ssa_gs_val = KeGetCurrentThread();
    return (struct _ETHREAD*)(uintptr_t)(((uint64_t)(uintptr_t)ssa_gs_val));
}

/*
 * Function : PsGetProcessId @ 0x1400bdce0 (8 bytes)
 * Z3 Proof : VERIFIED_EQUIVALENT (3.85 ms, 33 SMT assertions)
 * Verified : win64_callee_saved_abi, x86_64_to_llvm_ir_equivalence
 */
void* __attribute__((ms_abi)) PsGetProcessId(struct _EPROCESS* Process) {
    uint64_t __attribute__((unused)) ssa_mem_ld = (uint64_t)(uintptr_t)(Process->UniqueProcessId);
    return (void*)(uintptr_t)(ssa_mem_ld);
}

/*
 * Function : PsGetProcessInheritedFromUniqueProcessId @ 0x14007d2f0 (8 bytes)
 * Z3 Proof : VERIFIED_EQUIVALENT (3.81 ms, 33 SMT assertions)
 * Verified : win64_callee_saved_abi, x86_64_to_llvm_ir_equivalence
 */
void* __attribute__((ms_abi)) PsGetProcessInheritedFromUniqueProcessId(struct _EPROCESS* Process) {
    uint64_t __attribute__((unused)) ssa_mem_ld = (uint64_t)(uintptr_t)(Process->InheritedFromUniqueProcessId);
    return (void*)(uintptr_t)(ssa_mem_ld);
}

/*
 * Function : PsGetProcessImageFileName @ 0x140109830 (8 bytes)
 * Z3 Proof : VERIFIED_EQUIVALENT (3.21 ms, 33 SMT assertions)
 * Verified : win64_callee_saved_abi, x86_64_to_llvm_ir_equivalence
 */
uint8_t* __attribute__((ms_abi)) PsGetProcessImageFileName(struct _EPROCESS* Process) {
    return (uint8_t*)(uintptr_t)(((uint64_t)(((uint64_t)(uintptr_t)Process)) + (uint64_t)(0x450ULL)));
}

/*
 * Function : PsGetProcessExitStatus @ 0x14070c880 (7 bytes)
 * Z3 Proof : VERIFIED_EQUIVALENT (3.58 ms, 33 SMT assertions)
 * Verified : win64_callee_saved_abi, x86_64_to_llvm_ir_equivalence
 */
NTSTATUS __attribute__((ms_abi)) PsGetProcessExitStatus(struct _EPROCESS* Process) {
    uint32_t __attribute__((unused)) ssa_mem_ld = (uint32_t)(Process->ExitStatus);
    return (NTSTATUS)(((uint64_t)(uint32_t)(ssa_mem_ld)));
}

/*
 * Function : PsGetProcessPeb @ 0x1401085e0 (8 bytes)
 * Z3 Proof : VERIFIED_EQUIVALENT (3.66 ms, 33 SMT assertions)
 * Verified : win64_callee_saved_abi, x86_64_to_llvm_ir_equivalence
 */
void* __attribute__((ms_abi)) PsGetProcessPeb(struct _EPROCESS* Process) {
    struct _PEB* __attribute__((unused)) ssa_mem_ld = (struct _PEB*)(uintptr_t)(Process->Peb);
    return (void*)(uintptr_t)(((uint64_t)(uintptr_t)ssa_mem_ld));
}

/*
 * Function : PsGetProcessSectionBaseAddress @ 0x14004c7d0 (8 bytes)
 * Z3 Proof : VERIFIED_EQUIVALENT (3.85 ms, 33 SMT assertions)
 * Verified : win64_callee_saved_abi, x86_64_to_llvm_ir_equivalence
 */
void* __attribute__((ms_abi)) PsGetProcessSectionBaseAddress(struct _EPROCESS* Process) {
    uint64_t __attribute__((unused)) ssa_mem_ld = (uint64_t)(uintptr_t)(Process->SectionBaseAddress);
    return (void*)(uintptr_t)(ssa_mem_ld);
}

/*
 * Function : PsIsSystemProcess @ 0x1400b7d00 (12 bytes)
 * Z3 Proof : VERIFIED_EQUIVALENT (4.04 ms, 33 SMT assertions)
 * Verified : win64_callee_saved_abi, x86_64_to_llvm_ir_equivalence
 */
BOOLEAN __attribute__((ms_abi)) PsIsSystemProcess(struct _EPROCESS* Process) {
    uint32_t __attribute__((unused)) ssa_mem_ld = (uint32_t)(Process->Flags3);
    return (BOOLEAN)(((uint64_t)(uint32_t)(((uint32_t)(((uint32_t)(ssa_mem_ld) >> (uint32_t)(12ULL))) & (uint32_t)(0xfff01ULL)))));
}

/*
 * Function : PsIsProtectedProcess @ 0x14004c7e0 (16 bytes)
 * Z3 Proof : VERIFIED_EQUIVALENT (3.83 ms, 33 SMT assertions)
 * Verified : win64_callee_saved_abi, x86_64_to_llvm_ir_equivalence
 */
BOOLEAN __attribute__((ms_abi)) PsIsProtectedProcess(struct _EPROCESS* Process) {
    uint8_t __attribute__((unused)) ssa_mem_ld = (uint8_t)(Process->Protection.Level);
    return (BOOLEAN)(((uint64_t)(uint8_t)(((uint8_t)(((uint8_t)(ssa_mem_ld) & (uint8_t)(7ULL))) != (uint8_t)(0ULL)))));
}

/*
 * Function : PsIsProtectedProcessLight @ 0x140028eb0 (18 bytes)
 * Z3 Proof : VERIFIED_EQUIVALENT (3.95 ms, 33 SMT assertions)
 * Verified : win64_callee_saved_abi, x86_64_to_llvm_ir_equivalence
 */
BOOLEAN __attribute__((ms_abi)) PsIsProtectedProcessLight(struct _EPROCESS* Process) {
    uint8_t __attribute__((unused)) ssa_mem_ld = (uint8_t)(Process->Protection.Level);
    return (BOOLEAN)(((uint64_t)(uint8_t)(((uint8_t)(((uint8_t)(ssa_mem_ld) & (uint8_t)(7ULL))) == (uint8_t)(1ULL)))));
}

/*
 * Function : PsGetThreadId @ 0x1401063d0 (8 bytes)
 * Z3 Proof : VERIFIED_EQUIVALENT (3.77 ms, 33 SMT assertions)
 * Verified : win64_callee_saved_abi, x86_64_to_llvm_ir_equivalence
 */
void* __attribute__((ms_abi)) PsGetThreadId(struct _ETHREAD* Thread) {
    uint64_t __attribute__((unused)) ssa_mem_ld = (uint64_t)(uintptr_t)(Thread->Cid.UniqueThread);
    return (void*)(uintptr_t)(ssa_mem_ld);
}

/*
 * Function : PsGetThreadProcess @ 0x140050520 (8 bytes)
 * Z3 Proof : VERIFIED_EQUIVALENT (3.87 ms, 33 SMT assertions)
 * Verified : win64_callee_saved_abi, x86_64_to_llvm_ir_equivalence
 */
struct _EPROCESS* __attribute__((ms_abi)) PsGetThreadProcess(struct _KTHREAD* Thread) {
    struct _KPROCESS* __attribute__((unused)) ssa_mem_ld = (struct _KPROCESS*)(uintptr_t)(Thread->Process);
    return (struct _EPROCESS*)(uintptr_t)(((uint64_t)(uintptr_t)ssa_mem_ld));
}

/*
 * Function : PsGetThreadProcessId @ 0x14010af10 (8 bytes)
 * Z3 Proof : VERIFIED_EQUIVALENT (3.64 ms, 33 SMT assertions)
 * Verified : win64_callee_saved_abi, x86_64_to_llvm_ir_equivalence
 */
void* __attribute__((ms_abi)) PsGetThreadProcessId(struct _ETHREAD* Thread) {
    uint64_t __attribute__((unused)) ssa_mem_ld = (uint64_t)(uintptr_t)(Thread->Cid.UniqueProcess);
    return (void*)(uintptr_t)(ssa_mem_ld);
}

/*
 * Function : PsGetThreadTeb @ 0x1400f72a0 (8 bytes)
 * Z3 Proof : VERIFIED_EQUIVALENT (3.82 ms, 33 SMT assertions)
 * Verified : win64_callee_saved_abi, x86_64_to_llvm_ir_equivalence
 */
void* __attribute__((ms_abi)) PsGetThreadTeb(struct _KTHREAD* Thread) {
    uint64_t __attribute__((unused)) ssa_mem_ld = (uint64_t)(uintptr_t)(Thread->Teb);
    return (void*)(uintptr_t)(ssa_mem_ld);
}

/*
 * Function : PsIsSystemThread @ 0x1401091e0 (9 bytes)
 * Z3 Proof : VERIFIED_EQUIVALENT (4.07 ms, 33 SMT assertions)
 * Verified : win64_callee_saved_abi, x86_64_to_llvm_ir_equivalence
 */
BOOLEAN __attribute__((ms_abi)) PsIsSystemThread(struct _KTHREAD* Thread) {
    uint32_t __attribute__((unused)) ssa_mem_ld = (uint32_t)(Thread->MiscFlags);
    return (BOOLEAN)(((uint64_t)(uint32_t)(((uint32_t)(((uint32_t)(ssa_mem_ld) >> (uint32_t)(10ULL))) & (uint32_t)(0x3fff01ULL)))));
}

/*
 * Function : PsGetSiloMonitorContextSlot @ 0x140583730 (4 bytes)
 * Z3 Proof : VERIFIED_EQUIVALENT (3.57 ms, 33 SMT assertions)
 * Verified : win64_callee_saved_abi, x86_64_to_llvm_ir_equivalence
 */
uint32_t __attribute__((ms_abi)) PsGetSiloMonitorContextSlot(uint32_t* arg1) {
    uint32_t __attribute__((unused)) ssa_mem_ld = (uint32_t)((*(uint32_t*)(uintptr_t)(((uint64_t)(((uint64_t)(uintptr_t)arg1)) + (uint64_t)(0x14ULL)))));
    return (uint32_t)(((uint64_t)(uint32_t)(ssa_mem_ld)));
}

/*
 * Function : PsGetJobLock @ 0x1400122f0 (5 bytes)
 * Z3 Proof : VERIFIED_EQUIVALENT (3.03 ms, 33 SMT assertions)
 * Verified : win64_callee_saved_abi, x86_64_to_llvm_ir_equivalence
 */
uint64_t __attribute__((ms_abi)) PsGetJobLock(uint64_t* arg1) {
    return (uint64_t)(((uint64_t)(((uint64_t)(uintptr_t)arg1)) + (uint64_t)(0x38ULL)));
}

/*
 * Function : PsGetJobUIRestrictionsClass @ 0x140113fe0 (7 bytes)
 * Z3 Proof : VERIFIED_EQUIVALENT (3.46 ms, 33 SMT assertions)
 * Verified : win64_callee_saved_abi, x86_64_to_llvm_ir_equivalence
 */
uint32_t __attribute__((ms_abi)) PsGetJobUIRestrictionsClass(uint32_t* arg1) {
    uint32_t __attribute__((unused)) ssa_mem_ld = (uint32_t)((*(uint32_t*)(uintptr_t)(((uint64_t)(((uint64_t)(uintptr_t)arg1)) + (uint64_t)(0x1c0ULL)))));
    return (uint32_t)(((uint64_t)(uint32_t)(ssa_mem_ld)));
}

/*
 * Function : PsGetProcessPriorityClass @ 0x140245890 (7 bytes)
 * Z3 Proof : VERIFIED_EQUIVALENT (3.23 ms, 33 SMT assertions)
 * Verified : win64_callee_saved_abi, x86_64_to_llvm_ir_equivalence
 */
uint8_t __attribute__((ms_abi)) PsGetProcessPriorityClass(uint8_t* arg1) {
    uint8_t __attribute__((unused)) ssa_mem_ld = (uint8_t)((*(uint8_t*)(uintptr_t)(((uint64_t)(((uint64_t)(uintptr_t)arg1)) + (uint64_t)(0x45fULL)))));
    return (uint8_t)(((uint64_t)(uint8_t)(ssa_mem_ld)));
}

/*
 * Function : PsGetProcessProtection @ 0x140111080 (7 bytes)
 * Z3 Proof : VERIFIED_EQUIVALENT (3.25 ms, 33 SMT assertions)
 * Verified : win64_callee_saved_abi, x86_64_to_llvm_ir_equivalence
 */
uint8_t __attribute__((ms_abi)) PsGetProcessProtection(uint8_t* arg1) {
    uint8_t __attribute__((unused)) ssa_mem_ld = (uint8_t)((*(uint8_t*)(uintptr_t)(((uint64_t)(((uint64_t)(uintptr_t)arg1)) + (uint64_t)(0x6caULL)))));
    return (uint8_t)(((uint64_t)(uint8_t)(ssa_mem_ld)));
}

/*
 * Function : PsSetProcessPriorityClass @ 0x1402459f0 (7 bytes)
 * Z3 Proof : VERIFIED_EQUIVALENT (3.25 ms, 34 SMT assertions)
 * Verified : win64_callee_saved_abi, x86_64_to_llvm_ir_equivalence
 */
void __attribute__((ms_abi)) PsSetProcessPriorityClass(uint8_t* arg1, uint8_t arg2) {
    (*(uint8_t*)(uintptr_t)(((uint64_t)(((uint64_t)(uintptr_t)arg1)) + (uint64_t)(0x45fULL)))) = (uint8_t)(((uint8_t)(uint64_t)(((uint64_t)arg2))));
    return;
}

/*
 * Function : PsGetProcessCreateTimeQuadPart @ 0x1401037a0 (8 bytes)
 * Z3 Proof : VERIFIED_EQUIVALENT (3.71 ms, 33 SMT assertions)
 * Verified : win64_callee_saved_abi, x86_64_to_llvm_ir_equivalence
 */
uint64_t __attribute__((ms_abi)) PsGetProcessCreateTimeQuadPart(uint64_t* arg1) {
    uint64_t __attribute__((unused)) ssa_mem_ld = (uint64_t)((*(uint64_t*)(uintptr_t)(((uint64_t)(((uint64_t)(uintptr_t)arg1)) + (uint64_t)(0x308ULL)))));
    return (uint64_t)(ssa_mem_ld);
}

/*
 * Function : PsGetProcessDebugPort @ 0x14010e8b0 (8 bytes)
 * Z3 Proof : VERIFIED_EQUIVALENT (3.65 ms, 33 SMT assertions)
 * Verified : win64_callee_saved_abi, x86_64_to_llvm_ir_equivalence
 */
uint64_t __attribute__((ms_abi)) PsGetProcessDebugPort(uint64_t* arg1) {
    uint64_t __attribute__((unused)) ssa_mem_ld = (uint64_t)((*(uint64_t*)(uintptr_t)(((uint64_t)(((uint64_t)(uintptr_t)arg1)) + (uint64_t)(0x420ULL)))));
    return (uint64_t)(ssa_mem_ld);
}

/*
 * Function : PsGetProcessDxgProcess @ 0x1405588a0 (8 bytes)
 * Z3 Proof : VERIFIED_EQUIVALENT (3.88 ms, 33 SMT assertions)
 * Verified : win64_callee_saved_abi, x86_64_to_llvm_ir_equivalence
 */
uint64_t __attribute__((ms_abi)) PsGetProcessDxgProcess(uint64_t* arg1) {
    uint64_t __attribute__((unused)) ssa_mem_ld = (uint64_t)((*(uint64_t*)(uintptr_t)(((uint64_t)(((uint64_t)(uintptr_t)arg1)) + (uint64_t)(0x7b8ULL)))));
    return (uint64_t)(ssa_mem_ld);
}

/*
 * Function : PsGetProcessJob @ 0x1401091f0 (8 bytes)
 * Z3 Proof : VERIFIED_EQUIVALENT (3.59 ms, 33 SMT assertions)
 * Verified : win64_callee_saved_abi, x86_64_to_llvm_ir_equivalence
 */
uint64_t __attribute__((ms_abi)) PsGetProcessJob(uint64_t* arg1) {
    uint64_t __attribute__((unused)) ssa_mem_ld = (uint64_t)((*(uint64_t*)(uintptr_t)(((uint64_t)(((uint64_t)(uintptr_t)arg1)) + (uint64_t)(0x3b0ULL)))));
    return (uint64_t)(ssa_mem_ld);
}

/*
 * Function : PsGetProcessSecurityPort @ 0x1405bc590 (8 bytes)
 * Z3 Proof : VERIFIED_EQUIVALENT (3.84 ms, 33 SMT assertions)
 * Verified : win64_callee_saved_abi, x86_64_to_llvm_ir_equivalence
 */
uint64_t __attribute__((ms_abi)) PsGetProcessSecurityPort(uint64_t* arg1) {
    uint64_t __attribute__((unused)) ssa_mem_ld = (uint64_t)((*(uint64_t*)(uintptr_t)(((uint64_t)(((uint64_t)(uintptr_t)arg1)) + (uint64_t)(0x460ULL)))));
    return (uint64_t)(ssa_mem_ld);
}

/*
 * Function : PsGetProcessSequenceNumber @ 0x14010f5b0 (8 bytes)
 * Z3 Proof : VERIFIED_EQUIVALENT (3.56 ms, 33 SMT assertions)
 * Verified : win64_callee_saved_abi, x86_64_to_llvm_ir_equivalence
 */
uint64_t __attribute__((ms_abi)) PsGetProcessSequenceNumber(uint64_t* arg1) {
    uint64_t __attribute__((unused)) ssa_mem_ld = (uint64_t)((*(uint64_t*)(uintptr_t)(((uint64_t)(((uint64_t)(uintptr_t)arg1)) + (uint64_t)(0x750ULL)))));
    return (uint64_t)(ssa_mem_ld);
}

/*
 * Function : PsGetProcessServerSilo @ 0x1400bd910 (8 bytes)
 * Z3 Proof : VERIFIED_EQUIVALENT (3.88 ms, 33 SMT assertions)
 * Verified : win64_callee_saved_abi, x86_64_to_llvm_ir_equivalence
 */
uint64_t __attribute__((ms_abi)) PsGetProcessServerSilo(uint64_t* arg1) {
    uint64_t __attribute__((unused)) ssa_mem_ld = (uint64_t)((*(uint64_t*)(uintptr_t)(((uint64_t)(((uint64_t)(uintptr_t)arg1)) + (uint64_t)(0x6c0ULL)))));
    return (uint64_t)(ssa_mem_ld);
}

/*
 * Function : PsGetProcessWin32Process @ 0x1401082c0 (8 bytes)
 * Z3 Proof : VERIFIED_EQUIVALENT (3.55 ms, 33 SMT assertions)
 * Verified : win64_callee_saved_abi, x86_64_to_llvm_ir_equivalence
 */
uint64_t __attribute__((ms_abi)) PsGetProcessWin32Process(uint64_t* arg1) {
    uint64_t __attribute__((unused)) ssa_mem_ld = (uint64_t)((*(uint64_t*)(uintptr_t)(((uint64_t)(((uint64_t)(uintptr_t)arg1)) + (uint64_t)(0x3a8ULL)))));
    return (uint64_t)(ssa_mem_ld);
}

/*
 * Function : PsGetProcessWin32WindowStation @ 0x14010bb10 (8 bytes)
 * Z3 Proof : VERIFIED_EQUIVALENT (3.73 ms, 33 SMT assertions)
 * Verified : win64_callee_saved_abi, x86_64_to_llvm_ir_equivalence
 */
uint64_t __attribute__((ms_abi)) PsGetProcessWin32WindowStation(uint64_t* arg1) {
    uint64_t __attribute__((unused)) ssa_mem_ld = (uint64_t)((*(uint64_t*)(uintptr_t)(((uint64_t)(((uint64_t)(uintptr_t)arg1)) + (uint64_t)(0x3d8ULL)))));
    return (uint64_t)(ssa_mem_ld);
}

/*
 * Function : PsGetThreadCreateTime @ 0x1402458a0 (8 bytes)
 * Z3 Proof : VERIFIED_EQUIVALENT (3.55 ms, 33 SMT assertions)
 * Verified : win64_callee_saved_abi, x86_64_to_llvm_ir_equivalence
 */
uint64_t __attribute__((ms_abi)) PsGetThreadCreateTime(uint64_t* arg1) {
    uint64_t __attribute__((unused)) ssa_mem_ld = (uint64_t)((*(uint64_t*)(uintptr_t)(((uint64_t)(((uint64_t)(uintptr_t)arg1)) + (uint64_t)(0x5f0ULL)))));
    return (uint64_t)(ssa_mem_ld);
}

/*
 * Function : PsGetThreadWin32Thread @ 0x1400f4420 (8 bytes)
 * Z3 Proof : VERIFIED_EQUIVALENT (3.75 ms, 33 SMT assertions)
 * Verified : win64_callee_saved_abi, x86_64_to_llvm_ir_equivalence
 */
uint64_t __attribute__((ms_abi)) PsGetThreadWin32Thread(uint64_t* arg1) {
    uint64_t __attribute__((unused)) ssa_mem_ld = (uint64_t)((*(uint64_t*)(uintptr_t)(((uint64_t)(((uint64_t)(uintptr_t)arg1)) + (uint64_t)(0x1c8ULL)))));
    return (uint64_t)(ssa_mem_ld);
}

/*
 * Function : PsReferenceKernelStack @ 0x140583400 (8 bytes)
 * Z3 Proof : VERIFIED_EQUIVALENT (4.12 ms, 34 SMT assertions)
 * Verified : win64_callee_saved_abi, x86_64_to_llvm_ir_equivalence
 */
void __attribute__((ms_abi)) PsReferenceKernelStack(uint32_t* arg1) {
    uint32_t __attribute__((unused)) ssa_mem_ld = (uint32_t)((*(uint32_t*)(uintptr_t)(((uint64_t)(((uint64_t)(uintptr_t)arg1)) + (uint64_t)(0x770ULL)))));
    (*(uint32_t*)(uintptr_t)(((uint64_t)(((uint64_t)(uintptr_t)arg1)) + (uint64_t)(0x770ULL)))) = (uint32_t)(((uint32_t)(ssa_mem_ld) + (uint32_t)(1ULL)));
    return;
}

/*
 * Function : PsSetProcessDxgProcess @ 0x14057dba0 (8 bytes)
 * Z3 Proof : VERIFIED_EQUIVALENT (4.14 ms, 34 SMT assertions)
 * Verified : win64_callee_saved_abi, x86_64_to_llvm_ir_equivalence
 */
void __attribute__((ms_abi)) PsSetProcessDxgProcess(uint64_t* arg1, uint64_t arg2) {
    (*(uint64_t*)(uintptr_t)(((uint64_t)(((uint64_t)(uintptr_t)arg1)) + (uint64_t)(0x7b8ULL)))) = (uint64_t)(((uint64_t)arg2));
    return;
}

/*
 * Function : PsSetProcessWindowStation @ 0x14057fd50 (8 bytes)
 * Z3 Proof : VERIFIED_EQUIVALENT (3.85 ms, 34 SMT assertions)
 * Verified : win64_callee_saved_abi, x86_64_to_llvm_ir_equivalence
 */
void __attribute__((ms_abi)) PsSetProcessWindowStation(uint64_t* arg1, uint64_t arg2) {
    (*(uint64_t*)(uintptr_t)(((uint64_t)(((uint64_t)(uintptr_t)arg1)) + (uint64_t)(0x3d8ULL)))) = (uint64_t)(((uint64_t)arg2));
    return;
}

/*
 * Function : PsIsThreadTerminating @ 0x14007a9f0 (10 bytes)
 * Z3 Proof : VERIFIED_EQUIVALENT (3.95 ms, 33 SMT assertions)
 * Verified : win64_callee_saved_abi, x86_64_to_llvm_ir_equivalence
 */
uint32_t __attribute__((ms_abi)) PsIsThreadTerminating(uint32_t* arg1) {
    uint32_t __attribute__((unused)) ssa_mem_ld = (uint32_t)((*(uint32_t*)(uintptr_t)(((uint64_t)(((uint64_t)(uintptr_t)arg1)) + (uint64_t)(0x6d0ULL)))));
    return (uint32_t)(((uint64_t)(uint32_t)(((uint32_t)(ssa_mem_ld) & (uint32_t)(1ULL)))));
}

/*
 * Function : PsLookupThreadByThreadId @ 0x14051d490 (10 bytes)
 * Z3 Proof : VERIFIED_EQUIVALENT (24.06 ms, 34 SMT assertions)
 * Verified : x86_64_to_llvm_ir_equivalence
 */
void __attribute__((ms_abi)) PsLookupThreadByThreadId(void) {
    return;
}

/*
 * Function : PsSetProcessSecurityPort @ 0x14070cb20 (10 bytes)
 * Z3 Proof : VERIFIED_EQUIVALENT (3.98 ms, 33 SMT assertions)
 * Verified : win64_callee_saved_abi, x86_64_to_llvm_ir_equivalence
 */
uint64_t __attribute__((ms_abi)) PsSetProcessSecurityPort(uint64_t* arg1, uint64_t arg2) {
    (*(uint64_t*)(uintptr_t)(((uint64_t)(((uint64_t)(uintptr_t)arg1)) + (uint64_t)(0x460ULL)))) = (uint64_t)(((uint64_t)arg2));
    return (uint64_t)(0ULL);
}

/*
 * Function : PsIsDiskCountersEnabled @ 0x140136730 (11 bytes)
 * Z3 Proof : VERIFIED_EQUIVALENT (3.67 ms, 33 SMT assertions)
 * Verified : win64_callee_saved_abi, x86_64_to_llvm_ir_equivalence
 */
BOOLEAN __attribute__((ms_abi)) PsIsDiskCountersEnabled(void) {
    uint32_t __attribute__((unused)) ssa_mem_ld = (uint32_t)((*(uint32_t*)(uintptr_t)(((uint64_t)(uintptr_t)&PsDisableDiskCounters))));
    return (BOOLEAN)(((uint64_t)(uint8_t)(((uint32_t)(ssa_mem_ld) == (uint32_t)(0ULL)))));
}

/*
 * Function : PsIsHostSilo @ 0x140055900 (11 bytes)
 * Z3 Proof : VERIFIED_EQUIVALENT (3.52 ms, 33 SMT assertions)
 * Verified : win64_callee_saved_abi, x86_64_to_llvm_ir_equivalence
 */
uint8_t __attribute__((ms_abi)) PsIsHostSilo(uint64_t arg1) {
    return (uint8_t)(((uint64_t)(uint8_t)(((uint64_t)(((uint64_t)arg1)) == (uint64_t)(0ULL)))));
}

/*
 * Function : PsGetProcessExitProcessCalled @ 0x140113000 (12 bytes)
 * Z3 Proof : VERIFIED_EQUIVALENT (4.36 ms, 33 SMT assertions)
 * Verified : win64_callee_saved_abi, x86_64_to_llvm_ir_equivalence
 */
uint32_t __attribute__((ms_abi)) PsGetProcessExitProcessCalled(uint32_t* arg1) {
    uint32_t __attribute__((unused)) ssa_mem_ld = (uint32_t)((*(uint32_t*)(uintptr_t)(((uint64_t)(((uint64_t)(uintptr_t)arg1)) + (uint64_t)(0x304ULL)))));
    return (uint32_t)(((uint64_t)(uint32_t)(((uint32_t)(((uint32_t)(ssa_mem_ld) >> (uint32_t)(2ULL))) & (uint32_t)(0x3fffff01ULL)))));
}

/*
 * Function : PsGetThreadHardErrorsAreDisabled @ 0x14013f120 (12 bytes)
 * Z3 Proof : VERIFIED_EQUIVALENT (4.06 ms, 33 SMT assertions)
 * Verified : win64_callee_saved_abi, x86_64_to_llvm_ir_equivalence
 */
uint8_t __attribute__((ms_abi)) PsGetThreadHardErrorsAreDisabled(uint8_t* arg1) {
    uint8_t __attribute__((unused)) ssa_mem_ld = (uint8_t)((*(uint8_t*)(uintptr_t)(((uint64_t)(((uint64_t)(uintptr_t)arg1)) + (uint64_t)(0x6d0ULL)))));
    return (uint8_t)(((uint64_t)(uint8_t)(((uint8_t)(((uint8_t)(ssa_mem_ld) >> (uint8_t)(4ULL))) & (uint8_t)(1ULL)))));
}

/*
 * Function : PsIsProcessBeingDebugged @ 0x14070c890 (12 bytes)
 * Z3 Proof : VERIFIED_EQUIVALENT (4.12 ms, 33 SMT assertions)
 * Verified : win64_callee_saved_abi, x86_64_to_llvm_ir_equivalence
 */
BOOLEAN __attribute__((ms_abi)) PsIsProcessBeingDebugged(uint64_t* arg1) {
    uint64_t __attribute__((unused)) ssa_mem_ld = (uint64_t)((*(uint64_t*)(uintptr_t)(((uint64_t)(((uint64_t)(uintptr_t)arg1)) + (uint64_t)(0x420ULL)))));
    return (BOOLEAN)(((uint64_t)(uint8_t)(((uint64_t)(ssa_mem_ld) != (uint64_t)(0ULL)))));
}

/*
 * Function : PsIsProcessCommitRelinquished @ 0x14070c8b0 (12 bytes)
 * Z3 Proof : VERIFIED_EQUIVALENT (4.24 ms, 33 SMT assertions)
 * Verified : win64_callee_saved_abi, x86_64_to_llvm_ir_equivalence
 */
uint32_t __attribute__((ms_abi)) PsIsProcessCommitRelinquished(uint32_t* arg1) {
    uint32_t __attribute__((unused)) ssa_mem_ld = (uint32_t)((*(uint32_t*)(uintptr_t)(((uint64_t)(((uint64_t)(uintptr_t)arg1)) + (uint64_t)(0x6ccULL)))));
    return (uint32_t)(((uint64_t)(uint32_t)(((uint32_t)(((uint32_t)(ssa_mem_ld) >> (uint32_t)(8ULL))) & (uint32_t)(0xffff01ULL)))));
}

/*
 * Function : PsIsThreadImpersonating @ 0x1405b7680 (12 bytes)
 * Z3 Proof : VERIFIED_EQUIVALENT (4.00 ms, 33 SMT assertions)
 * Verified : win64_callee_saved_abi, x86_64_to_llvm_ir_equivalence
 */
uint32_t __attribute__((ms_abi)) PsIsThreadImpersonating(uint32_t* arg1) {
    uint32_t __attribute__((unused)) ssa_mem_ld = (uint32_t)((*(uint32_t*)(uintptr_t)(((uint64_t)(((uint64_t)(uintptr_t)arg1)) + (uint64_t)(0x6d0ULL)))));
    return (uint32_t)(((uint64_t)(uint32_t)(((uint32_t)(((uint32_t)(ssa_mem_ld) >> (uint32_t)(3ULL))) & (uint32_t)(0x1fffff01ULL)))));
}

/*
 * Function : PsIsWin32KFilterAuditEnabledForProcess @ 0x14010f8a0 (12 bytes)
 * Z3 Proof : VERIFIED_EQUIVALENT (4.03 ms, 33 SMT assertions)
 * Verified : win64_callee_saved_abi, x86_64_to_llvm_ir_equivalence
 */
uint32_t __attribute__((ms_abi)) PsIsWin32KFilterAuditEnabledForProcess(uint32_t* arg1) {
    uint32_t __attribute__((unused)) ssa_mem_ld = (uint32_t)((*(uint32_t*)(uintptr_t)(((uint64_t)(((uint64_t)(uintptr_t)arg1)) + (uint64_t)(0x828ULL)))));
    return (uint32_t)(((uint64_t)(uint32_t)(((uint32_t)(((uint32_t)(ssa_mem_ld) >> (uint32_t)(15ULL))) & (uint32_t)(0x1ff01ULL)))));
}

/*
 * Function : PsIsWin32KFilterEnabledForProcess @ 0x1401079b0 (12 bytes)
 * Z3 Proof : VERIFIED_EQUIVALENT (4.04 ms, 33 SMT assertions)
 * Verified : win64_callee_saved_abi, x86_64_to_llvm_ir_equivalence
 */
uint32_t __attribute__((ms_abi)) PsIsWin32KFilterEnabledForProcess(uint32_t* arg1) {
    uint32_t __attribute__((unused)) ssa_mem_ld = (uint32_t)((*(uint32_t*)(uintptr_t)(((uint64_t)(((uint64_t)(uintptr_t)arg1)) + (uint64_t)(0x828ULL)))));
    return (uint32_t)(((uint64_t)(uint32_t)(((uint32_t)(((uint32_t)(ssa_mem_ld) >> (uint32_t)(14ULL))) & (uint32_t)(0x3ff01ULL)))));
}

/*
 * Function : PsSetLegoNotifyRoutine @ 0x14070f650 (13 bytes)
 * Z3 Proof : VERIFIED_EQUIVALENT (3.62 ms, 33 SMT assertions)
 * Verified : win64_callee_saved_abi, x86_64_to_llvm_ir_equivalence
 */
uint32_t __attribute__((ms_abi)) PsSetLegoNotifyRoutine(uint64_t arg1) {
    (*(uint64_t*)(uintptr_t)(((uint64_t)(uintptr_t)&PspLegoNotifyRoutine))) = (uint64_t)(((uint64_t)arg1));
    return (uint32_t)(0x2d0ULL);
}

/*
 * Function : PsGetCurrentThreadStackBase @ 0x1402457e0 (14 bytes)
 * Z3 Proof : VERIFIED_EQUIVALENT (4.44 ms, 33 SMT assertions)
 * Verified : win64_callee_saved_abi, x86_64_to_llvm_ir_equivalence
 */
uint64_t __attribute__((ms_abi)) PsGetCurrentThreadStackBase(void) {
    struct _KTHREAD* __attribute__((unused)) ssa_gs_val = KeGetCurrentThread();
    uint64_t __attribute__((unused)) ssa_mem_ld = (uint64_t)(uintptr_t)(ssa_gs_val->StackBase);
    return (uint64_t)(ssa_mem_ld);
}

/*
 * Function : PsGetCurrentThreadStackLimit @ 0x140245800 (14 bytes)
 * Z3 Proof : VERIFIED_EQUIVALENT (4.32 ms, 33 SMT assertions)
 * Verified : win64_callee_saved_abi, x86_64_to_llvm_ir_equivalence
 */
uint64_t __attribute__((ms_abi)) PsGetCurrentThreadStackLimit(void) {
    struct _KTHREAD* __attribute__((unused)) ssa_gs_val = KeGetCurrentThread();
    uint64_t __attribute__((unused)) ssa_mem_ld = (uint64_t)(uintptr_t)(ssa_gs_val->StackLimit);
    return (uint64_t)(ssa_mem_ld);
}

/*
 * Function : PsGetCurrentThreadPreviousMode @ 0x140100c90 (16 bytes)
 * Z3 Proof : VERIFIED_EQUIVALENT (4.20 ms, 33 SMT assertions)
 * Verified : win64_callee_saved_abi, x86_64_to_llvm_ir_equivalence
 */
uint64_t __attribute__((ms_abi)) PsGetCurrentThreadPreviousMode(void) {
    struct _KTHREAD* __attribute__((unused)) ssa_gs_val = KeGetCurrentThread();
    uint8_t __attribute__((unused)) ssa_mem_ld = (uint8_t)(ssa_gs_val->PreviousMode);
    return (uint64_t)(((uint64_t)(((uint64_t)(((uint64_t)(uintptr_t)ssa_gs_val)) & (uint64_t)(0xffffffffffffff00ULL))) | (uint64_t)(((uint64_t)(uint8_t)(ssa_mem_ld)))));
}

/*
 * Function : PsGetJobSessionId @ 0x140245830 (16 bytes)
 * Z3 Proof : VERIFIED_EQUIVALENT (24.36 ms, 33 SMT assertions)
 * Verified : win64_callee_saved_abi, x86_64_to_llvm_ir_equivalence
 */
uint32_t __attribute__((ms_abi)) PsGetJobSessionId(uint32_t* arg1) {
    uint32_t __attribute__((unused)) ssa_mem_ld = (uint32_t)((*(uint32_t*)(uintptr_t)(((uint64_t)(((uint64_t)(uintptr_t)arg1)) + (uint64_t)(0x1e0ULL)))));
    return (uint32_t)(((uint64_t)(uint32_t)(((((uint32_t)(ssa_mem_ld) > (uint32_t)(0xfffffffffffffffdULL))) ? (0xffffffffffffffffULL) : (ssa_mem_ld)))));
}

/*
 * Function : PsDetachSiloFromCurrentThread @ 0x1400478a0 (17 bytes)
 * Z3 Proof : VERIFIED_EQUIVALENT (4.62 ms, 33 SMT assertions)
 * Verified : win64_callee_saved_abi, x86_64_to_llvm_ir_equivalence
 */
uint64_t __attribute__((ms_abi)) PsDetachSiloFromCurrentThread(uint64_t arg1) {
    struct _KTHREAD* __attribute__((unused)) ssa_gs_val = KeGetCurrentThread();
    (*(uint64_t*)((uint8_t*)(uintptr_t)(ssa_gs_val) + 0x7d0)) = (uint64_t)(((uint64_t)arg1));
    return (uint64_t)(((uint64_t)(uintptr_t)ssa_gs_val));
}

/*
 * Function : PsGetCurrentProcessId @ 0x1400bdf30 (17 bytes)
 * Z3 Proof : VERIFIED_EQUIVALENT (4.60 ms, 33 SMT assertions)
 * Verified : win64_callee_saved_abi, x86_64_to_llvm_ir_equivalence
 */
uint64_t __attribute__((ms_abi)) PsGetCurrentProcessId(void) {
    struct _KTHREAD* __attribute__((unused)) ssa_gs_val = KeGetCurrentThread();
    uint64_t __attribute__((unused)) ssa_mem_ld = (uint64_t)((*(uint64_t*)((uint8_t*)(uintptr_t)(ssa_gs_val) + 0x638)));
    return (uint64_t)(ssa_mem_ld);
}

/*
 * Function : PsGetCurrentThreadId @ 0x1400fcf60 (17 bytes)
 * Z3 Proof : VERIFIED_EQUIVALENT (4.25 ms, 33 SMT assertions)
 * Verified : win64_callee_saved_abi, x86_64_to_llvm_ir_equivalence
 */
uint64_t __attribute__((ms_abi)) PsGetCurrentThreadId(void) {
    struct _KTHREAD* __attribute__((unused)) ssa_gs_val = KeGetCurrentThread();
    uint64_t __attribute__((unused)) ssa_mem_ld = (uint64_t)((*(uint64_t*)((uint8_t*)(uintptr_t)(ssa_gs_val) + 0x640)));
    return (uint64_t)(ssa_mem_ld);
}

/*
 * Function : PsGetCurrentThreadProcess @ 0x140140dd0 (17 bytes)
 * Z3 Proof : VERIFIED_EQUIVALENT (4.35 ms, 33 SMT assertions)
 * Verified : win64_callee_saved_abi, x86_64_to_llvm_ir_equivalence
 */
uint64_t __attribute__((ms_abi)) PsGetCurrentThreadProcess(void) {
    struct _KTHREAD* __attribute__((unused)) ssa_gs_val = KeGetCurrentThread();
    struct _KPROCESS* __attribute__((unused)) ssa_mem_ld = (struct _KPROCESS*)(uintptr_t)(ssa_gs_val->Process);
    return (uint64_t)(((uint64_t)(uintptr_t)ssa_mem_ld));
}

/*
 * Function : PsGetCurrentThreadWin32Thread @ 0x1400ed700 (17 bytes)
 * Z3 Proof : VERIFIED_EQUIVALENT (4.26 ms, 33 SMT assertions)
 * Verified : win64_callee_saved_abi, x86_64_to_llvm_ir_equivalence
 */
uint64_t __attribute__((ms_abi)) PsGetCurrentThreadWin32Thread(void) {
    struct _KTHREAD* __attribute__((unused)) ssa_gs_val = KeGetCurrentThread();
    uint64_t __attribute__((unused)) ssa_mem_ld = (uint64_t)(uintptr_t)(ssa_gs_val->Win32Thread);
    return (uint64_t)(ssa_mem_ld);
}

/*
 * Function : PsIsCurrentThreadPrefetching @ 0x1400fc3e0 (21 bytes)
 * Z3 Proof : VERIFIED_EQUIVALENT (4.77 ms, 33 SMT assertions)
 * Verified : win64_callee_saved_abi, x86_64_to_llvm_ir_equivalence
 */
uint64_t __attribute__((ms_abi)) PsIsCurrentThreadPrefetching(void) {
    struct _KTHREAD* __attribute__((unused)) ssa_gs_val = KeGetCurrentThread();
    uint8_t __attribute__((unused)) ssa_mem_ld = (uint8_t)((*(uint8_t*)((uint8_t*)(uintptr_t)(ssa_gs_val) + 0x6d8)));
    return (uint64_t)(((uint64_t)(((uint64_t)(((uint64_t)(uintptr_t)ssa_gs_val)) & (uint64_t)(0xffffffffffffff00ULL))) | (uint64_t)(((uint64_t)(uint8_t)(((uint8_t)(((uint8_t)(ssa_mem_ld) >> (uint8_t)(6ULL))) & (uint8_t)(1ULL)))))));
}

/*
 * Function : PsSetThreadHardErrorsAreDisabled @ 0x14013de10 (22 bytes)
 * Z3 Proof : VERIFIED_EQUIVALENT (16.97 ms, 34 SMT assertions)
 * Verified : win64_callee_saved_abi, x86_64_to_llvm_ir_equivalence
 */
void __attribute__((ms_abi)) PsSetThreadHardErrorsAreDisabled(uint32_t* arg1, uint8_t arg2) {
    uint32_t __attribute__((unused)) ssa_mem_ld_1 = (uint32_t)((*(uint32_t*)(uintptr_t)(((uint64_t)(((uint64_t)(uintptr_t)arg1)) + (uint64_t)(0x6d0ULL)))));
    (*(uint32_t*)(uintptr_t)(((uint64_t)(((uint64_t)(uintptr_t)arg1)) + (uint64_t)(0x6d0ULL)))) = (uint32_t)(((uint32_t)(((uint32_t)(ssa_mem_ld_1) & (uint32_t)(0xffffffffffffffefULL))) | (uint32_t)(((((uint8_t)(((uint8_t)(uint64_t)(((uint64_t)arg2)))) == (uint8_t)(0ULL))) ? (0ULL) : (0x10ULL)))));
    return;
}

/*
 * Function : PsGetWin32KFilterSet @ 0x1400fed00 (23 bytes)
 * Z3 Proof : VERIFIED_EQUIVALENT (4.96 ms, 33 SMT assertions)
 * Verified : win64_callee_saved_abi, x86_64_to_llvm_ir_equivalence
 */
uint64_t __attribute__((ms_abi)) PsGetWin32KFilterSet(void) {
    struct _KTHREAD* __attribute__((unused)) ssa_gs_val = KeGetCurrentThread();
    struct _KPROCESS* __attribute__((unused)) ssa_mem_ld = (struct _KPROCESS*)(uintptr_t)(ssa_gs_val->ApcState.Process);
    uint32_t __attribute__((unused)) ssa_mem_ld_1 = (uint32_t)((*(uint32_t*)((uint8_t*)(uintptr_t)(ssa_mem_ld) + 0x7c0)));
    return (uint64_t)(((uint64_t)(uint32_t)(ssa_mem_ld_1)));
}

/*
 * Function : PsRestoreImpersonation @ 0x14051c8a0 (23 bytes)
 * Z3 Proof : VERIFIED_EQUIVALENT (22.90 ms, 34 SMT assertions)
 * Verified : x86_64_to_llvm_ir_equivalence
 */
void __attribute__((ms_abi)) PsRestoreImpersonation(void) {
    struct _KTHREAD* __attribute__((unused)) ssa_gs_val = KeGetCurrentThread();
    return;
}

/*
 * Function : PsWow64GetProcessMachine @ 0x1404e9dd0 (23 bytes)
 * Z3 Proof : VERIFIED_EQUIVALENT (4.68 ms, 33 SMT assertions)
 * Verified : win64_callee_saved_abi, x86_64_to_llvm_ir_equivalence
 */
uint64_t __attribute__((ms_abi)) PsWow64GetProcessMachine(uint64_t* arg1) {
    uint64_t __attribute__((unused)) ssa_common_ret_op = 0;
    uint64_t __attribute__((unused)) ssa_mem_ld = (uint64_t)((*(uint64_t*)(uintptr_t)(((uint64_t)(((uint64_t)(uintptr_t)arg1)) + (uint64_t)(0x428ULL)))));
    if (((uint64_t)(ssa_mem_ld) == (uint64_t)(0ULL))) {
        ssa_common_ret_op = (uint64_t)(0x8664ULL);
    } else {
        uint16_t __attribute__((unused)) ssa_mem_ld_1 = (uint16_t)((*(uint16_t*)(uintptr_t)(((uint64_t)(ssa_mem_ld) + (uint64_t)(8ULL)))));
        ssa_common_ret_op = (uint64_t)(((uint64_t)(uint16_t)(ssa_mem_ld_1)));
    }
    return (uint64_t)(ssa_common_ret_op);
}

/*
 * Function : PsAttachSiloToCurrentThread @ 0x140047880 (24 bytes)
 * Z3 Proof : VERIFIED_EQUIVALENT (5.39 ms, 33 SMT assertions)
 * Verified : win64_callee_saved_abi, x86_64_to_llvm_ir_equivalence
 */
uint64_t __attribute__((ms_abi)) PsAttachSiloToCurrentThread(uint64_t arg1) {
    struct _KTHREAD* __attribute__((unused)) ssa_gs_val = KeGetCurrentThread();
    uint64_t __attribute__((unused)) ssa_mem_ld = (uint64_t)((*(uint64_t*)((uint8_t*)(uintptr_t)(ssa_gs_val) + 0x7d0)));
    (*(uint64_t*)((uint8_t*)(uintptr_t)(ssa_gs_val) + 0x7d0)) = (uint64_t)(((uint64_t)arg1));
    return (uint64_t)(ssa_mem_ld);
}

/*
 * Function : PsGetCurrentProcessWin32Process @ 0x1400f34c0 (24 bytes)
 * Z3 Proof : VERIFIED_EQUIVALENT (5.10 ms, 33 SMT assertions)
 * Verified : win64_callee_saved_abi, x86_64_to_llvm_ir_equivalence
 */
uint64_t __attribute__((ms_abi)) PsGetCurrentProcessWin32Process(void) {
    struct _KTHREAD* __attribute__((unused)) ssa_gs_val = KeGetCurrentThread();
    struct _KPROCESS* __attribute__((unused)) ssa_mem_ld = (struct _KPROCESS*)(uintptr_t)(ssa_gs_val->ApcState.Process);
    uint64_t __attribute__((unused)) ssa_mem_ld_1 = (uint64_t)((*(uint64_t*)((uint8_t*)(uintptr_t)(ssa_mem_ld) + 0x3a8)));
    return (uint64_t)(ssa_mem_ld_1);
}

/*
 * Function : PsGetProcessExitTime @ 0x14056e760 (24 bytes)
 * Z3 Proof : VERIFIED_EQUIVALENT (5.23 ms, 33 SMT assertions)
 * Verified : win64_callee_saved_abi, x86_64_to_llvm_ir_equivalence
 */
uint64_t __attribute__((ms_abi)) PsGetProcessExitTime(void) {
    struct _KTHREAD* __attribute__((unused)) ssa_gs_val = KeGetCurrentThread();
    struct _KPROCESS* __attribute__((unused)) ssa_mem_ld = (struct _KPROCESS*)(uintptr_t)(ssa_gs_val->ApcState.Process);
    uint64_t __attribute__((unused)) ssa_mem_ld_1 = (uint64_t)((*(uint64_t*)((uint8_t*)(uintptr_t)(ssa_mem_ld) + 0x690)));
    return (uint64_t)(ssa_mem_ld_1);
}

/*
 * Function : PsGetProcessStartKey @ 0x14004bd80 (24 bytes)
 * Z3 Proof : VERIFIED_EQUIVALENT (4.44 ms, 33 SMT assertions)
 * Verified : win64_callee_saved_abi, x86_64_to_llvm_ir_equivalence
 */
uint64_t __attribute__((ms_abi)) PsGetProcessStartKey(uint64_t* arg1) {
    uint32_t __attribute__((unused)) ssa_mem_ld = (uint32_t)((*(uint32_t*)(uintptr_t)(0xfffff780000002c4ULL)));
    uint64_t __attribute__((unused)) ssa_mem_ld_1 = (uint64_t)((*(uint64_t*)(uintptr_t)(((uint64_t)(((uint64_t)(uintptr_t)arg1)) + (uint64_t)(0x750ULL)))));
    return (uint64_t)(((uint64_t)(((uint64_t)(((uint64_t)(uint32_t)(ssa_mem_ld))) << (uint64_t)(0x30ULL))) | (uint64_t)(ssa_mem_ld_1)));
}

/*
 * Function : PsChargeProcessNonPagedPoolQuota @ 0x1400a5080 (26 bytes)
 * Z3 Proof : VERIFIED_EQUIVALENT (6.69 ms, 33 SMT assertions)
 * Verified : win64_callee_saved_abi, x86_64_to_llvm_ir_equivalence
 */
uint64_t __attribute__((ms_abi)) PsChargeProcessNonPagedPoolQuota(uint64_t arg1, uint64_t arg2) {
    return (uint64_t)(0ULL);
}

/*
 * Function : PsGetThreadFreezeCount @ 0x140114120 (26 bytes)
 * Z3 Proof : VERIFIED_EQUIVALENT (5.47 ms, 33 SMT assertions)
 * Verified : win64_callee_saved_abi, x86_64_to_llvm_ir_equivalence
 */
uint32_t __attribute__((ms_abi)) PsGetThreadFreezeCount(uint64_t* arg1, PKPROCESS arg2) {
    uint64_t __attribute__((unused)) ssa_mem_ld = (uint64_t)((*(uint64_t*)(uintptr_t)(((uint64_t)(((uint64_t)(uintptr_t)arg1)) + (uint64_t)(0x220ULL)))));
    uint32_t __attribute__((unused)) ssa_mem_ld_1 = (uint32_t)((*(uint32_t*)(uintptr_t)(((uint64_t)(ssa_mem_ld) + (uint64_t)(0x1b8ULL)))));
    uint32_t __attribute__((unused)) ssa_mem_ld_2 = (uint32_t)((*(uint32_t*)(uintptr_t)(((uint64_t)(ssa_mem_ld) + (uint64_t)(0x268ULL)))));
    return (uint32_t)(((uint64_t)(uint32_t)(((uint32_t)(((uint32_t)(((uint32_t)(ssa_mem_ld_1) >> (uint32_t)(3ULL))) & (uint32_t)(1ULL))) + (uint32_t)(ssa_mem_ld_2)))));
}

/*
 * Function : PsIsWin32KFilterAuditEnabled @ 0x14010f870 (28 bytes)
 * Z3 Proof : VERIFIED_EQUIVALENT (5.35 ms, 33 SMT assertions)
 * Verified : win64_callee_saved_abi, x86_64_to_llvm_ir_equivalence
 */
uint64_t __attribute__((ms_abi)) PsIsWin32KFilterAuditEnabled(void) {
    struct _KTHREAD* __attribute__((unused)) ssa_gs_val = KeGetCurrentThread();
    struct _KPROCESS* __attribute__((unused)) ssa_mem_ld = (struct _KPROCESS*)(uintptr_t)(ssa_gs_val->ApcState.Process);
    uint32_t __attribute__((unused)) ssa_mem_ld_1 = (uint32_t)((*(uint32_t*)((uint8_t*)(uintptr_t)(ssa_mem_ld) + 0x828)));
    return (uint64_t)(((uint64_t)(uint32_t)(((uint32_t)(((uint32_t)(ssa_mem_ld_1) >> (uint32_t)(15ULL))) & (uint32_t)(0x1ff01ULL)))));
}

/*
 * Function : PsIsWin32KFilterEnabled @ 0x140107980 (28 bytes)
 * Z3 Proof : VERIFIED_EQUIVALENT (5.37 ms, 33 SMT assertions)
 * Verified : win64_callee_saved_abi, x86_64_to_llvm_ir_equivalence
 */
uint64_t __attribute__((ms_abi)) PsIsWin32KFilterEnabled(void) {
    struct _KTHREAD* __attribute__((unused)) ssa_gs_val = KeGetCurrentThread();
    struct _KPROCESS* __attribute__((unused)) ssa_mem_ld = (struct _KPROCESS*)(uintptr_t)(ssa_gs_val->ApcState.Process);
    uint32_t __attribute__((unused)) ssa_mem_ld_1 = (uint32_t)((*(uint32_t*)((uint8_t*)(uintptr_t)(ssa_mem_ld) + 0x828)));
    return (uint64_t)(((uint64_t)(uint32_t)(((uint32_t)(((uint32_t)(ssa_mem_ld_1) >> (uint32_t)(14ULL))) & (uint32_t)(0x3ff01ULL)))));
}

/*
 * Function : PsEnterPriorityRegion @ 0x1400f3520 (31 bytes)
 * Z3 Proof : VERIFIED_EQUIVALENT (4.28 ms, 33 SMT assertions)
 * Verified : win64_callee_saved_abi, x86_64_to_llvm_ir_equivalence
 */
uint64_t __attribute__((ms_abi)) PsEnterPriorityRegion(void) {
    struct _KTHREAD* __attribute__((unused)) ssa_gs_val = KeGetCurrentThread();
    uint64_t __attribute__((unused)) ssa_call_res = PsBoostThreadIoEx(((uint64_t)(uintptr_t)ssa_gs_val), 0ULL, 0ULL, 0ULL);
    return (uint64_t)(ssa_call_res);
}

/*
 * Function : PsGetCurrentThreadWin32ThreadAndEnterCriticalRegion @ 0x1400eb9c0 (34 bytes)
 * Z3 Proof : VERIFIED_EQUIVALENT (6.71 ms, 33 SMT assertions)
 * Verified : win64_callee_saved_abi, x86_64_to_llvm_ir_equivalence
 */
uint64_t __attribute__((ms_abi)) PsGetCurrentThreadWin32ThreadAndEnterCriticalRegion(uint64_t* arg1) {
    struct _KTHREAD* __attribute__((unused)) ssa_gs_val = KeGetCurrentThread();
    uint64_t __attribute__((unused)) ssa_mem_ld = (uint64_t)((*(uint64_t*)((uint8_t*)(uintptr_t)(ssa_gs_val) + 0x638)));
    (*(uint64_t*)(uintptr_t)(((uint64_t)(uintptr_t)arg1))) = (uint64_t)(ssa_mem_ld);
    uint16_t __attribute__((unused)) ssa_mem_ld_1 = (uint16_t)(ssa_gs_val->KernelApcDisable);
    ssa_gs_val->KernelApcDisable = (int16_t)(((uint16_t)(ssa_mem_ld_1) + (uint16_t)(0xffffffffffffffffULL)));
    uint64_t __attribute__((unused)) ssa_mem_ld_2 = (uint64_t)(uintptr_t)(ssa_gs_val->Win32Thread);
    return (uint64_t)(ssa_mem_ld_2);
}
