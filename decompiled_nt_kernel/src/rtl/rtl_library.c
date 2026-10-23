/*
 * Reconstructed Windows NT Kernel Subsystem: Kernel Run-Time Library, Bitmaps & Time Epoch Math (Rtl)
 * Source Binary : ntoskrnl.exe (10.0.16299.15 x64, RSDS 83DB4240-4EFD-4AB6-AFB6-FA864B700CB3)
 * Functions     : 11 Z3-verified / assembly-preserved routines
 */
#include "../../include/nt_reconstructed_types.h"

/*
 * [PRESERVED NT KERNEL ASSEMBLY ROUTINE] RtlCompareMemory @ 0x140169a60
 * Automated non-C detection prevented destructive C structural forcing:
 *   - Hand-coded x86-64 kernel assembly instruction 'repe cmpsb' at 0x140169a82
 *   - Hand-coded x86-64 kernel assembly instruction 'repe cmpsq' at 0x140169a96
 *   - Hand-coded x86-64 kernel assembly instruction 'repe cmpsb' at 0x140169ab2
 *   - Hand-coded x86-64 kernel assembly instruction 'repe cmpsb' at 0x140169aca
 *   Live-In Registers : rdi, rsi, rcx, rdx, r8
 *   Clobbered State   : rdi, rsi, rdx, r9, rcx, r8, rax, memory, cc
 */
size_t __attribute__((naked, ms_abi)) RtlCompareMemory(const void* Source1, const void* Source2, size_t Length) {
    __asm__ __volatile__ (
        ".intel_syntax noprefix\n\t"
        "push rdi\n\t"
        "push rsi\n\t"
        "mov rsi, rcx\n\t"
        "mov rdi, rdx\n\t"
        "xor edx, ecx\n\t"
        "and edx, 7\n\t"
        "jne .L_RtlCompareMemory_140169ac2\n\t"
        ".L_RtlCompareMemory_140169a6f:\n\t"
        "cmp r8, 8\n\t"
        "jb .L_RtlCompareMemory_140169ac2\n\t"
        ".L_RtlCompareMemory_140169a75:\n\t"
        "mov r9, rdi\n\t"
        "neg ecx\n\t"
        "and ecx, 7\n\t"
        "je .L_RtlCompareMemory_140169a86\n\t"
        ".L_RtlCompareMemory_140169a7f:\n\t"
        "sub r8, rcx\n\t"
        "repe cmpsb\n\t"
        "jne .L_RtlCompareMemory_140169ab6\n\t"
        ".L_RtlCompareMemory_140169a86:\n\t"
        "mov rcx, r8\n\t"
        "and rcx, 0xfffffffffffffff8\n\t"
        "je .L_RtlCompareMemory_140169aaa\n\t"
        ".L_RtlCompareMemory_140169a8f:\n\t"
        "sub r8, rcx\n\t"
        "shr rcx, 3\n\t"
        "repe cmpsq\n\t"
        "je .L_RtlCompareMemory_140169aaa\n\t"
        ".L_RtlCompareMemory_140169a9b:\n\t"
        "inc rcx\n\t"
        "sub rsi, 8\n\t"
        "sub rdi, 8\n\t"
        "shl rcx, 3\n\t"
        ".L_RtlCompareMemory_140169aaa:\n\t"
        "add r8, rcx\n\t"
        "je .L_RtlCompareMemory_140169ab9\n\t"
        ".L_RtlCompareMemory_140169aaf:\n\t"
        "mov rcx, r8\n\t"
        "repe cmpsb\n\t"
        "je .L_RtlCompareMemory_140169ab9\n\t"
        ".L_RtlCompareMemory_140169ab6:\n\t"
        "dec rdi\n\t"
        ".L_RtlCompareMemory_140169ab9:\n\t"
        "sub rdi, r9\n\t"
        "mov rax, rdi\n\t"
        "pop rsi\n\t"
        "pop rdi\n\t"
        "ret\n\t"
        ".L_RtlCompareMemory_140169ac2:\n\t"
        "test r8, r8\n\t"
        "je .L_RtlCompareMemory_140169ad4\n\t"
        ".L_RtlCompareMemory_140169ac7:\n\t"
        "mov rcx, r8\n\t"
        "repe cmpsb\n\t"
        "je .L_RtlCompareMemory_140169ad4\n\t"
        ".L_RtlCompareMemory_140169ace:\n\t"
        "inc rcx\n\t"
        "sub r8, rcx\n\t"
        ".L_RtlCompareMemory_140169ad4:\n\t"
        "mov rax, r8\n\t"
        "pop rsi\n\t"
        "pop rdi\n\t"
        "ret\n\t"
        ".att_syntax prefix"
    );
}

/*
 * Function : RtlInitUnicodeString @ 0x140055c20 (55 bytes)
 * Z3 Proof : VERIFIED_EQUIVALENT (2535.02 ms, 34 SMT assertions)
 * Verified : win64_callee_saved_abi, x86_64_to_llvm_ir_equivalence
 */
void __attribute__((ms_abi)) RtlInitUnicodeString(struct _UNICODE_STRING* DestinationString, const uint16_t* SourceString) {
    uint64_t __attribute__((unused)) ssa_common_ret_op = 0;
    uint64_t __attribute__((unused)) ssa_reg_rax_0 = 0;
    DestinationString->Buffer = (uint16_t*)(uintptr_t)(((uint64_t)(uintptr_t)SourceString));
    (*(uint32_t*)(uintptr_t)(((uint64_t)(uintptr_t)DestinationString))) = (uint32_t)(0ULL);
    if (((uint64_t)(((uint64_t)(uintptr_t)SourceString)) == (uint64_t)(0ULL))) {
        ssa_common_ret_op = (uint64_t)(0ULL);
        goto L_common_ret;
    }
    ssa_reg_rax_0 = (uint64_t)(0xffffffffffffffffULL);
    goto L_bb_140055c32;
L_common_ret:
    return;
L_bb_140055c32:
    uint64_t __attribute__((unused)) ssa__64 = (uint64_t)(((uint64_t)(ssa_reg_rax_0) + (uint64_t)(1ULL)));
    uint64_t __attribute__((unused)) ssa__79 = (uint64_t)(((uint64_t)(ssa__64) << (uint64_t)(1ULL)));
    uint16_t __attribute__((unused)) ssa_mem_ld = (uint16_t)((*(uint16_t*)(uintptr_t)(((uint64_t)(ssa__79) + (uint64_t)(((uint64_t)(uintptr_t)SourceString))))));
    if (((uint16_t)(ssa_mem_ld) == (uint16_t)(0ULL))) {
        goto L_bb_140055c3c;
    }
    ssa_reg_rax_0 = (uint64_t)(ssa__64);
    goto L_bb_140055c32;
L_bb_140055c3c:
    uint16_t __attribute__((unused)) ssa_spec_select = (uint16_t)(((((uint64_t)(ssa__79) > (uint64_t)(0xfffdULL))) ? (0xfffffffffffffffcULL) : (((uint16_t)(uint64_t)(ssa__79)))));
    DestinationString->Length = (uint16_t)(ssa_spec_select);
    uint16_t __attribute__((unused)) ssa__136 = (uint16_t)(((uint16_t)(ssa_spec_select) + (uint16_t)(2ULL)));
    DestinationString->MaximumLength = (uint16_t)(ssa__136);
    ssa_common_ret_op = (uint64_t)(((uint64_t)(uint16_t)(ssa__136)));
    goto L_common_ret;
}

/*
 * Function : RtlInitAnsiString @ 0x14004c800 (55 bytes)
 * Z3 Proof : VERIFIED_EQUIVALENT (2536.93 ms, 34 SMT assertions)
 * Verified : win64_callee_saved_abi, x86_64_to_llvm_ir_equivalence
 */
void __attribute__((ms_abi)) RtlInitAnsiString(struct _STRING* DestinationString, const char* SourceString) {
    uint64_t __attribute__((unused)) ssa_common_ret_op = 0;
    uint64_t __attribute__((unused)) ssa_reg_rax_0 = 0;
    DestinationString->Buffer = (char*)(uintptr_t)(((uint64_t)(uintptr_t)SourceString));
    (*(uint32_t*)(uintptr_t)(((uint64_t)(uintptr_t)DestinationString))) = (uint32_t)(0ULL);
    if (((uint64_t)(((uint64_t)(uintptr_t)SourceString)) == (uint64_t)(0ULL))) {
        ssa_common_ret_op = (uint64_t)(0ULL);
        goto L_common_ret;
    }
    ssa_reg_rax_0 = (uint64_t)(0xffffffffffffffffULL);
    goto L_bb_14004c814;
L_common_ret:
    return;
L_bb_14004c814:
    uint64_t __attribute__((unused)) ssa__64 = (uint64_t)(((uint64_t)(ssa_reg_rax_0) + (uint64_t)(1ULL)));
    uint8_t __attribute__((unused)) ssa_mem_ld = (uint8_t)((*(uint8_t*)(uintptr_t)(((uint64_t)(ssa__64) + (uint64_t)(((uint64_t)(uintptr_t)SourceString))))));
    if (((uint8_t)(ssa_mem_ld) == (uint8_t)(0ULL))) {
        goto L_bb_14004c81d;
    }
    ssa_reg_rax_0 = (uint64_t)(ssa__64);
    goto L_bb_14004c814;
L_bb_14004c81d:
    uint16_t __attribute__((unused)) ssa_r_ax = (uint16_t)(((uint16_t)(uint64_t)((((uint64_t)(ssa__64) < (uint64_t)(0xfffeULL)) ? (uint64_t)(ssa__64) : (uint64_t)(0xfffeULL)))));
    DestinationString->Length = (uint16_t)(ssa_r_ax);
    uint16_t __attribute__((unused)) ssa__125 = (uint16_t)(((uint16_t)(ssa_r_ax) + (uint16_t)(1ULL)));
    DestinationString->MaximumLength = (uint16_t)(ssa__125);
    ssa_common_ret_op = (uint64_t)(((uint64_t)(uint16_t)(ssa__125)));
    goto L_common_ret;
}

/*
 * Function : RtlInitializeBitMap @ 0x140105310 (8 bytes)
 * Z3 Proof : VERIFIED_EQUIVALENT (4.99 ms, 34 SMT assertions)
 * Verified : win64_callee_saved_abi, x86_64_to_llvm_ir_equivalence
 */
void __attribute__((ms_abi)) RtlInitializeBitMap(struct _RTL_BITMAP* BitMapHeader, uint32_t* BitMapBuffer, ULONG SizeOfBitMap) {
    BitMapHeader->SizeOfBitMap = (uint32_t)(((uint32_t)(uint64_t)(((uint64_t)SizeOfBitMap))));
    BitMapHeader->Buffer = (uint32_t*)(uintptr_t)(((uint64_t)(uintptr_t)BitMapBuffer));
    return;
}

/*
 * Function : RtlTestBit @ 0x140104290 (11 bytes)
 * Z3 Proof : VERIFIED_EQUIVALENT (34.39 ms, 33 SMT assertions)
 * Verified : win64_callee_saved_abi, x86_64_to_llvm_ir_equivalence
 */
BOOLEAN __attribute__((ms_abi)) RtlTestBit(struct _RTL_BITMAP* BitMapHeader, ULONG BitNumber) {
    uint64_t __attribute__((unused)) ssa_mem_ld = (uint64_t)(uintptr_t)(BitMapHeader->Buffer);
    uint32_t __attribute__((unused)) ssa_r_edx = (uint32_t)(((uint32_t)(uint64_t)(((uint64_t)BitNumber))));
    uint32_t __attribute__((unused)) ssa_bt_word = (uint32_t)((*(uint32_t*)(uintptr_t)(((uint64_t)(ssa_mem_ld) + (uint64_t)(((uint64_t)(((uint64_t)(int64_t)(int32_t)(((int32_t)(ssa_r_edx) >> (int32_t)(5ULL))))) << (uint64_t)(2ULL)))))));
    return (BOOLEAN)(((uint64_t)(((uint64_t)(ssa_mem_ld) & (uint64_t)(0xffffffffffffff00ULL))) | (uint64_t)(((uint64_t)(uint8_t)(((uint8_t)(((uint8_t)(uint32_t)(((uint32_t)(ssa_bt_word) >> (uint32_t)(((uint32_t)(ssa_r_edx) & (uint32_t)(0x1fULL))))))) & (uint8_t)(1ULL)))))));
}

/*
 * Function : RtlSetBit @ 0x140109ff0 (8 bytes)
 * Z3 Proof : VERIFIED_EQUIVALENT (752.03 ms, 34 SMT assertions)
 * Verified : win64_callee_saved_abi, x86_64_to_llvm_ir_equivalence
 */
void __attribute__((ms_abi)) RtlSetBit(struct _RTL_BITMAP* BitMapHeader, ULONG BitNumber) {
    uint64_t __attribute__((unused)) ssa_mem_ld = (uint64_t)(uintptr_t)(BitMapHeader->Buffer);
    uint32_t __attribute__((unused)) ssa_r_edx = (uint32_t)(((uint32_t)(uint64_t)(((uint64_t)BitNumber))));
    uint32_t __attribute__((unused)) ssa_bt_word = (uint32_t)((*(uint32_t*)(uintptr_t)(((uint64_t)(ssa_mem_ld) + (uint64_t)(((uint64_t)(((uint64_t)(int64_t)(int32_t)(((int32_t)(ssa_r_edx) >> (int32_t)(5ULL))))) << (uint64_t)(2ULL)))))));
    (*(uint32_t*)(uintptr_t)(((uint64_t)(ssa_mem_ld) + (uint64_t)(((uint64_t)(((uint64_t)(int64_t)(int32_t)(((int32_t)(ssa_r_edx) >> (int32_t)(5ULL))))) << (uint64_t)(2ULL)))))) = (uint32_t)(((uint32_t)(ssa_bt_word) | (uint32_t)(((uint32_t)(1ULL) << (uint32_t)(((uint32_t)(ssa_r_edx) & (uint32_t)(0x1fULL)))))));
    return;
}

/*
 * Function : RtlClearBit @ 0x14010a880 (8 bytes)
 * Z3 Proof : VERIFIED_EQUIVALENT (494.51 ms, 34 SMT assertions)
 * Verified : win64_callee_saved_abi, x86_64_to_llvm_ir_equivalence
 */
void __attribute__((ms_abi)) RtlClearBit(struct _RTL_BITMAP* BitMapHeader, ULONG BitNumber) {
    uint64_t __attribute__((unused)) ssa_mem_ld = (uint64_t)(uintptr_t)(BitMapHeader->Buffer);
    uint32_t __attribute__((unused)) ssa_r_edx = (uint32_t)(((uint32_t)(uint64_t)(((uint64_t)BitNumber))));
    uint32_t __attribute__((unused)) ssa_bt_word = (uint32_t)((*(uint32_t*)(uintptr_t)(((uint64_t)(ssa_mem_ld) + (uint64_t)(((uint64_t)(((uint64_t)(int64_t)(int32_t)(((int32_t)(ssa_r_edx) >> (int32_t)(5ULL))))) << (uint64_t)(2ULL)))))));
    (*(uint32_t*)(uintptr_t)(((uint64_t)(ssa_mem_ld) + (uint64_t)(((uint64_t)(((uint64_t)(int64_t)(int32_t)(((int32_t)(ssa_r_edx) >> (int32_t)(5ULL))))) << (uint64_t)(2ULL)))))) = (uint32_t)(((uint32_t)(ssa_bt_word) & (uint32_t)(((uint32_t)(((uint32_t)(1ULL) << (uint32_t)(((uint32_t)(ssa_r_edx) & (uint32_t)(0x1fULL))))) ^ (uint32_t)(0xffffffffffffffffULL)))));
    return;
}

/*
 * Function : RtlExtendedMagicDivide @ 0x14002f98c (42 bytes)
 * Z3 Proof : VERIFIED_EQUIVALENT (62.18 ms, 33 SMT assertions)
 * Verified : win64_callee_saved_abi, x86_64_to_llvm_ir_equivalence
 */
int64_t __attribute__((ms_abi)) RtlExtendedMagicDivide(int64_t Dividend, int64_t MagicDivisor, int8_t ShiftCount) {
    uint64_t __attribute__((unused)) ssa__56 = (uint64_t)(((uint64_t)(((uint64_t)(unsigned __int128)(((unsigned __int128)(((unsigned __int128)(((unsigned __int128)(uint64_t)(((uint64_t)MagicDivisor)))) * (unsigned __int128)(((unsigned __int128)(uint64_t)(((((int64_t)(((uint64_t)Dividend)) < (int64_t)0) ? ((uint64_t)0 - (uint64_t)(((uint64_t)Dividend))) : (uint64_t)(((uint64_t)Dividend))))))))) >> (unsigned __int128)(0x40ULL))))) >> (uint64_t)(((uint64_t)(((uint64_t)ShiftCount)) & (uint64_t)(0x3fULL)))));
    return (int64_t)(((((int64_t)(((uint64_t)Dividend)) < (int64_t)(0ULL))) ? (((uint64_t)(0ULL) - (uint64_t)(ssa__56))) : (ssa__56)));
}

/*
 * Function : RtlSecondsSince1970ToTime @ 0x1401406f0 (32 bytes)
 * Z3 Proof : VERIFIED_EQUIVALENT (5.27 ms, 34 SMT assertions)
 * Verified : win64_callee_saved_abi, x86_64_to_llvm_ir_equivalence
 */
void __attribute__((ms_abi)) RtlSecondsSince1970ToTime(ULONG ElapsedSeconds, union _LARGE_INTEGER* Time) {
    uint64_t __attribute__((unused)) ssa__61 = (uint64_t)(((uint64_t)(((uint64_t)(((uint64_t)(((uint64_t)ElapsedSeconds)) & (uint64_t)(0xffffffffULL))) * (uint64_t)(0x989680ULL))) + (uint64_t)(0x19db1ded53e8000ULL)));
    Time->QuadPart = (int64_t)(ssa__61);
    return;
}

/*
 * Function : RtlSecondsSince1980ToTime @ 0x140248050 (32 bytes)
 * Z3 Proof : VERIFIED_EQUIVALENT (5.06 ms, 34 SMT assertions)
 * Verified : win64_callee_saved_abi, x86_64_to_llvm_ir_equivalence
 */
void __attribute__((ms_abi)) RtlSecondsSince1980ToTime(ULONG ElapsedSeconds, union _LARGE_INTEGER* Time) {
    uint64_t __attribute__((unused)) ssa__61 = (uint64_t)(((uint64_t)(((uint64_t)(((uint64_t)(((uint64_t)ElapsedSeconds)) & (uint64_t)(0xffffffffULL))) * (uint64_t)(0x989680ULL))) + (uint64_t)(0x1a8e79fe1d58000ULL)));
    Time->QuadPart = (int64_t)(ssa__61);
    return;
}

/*
 * Function : RtlTimeToSecondsSince1970 @ 0x140248080 (57 bytes)
 * Z3 Proof : VERIFIED_EQUIVALENT (28.58 ms, 33 SMT assertions)
 * Verified : win64_callee_saved_abi, x86_64_to_llvm_ir_equivalence
 */
BOOLEAN __attribute__((ms_abi)) RtlTimeToSecondsSince1970(union _LARGE_INTEGER* Time, ULONG* ElapsedSeconds) {
    uint64_t __attribute__((unused)) ssa_reg_rax_0 = 0;
    uint64_t __attribute__((unused)) ssa_mem_ld = (uint64_t)(Time->QuadPart);
    uint64_t __attribute__((unused)) ssa_call_res = (uint64_t)RtlExtendedMagicDivide((int64_t)(ssa_mem_ld), (int64_t)(0xd6bf94d5e57a42bdULL), (int8_t)(0x17ULL));
    uint64_t __attribute__((unused)) ssa__50 = (uint64_t)(((uint64_t)(ssa_call_res) + (uint64_t)(0xfffffffd49ef6f00ULL)));
    if (((uint64_t)(ssa__50) < (uint64_t)(0x100000000ULL))) {
        (*(uint32_t*)(uintptr_t)(((uint64_t)(uintptr_t)ElapsedSeconds))) = (uint32_t)(((uint32_t)(uint64_t)(ssa__50)));
        ssa_reg_rax_0 = (uint64_t)(((uint64_t)(((uint64_t)(ssa__50) & (uint64_t)(0xffffff00ULL))) | (uint64_t)(1ULL)));
    } else {
        ssa_reg_rax_0 = (uint64_t)(((uint64_t)(ssa__50) & (uint64_t)(0xffffffffffffff00ULL)));
    }
    return (BOOLEAN)(ssa_reg_rax_0);
}
