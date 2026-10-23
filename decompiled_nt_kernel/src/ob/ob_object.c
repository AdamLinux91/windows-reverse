/*
 * Reconstructed Windows NT Kernel Subsystem: Executive Object Manager & Cookie Type Table (Ob/Obp)
 * Source Binary : ntoskrnl.exe (10.0.16299.15 x64, RSDS 83DB4240-4EFD-4AB6-AFB6-FA864B700CB3)
 * Functions     : 13 Z3-verified / assembly-preserved routines
 */
#include "../../include/nt_reconstructed_types.h"

/*
 * Function : ObGetObjectType @ 0x14056d640 (40 bytes)
 * Z3 Proof : VERIFIED_EQUIVALENT (21.31 ms, 33 SMT assertions)
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
 * Z3 Proof : VERIFIED_EQUIVALENT (3.08 ms, 33 SMT assertions)
 * Verified : win64_callee_saved_abi, x86_64_to_llvm_ir_equivalence
 */
uint16_t __attribute__((ms_abi)) ObGetFilterVersion(void) {
    return (uint16_t)(0x100ULL);
}

/*
 * Function : ObQueryNameInfo @ 0x140566180 (39 bytes)
 * Z3 Proof : VERIFIED_EQUIVALENT (5.43 ms, 33 SMT assertions)
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
 * Z3 Proof : VERIFIED_EQUIVALENT (3.59 ms, 33 SMT assertions)
 * Verified : win64_callee_saved_abi, x86_64_to_llvm_ir_equivalence
 */
int64_t __attribute__((ms_abi)) ObDereferenceObject(void* Object) {
    uint64_t __attribute__((unused)) ssa_call_res = (uint64_t)ObfDereferenceObjectWithTag((void*)(uintptr_t)(((uint64_t)(uintptr_t)Object)), (ULONG)(0x746c6644ULL));
    return (int64_t)(ssa_call_res);
}

/*
 * Function : ObIsKernelHandle @ 0x140230bb0 (16 bytes)
 * Z3 Proof : VERIFIED_EQUIVALENT (3.62 ms, 33 SMT assertions)
 * Verified : win64_callee_saved_abi, x86_64_to_llvm_ir_equivalence
 */
BOOLEAN __attribute__((ms_abi)) ObIsKernelHandle(void* Handle) {
    uint64_t __attribute__((unused)) ssa_call_res = (uint64_t)ObpIsKernelHandle((void*)(uintptr_t)(((uint64_t)(uintptr_t)Handle)), (BOOLEAN)(0ULL));
    return (BOOLEAN)(ssa_call_res);
}

/*
 * Function : ObReferenceSecurityDescriptor @ 0x14056acf0 (18 bytes)
 * Z3 Proof : VERIFIED_EQUIVALENT (5.20 ms, 33 SMT assertions)
 * Verified : win64_callee_saved_abi, x86_64_to_llvm_ir_equivalence
 */
int32_t __attribute__((ms_abi)) ObReferenceSecurityDescriptor(uint64_t* arg1, uint32_t arg2) {
    uint64_t __attribute__((unused)) ssa_mem_ld = (uint64_t)((*(uint64_t*)(uintptr_t)(((uint64_t)(((uint64_t)(uintptr_t)arg1)) + (uint64_t)(0xfffffffffffffff0ULL)))));
    (*(uint64_t*)(uintptr_t)(((uint64_t)(((uint64_t)(uintptr_t)arg1)) + (uint64_t)(0xfffffffffffffff0ULL)))) = (uint64_t)(((uint64_t)(ssa_mem_ld) + (uint64_t)(((uint64_t)(((uint64_t)arg2)) & (uint64_t)(0xffffffffULL)))));
    return (int32_t)(ssa_mem_ld);
}

/*
 * Function : ObCreateObjectType @ 0x1405a61b0 (22 bytes)
 * Z3 Proof : VERIFIED_EQUIVALENT (3.97 ms, 33 SMT assertions)
 * Verified : win64_callee_saved_abi, x86_64_to_llvm_ir_equivalence
 */
uint64_t __attribute__((ms_abi)) ObCreateObjectType(uint64_t arg1, uint64_t arg2, uint64_t arg3, uint64_t arg4) {
    uint64_t __attribute__((unused)) ssa_call_res = ObCreateObjectTypeEx(((uint64_t)arg1), ((uint64_t)arg2), ((uint64_t)arg3), 0ULL);
    return (uint64_t)(ssa_call_res);
}

/*
 * Function : ObpLookupDirectoryEntry @ 0x1405a69e4 (27 bytes)
 * Z3 Proof : VERIFIED_EQUIVALENT (4.00 ms, 33 SMT assertions)
 * Verified : win64_callee_saved_abi, x86_64_to_llvm_ir_equivalence
 */
uint64_t __attribute__((ms_abi)) ObpLookupDirectoryEntry(uint64_t arg1, uint64_t arg2, uint64_t arg3, uint64_t arg4) {
    uint64_t __attribute__((unused)) ssa_call_res = ObpLookupDirectoryEntryEx(((uint64_t)arg1), ((uint64_t)arg2), ((uint64_t)arg3), 0ULL);
    return (uint64_t)(ssa_call_res);
}

/*
 * Function : ObpIncrPointerCount @ 0x1400af260 (32 bytes)
 * Z3 Proof : VERIFIED_EQUIVALENT (34.58 ms, 33 SMT assertions)
 * Verified : x86_64_to_llvm_ir_equivalence
 */
int64_t __attribute__((ms_abi)) ObpIncrPointerCount(PAUX_ACCESS_DATA arg1, PRTL_DYNAMIC_HASH_TABLE arg2) {
    uint64_t __attribute__((unused)) ssa_common_ret_op = 0;
    struct _PRIVILEGE_SET* __attribute__((unused)) ssa_mem_ld = (struct _PRIVILEGE_SET*)(uintptr_t)(arg1->PrivilegesUsed);
    uint64_t __attribute__((unused)) ssa__42 = (uint64_t)(((uint64_t)(((uint64_t)(uintptr_t)ssa_mem_ld)) + (uint64_t)(1ULL)));
    arg1->PrivilegesUsed = (struct _PRIVILEGE_SET*)(uintptr_t)(ssa__42);
    if (((((uint64_t)(((uint64_t)(uintptr_t)ssa_mem_ld)) == (uint64_t)(0ULL))) || (((int64_t)(((uint64_t)(ssa__42) | (uint64_t)(((uint64_t)(uintptr_t)ssa_mem_ld)))) < (int64_t)(0ULL))))) {
        uint64_t __attribute__((unused)) ssa_call_res = KeBugCheckEx(0x18ULL, 0ULL, ((uint64_t)(((uint64_t)arg1)) + (uint64_t)(0x30ULL)), 0x10ULL);
        ssa_common_ret_op = (uint64_t)(ssa_call_res);
    }
    else {
        ssa_common_ret_op = (uint64_t)(ssa__42);
    }
    return (int64_t)(ssa_common_ret_op);
}

/*
 * Function : ObpIncrPointerCountEx @ 0x1400618c8 (32 bytes)
 * Z3 Proof : VERIFIED_EQUIVALENT (67.39 ms, 33 SMT assertions)
 * Verified : x86_64_to_llvm_ir_equivalence
 */
int64_t __attribute__((ms_abi)) ObpIncrPointerCountEx(PAUX_ACCESS_DATA arg1, PRTL_DYNAMIC_HASH_TABLE arg2) {
    uint64_t __attribute__((unused)) ssa_common_ret_op = 0;
    struct _PRIVILEGE_SET* __attribute__((unused)) ssa_mem_ld = (struct _PRIVILEGE_SET*)(uintptr_t)(arg1->PrivilegesUsed);
    uint64_t __attribute__((unused)) ssa__43 = (uint64_t)(((uint64_t)(((uint64_t)(uintptr_t)ssa_mem_ld)) + (uint64_t)(((int64_t)(((uint64_t)(((uint64_t)arg2)) << (uint64_t)(0x20ULL))) >> (int64_t)(0x20ULL)))));
    arg1->PrivilegesUsed = (struct _PRIVILEGE_SET*)(uintptr_t)(ssa__43);
    if (((int64_t)(((uint64_t)(uintptr_t)ssa_mem_ld)) < (int64_t)(1ULL))) {
        uint64_t __attribute__((unused)) ssa_call_res = KeBugCheckEx(0x18ULL, 0ULL, ((uint64_t)(((uint64_t)arg1)) + (uint64_t)(0x30ULL)), 0x10ULL);
        ssa_common_ret_op = (uint64_t)(ssa_call_res);
    }
    else {
        ssa_common_ret_op = (uint64_t)(ssa__43);
    }
    return (int64_t)(ssa_common_ret_op);
}

/*
 * Function : ObpDirectoryTeardownCallback @ 0x140780d10 (36 bytes)
 * Z3 Proof : VERIFIED_EQUIVALENT (6.59 ms, 33 SMT assertions)
 * Verified : win64_callee_saved_abi, x86_64_to_llvm_ir_equivalence
 */
uint64_t __attribute__((ms_abi)) ObpDirectoryTeardownCallback(uint64_t* arg1) {
    uint64_t __attribute__((unused)) ssa_mem_ld = (uint64_t)((*(uint64_t*)(uintptr_t)(((uint64_t)(uintptr_t)arg1))));
    uint64_t __attribute__((unused)) ssa_call_res = ObMakeTemporaryObject(ssa_mem_ld, 0ULL, 0ULL, 0ULL);
    uint64_t __attribute__((unused)) ssa_mem_ld_1 = (uint64_t)((*(uint64_t*)(uintptr_t)(((uint64_t)(uintptr_t)arg1))));
    uint64_t __attribute__((unused)) ssa_call_res_1 = (uint64_t)ObfDereferenceObjectWithTag((void*)(uintptr_t)(ssa_mem_ld_1), (ULONG)(0x7254624fULL));
    return (uint64_t)(ssa_call_res_1);
}

/*
 * Function : ObDeassignSecurity @ 0x1404e8380 (37 bytes)
 * Z3 Proof : VERIFIED_EQUIVALENT (6.28 ms, 33 SMT assertions)
 * Verified : win64_callee_saved_abi, x86_64_to_llvm_ir_equivalence
 */
uint64_t __attribute__((ms_abi)) ObDeassignSecurity(uint64_t* arg1) {
    uint64_t __attribute__((unused)) ssa_mem_ld = (uint64_t)((*(uint64_t*)(uintptr_t)(((uint64_t)(uintptr_t)arg1))));
    (*(uint64_t*)(uintptr_t)(((uint64_t)(uintptr_t)arg1))) = (uint64_t)(0ULL);
    uint64_t __attribute__((unused)) ssa_call_res = ObDereferenceSecurityDescriptor(((uint64_t)(ssa_mem_ld) & (uint64_t)(0xfffffffffffffff0ULL)), ((uint64_t)(uint32_t)(((uint32_t)(((uint32_t)(((uint32_t)(uint64_t)(ssa_mem_ld))) & (uint32_t)(15ULL))) + (uint32_t)(1ULL)))), 0ULL, 0ULL);
    return (uint64_t)(0ULL);
}

/*
 * Function : ObpReferenceNamedObject @ 0x140047904 (54 bytes)
 * Z3 Proof : VERIFIED_EQUIVALENT (8.80 ms, 33 SMT assertions)
 * Verified : win64_callee_saved_abi, x86_64_to_llvm_ir_equivalence
 */
uint32_t __attribute__((ms_abi)) ObpReferenceNamedObject(uint64_t arg1) {
    uint64_t __attribute__((unused)) ssa_call_res = ObfReferenceObjectWithTag(((uint64_t)(((uint64_t)arg1)) + (uint64_t)(0x30ULL)), 0x746c6644ULL, 0ULL, 0ULL);
    uint8_t __attribute__((unused)) ssa_mem_ld = (uint8_t)((*(uint8_t*)(uintptr_t)(((uint64_t)(((uint64_t)arg1)) + (uint64_t)(0x1aULL)))));
    uint8_t __attribute__((unused)) ssa_mem_ld_1 = (uint8_t)((*(uint8_t*)(uintptr_t)(((uint64_t)(((uint64_t)(uintptr_t)&ObpInfoMaskToOffset)) + (uint64_t)(((uint64_t)(uint8_t)(((uint8_t)(ssa_mem_ld) & (uint8_t)(3ULL)))))))));
    uint64_t __attribute__((unused)) ssa__79 = (uint64_t)(((uint64_t)(uint8_t)(ssa_mem_ld_1)));
    uint32_t __attribute__((unused)) ssa_mem_ld_2 = (uint32_t)((*(uint32_t*)(uintptr_t)(((uint64_t)(((uint64_t)(((uint64_t)arg1)) + (uint64_t)(0x18ULL))) - (uint64_t)(ssa__79)))));
    (*(uint32_t*)(uintptr_t)(((uint64_t)(((uint64_t)(((uint64_t)arg1)) + (uint64_t)(0x18ULL))) - (uint64_t)(ssa__79)))) = (uint32_t)(((uint32_t)(ssa_mem_ld_2) + (uint32_t)(1ULL)));
    return (uint32_t)(ssa__79);
}
