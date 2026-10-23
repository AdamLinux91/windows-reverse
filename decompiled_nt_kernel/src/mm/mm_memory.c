/*
 * Reconstructed Windows NT Kernel Subsystem: Virtual Memory Manager & MDL Subsystem (Mm)
 * Source Binary : ntoskrnl.exe (10.0.16299.15 x64, RSDS 83DB4240-4EFD-4AB6-AFB6-FA864B700CB3)
 * Functions     : 4 Z3-verified / assembly-preserved routines
 */
#include "../../include/nt_reconstructed_types.h"

/*
 * Function : MmSizeOfMdl @ 0x1400fcd80 (29 bytes)
 * Z3 Proof : VERIFIED_EQUIVALENT (9.92 ms, 33 SMT assertions)
 * Verified : win64_callee_saved_abi, x86_64_to_llvm_ir_equivalence
 */
size_t __attribute__((ms_abi)) MmSizeOfMdl(void* Base, size_t Length) {
    return (size_t)(((uint64_t)(((uint64_t)(((uint64_t)(((uint64_t)(((uint64_t)(((uint64_t)Length)) + (uint64_t)(0xfffULL))) + (uint64_t)(((uint64_t)(((uint64_t)(uintptr_t)Base)) & (uint64_t)(0xfffULL))))) >> (uint64_t)(9ULL))) & (uint64_t)(0x7ffffffffffff8ULL))) + (uint64_t)(0x30ULL)));
}

/*
 * Function : MmGetMaximumFileSectionSize @ 0x1405de6b0 (11 bytes)
 * Z3 Proof : VERIFIED_EQUIVALENT (2.95 ms, 33 SMT assertions)
 * Verified : win64_callee_saved_abi, x86_64_to_llvm_ir_equivalence
 */
uint64_t __attribute__((ms_abi)) MmGetMaximumFileSectionSize(void) {
    return (uint64_t)(0x3ffffffffff000ULL);
}

/*
 * Function : MmIsAddressValid @ 0x140212850 (14 bytes)
 * Z3 Proof : VERIFIED_EQUIVALENT (3.32 ms, 33 SMT assertions)
 * Verified : win64_callee_saved_abi, x86_64_to_llvm_ir_equivalence
 */
BOOLEAN __attribute__((ms_abi)) MmIsAddressValid(void* VirtualAddress) {
    uint64_t __attribute__((unused)) ssa_call_res = (uint64_t)MiIsAddressValid((void*)(uintptr_t)(((uint64_t)(uintptr_t)VirtualAddress)));
    return (BOOLEAN)(ssa_call_res);
}

/*
 * Function : MmFreePagesFromMdl @ 0x14002ec20 (16 bytes)
 * Z3 Proof : VERIFIED_EQUIVALENT (3.93 ms, 34 SMT assertions)
 * Verified : win64_callee_saved_abi, x86_64_to_llvm_ir_equivalence
 */
void __attribute__((ms_abi)) MmFreePagesFromMdl(struct _MDL* MemoryDescriptorList) {
    uint64_t __attribute__((unused)) ssa_call_res = MiFreePagesFromMdl(((uint64_t)(uintptr_t)MemoryDescriptorList), 0ULL, 0ULL, 0ULL);
    return;
}
