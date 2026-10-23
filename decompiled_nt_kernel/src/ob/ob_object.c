/*
 * Reconstructed Windows NT Kernel Subsystem: Executive Object Manager & Cookie Type Table (Ob)
 * Source Binary : ntoskrnl.exe (10.0.16299.15 x64, RSDS 83DB4240-4EFD-4AB6-AFB6-FA864B700CB3)
 * Functions     : 5 Z3-verified / assembly-preserved routines
 */
#include "../../include/nt_reconstructed_types.h"

/*
 * Function : ObGetObjectType @ 0x14056d640 (40 bytes)
 * Z3 Proof : VERIFIED_EQUIVALENT (22.32 ms, 33 SMT assertions)
 * Verified : win64_callee_saved_abi, x86_64_to_llvm_ir_equivalence
 */
struct _OBJECT_TYPE* __attribute__((ms_abi)) ObGetObjectType(void* Object) {
    uint8_t __attribute__((unused)) ssa_mem_ld = (uint8_t)((CONTAINING_RECORD(Object, struct _OBJECT_HEADER, Body))->TypeIndex);
    uint8_t __attribute__((unused)) ssa_mem_ld_1 = (uint8_t)((*(uint8_t*)(uintptr_t)(((uint64_t)(uintptr_t)&ObHeaderCookie))));
    uint64_t __attribute__((unused)) ssa_mem_ld_2 = (uint64_t)((*(uint64_t*)(uintptr_t)(((uint64_t)(((uint64_t)(uintptr_t)&ObTypeIndexTable)) + (uint64_t)(((uint64_t)(uint32_t)(((uint32_t)(((uint32_t)(((uint32_t)(((uint32_t)(((uint32_t)(((uint32_t)(((uint32_t)(uint64_t)(((uint64_t)(uintptr_t)Object)))) + (uint32_t)(0xffd0ULL))) >> (uint32_t)(8ULL))) & (uint32_t)(0xffULL))) ^ (uint32_t)(((uint32_t)(uint8_t)(ssa_mem_ld))))) ^ (uint32_t)(((uint32_t)(uint8_t)(ssa_mem_ld_1))))) << (uint32_t)(3ULL)))))))));
    return (struct _OBJECT_TYPE*)(uintptr_t)(ssa_mem_ld_2);
}

/*
 * Function : ObGetFilterVersion @ 0x1406e73d0 (6 bytes)
 * Z3 Proof : VERIFIED_EQUIVALENT (2.78 ms, 33 SMT assertions)
 * Verified : win64_callee_saved_abi, x86_64_to_llvm_ir_equivalence
 */
uint16_t __attribute__((ms_abi)) ObGetFilterVersion(void) {
    return (uint16_t)(0x100ULL);
}

/*
 * Function : ObQueryNameInfo @ 0x140566180 (39 bytes)
 * Z3 Proof : VERIFIED_EQUIVALENT (4.95 ms, 33 SMT assertions)
 * Verified : win64_callee_saved_abi, x86_64_to_llvm_ir_equivalence
 */
void* __attribute__((ms_abi)) ObQueryNameInfo(void* Object) {
    uint64_t __attribute__((unused)) ssa_reg_rcx_0 = 0;
    uint8_t __attribute__((unused)) ssa_mem_ld = (uint8_t)((CONTAINING_RECORD(Object, struct _OBJECT_HEADER, Body))->InfoMask);
    if (((uint8_t)(((uint8_t)(ssa_mem_ld) & (uint8_t)(2ULL))) == (uint8_t)(0ULL))) {
        ssa_reg_rcx_0 = (uint64_t)(0ULL);
    } else {
        uint8_t __attribute__((unused)) ssa_mem_ld_1 = (uint8_t)((*(uint8_t*)(uintptr_t)(((uint64_t)(((uint64_t)(uintptr_t)&ObpInfoMaskToOffset)) + (uint64_t)(((uint64_t)(uint8_t)(((uint8_t)(ssa_mem_ld) & (uint8_t)(3ULL)))))))));
        ssa_reg_rcx_0 = (uint64_t)(((uint64_t)(((uint64_t)(((uint64_t)(uintptr_t)Object)) + (uint64_t)(0xffffffffffffffd0ULL))) - (uint64_t)(((uint64_t)(uint8_t)(ssa_mem_ld_1)))));
    }
    return (void*)(uintptr_t)(ssa_reg_rcx_0);
}

/*
 * Function : ObDereferenceObject @ 0x140047050 (19 bytes)
 * Z3 Proof : VERIFIED_EQUIVALENT (3.44 ms, 33 SMT assertions)
 * Verified : win64_callee_saved_abi, x86_64_to_llvm_ir_equivalence
 */
int64_t __attribute__((ms_abi)) ObDereferenceObject(void* Object) {
    uint64_t __attribute__((unused)) ssa_call_res = (uint64_t)ObfDereferenceObjectWithTag((void*)(uintptr_t)(((uint64_t)(uintptr_t)Object)), (ULONG)(0x746c6644ULL));
    return (int64_t)(ssa_call_res);
}

/*
 * Function : ObIsKernelHandle @ 0x140230bb0 (16 bytes)
 * Z3 Proof : VERIFIED_EQUIVALENT (3.29 ms, 33 SMT assertions)
 * Verified : win64_callee_saved_abi, x86_64_to_llvm_ir_equivalence
 */
BOOLEAN __attribute__((ms_abi)) ObIsKernelHandle(void* Handle) {
    uint64_t __attribute__((unused)) ssa_call_res = (uint64_t)ObpIsKernelHandle((void*)(uintptr_t)(((uint64_t)(uintptr_t)Handle)), (BOOLEAN)(0ULL));
    return (BOOLEAN)(ssa_call_res);
}
