/*
 * Reconstructed Windows NT Kernel Subsystem: Asynchronous Local IPC Message Attributes & Headers (Alpc/Lpc)
 * Source Binary : ntoskrnl.exe (10.0.16299.15 x64, RSDS 83DB4240-4EFD-4AB6-AFB6-FA864B700CB3)
 * Functions     : 8 Z3-verified / assembly-preserved routines
 */
#include "../../include/nt_reconstructed_types.h"

/*
 * Function : AlpcpLockForCachedReferenceBlob @ 0x1404fdfdc (55 bytes)
 * Z3 Proof : VERIFIED_EQUIVALENT (57.59 ms, 33 SMT assertions)
 * Verified : x86_64_to_llvm_ir_equivalence
 */
int64_t __attribute__((ms_abi)) AlpcpLockForCachedReferenceBlob(uint64_t arg1) {
    uint64_t __attribute__((unused)) ssa_common_ret_op = 0;
    uint64_t __attribute__((unused)) ssa_call_res = ExAcquirePushLockExclusiveEx(((uint64_t)(((uint64_t)arg1)) + (uint64_t)(0xfffffffffffffff0ULL)), 0ULL, 0ULL, 0ULL);
    uint8_t __attribute__((unused)) ssa_mem_ld = (uint8_t)((*(uint8_t*)(uintptr_t)(((uint64_t)(((uint64_t)arg1)) + (uint64_t)(0xffffffffffffffe0ULL)))));
    (*(uint8_t*)(uintptr_t)(((uint64_t)(((uint64_t)arg1)) + (uint64_t)(0xffffffffffffffe0ULL)))) = (uint8_t)(((uint8_t)(ssa_mem_ld) | (uint8_t)(1ULL)));
    uint64_t __attribute__((unused)) ssa_mem_ld_1 = (uint64_t)((*(uint64_t*)(uintptr_t)(((uint64_t)(((uint64_t)arg1)) + (uint64_t)(0xffffffffffffffe8ULL)))));
    uint64_t __attribute__((unused)) ssa__85 = (uint64_t)(((uint64_t)(ssa_mem_ld_1) + (uint64_t)(0x10000ULL)));
    (*(uint64_t*)(uintptr_t)(((uint64_t)(((uint64_t)arg1)) + (uint64_t)(0xffffffffffffffe8ULL)))) = (uint64_t)(ssa__85);
    if (((int64_t)(ssa__85) < (int64_t)(1ULL))) {
        uint64_t __attribute__((unused)) ssa_call_res_1 = KeBugCheckEx(0x18ULL, 0ULL, ((uint64_t)arg1), 0x26ULL);
        ssa_common_ret_op = (uint64_t)(ssa_call_res_1);
    }
    else {
        ssa_common_ret_op = (uint64_t)(ssa__85);
    }
    return (int64_t)(ssa_common_ret_op);
}

/*
 * Function : AlpcpSetOwnerPortMessage @ 0x14048ed98 (75 bytes)
 * Z3 Proof : VERIFIED_EQUIVALENT (13.91 ms, 33 SMT assertions)
 * Verified : win64_callee_saved_abi, x86_64_to_llvm_ir_equivalence
 */
uint32_t __attribute__((ms_abi)) AlpcpSetOwnerPortMessage(uint64_t arg1, uint64_t arg2, uint8_t arg3) {
    if (((uint8_t)(((uint8_t)(uint64_t)(((uint64_t)arg3)))) == (uint8_t)(0ULL))) {
    } else {
        uint64_t __attribute__((unused)) ssa_call_res = ObfReferenceObject(((uint64_t)arg2), ((uint64_t)arg2), ((uint64_t)arg3), 0ULL);
    }
    uint32_t __attribute__((unused)) ssa_mem_ld_1 = (uint32_t)((*(uint32_t*)(uintptr_t)(((uint64_t)(((uint64_t)arg1)) + (uint64_t)(0x28ULL)))));
    uint32_t __attribute__((unused)) ssa__104 = (uint32_t)(((uint32_t)(((uint32_t)(ssa_mem_ld_1) ^ (uint32_t)(((uint32_t)(((uint32_t)(uint64_t)(((uint64_t)arg3)))) << (uint32_t)(12ULL))))) & (uint32_t)(0x1000ULL)));
    (*(uint64_t*)(uintptr_t)(((uint64_t)(((uint64_t)arg1)) + (uint64_t)(0x18ULL)))) = (uint64_t)(((uint64_t)arg2));
    uint32_t __attribute__((unused)) ssa_mem_ld_2 = (uint32_t)((*(uint32_t*)(uintptr_t)(((uint64_t)(((uint64_t)arg1)) + (uint64_t)(0x28ULL)))));
    (*(uint32_t*)(uintptr_t)(((uint64_t)(((uint64_t)arg1)) + (uint64_t)(0x28ULL)))) = (uint32_t)(((uint32_t)(ssa_mem_ld_2) ^ (uint32_t)(ssa__104)));
    return (uint32_t)(((uint64_t)(uint32_t)(ssa__104)));
}

/*
 * Function : AlpcGetHeaderSize @ 0x14004d1f0 (78 bytes)
 * Z3 Proof : VERIFIED_EQUIVALENT (25.23 ms, 33 SMT assertions)
 * Verified : win64_callee_saved_abi, x86_64_to_llvm_ir_equivalence
 */
int32_t __attribute__((ms_abi)) AlpcGetHeaderSize(PRTL_DYNAMIC_HASH_TABLE arg1) {
    uint32_t __attribute__((unused)) ssa_r_ecx = (uint32_t)(((uint32_t)(uint64_t)(((uint64_t)arg1))));
    uint32_t __attribute__((unused)) ssa__94 = (uint32_t)(((uint32_t)(((uint32_t)(((((int32_t)(ssa_r_ecx) < (int32_t)(0ULL))) ? (0x20ULL) : (8ULL))) + (uint32_t)(((uint32_t)(((uint32_t)(ssa_r_ecx) >> (uint32_t)(0x19ULL))) & (uint32_t)(0x20ULL))))) + (uint32_t)(((uint32_t)(((uint32_t)(ssa_r_ecx) >> (uint32_t)(0x18ULL))) & (uint32_t)(0x20ULL)))));
    uint32_t __attribute__((unused)) ssa__111 = (uint32_t)(((((uint32_t)(((uint32_t)(ssa_r_ecx) & (uint32_t)(0x10000000ULL))) == (uint32_t)(0ULL))) ? (ssa__94) : (((uint32_t)(ssa__94) + (uint32_t)(0x18ULL)))));
    return (int32_t)(((uint64_t)(uint32_t)(((uint32_t)(((uint32_t)(((uint32_t)(((uint32_t)(ssa_r_ecx) >> (uint32_t)(0x16ULL))) & (uint32_t)(8ULL))) + (uint32_t)(((uint32_t)(((uint32_t)(ssa_r_ecx) >> (uint32_t)(0x17ULL))) & (uint32_t)(8ULL))))) + (uint32_t)(((((uint32_t)(((uint32_t)(ssa_r_ecx) & (uint32_t)(0x8000000ULL))) == (uint32_t)(0ULL))) ? (ssa__111) : (((uint32_t)(ssa__111) + (uint32_t)(0x18ULL)))))))));
}

/*
 * Function : AlpcRegionDestroyProcedure @ 0x140568a90 (83 bytes)
 * Z3 Proof : VERIFIED_EQUIVALENT (12.76 ms, 33 SMT assertions)
 * Verified : win64_callee_saved_abi, x86_64_to_llvm_ir_equivalence
 */
uint64_t __attribute__((ms_abi)) AlpcRegionDestroyProcedure(uint64_t* arg1, PLIST_ENTRY arg2) {
    uint64_t __attribute__((unused)) ssa_mem_ld = (uint64_t)((*(uint64_t*)(uintptr_t)(((uint64_t)(((uint64_t)(uintptr_t)arg1)) + (uint64_t)(0x10ULL)))));
    uint64_t __attribute__((unused)) ssa_call_res = (uint64_t)AlpcpLockForCachedReferenceBlob((uint64_t)(ssa_mem_ld));
    uint64_t __attribute__((unused)) ssa_mem_ld_1 = (uint64_t)((*(uint64_t*)(uintptr_t)(((uint64_t)(((uint64_t)(uintptr_t)arg1)) + (uint64_t)(8ULL)))));
    uint64_t __attribute__((unused)) ssa_mem_ld_2 = (uint64_t)((*(uint64_t*)(uintptr_t)(((uint64_t)(uintptr_t)arg1))));
    (*(uint64_t*)(uintptr_t)(ssa_mem_ld_1)) = (uint64_t)(ssa_mem_ld_2);
    uint64_t __attribute__((unused)) ssa_mem_ld_3 = (uint64_t)((*(uint64_t*)(uintptr_t)(((uint64_t)(uintptr_t)arg1))));
    uint64_t __attribute__((unused)) ssa_mem_ld_4 = (uint64_t)((*(uint64_t*)(uintptr_t)(((uint64_t)(((uint64_t)(uintptr_t)arg1)) + (uint64_t)(8ULL)))));
    (*(uint64_t*)(uintptr_t)(((uint64_t)(ssa_mem_ld_3) + (uint64_t)(8ULL)))) = (uint64_t)(ssa_mem_ld_4);
    uint32_t __attribute__((unused)) ssa_mem_ld_5 = (uint32_t)((*(uint32_t*)(uintptr_t)(((uint64_t)(ssa_mem_ld) + (uint64_t)(0x34ULL)))));
    (*(uint32_t*)(uintptr_t)(((uint64_t)(ssa_mem_ld) + (uint64_t)(0x34ULL)))) = (uint32_t)(((uint32_t)(ssa_mem_ld_5) + (uint32_t)(0xffffffffffffffffULL)));
    uint64_t __attribute__((unused)) ssa_call_res_1 = AlpcpUnlockBlob(ssa_mem_ld, ssa_mem_ld_3, 0ULL, 0ULL);
    uint64_t __attribute__((unused)) ssa_call_res_2 = AlpcpDereferenceBlobEx(ssa_mem_ld, 1ULL, 0ULL, 0ULL);
    return (uint64_t)(0ULL);
}

extern void __nt_asm_ext_14005ffd0(void);
extern void __nt_asm_ext_1400c45a0(void);
extern void __nt_asm_ext_140013438(void);

/*
 * [PRESERVED NT KERNEL ASSEMBLY ROUTINE] AlpcpFreeCompletionPacketLookaside @ 0x1400133d8
 * Automated non-C detection prevented destructive C structural forcing:
 *   - Privileged CPU/CSR instruction 'mov cr8, rax' at 0x140013409
 *   - Privileged/control-register routine (1/24 insns) preserved as native x86-64 assembly
 *   Live-In Registers : rbx, rdi, rcx
 *   Clobbered State   : rdi, rdx, rbx, rcx, rax, memory, cc
 */
uint32_t __attribute__((naked, ms_abi)) AlpcpFreeCompletionPacketLookaside(uint64_t arg1) {
    __asm__ __volatile__ (
        ".intel_syntax noprefix\n\t"
        "mov qword ptr [rsp + 8], rbx\n\t"
        "push rdi\n\t"
        "sub rsp, 0x40\n\t"
        "lea rdx, [rsp + 0x20]\n\t"
        "mov rbx, rcx\n\t"
        "call __nt_asm_ext_14005ffd0\n\t"
        "cmp dword ptr [rbx + 0xc], 0\n\t"
        "jne .L_AlpcpFreeCompletionPacketLookaside_140013424\n\t"
        ".L_AlpcpFreeCompletionPacketLookaside_1400133f5:\n\t"
        "mov edi, 1\n\t"
        ".L_AlpcpFreeCompletionPacketLookaside_1400133fa:\n\t"
        "lea rcx, [rsp + 0x20]\n\t"
        "call __nt_asm_ext_1400c45a0\n\t"
        "movzx eax, byte ptr [rsp + 0x30]\n\t"
        "mov cr8, rax\n\t"
        "test edi, edi\n\t"
        "je .L_AlpcpFreeCompletionPacketLookaside_140013419\n\t"
        ".L_AlpcpFreeCompletionPacketLookaside_140013411:\n\t"
        "mov rcx, rbx\n\t"
        "call __nt_asm_ext_140013438\n\t"
        ".L_AlpcpFreeCompletionPacketLookaside_140013419:\n\t"
        "mov rbx, qword ptr [rsp + 0x50]\n\t"
        "add rsp, 0x40\n\t"
        "pop rdi\n\t"
        "ret\n\t"
        ".L_AlpcpFreeCompletionPacketLookaside_140013424:\n\t"
        "xor edi, edi\n\t"
        "mov dword ptr [rbx + 0x18], 1\n\t"
        "jmp .L_AlpcpFreeCompletionPacketLookaside_1400133fa\n\t"
        ".att_syntax prefix"
    );
}

/*
 * Function : AlpcpGetMessageAttributeOffset @ 0x1405560cc (95 bytes)
 * Z3 Proof : VERIFIED_EQUIVALENT (38.22 ms, 33 SMT assertions)
 * Verified : win64_callee_saved_abi, x86_64_to_llvm_ir_equivalence
 */
int64_t __attribute__((ms_abi)) AlpcpGetMessageAttributeOffset(uint64_t* arg1, PKDPC arg2) {
    uint32_t __attribute__((unused)) ssa__36 = (uint32_t)(((uint32_t)(((uint32_t)(((uint32_t)(uint64_t)(((uint64_t)arg2)))) * (uint32_t)(0xfffffffffffffffeULL))) & (uint32_t)(((uint32_t)(uint64_t)(((uint64_t)(uintptr_t)arg1))))));
    uint64_t __attribute__((unused)) ssa__106 = (uint64_t)(((uint64_t)(uint32_t)(((uint32_t)(((uint32_t)(((((int32_t)(ssa__36) < (int32_t)(0ULL))) ? (0x20ULL) : (8ULL))) + (uint32_t)(((uint32_t)(((uint32_t)(ssa__36) >> (uint32_t)(0x19ULL))) & (uint32_t)(0x20ULL))))) + (uint32_t)(((uint32_t)(((uint32_t)(ssa__36) >> (uint32_t)(0x18ULL))) & (uint32_t)(0x20ULL)))))));
    uint64_t __attribute__((unused)) ssa__120 = (uint64_t)(((((uint32_t)(((uint32_t)(ssa__36) & (uint32_t)(0x10000000ULL))) == (uint32_t)(0ULL))) ? (ssa__106) : (((uint64_t)(ssa__106) + (uint64_t)(0x18ULL)))));
    return (int64_t)(((uint64_t)(((((uint32_t)(((uint32_t)(ssa__36) & (uint32_t)(0x8000000ULL))) == (uint32_t)(0ULL))) ? (ssa__120) : (((uint64_t)(ssa__120) + (uint64_t)(0x18ULL))))) + (uint64_t)(((uint64_t)(uint32_t)(((uint32_t)(((uint32_t)(ssa__36) >> (uint32_t)(0x16ULL))) & (uint32_t)(8ULL)))))));
}

/*
 * Function : AlpcpGetMessageAttributeOffset32 @ 0x1405682f4 (95 bytes)
 * Z3 Proof : VERIFIED_EQUIVALENT (39.95 ms, 33 SMT assertions)
 * Verified : win64_callee_saved_abi, x86_64_to_llvm_ir_equivalence
 */
int64_t __attribute__((ms_abi)) AlpcpGetMessageAttributeOffset32(uint64_t* arg1, struct _RECON_ALPCPGETMESSAGEATTRIBUTEOFFSET32_RDX* arg2) {
    uint32_t __attribute__((unused)) ssa__36 = (uint32_t)(((uint32_t)(((uint32_t)(((uint32_t)(uint64_t)(((uint64_t)(uintptr_t)arg2)))) * (uint32_t)(0xfffffffffffffffeULL))) & (uint32_t)(((uint32_t)(uint64_t)(((uint64_t)(uintptr_t)arg1))))));
    uint64_t __attribute__((unused)) ssa__92 = (uint64_t)(((uint64_t)(uint32_t)(((uint32_t)(((((int32_t)(ssa__36) < (int32_t)(0ULL))) ? (0x14ULL) : (8ULL))) + (uint32_t)(((uint32_t)(((uint32_t)(ssa__36) >> (uint32_t)(0x1aULL))) & (uint32_t)(0x10ULL)))))));
    uint64_t __attribute__((unused)) ssa__120 = (uint64_t)(((uint64_t)(((((uint32_t)(((uint32_t)(ssa__36) & (uint32_t)(0x20000000ULL))) == (uint32_t)(0ULL))) ? (ssa__92) : (((uint64_t)(ssa__92) + (uint64_t)(0x14ULL))))) + (uint64_t)(((uint64_t)(uint32_t)(((uint32_t)(((uint32_t)(ssa__36) >> (uint32_t)(0x18ULL))) & (uint32_t)(0x10ULL)))))));
    return (int64_t)(((uint64_t)(((((uint32_t)(((uint32_t)(ssa__36) & (uint32_t)(0x8000000ULL))) == (uint32_t)(0ULL))) ? (ssa__120) : (((uint64_t)(ssa__120) + (uint64_t)(0x18ULL))))) + (uint64_t)(((uint64_t)(uint32_t)(((uint32_t)(((uint32_t)(ssa__36) >> (uint32_t)(0x16ULL))) & (uint32_t)(8ULL)))))));
}

/*
 * Function : LpcRequestPort @ 0x1404fead0 (100 bytes)
 * Z3 Proof : VERIFIED_EQUIVALENT (126.53 ms, 33 SMT assertions)
 * Verified : x86_64_to_llvm_ir_equivalence
 */
uint64_t __attribute__((ms_abi)) LpcRequestPort(uint64_t arg1, uint32_t* arg2) {
    uint8_t __attribute__((aligned(16))) ssa_stack_frame[1024];
    (*(uint64_t*)(uintptr_t)((((uint64_t)(uintptr_t)ssa_stack_frame) + 776ULL))) = (uint64_t)(0ULL);
    (*(uint64_t*)(uintptr_t)((((uint64_t)(uintptr_t)ssa_stack_frame) + 760ULL))) = (uint64_t)(0ULL);
    uint64_t __attribute__((unused)) ssa_call_res = memset((((uint64_t)(uintptr_t)ssa_stack_frame) + 704ULL), 0ULL, 0x38ULL, 0ULL);
    struct _KTHREAD* __attribute__((unused)) ssa_gs_val = KeGetCurrentThread();
    uint16_t __attribute__((unused)) ssa_mem_ld = (uint16_t)(ssa_gs_val->KernelApcDisable);
    ssa_gs_val->KernelApcDisable = (int16_t)(((uint16_t)(ssa_mem_ld) + (uint16_t)(0xffffffffffffffffULL)));
    (*(uint64_t*)(uintptr_t)((((uint64_t)(uintptr_t)ssa_stack_frame) + 696ULL))) = (uint64_t)(((uint64_t)arg1));
    (*(uint32_t*)(uintptr_t)((((uint64_t)(uintptr_t)ssa_stack_frame) + 744ULL))) = (uint32_t)(0x10002ULL);
    uint64_t __attribute__((unused)) ssa_call_res_1 = AlpcpSendMessage((((uint64_t)(uintptr_t)ssa_stack_frame) + 696ULL), ((uint64_t)(uintptr_t)arg2), 0ULL, 0ULL);
    uint64_t __attribute__((unused)) ssa_call_res_2 = KeLeaveCriticalRegion((((uint64_t)(uintptr_t)ssa_stack_frame) + 696ULL), ((uint64_t)(uintptr_t)arg2), 0ULL, 0ULL);
    return (uint64_t)(((uint64_t)(ssa_call_res_1) & (uint64_t)(0xffffffffULL)));
}
