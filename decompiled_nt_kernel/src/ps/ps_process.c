/*
 * Reconstructed Windows NT Kernel Subsystem: Process & Thread Structure Executive (Ps)
 * Source Binary : ntoskrnl.exe (10.0.16299.15 x64, RSDS 83DB4240-4EFD-4AB6-AFB6-FA864B700CB3)
 * Functions     : 16 Z3-verified / assembly-preserved routines
 */
#include "../../include/nt_reconstructed_types.h"

/*
 * Function : PsGetCurrentProcess @ 0x1400e9990 (17 bytes)
 * Z3 Proof : VERIFIED_EQUIVALENT (5.42 ms, 33 SMT assertions)
 * Verified : win64_callee_saved_abi, x86_64_to_llvm_ir_equivalence
 */
struct _EPROCESS* __attribute__((ms_abi)) PsGetCurrentProcess(void) {
    struct _KTHREAD* __attribute__((unused)) ssa_gs_val = KeGetCurrentThread();
    struct _KPROCESS* __attribute__((unused)) ssa_mem_ld = (struct _KPROCESS*)(uintptr_t)(ssa_gs_val->ApcState.Process);
    return (struct _EPROCESS*)(uintptr_t)(((uint64_t)(uintptr_t)ssa_mem_ld));
}

/*
 * Function : PsGetCurrentThread @ 0x14015f4e0 (10 bytes)
 * Z3 Proof : VERIFIED_EQUIVALENT (3.84 ms, 33 SMT assertions)
 * Verified : win64_callee_saved_abi, x86_64_to_llvm_ir_equivalence
 */
struct _ETHREAD* __attribute__((ms_abi)) PsGetCurrentThread(void) {
    struct _KTHREAD* __attribute__((unused)) ssa_gs_val = KeGetCurrentThread();
    return (struct _ETHREAD*)(uintptr_t)(((uint64_t)(uintptr_t)ssa_gs_val));
}

/*
 * Function : PsGetProcessId @ 0x1400bdce0 (8 bytes)
 * Z3 Proof : VERIFIED_EQUIVALENT (4.24 ms, 33 SMT assertions)
 * Verified : win64_callee_saved_abi, x86_64_to_llvm_ir_equivalence
 */
void* __attribute__((ms_abi)) PsGetProcessId(struct _EPROCESS* Process) {
    uint64_t __attribute__((unused)) ssa_mem_ld = (uint64_t)(uintptr_t)(Process->UniqueProcessId);
    return (void*)(uintptr_t)(ssa_mem_ld);
}

/*
 * Function : PsGetProcessInheritedFromUniqueProcessId @ 0x14007d2f0 (8 bytes)
 * Z3 Proof : VERIFIED_EQUIVALENT (3.90 ms, 33 SMT assertions)
 * Verified : win64_callee_saved_abi, x86_64_to_llvm_ir_equivalence
 */
void* __attribute__((ms_abi)) PsGetProcessInheritedFromUniqueProcessId(struct _EPROCESS* Process) {
    uint64_t __attribute__((unused)) ssa_mem_ld = (uint64_t)(uintptr_t)(Process->InheritedFromUniqueProcessId);
    return (void*)(uintptr_t)(ssa_mem_ld);
}

/*
 * Function : PsGetProcessImageFileName @ 0x140109830 (8 bytes)
 * Z3 Proof : VERIFIED_EQUIVALENT (3.99 ms, 33 SMT assertions)
 * Verified : win64_callee_saved_abi, x86_64_to_llvm_ir_equivalence
 */
uint8_t* __attribute__((ms_abi)) PsGetProcessImageFileName(struct _EPROCESS* Process) {
    return (uint8_t*)(uintptr_t)(((uint64_t)(((uint64_t)(uintptr_t)Process)) + (uint64_t)(0x450ULL)));
}

/*
 * Function : PsGetProcessExitStatus @ 0x14070c880 (7 bytes)
 * Z3 Proof : VERIFIED_EQUIVALENT (3.65 ms, 33 SMT assertions)
 * Verified : win64_callee_saved_abi, x86_64_to_llvm_ir_equivalence
 */
NTSTATUS __attribute__((ms_abi)) PsGetProcessExitStatus(struct _EPROCESS* Process) {
    uint32_t __attribute__((unused)) ssa_mem_ld = (uint32_t)(Process->ExitStatus);
    return (NTSTATUS)(((uint64_t)(uint32_t)(ssa_mem_ld)));
}

/*
 * Function : PsGetProcessPeb @ 0x1401085e0 (8 bytes)
 * Z3 Proof : VERIFIED_EQUIVALENT (4.21 ms, 33 SMT assertions)
 * Verified : win64_callee_saved_abi, x86_64_to_llvm_ir_equivalence
 */
void* __attribute__((ms_abi)) PsGetProcessPeb(struct _EPROCESS* Process) {
    struct _PEB* __attribute__((unused)) ssa_mem_ld = (struct _PEB*)(uintptr_t)(Process->Peb);
    return (void*)(uintptr_t)(((uint64_t)(uintptr_t)ssa_mem_ld));
}

/*
 * Function : PsGetProcessSectionBaseAddress @ 0x14004c7d0 (8 bytes)
 * Z3 Proof : VERIFIED_EQUIVALENT (3.93 ms, 33 SMT assertions)
 * Verified : win64_callee_saved_abi, x86_64_to_llvm_ir_equivalence
 */
void* __attribute__((ms_abi)) PsGetProcessSectionBaseAddress(struct _EPROCESS* Process) {
    uint64_t __attribute__((unused)) ssa_mem_ld = (uint64_t)(uintptr_t)(Process->SectionBaseAddress);
    return (void*)(uintptr_t)(ssa_mem_ld);
}

/*
 * Function : PsIsSystemProcess @ 0x1400b7d00 (12 bytes)
 * Z3 Proof : VERIFIED_EQUIVALENT (4.17 ms, 33 SMT assertions)
 * Verified : win64_callee_saved_abi, x86_64_to_llvm_ir_equivalence
 */
BOOLEAN __attribute__((ms_abi)) PsIsSystemProcess(struct _EPROCESS* Process) {
    uint32_t __attribute__((unused)) ssa_mem_ld = (uint32_t)(Process->Flags3);
    return (BOOLEAN)(((uint64_t)(uint32_t)(((uint32_t)(((uint32_t)(ssa_mem_ld) >> (uint32_t)(12ULL))) & (uint32_t)(0xfff01ULL)))));
}

/*
 * Function : PsIsProtectedProcess @ 0x14004c7e0 (16 bytes)
 * Z3 Proof : VERIFIED_EQUIVALENT (3.72 ms, 33 SMT assertions)
 * Verified : win64_callee_saved_abi, x86_64_to_llvm_ir_equivalence
 */
BOOLEAN __attribute__((ms_abi)) PsIsProtectedProcess(struct _EPROCESS* Process) {
    uint8_t __attribute__((unused)) ssa_mem_ld = (uint8_t)(Process->Protection.Level);
    return (BOOLEAN)(((uint64_t)(uint8_t)(((uint8_t)(((uint8_t)(ssa_mem_ld) & (uint8_t)(7ULL))) != (uint8_t)(0ULL)))));
}

/*
 * Function : PsIsProtectedProcessLight @ 0x140028eb0 (18 bytes)
 * Z3 Proof : VERIFIED_EQUIVALENT (3.74 ms, 33 SMT assertions)
 * Verified : win64_callee_saved_abi, x86_64_to_llvm_ir_equivalence
 */
BOOLEAN __attribute__((ms_abi)) PsIsProtectedProcessLight(struct _EPROCESS* Process) {
    uint8_t __attribute__((unused)) ssa_mem_ld = (uint8_t)(Process->Protection.Level);
    return (BOOLEAN)(((uint64_t)(uint8_t)(((uint8_t)(((uint8_t)(ssa_mem_ld) & (uint8_t)(7ULL))) == (uint8_t)(1ULL)))));
}

/*
 * Function : PsGetThreadId @ 0x1401063d0 (8 bytes)
 * Z3 Proof : VERIFIED_EQUIVALENT (3.82 ms, 33 SMT assertions)
 * Verified : win64_callee_saved_abi, x86_64_to_llvm_ir_equivalence
 */
void* __attribute__((ms_abi)) PsGetThreadId(struct _ETHREAD* Thread) {
    uint64_t __attribute__((unused)) ssa_mem_ld = (uint64_t)(uintptr_t)(Thread->Cid.UniqueThread);
    return (void*)(uintptr_t)(ssa_mem_ld);
}

/*
 * Function : PsGetThreadProcess @ 0x140050520 (8 bytes)
 * Z3 Proof : VERIFIED_EQUIVALENT (3.88 ms, 33 SMT assertions)
 * Verified : win64_callee_saved_abi, x86_64_to_llvm_ir_equivalence
 */
struct _EPROCESS* __attribute__((ms_abi)) PsGetThreadProcess(struct _KTHREAD* Thread) {
    struct _KPROCESS* __attribute__((unused)) ssa_mem_ld = (struct _KPROCESS*)(uintptr_t)(Thread->Process);
    return (struct _EPROCESS*)(uintptr_t)(((uint64_t)(uintptr_t)ssa_mem_ld));
}

/*
 * Function : PsGetThreadProcessId @ 0x14010af10 (8 bytes)
 * Z3 Proof : VERIFIED_EQUIVALENT (4.17 ms, 33 SMT assertions)
 * Verified : win64_callee_saved_abi, x86_64_to_llvm_ir_equivalence
 */
void* __attribute__((ms_abi)) PsGetThreadProcessId(struct _ETHREAD* Thread) {
    uint64_t __attribute__((unused)) ssa_mem_ld = (uint64_t)(uintptr_t)(Thread->Cid.UniqueProcess);
    return (void*)(uintptr_t)(ssa_mem_ld);
}

/*
 * Function : PsGetThreadTeb @ 0x1400f72a0 (8 bytes)
 * Z3 Proof : VERIFIED_EQUIVALENT (3.81 ms, 33 SMT assertions)
 * Verified : win64_callee_saved_abi, x86_64_to_llvm_ir_equivalence
 */
void* __attribute__((ms_abi)) PsGetThreadTeb(struct _KTHREAD* Thread) {
    uint64_t __attribute__((unused)) ssa_mem_ld = (uint64_t)(uintptr_t)(Thread->Teb);
    return (void*)(uintptr_t)(ssa_mem_ld);
}

/*
 * Function : PsIsSystemThread @ 0x1401091e0 (9 bytes)
 * Z3 Proof : VERIFIED_EQUIVALENT (4.16 ms, 33 SMT assertions)
 * Verified : win64_callee_saved_abi, x86_64_to_llvm_ir_equivalence
 */
BOOLEAN __attribute__((ms_abi)) PsIsSystemThread(struct _KTHREAD* Thread) {
    uint32_t __attribute__((unused)) ssa_mem_ld = (uint32_t)(Thread->MiscFlags);
    return (BOOLEAN)(((uint64_t)(uint32_t)(((uint32_t)(((uint32_t)(ssa_mem_ld) >> (uint32_t)(10ULL))) & (uint32_t)(0x3fff01ULL)))));
}
