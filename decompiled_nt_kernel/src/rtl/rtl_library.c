/*
 * Reconstructed Windows NT Kernel Subsystem: Kernel Run-Time Library, Trees, Bitmaps, Strings & Time (Rtl)
 * Source Binary : ntoskrnl.exe (10.0.16299.15 x64, RSDS 83DB4240-4EFD-4AB6-AFB6-FA864B700CB3)
 * Functions     : 76 Z3-verified / assembly-preserved routines
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
 * Z3 Proof : VERIFIED_EQUIVALENT (2558.76 ms, 34 SMT assertions)
 * Verified : win64_callee_saved_abi, x86_64_to_llvm_ir_equivalence
 */
void __attribute__((ms_abi)) RtlInitUnicodeString(struct _UNICODE_STRING* DestinationString, const uint16_t* SourceString) {
    uint64_t __attribute__((unused)) ssa_common_ret_op = 0;
    uint64_t __attribute__((unused)) ssa_reg_rax_0 = 0;
    DestinationString->Buffer = (uint16_t*)(uintptr_t)(((uint64_t)(uintptr_t)SourceString));
    (*(uint32_t*)((uint8_t*)(uintptr_t)(DestinationString))) = (uint32_t)(0ULL);
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
 * Z3 Proof : VERIFIED_EQUIVALENT (2415.85 ms, 34 SMT assertions)
 * Verified : win64_callee_saved_abi, x86_64_to_llvm_ir_equivalence
 */
void __attribute__((ms_abi)) RtlInitAnsiString(struct _STRING* DestinationString, const char* SourceString) {
    uint64_t __attribute__((unused)) ssa_common_ret_op = 0;
    uint64_t __attribute__((unused)) ssa_reg_rax_0 = 0;
    DestinationString->Buffer = (char*)(uintptr_t)(((uint64_t)(uintptr_t)SourceString));
    (*(uint32_t*)((uint8_t*)(uintptr_t)(DestinationString))) = (uint32_t)(0ULL);
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
 * Z3 Proof : VERIFIED_EQUIVALENT (5.07 ms, 34 SMT assertions)
 * Verified : win64_callee_saved_abi, x86_64_to_llvm_ir_equivalence
 */
void __attribute__((ms_abi)) RtlInitializeBitMap(struct _RTL_BITMAP* BitMapHeader, uint32_t* BitMapBuffer, ULONG SizeOfBitMap) {
    BitMapHeader->SizeOfBitMap = (uint32_t)(((uint32_t)(uint64_t)(((uint64_t)SizeOfBitMap))));
    BitMapHeader->Buffer = (uint32_t*)(uintptr_t)(((uint64_t)(uintptr_t)BitMapBuffer));
    return;
}

/*
 * Function : RtlTestBit @ 0x140104290 (11 bytes)
 * Z3 Proof : VERIFIED_EQUIVALENT (55.22 ms, 33 SMT assertions)
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
 * Z3 Proof : VERIFIED_EQUIVALENT (590.43 ms, 34 SMT assertions)
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
 * Z3 Proof : VERIFIED_EQUIVALENT (457.00 ms, 34 SMT assertions)
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
 * Z3 Proof : VERIFIED_EQUIVALENT (74.51 ms, 33 SMT assertions)
 * Verified : win64_callee_saved_abi, x86_64_to_llvm_ir_equivalence
 */
int64_t __attribute__((ms_abi)) RtlExtendedMagicDivide(int64_t Dividend, int64_t MagicDivisor, int8_t ShiftCount) {
    uint64_t __attribute__((unused)) ssa__56 = (uint64_t)(((uint64_t)(((uint64_t)(unsigned __int128)(((unsigned __int128)(((unsigned __int128)(((unsigned __int128)(uint64_t)(((uint64_t)MagicDivisor)))) * (unsigned __int128)(((unsigned __int128)(uint64_t)(((((int64_t)(((uint64_t)Dividend)) < (int64_t)0) ? ((uint64_t)0 - (uint64_t)(((uint64_t)Dividend))) : (uint64_t)(((uint64_t)Dividend))))))))) >> (unsigned __int128)(0x40ULL))))) >> (uint64_t)(((uint64_t)(((uint64_t)ShiftCount)) & (uint64_t)(0x3fULL)))));
    return (int64_t)(((((int64_t)(((uint64_t)Dividend)) < (int64_t)(0ULL))) ? (((uint64_t)(0ULL) - (uint64_t)(ssa__56))) : (ssa__56)));
}

/*
 * Function : RtlSecondsSince1970ToTime @ 0x1401406f0 (32 bytes)
 * Z3 Proof : VERIFIED_EQUIVALENT (6.35 ms, 34 SMT assertions)
 * Verified : win64_callee_saved_abi, x86_64_to_llvm_ir_equivalence
 */
void __attribute__((ms_abi)) RtlSecondsSince1970ToTime(ULONG ElapsedSeconds, union _LARGE_INTEGER* Time) {
    uint64_t __attribute__((unused)) ssa__61 = (uint64_t)(((uint64_t)(((uint64_t)(((uint64_t)(((uint64_t)ElapsedSeconds)) & (uint64_t)(0xffffffffULL))) * (uint64_t)(0x989680ULL))) + (uint64_t)(0x19db1ded53e8000ULL)));
    Time->QuadPart = (int64_t)(ssa__61);
    return;
}

/*
 * Function : RtlSecondsSince1980ToTime @ 0x140248050 (32 bytes)
 * Z3 Proof : VERIFIED_EQUIVALENT (5.99 ms, 34 SMT assertions)
 * Verified : win64_callee_saved_abi, x86_64_to_llvm_ir_equivalence
 */
void __attribute__((ms_abi)) RtlSecondsSince1980ToTime(ULONG ElapsedSeconds, union _LARGE_INTEGER* Time) {
    uint64_t __attribute__((unused)) ssa__61 = (uint64_t)(((uint64_t)(((uint64_t)(((uint64_t)(((uint64_t)ElapsedSeconds)) & (uint64_t)(0xffffffffULL))) * (uint64_t)(0x989680ULL))) + (uint64_t)(0x1a8e79fe1d58000ULL)));
    Time->QuadPart = (int64_t)(ssa__61);
    return;
}

/*
 * Function : RtlTimeToSecondsSince1970 @ 0x140248080 (57 bytes)
 * Z3 Proof : VERIFIED_EQUIVALENT (37.94 ms, 33 SMT assertions)
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

/*
 * Function : RtlNumberGenericTableElements @ 0x14010d440 (4 bytes)
 * Z3 Proof : VERIFIED_EQUIVALENT (3.48 ms, 33 SMT assertions)
 * Verified : win64_callee_saved_abi, x86_64_to_llvm_ir_equivalence
 */
uint32_t __attribute__((ms_abi)) RtlNumberGenericTableElements(uint32_t* arg1) {
    uint32_t __attribute__((unused)) ssa_mem_ld = (uint32_t)((*(uint32_t*)(uintptr_t)(((uint64_t)(((uint64_t)(uintptr_t)arg1)) + (uint64_t)(0x24ULL)))));
    return (uint32_t)(((uint64_t)(uint32_t)(ssa_mem_ld)));
}

/*
 * Function : RtlNumberGenericTableElementsAvl @ 0x1400e9ed0 (4 bytes)
 * Z3 Proof : VERIFIED_EQUIVALENT (3.35 ms, 33 SMT assertions)
 * Verified : win64_callee_saved_abi, x86_64_to_llvm_ir_equivalence
 */
uint32_t __attribute__((ms_abi)) RtlNumberGenericTableElementsAvl(uint32_t* arg1) {
    uint32_t __attribute__((unused)) ssa_mem_ld = (uint32_t)((*(uint32_t*)(uintptr_t)(((uint64_t)(((uint64_t)(uintptr_t)arg1)) + (uint64_t)(0x2cULL)))));
    return (uint32_t)(((uint64_t)(uint32_t)(ssa_mem_ld)));
}

/*
 * Function : RtlRunOnceInitialize @ 0x1405a5640 (5 bytes)
 * Z3 Proof : VERIFIED_EQUIVALENT (4.28 ms, 34 SMT assertions)
 * Verified : win64_callee_saved_abi, x86_64_to_llvm_ir_equivalence
 */
void __attribute__((ms_abi)) RtlRunOnceInitialize(uint64_t* arg1) {
    (*(uint64_t*)(uintptr_t)(((uint64_t)(uintptr_t)arg1))) = (uint64_t)(0ULL);
    return;
}

/*
 * Function : RtlSubAuthorityCountSid @ 0x140051bf0 (5 bytes)
 * Z3 Proof : VERIFIED_EQUIVALENT (2.92 ms, 33 SMT assertions)
 * Verified : win64_callee_saved_abi, x86_64_to_llvm_ir_equivalence
 */
uint64_t __attribute__((ms_abi)) RtlSubAuthorityCountSid(uint64_t* arg1) {
    return (uint64_t)(((uint64_t)(((uint64_t)(uintptr_t)arg1)) + (uint64_t)(1ULL)));
}

/*
 * Function : RtlAreAnyAccessesGranted @ 0x140449b90 (6 bytes)
 * Z3 Proof : VERIFIED_EQUIVALENT (3.32 ms, 33 SMT assertions)
 * Verified : win64_callee_saved_abi, x86_64_to_llvm_ir_equivalence
 */
BOOLEAN __attribute__((ms_abi)) RtlAreAnyAccessesGranted(uint32_t arg1, uint32_t arg2) {
    return (BOOLEAN)(((uint64_t)(uint8_t)(((uint32_t)(((uint32_t)(uint64_t)(((uint64_t)(((uint64_t)arg2)) & (uint64_t)(((uint64_t)arg1)))))) != (uint32_t)(0ULL)))));
}

/*
 * Function : RtlEqualWnfChangeStamps @ 0x14071a670 (6 bytes)
 * Z3 Proof : VERIFIED_EQUIVALENT (3.54 ms, 33 SMT assertions)
 * Verified : win64_callee_saved_abi, x86_64_to_llvm_ir_equivalence
 */
BOOLEAN __attribute__((ms_abi)) RtlEqualWnfChangeStamps(uint32_t arg1, uint32_t arg2) {
    return (BOOLEAN)(((uint64_t)(uint8_t)(((uint32_t)(((uint32_t)(uint64_t)(((uint64_t)arg1)))) == (uint32_t)(((uint32_t)(uint64_t)(((uint64_t)arg2))))))));
}

/*
 * Function : XIPDispatch @ 0x140280300 (6 bytes)
 * Z3 Proof : VERIFIED_EQUIVALENT (2.87 ms, 33 SMT assertions)
 * Verified : win64_callee_saved_abi, x86_64_to_llvm_ir_equivalence
 */
NTSTATUS __attribute__((ms_abi)) XIPDispatch(void) {
    return (NTSTATUS)(0xc000000eULL);
}

/*
 * Function : FirstEntrySList @ 0x1401693c0 (7 bytes)
 * Z3 Proof : VERIFIED_EQUIVALENT (4.24 ms, 33 SMT assertions)
 * Verified : win64_callee_saved_abi, x86_64_to_llvm_ir_equivalence
 */
uint64_t __attribute__((ms_abi)) FirstEntrySList(uint64_t* arg1) {
    uint64_t __attribute__((unused)) ssa_mem_ld = (uint64_t)((*(uint64_t*)(uintptr_t)(((uint64_t)(((uint64_t)(uintptr_t)arg1)) + (uint64_t)(8ULL)))));
    return (uint64_t)(((uint64_t)(ssa_mem_ld) & (uint64_t)(0xfffffffffffffff0ULL)));
}

/*
 * Function : RtlCopyLuid @ 0x140570140 (7 bytes)
 * Z3 Proof : VERIFIED_EQUIVALENT (4.34 ms, 33 SMT assertions)
 * Verified : win64_callee_saved_abi, x86_64_to_llvm_ir_equivalence
 */
uint64_t __attribute__((ms_abi)) RtlCopyLuid(uint64_t* arg1, uint64_t* arg2) {
    uint64_t __attribute__((unused)) ssa_mem_ld = (uint64_t)((*(uint64_t*)(uintptr_t)(((uint64_t)(uintptr_t)arg2))));
    (*(uint64_t*)(uintptr_t)(((uint64_t)(uintptr_t)arg1))) = (uint64_t)(ssa_mem_ld);
    return (uint64_t)(ssa_mem_ld);
}

/*
 * Function : RtlGetNtGlobalFlags @ 0x1407198f0 (7 bytes)
 * Z3 Proof : VERIFIED_EQUIVALENT (3.64 ms, 33 SMT assertions)
 * Verified : win64_callee_saved_abi, x86_64_to_llvm_ir_equivalence
 */
uint32_t __attribute__((ms_abi)) RtlGetNtGlobalFlags(void) {
    uint32_t __attribute__((unused)) ssa_mem_ld = (uint32_t)((*(uint32_t*)(uintptr_t)(((uint64_t)(uintptr_t)&NtGlobalFlag))));
    return (uint32_t)(((uint64_t)(uint32_t)(ssa_mem_ld)));
}

/*
 * Function : srand @ 0x140144190 (7 bytes)
 * Z3 Proof : VERIFIED_EQUIVALENT (3.52 ms, 34 SMT assertions)
 * Verified : win64_callee_saved_abi, x86_64_to_llvm_ir_equivalence
 */
void __attribute__((ms_abi)) srand(uint32_t arg1) {
    (*(uint32_t*)(uintptr_t)(((uint64_t)(uintptr_t)&__nt_data_14034e510))) = (uint32_t)(((uint32_t)(uint64_t)(((uint64_t)arg1))));
    return;
}

/*
 * Function : RtlAreAllAccessesGranted @ 0x14055b7f0 (8 bytes)
 * Z3 Proof : VERIFIED_EQUIVALENT (3.52 ms, 33 SMT assertions)
 * Verified : win64_callee_saved_abi, x86_64_to_llvm_ir_equivalence
 */
BOOLEAN __attribute__((ms_abi)) RtlAreAllAccessesGranted(uint32_t arg1, uint32_t arg2) {
    return (BOOLEAN)(((uint64_t)(uint8_t)(((uint32_t)(((uint32_t)(((uint32_t)(uint64_t)(((uint64_t)arg2)))) & (uint32_t)(((uint32_t)(((uint32_t)(uint64_t)(((uint64_t)arg1)))) ^ (uint32_t)(0xffffffffffffffffULL))))) == (uint32_t)(0ULL)))));
}

/*
 * Function : RtlInitializeBitMapEx @ 0x14024aad0 (8 bytes)
 * Z3 Proof : VERIFIED_EQUIVALENT (4.61 ms, 34 SMT assertions)
 * Verified : win64_callee_saved_abi, x86_64_to_llvm_ir_equivalence
 */
void __attribute__((ms_abi)) RtlInitializeBitMapEx(PLIST_ENTRY arg1, uint64_t arg2, uint64_t arg3) {
    arg1->Flink = (struct _LIST_ENTRY*)(uintptr_t)(((uint64_t)arg3));
    arg1->Blink = (struct _LIST_ENTRY*)(uintptr_t)(((uint64_t)arg2));
    return;
}

/*
 * Function : RtlIsGenericTableEmpty @ 0x14013ce70 (8 bytes)
 * Z3 Proof : VERIFIED_EQUIVALENT (3.91 ms, 33 SMT assertions)
 * Verified : win64_callee_saved_abi, x86_64_to_llvm_ir_equivalence
 */
BOOLEAN __attribute__((ms_abi)) RtlIsGenericTableEmpty(uint64_t* arg1) {
    uint64_t __attribute__((unused)) ssa_mem_ld = (uint64_t)((*(uint64_t*)(uintptr_t)(((uint64_t)(uintptr_t)arg1))));
    return (BOOLEAN)(((uint64_t)(uint8_t)(((uint64_t)(ssa_mem_ld) == (uint64_t)(0ULL)))));
}

/*
 * Function : RtlIsGenericTableEmptyAvl @ 0x140023510 (8 bytes)
 * Z3 Proof : VERIFIED_EQUIVALENT (3.66 ms, 33 SMT assertions)
 * Verified : win64_callee_saved_abi, x86_64_to_llvm_ir_equivalence
 */
BOOLEAN __attribute__((ms_abi)) RtlIsGenericTableEmptyAvl(uint32_t* arg1) {
    uint32_t __attribute__((unused)) ssa_mem_ld = (uint32_t)((*(uint32_t*)(uintptr_t)(((uint64_t)(((uint64_t)(uintptr_t)arg1)) + (uint64_t)(0x2cULL)))));
    return (BOOLEAN)(((uint64_t)(uint8_t)(((uint32_t)(ssa_mem_ld) == (uint32_t)(0ULL)))));
}

/*
 * Function : RtlClearBitEx @ 0x140249700 (9 bytes)
 * Z3 Proof : VERIFIED_EQUIVALENT (5.99 ms, 33 SMT assertions)
 * Verified : win64_callee_saved_abi, x86_64_to_llvm_ir_equivalence
 */
uint64_t __attribute__((ms_abi)) RtlClearBitEx(uint64_t* arg1, uint64_t arg2) {
    uint64_t __attribute__((unused)) ssa_mem_ld = (uint64_t)((*(uint64_t*)(uintptr_t)(((uint64_t)(((uint64_t)(uintptr_t)arg1)) + (uint64_t)(8ULL)))));
    uint64_t __attribute__((unused)) ssa_bt_word = (uint64_t)((*(uint64_t*)(uintptr_t)(((uint64_t)(ssa_mem_ld) + (uint64_t)(((uint64_t)(((int64_t)(((uint64_t)arg2)) >> (int64_t)(3ULL))) & (uint64_t)(0xfffffffffffffff8ULL)))))));
    (*(uint64_t*)(uintptr_t)(((uint64_t)(ssa_mem_ld) + (uint64_t)(((uint64_t)(((int64_t)(((uint64_t)arg2)) >> (int64_t)(3ULL))) & (uint64_t)(0xfffffffffffffff8ULL)))))) = (uint64_t)(((uint64_t)(ssa_bt_word) & (uint64_t)(((uint64_t)(((uint64_t)(1ULL) << (uint64_t)(((uint64_t)(((uint64_t)arg2)) & (uint64_t)(0x3fULL))))) ^ (uint64_t)(0xffffffffffffffffULL)))));
    return (uint64_t)(ssa_mem_ld);
}

/*
 * Function : RtlSetBitEx @ 0x14024b200 (9 bytes)
 * Z3 Proof : VERIFIED_EQUIVALENT (6.11 ms, 33 SMT assertions)
 * Verified : win64_callee_saved_abi, x86_64_to_llvm_ir_equivalence
 */
uint64_t __attribute__((ms_abi)) RtlSetBitEx(uint64_t* arg1, uint64_t arg2) {
    uint64_t __attribute__((unused)) ssa_mem_ld = (uint64_t)((*(uint64_t*)(uintptr_t)(((uint64_t)(((uint64_t)(uintptr_t)arg1)) + (uint64_t)(8ULL)))));
    uint64_t __attribute__((unused)) ssa_bt_word = (uint64_t)((*(uint64_t*)(uintptr_t)(((uint64_t)(ssa_mem_ld) + (uint64_t)(((uint64_t)(((int64_t)(((uint64_t)arg2)) >> (int64_t)(3ULL))) & (uint64_t)(0xfffffffffffffff8ULL)))))));
    (*(uint64_t*)(uintptr_t)(((uint64_t)(ssa_mem_ld) + (uint64_t)(((uint64_t)(((int64_t)(((uint64_t)arg2)) >> (int64_t)(3ULL))) & (uint64_t)(0xfffffffffffffff8ULL)))))) = (uint64_t)(((uint64_t)(ssa_bt_word) | (uint64_t)(((uint64_t)(1ULL) << (uint64_t)(((uint64_t)(((uint64_t)arg2)) & (uint64_t)(0x3fULL)))))));
    return (uint64_t)(ssa_mem_ld);
}

/*
 * Function : RtlClearBits @ 0x1400bdf60 (10 bytes)
 * Z3 Proof : VERIFIED_EQUIVALENT (19.79 ms, 34 SMT assertions)
 * Verified : x86_64_to_llvm_ir_equivalence
 */
void __attribute__((ms_abi)) RtlClearBits(uint64_t arg1, uint64_t arg2, uint32_t arg3) {
    return;
}

/*
 * Function : RtlClearBitsEx @ 0x140074860 (10 bytes)
 * Z3 Proof : VERIFIED_EQUIVALENT (18.49 ms, 34 SMT assertions)
 * Verified : x86_64_to_llvm_ir_equivalence
 */
void __attribute__((ms_abi)) RtlClearBitsEx(uint64_t arg1, uint64_t arg2, uint64_t arg3) {
    return;
}

/*
 * Function : RtlGetNonVolatileToken @ 0x140248670 (10 bytes)
 * Z3 Proof : VERIFIED_EQUIVALENT (3.69 ms, 33 SMT assertions)
 * Verified : win64_callee_saved_abi, x86_64_to_llvm_ir_equivalence
 */
uint64_t __attribute__((ms_abi)) RtlGetNonVolatileToken(uint64_t arg1, uint64_t arg2, uint64_t* arg3) {
    (*(uint64_t*)(uintptr_t)(((uint64_t)(uintptr_t)arg3))) = (uint64_t)(3ULL);
    return (uint64_t)(0ULL);
}

/*
 * Function : RtlIsPartialPlaceholder @ 0x14071c0a0 (10 bytes)
 * Z3 Proof : VERIFIED_EQUIVALENT (3.42 ms, 33 SMT assertions)
 * Verified : win64_callee_saved_abi, x86_64_to_llvm_ir_equivalence
 */
BOOLEAN __attribute__((ms_abi)) RtlIsPartialPlaceholder(uint32_t arg1) {
    return (BOOLEAN)(((uint64_t)(uint8_t)(((uint64_t)(((uint64_t)(((uint64_t)arg1)) & (uint64_t)(0x440000ULL))) != (uint64_t)(0ULL)))));
}

/*
 * Function : RtlSetBits @ 0x1400a5200 (10 bytes)
 * Z3 Proof : VERIFIED_EQUIVALENT (18.67 ms, 34 SMT assertions)
 * Verified : x86_64_to_llvm_ir_equivalence
 */
void __attribute__((ms_abi)) RtlSetBits(uint64_t arg1, uint64_t arg2, uint32_t arg3) {
    return;
}

/*
 * Function : PfxInitialize @ 0x140716a30 (11 bytes)
 * Z3 Proof : VERIFIED_EQUIVALENT (4.40 ms, 34 SMT assertions)
 * Verified : win64_callee_saved_abi, x86_64_to_llvm_ir_equivalence
 */
void __attribute__((ms_abi)) PfxInitialize(PRTL_BITMAP arg1) {
    arg1->SizeOfBitMap = (uint32_t)(0x200ULL);
    arg1->Buffer = (uint32_t*)(uintptr_t)(((uint64_t)arg1));
    return;
}

/*
 * Function : RtlSubAuthoritySid @ 0x140051c00 (11 bytes)
 * Z3 Proof : VERIFIED_EQUIVALENT (9.16 ms, 33 SMT assertions)
 * Verified : win64_callee_saved_abi, x86_64_to_llvm_ir_equivalence
 */
uint64_t __attribute__((ms_abi)) RtlSubAuthoritySid(uint64_t* arg1, uint32_t arg2) {
    return (uint64_t)(((uint64_t)(((uint64_t)(((uint64_t)(uintptr_t)arg1)) + (uint64_t)(8ULL))) + (uint64_t)(((uint64_t)(((uint64_t)(((uint64_t)arg2)) << (uint64_t)(2ULL))) & (uint64_t)(0x3fffffffcULL)))));
}

/*
 * Function : RtlLengthSid @ 0x140050350 (12 bytes)
 * Z3 Proof : VERIFIED_EQUIVALENT (9.56 ms, 33 SMT assertions)
 * Verified : win64_callee_saved_abi, x86_64_to_llvm_ir_equivalence
 */
uint32_t __attribute__((ms_abi)) RtlLengthSid(uint8_t* arg1) {
    uint8_t __attribute__((unused)) ssa_mem_ld = (uint8_t)((*(uint8_t*)(uintptr_t)(((uint64_t)(((uint64_t)(uintptr_t)arg1)) + (uint64_t)(1ULL)))));
    return (uint32_t)(((uint64_t)(uint32_t)(((uint32_t)(((uint32_t)(((uint32_t)(uint8_t)(ssa_mem_ld))) << (uint32_t)(2ULL))) + (uint32_t)(8ULL)))));
}

/*
 * Function : RtlTestBitEx @ 0x14024b2f0 (12 bytes)
 * Z3 Proof : VERIFIED_EQUIVALENT (148.97 ms, 33 SMT assertions)
 * Verified : win64_callee_saved_abi, x86_64_to_llvm_ir_equivalence
 */
uint64_t __attribute__((ms_abi)) RtlTestBitEx(uint64_t* arg1, uint64_t arg2) {
    uint64_t __attribute__((unused)) ssa_mem_ld = (uint64_t)((*(uint64_t*)(uintptr_t)(((uint64_t)(((uint64_t)(uintptr_t)arg1)) + (uint64_t)(8ULL)))));
    uint64_t __attribute__((unused)) ssa_bt_word = (uint64_t)((*(uint64_t*)(uintptr_t)(((uint64_t)(ssa_mem_ld) + (uint64_t)(((uint64_t)(((int64_t)(((uint64_t)arg2)) >> (int64_t)(3ULL))) & (uint64_t)(0xfffffffffffffff8ULL)))))));
    return (uint64_t)(((uint64_t)(((uint64_t)(((uint64_t)(ssa_bt_word) >> (uint64_t)(((uint64_t)(((uint64_t)arg2)) & (uint64_t)(0x3fULL))))) & (uint64_t)(1ULL))) | (uint64_t)(((uint64_t)(ssa_mem_ld) & (uint64_t)(0xffffffffffffff00ULL)))));
}

/*
 * Function : RtlGetEnabledExtendedFeatures @ 0x14013f030 (14 bytes)
 * Z3 Proof : VERIFIED_EQUIVALENT (5.11 ms, 33 SMT assertions)
 * Verified : win64_callee_saved_abi, x86_64_to_llvm_ir_equivalence
 */
uint64_t __attribute__((ms_abi)) RtlGetEnabledExtendedFeatures(uint64_t arg1) {
    uint64_t __attribute__((unused)) ssa_mem_ld = (uint64_t)((*(uint64_t*)(uintptr_t)(0xfffff780000003d8ULL)));
    return (uint64_t)(((uint64_t)(ssa_mem_ld) & (uint64_t)(((uint64_t)arg1))));
}

/*
 * Function : RtlFreeNonVolatileToken @ 0x140248650 (15 bytes)
 * Z3 Proof : VERIFIED_EQUIVALENT (9.65 ms, 33 SMT assertions)
 * Verified : win64_callee_saved_abi, x86_64_to_llvm_ir_equivalence
 */
uint32_t __attribute__((ms_abi)) RtlFreeNonVolatileToken(uint8_t arg1) {
    return (uint32_t)(((((uint64_t)(((uint64_t)(((uint64_t)arg1)) & (uint64_t)(1ULL))) == (uint64_t)(0ULL))) ? (0xc000000dULL) : (0ULL)));
}

/*
 * Function : RtlIsMultiUsersInSessionSku @ 0x140718440 (15 bytes)
 * Z3 Proof : VERIFIED_EQUIVALENT (4.13 ms, 33 SMT assertions)
 * Verified : win64_callee_saved_abi, x86_64_to_llvm_ir_equivalence
 */
uint32_t __attribute__((ms_abi)) RtlIsMultiUsersInSessionSku(void) {
    uint32_t __attribute__((unused)) ssa_mem_ld = (uint32_t)((*(uint32_t*)(uintptr_t)(0xfffff780000002f0ULL)));
    return (uint32_t)(((uint64_t)(uint32_t)(((uint32_t)(((uint32_t)(ssa_mem_ld) >> (uint32_t)(9ULL))) & (uint32_t)(0x7fff01ULL)))));
}

/*
 * Function : RtlIsStateSeparationEnabled @ 0x1405a4350 (15 bytes)
 * Z3 Proof : VERIFIED_EQUIVALENT (4.07 ms, 33 SMT assertions)
 * Verified : win64_callee_saved_abi, x86_64_to_llvm_ir_equivalence
 */
uint32_t __attribute__((ms_abi)) RtlIsStateSeparationEnabled(void) {
    uint32_t __attribute__((unused)) ssa_mem_ld = (uint32_t)((*(uint32_t*)(uintptr_t)(0xfffff780000002f0ULL)));
    return (uint32_t)(((uint64_t)(uint32_t)(((uint32_t)(((uint32_t)(ssa_mem_ld) >> (uint32_t)(10ULL))) & (uint32_t)(0x3fff01ULL)))));
}

/*
 * Function : RtlFindNextForwardRunClearEx @ 0x14024a3d0 (17 bytes)
 * Z3 Proof : VERIFIED_EQUIVALENT (3.70 ms, 33 SMT assertions)
 * Verified : win64_callee_saved_abi, x86_64_to_llvm_ir_equivalence
 */
uint64_t __attribute__((ms_abi)) RtlFindNextForwardRunClearEx(uint64_t arg1, uint64_t arg2, uint64_t arg3) {
    uint64_t __attribute__((unused)) ssa_call_res = RtlFindNextForwardRunClearCappedEx(((uint64_t)arg1), ((uint64_t)arg2), ((uint64_t)arg3), ((uint64_t)arg3));
    return (uint64_t)(ssa_call_res);
}

/*
 * Function : RtlInitializeRangeList @ 0x1405b64e0 (17 bytes)
 * Z3 Proof : VERIFIED_EQUIVALENT (5.86 ms, 33 SMT assertions)
 * Verified : win64_callee_saved_abi, x86_64_to_llvm_ir_equivalence
 */
uint64_t __attribute__((ms_abi)) RtlInitializeRangeList(struct _ANON_TAG_1ba4* arg1) {
    arg1->FileOffset.QuadPart = (int64_t)(((uint64_t)(uintptr_t)arg1));
    arg1->SharedCacheMap = (struct _SHARED_CACHE_MAP*)(uintptr_t)(((uint64_t)(uintptr_t)arg1));
    arg1->FileObject = (struct _FILE_OBJECT*)(uintptr_t)(0ULL);
    arg1->Length = (uint32_t)(0ULL);
    return (uint64_t)(0ULL);
}

/*
 * Function : RtlInitializeUnicodePrefix @ 0x1405dc300 (17 bytes)
 * Z3 Proof : VERIFIED_EQUIVALENT (5.21 ms, 33 SMT assertions)
 * Verified : win64_callee_saved_abi, x86_64_to_llvm_ir_equivalence
 */
uint64_t __attribute__((ms_abi)) RtlInitializeUnicodePrefix(PKDPC arg1) {
    arg1->TargetInfoAsUlong = (uint32_t)(0x800ULL);
    arg1->ProcessorHistory = (uint64_t)(0ULL);
    arg1->DpcListEntry.Next = (struct _SINGLE_LIST_ENTRY*)(uintptr_t)(((uint64_t)arg1));
    return (uint64_t)(0ULL);
}

/*
 * Function : RtlOpenImageFileOptionsKey @ 0x140714d40 (17 bytes)
 * Z3 Proof : VERIFIED_EQUIVALENT (3.32 ms, 33 SMT assertions)
 * Verified : win64_callee_saved_abi, x86_64_to_llvm_ir_equivalence
 */
uint64_t __attribute__((ms_abi)) RtlOpenImageFileOptionsKey(uint64_t arg1, uint64_t arg2, uint64_t arg3) {
    uint64_t __attribute__((unused)) ssa_call_res = RtlpOpenImageFileOptionsKeyEx(((uint64_t)arg1), ((uint64_t)arg2), ((uint64_t)arg3), ((uint64_t)arg3));
    return (uint64_t)(ssa_call_res);
}

/*
 * Function : RtlCheckTokenMembership @ 0x140025310 (20 bytes)
 * Z3 Proof : VERIFIED_EQUIVALENT (4.00 ms, 33 SMT assertions)
 * Verified : win64_callee_saved_abi, x86_64_to_llvm_ir_equivalence
 */
uint64_t __attribute__((ms_abi)) RtlCheckTokenMembership(uint64_t arg1, uint64_t arg2, uint64_t arg3) {
    uint64_t __attribute__((unused)) ssa_call_res = RtlCheckTokenMembershipEx(((uint64_t)arg1), ((uint64_t)arg2), 0ULL, ((uint64_t)arg3));
    return (uint64_t)(ssa_call_res);
}

/*
 * Function : RtlEqualLuid @ 0x1407182c0 (20 bytes)
 * Z3 Proof : VERIFIED_EQUIVALENT (6.00 ms, 33 SMT assertions)
 * Verified : win64_callee_saved_abi, x86_64_to_llvm_ir_equivalence
 */
uint32_t __attribute__((ms_abi)) RtlEqualLuid(PLARGE_INTEGER arg1, PLARGE_INTEGER arg2) {
    uint64_t __attribute__((unused)) ssa_common_ret_op = 0;
    uint32_t __attribute__((unused)) ssa_mem_ld = (uint32_t)(arg2->HighPart);
    uint32_t __attribute__((unused)) ssa_mem_ld_1 = (uint32_t)(arg1->HighPart);
    if (((uint32_t)(ssa_mem_ld_1) == (uint32_t)(ssa_mem_ld))) {
        goto L_bb_1407182c8;
    }
    ssa_common_ret_op = (uint64_t)(0ULL);
    goto L_common_ret;
L_bb_1407182c8:
    uint32_t __attribute__((unused)) ssa_mem_ld_2 = (uint32_t)(arg2->LowPart);
    uint32_t __attribute__((unused)) ssa_mem_ld_3 = (uint32_t)(arg1->LowPart);
    if (((uint32_t)(ssa_mem_ld_3) == (uint32_t)(ssa_mem_ld_2))) {
        goto L_bb_1407182ce;
    }
    ssa_common_ret_op = (uint64_t)(0ULL);
L_common_ret:
    return (uint32_t)(ssa_common_ret_op);
L_bb_1407182ce:
    ssa_common_ret_op = (uint64_t)(((uint64_t)(uint32_t)(((uint32_t)(((uint32_t)(ssa_mem_ld_2) & (uint32_t)(0xffffffffffffff00ULL))) | (uint32_t)(1ULL)))));
    goto L_common_ret;
}

/*
 * Function : RtlIsNonEmptyDirectoryReparsePointAllowed @ 0x140111580 (20 bytes)
 * Z3 Proof : VERIFIED_EQUIVALENT (9.47 ms, 33 SMT assertions)
 * Verified : win64_callee_saved_abi, x86_64_to_llvm_ir_equivalence
 */
uint8_t __attribute__((ms_abi)) RtlIsNonEmptyDirectoryReparsePointAllowed(uint32_t arg1) {
    uint32_t __attribute__((unused)) ssa_r_ecx = (uint32_t)(((uint32_t)(uint64_t)(((uint64_t)arg1))));
    return (uint8_t)(((uint64_t)(uint8_t)(((((uint32_t)(ssa_r_ecx) == (uint32_t)(0xffffffff80000018ULL))) || (((uint32_t)(((uint32_t)(ssa_r_ecx) & (uint32_t)(0x10000000ULL))) != (uint32_t)(0ULL)))))));
}

/*
 * Function : RtlLengthRequiredSid @ 0x1404c4f60 (20 bytes)
 * Z3 Proof : VERIFIED_EQUIVALENT (21.59 ms, 33 SMT assertions)
 * Verified : win64_callee_saved_abi, x86_64_to_llvm_ir_equivalence
 */
uint32_t __attribute__((ms_abi)) RtlLengthRequiredSid(uint32_t arg1) {
    uint32_t __attribute__((unused)) ssa_r_ecx = (uint32_t)(((uint32_t)(uint64_t)(((uint64_t)arg1))));
    return (uint32_t)(((uint64_t)(uint32_t)(((((uint32_t)(ssa_r_ecx) > (uint32_t)(0x3ffffff7ULL))) ? (0xffffffffffffffffULL) : (((uint32_t)(((uint32_t)(ssa_r_ecx) << (uint32_t)(2ULL))) + (uint32_t)(8ULL)))))));
}

/*
 * Function : vsprintf @ 0x140144dc0 (20 bytes)
 * Z3 Proof : VERIFIED_EQUIVALENT (4.01 ms, 33 SMT assertions)
 * Verified : win64_callee_saved_abi, x86_64_to_llvm_ir_equivalence
 */
uint64_t __attribute__((ms_abi)) vsprintf(uint64_t arg1, uint64_t arg2, uint64_t arg3) {
    uint64_t __attribute__((unused)) ssa_call_res = _vsprintf_l(((uint64_t)arg1), ((uint64_t)arg2), 0ULL, ((uint64_t)arg3));
    return (uint64_t)(ssa_call_res);
}

/*
 * Function : RtlCopyUnicodeString @ 0x140055c60 (21 bytes)
 * Z3 Proof : VERIFIED_EQUIVALENT (23.07 ms, 33 SMT assertions)
 * Verified : x86_64_to_llvm_ir_equivalence
 */
uint32_t __attribute__((ms_abi)) RtlCopyUnicodeString(uint16_t* arg1, PUNICODE_STRING arg2) {
    uint64_t __attribute__((unused)) ssa_common_ret_op = 0;
    if (((uint64_t)(((uint64_t)arg2)) == (uint64_t)(0ULL))) {
        (*(uint16_t*)(uintptr_t)(((uint64_t)(uintptr_t)arg1))) = (uint16_t)(0ULL);
        ssa_common_ret_op = (uint64_t)(0ULL);
    } else {
        uint16_t __attribute__((unused)) ssa_mem_ld = (uint16_t)(arg2->Length);
        ssa_common_ret_op = (uint64_t)(((uint64_t)(uint16_t)(ssa_mem_ld)));
    }
    return (uint32_t)(ssa_common_ret_op);
}

/*
 * Function : RtlFindNextForwardRunClear @ 0x14003a080 (21 bytes)
 * Z3 Proof : VERIFIED_EQUIVALENT (4.68 ms, 33 SMT assertions)
 * Verified : win64_callee_saved_abi, x86_64_to_llvm_ir_equivalence
 */
uint64_t __attribute__((ms_abi)) RtlFindNextForwardRunClear(uint64_t arg1, uint64_t arg2, uint64_t arg3) {
    uint64_t __attribute__((unused)) ssa_call_res = RtlFindNextForwardRunClearCapped(((uint64_t)arg1), ((uint64_t)arg2), 0xffffffffULL, ((uint64_t)arg3));
    return (uint64_t)(ssa_call_res);
}

/*
 * Function : RtlGetDefaultCodePage @ 0x1405db910 (21 bytes)
 * Z3 Proof : VERIFIED_EQUIVALENT (4.09 ms, 33 SMT assertions)
 * Verified : win64_callee_saved_abi, x86_64_to_llvm_ir_equivalence
 */
uint32_t __attribute__((ms_abi)) RtlGetDefaultCodePage(uint16_t* arg1, uint16_t* arg2) {
    uint16_t __attribute__((unused)) ssa_mem_ld = (uint16_t)((*(uint16_t*)(uintptr_t)(((uint64_t)(uintptr_t)&NlsAnsiCodePage))));
    (*(uint16_t*)(uintptr_t)(((uint64_t)(uintptr_t)arg1))) = (uint16_t)(ssa_mem_ld);
    uint16_t __attribute__((unused)) ssa_mem_ld_1 = (uint16_t)((*(uint16_t*)(uintptr_t)(((uint64_t)(uintptr_t)&NlsOemCodePage))));
    (*(uint16_t*)(uintptr_t)(((uint64_t)(uintptr_t)arg2))) = (uint16_t)(ssa_mem_ld_1);
    return (uint32_t)(((uint64_t)(uint16_t)(ssa_mem_ld_1)));
}

/*
 * Function : LdrFindResource_U @ 0x1405a0fa0 (22 bytes)
 * Z3 Proof : VERIFIED_EQUIVALENT (4.00 ms, 33 SMT assertions)
 * Verified : win64_callee_saved_abi, x86_64_to_llvm_ir_equivalence
 */
uint64_t __attribute__((ms_abi)) LdrFindResource_U(uint64_t arg1, uint64_t arg2, uint64_t arg3, uint64_t arg4) {
    uint64_t __attribute__((unused)) ssa_call_res = LdrpSearchResourceSection_U(((uint64_t)arg1), ((uint64_t)arg2), ((uint64_t)arg3), 0ULL);
    return (uint64_t)(ssa_call_res);
}

/*
 * Function : RtlFindFirstRunClear @ 0x14000f940 (23 bytes)
 * Z3 Proof : VERIFIED_EQUIVALENT (3.71 ms, 33 SMT assertions)
 * Verified : win64_callee_saved_abi, x86_64_to_llvm_ir_equivalence
 */
uint64_t __attribute__((ms_abi)) RtlFindFirstRunClear(uint64_t arg1, uint64_t arg2, uint32_t arg3) {
    uint64_t __attribute__((unused)) ssa_call_res = RtlFindNextForwardRunClearCapped(((uint64_t)arg1), 0ULL, 0xffffffffULL, ((uint64_t)arg2));
    return (uint64_t)(ssa_call_res);
}

/*
 * Function : RtlCrc32 @ 0x14024b8c0 (24 bytes)
 * Z3 Proof : VERIFIED_EQUIVALENT (3.87 ms, 33 SMT assertions)
 * Verified : win64_callee_saved_abi, x86_64_to_llvm_ir_equivalence
 */
uint64_t __attribute__((ms_abi)) RtlCrc32(uint64_t arg1, uint64_t arg2, uint32_t arg3) {
    uint64_t __attribute__((unused)) ssa_call_res = RtlpComputeCrcInternal(((uint64_t)arg1), ((uint64_t)arg2), ((uint64_t)(((uint64_t)arg3)) & (uint64_t)(0xffffffffULL)), ((uint64_t)(uintptr_t)&Crc32Ctrl));
    return (uint64_t)(ssa_call_res);
}

/*
 * Function : RtlIsNtDdiVersionAvailable @ 0x140141d60 (24 bytes)
 * Z3 Proof : VERIFIED_EQUIVALENT (13.24 ms, 33 SMT assertions)
 * Verified : win64_callee_saved_abi, x86_64_to_llvm_ir_equivalence
 */
uint32_t __attribute__((ms_abi)) RtlIsNtDdiVersionAvailable(uint32_t arg1) {
    uint32_t __attribute__((unused)) ssa_r_ecx = (uint32_t)(((uint32_t)(uint64_t)(((uint64_t)arg1))));
    uint32_t __attribute__((unused)) ssa__37 = (uint32_t)(((uint32_t)(((uint32_t)(ssa_r_ecx) >> (uint32_t)(8ULL))) | (uint32_t)(ssa_r_ecx)));
    uint64_t __attribute__((unused)) ssa__44 = (uint64_t)(((uint64_t)(uint32_t)(ssa__37)));
    return (uint32_t)(((((uint8_t)(((uint8_t)(uint32_t)(ssa__37))) == (uint8_t)(0ULL))) ? (((uint64_t)(ssa__44) | (uint64_t)(((uint64_t)(uint8_t)(((uint32_t)(ssa_r_ecx) < (uint32_t)(0xa000005ULL))))))) : (((uint64_t)(ssa__44) & (uint64_t)(0xffffff00ULL)))));
}

/*
 * Function : LdrFindResourceDirectory_U @ 0x1407156c0 (25 bytes)
 * Z3 Proof : VERIFIED_EQUIVALENT (3.89 ms, 33 SMT assertions)
 * Verified : win64_callee_saved_abi, x86_64_to_llvm_ir_equivalence
 */
uint64_t __attribute__((ms_abi)) LdrFindResourceDirectory_U(uint64_t arg1, uint64_t arg2, uint64_t arg3, uint64_t arg4) {
    uint64_t __attribute__((unused)) ssa_call_res = LdrpSearchResourceSection_U(((uint64_t)arg1), ((uint64_t)arg2), ((uint64_t)arg3), 2ULL);
    return (uint64_t)(ssa_call_res);
}

/*
 * Function : RtlCreateHashTable @ 0x1401018c0 (25 bytes)
 * Z3 Proof : VERIFIED_EQUIVALENT (3.76 ms, 33 SMT assertions)
 * Verified : win64_callee_saved_abi, x86_64_to_llvm_ir_equivalence
 */
uint64_t __attribute__((ms_abi)) RtlCreateHashTable(uint64_t arg1, uint32_t arg2, uint32_t arg3) {
    uint64_t __attribute__((unused)) ssa_call_res = RtlpCreateHashTable(((uint64_t)arg1), 0x80ULL, ((uint64_t)(((uint64_t)arg2)) & (uint64_t)(0xffffffffULL)), ((uint64_t)(((uint64_t)arg3)) & (uint64_t)(0xffffffffULL)));
    return (uint64_t)(ssa_call_res);
}

/*
 * Function : RtlIsCloudFilesPlaceholder @ 0x14024d4e0 (26 bytes)
 * Z3 Proof : VERIFIED_EQUIVALENT (13.27 ms, 33 SMT assertions)
 * Verified : win64_callee_saved_abi, x86_64_to_llvm_ir_equivalence
 */
uint8_t __attribute__((ms_abi)) RtlIsCloudFilesPlaceholder(uint32_t arg1, uint32_t arg2) {
    return (uint8_t)(((uint64_t)(uint8_t)(((((uint64_t)(((uint64_t)(((uint64_t)arg1)) & (uint64_t)(0x400ULL))) != (uint64_t)(0ULL))) ? (((uint64_t)(((uint64_t)(((uint64_t)arg2)) & (uint64_t)(0xffff0fffULL))) == (uint64_t)(0x9000001aULL))) : (0ULL)))));
}

/*
 * Function : RtlGetControlSecurityDescriptor @ 0x1404bf250 (27 bytes)
 * Z3 Proof : VERIFIED_EQUIVALENT (5.03 ms, 33 SMT assertions)
 * Verified : win64_callee_saved_abi, x86_64_to_llvm_ir_equivalence
 */
NTSTATUS __attribute__((ms_abi)) RtlGetControlSecurityDescriptor(PSECURITY_DESCRIPTOR arg1, uint16_t* arg2, uint32_t* arg3) {
    uint64_t __attribute__((unused)) ssa_common_ret_op = 0;
    uint8_t __attribute__((unused)) ssa_mem_ld = (uint8_t)(arg1->Revision);
    (*(uint32_t*)(uintptr_t)(((uint64_t)(uintptr_t)arg3))) = (uint32_t)(((uint32_t)(uint8_t)(ssa_mem_ld)));
    uint8_t __attribute__((unused)) ssa_mem_ld_1 = (uint8_t)(arg1->Revision);
    if (((uint8_t)(ssa_mem_ld_1) == (uint8_t)(1ULL))) {
        uint16_t __attribute__((unused)) ssa_mem_ld_2 = (uint16_t)(arg1->Control);
        (*(uint16_t*)(uintptr_t)(((uint64_t)(uintptr_t)arg2))) = (uint16_t)(ssa_mem_ld_2);
        ssa_common_ret_op = (uint64_t)(0ULL);
    }
    else {
        ssa_common_ret_op = (uint64_t)(0xc0000058ULL);
    }
    return (NTSTATUS)(ssa_common_ret_op);
}

/*
 * Function : RtlIsServicePackVersionInstalled @ 0x14024d280 (27 bytes)
 * Z3 Proof : VERIFIED_EQUIVALENT (20.02 ms, 33 SMT assertions)
 * Verified : win64_callee_saved_abi, x86_64_to_llvm_ir_equivalence
 */
uint32_t __attribute__((ms_abi)) RtlIsServicePackVersionInstalled(uint32_t arg1) {
    return (uint32_t)(((uint64_t)(((uint64_t)(((uint64_t)arg1)) & (uint64_t)(0xffff0000ULL))) | (uint64_t)(((uint64_t)(uint8_t)(((uint64_t)(((uint64_t)(((uint64_t)arg1)) & (uint64_t)(0xffffff00ULL))) == (uint64_t)(0xa000000ULL)))))));
}

/*
 * Function : RtlGetSystemBootStatusEx @ 0x140594ad0 (28 bytes)
 * Z3 Proof : VERIFIED_EQUIVALENT (4.07 ms, 33 SMT assertions)
 * Verified : win64_callee_saved_abi, x86_64_to_llvm_ir_equivalence
 */
uint64_t __attribute__((ms_abi)) RtlGetSystemBootStatusEx(uint64_t arg1, uint64_t arg2, uint64_t arg3) {
    uint64_t __attribute__((unused)) ssa_call_res = RtlpSystemBootStatusRequest(0x1fULL, ((uint64_t)arg1), ((uint64_t)(((uint64_t)arg2)) & (uint64_t)(0xffffffffULL)), ((uint64_t)arg3));
    return (uint64_t)(ssa_call_res);
}

/*
 * Function : RtlGetTokenNamedObjectPath @ 0x140718410 (28 bytes)
 * Z3 Proof : VERIFIED_EQUIVALENT (9.56 ms, 33 SMT assertions)
 * Verified : win64_callee_saved_abi, x86_64_to_llvm_ir_equivalence
 */
uint64_t __attribute__((ms_abi)) RtlGetTokenNamedObjectPath(uint64_t arg1, uint8_t arg2, uint64_t arg3) {
    uint64_t __attribute__((unused)) ssa_call_res = RtlpGetTokenNamedObjectPath(((uint64_t)arg1), 0ULL, ((uint64_t)(uint8_t)(((uint8_t)(((uint8_t)(uint64_t)(((uint64_t)arg2)))) != (uint8_t)(0ULL)))), ((uint64_t)arg3));
    return (uint64_t)(ssa_call_res);
}

/*
 * Function : RtlSetSystemBootStatusEx @ 0x140719f70 (28 bytes)
 * Z3 Proof : VERIFIED_EQUIVALENT (3.55 ms, 33 SMT assertions)
 * Verified : win64_callee_saved_abi, x86_64_to_llvm_ir_equivalence
 */
uint64_t __attribute__((ms_abi)) RtlSetSystemBootStatusEx(uint64_t arg1, uint64_t arg2, uint64_t arg3) {
    uint64_t __attribute__((unused)) ssa_call_res = RtlpSystemBootStatusRequest(0x20ULL, ((uint64_t)arg1), ((uint64_t)(((uint64_t)arg2)) & (uint64_t)(0xffffffffULL)), ((uint64_t)arg3));
    return (uint64_t)(ssa_call_res);
}

/*
 * Function : SkAcquirePushLockExclusive @ 0x140246650 (29 bytes)
 * Z3 Proof : VERIFIED_EQUIVALENT (18.97 ms, 33 SMT assertions)
 * Verified : x86_64_to_llvm_ir_equivalence
 */
uint64_t __attribute__((ms_abi)) SkAcquirePushLockExclusive(void) {
    uint64_t __attribute__((unused)) ssa_call_res = KeBugCheckEx(0x123ULL, 0ULL, 0ULL, 0ULL);
    return (uint64_t)(ssa_call_res);
}

/*
 * Function : isdigit @ 0x140143a00 (29 bytes)
 * Z3 Proof : VERIFIED_EQUIVALENT (14.62 ms, 33 SMT assertions)
 * Verified : win64_callee_saved_abi, x86_64_to_llvm_ir_equivalence
 */
int64_t __attribute__((ms_abi)) isdigit(int64_t arg1, uint32_t arg2) {
    uint32_t __attribute__((unused)) ssa_v_0 = (uint32_t)(((uint32_t)(uint64_t)(((uint64_t)arg1))));
    uint64_t __attribute__((unused)) ssa_mem_ld = (uint64_t)((*(uint64_t*)(uintptr_t)(((uint64_t)(uintptr_t)&__nt_data_14034b150))));
    uint16_t __attribute__((unused)) ssa_mem_ld_1 = (uint16_t)((*(uint16_t*)(uintptr_t)(((uint64_t)(ssa_mem_ld) + (uint64_t)(((uint64_t)(((uint64_t)(int64_t)(int32_t)(((((uint32_t)(ssa_v_0) == (uint32_t)(0xffffffffffffffffULL))) ? (0xffffffffffffffffULL) : (((uint32_t)(ssa_v_0) & (uint32_t)(0xffULL))))))) << (uint64_t)(1ULL)))))));
    return (int64_t)(((uint64_t)(uint16_t)(((uint16_t)(ssa_mem_ld_1) & (uint16_t)(4ULL)))));
}

/*
 * Function : islower @ 0x140143a30 (29 bytes)
 * Z3 Proof : VERIFIED_EQUIVALENT (15.55 ms, 33 SMT assertions)
 * Verified : win64_callee_saved_abi, x86_64_to_llvm_ir_equivalence
 */
int64_t __attribute__((ms_abi)) islower(int64_t arg1, uint32_t arg2) {
    uint32_t __attribute__((unused)) ssa_v_0 = (uint32_t)(((uint32_t)(uint64_t)(((uint64_t)arg1))));
    uint64_t __attribute__((unused)) ssa_mem_ld = (uint64_t)((*(uint64_t*)(uintptr_t)(((uint64_t)(uintptr_t)&__nt_data_14034b150))));
    uint16_t __attribute__((unused)) ssa_mem_ld_1 = (uint16_t)((*(uint16_t*)(uintptr_t)(((uint64_t)(ssa_mem_ld) + (uint64_t)(((uint64_t)(((uint64_t)(int64_t)(int32_t)(((((uint32_t)(ssa_v_0) == (uint32_t)(0xffffffffffffffffULL))) ? (0xffffffffffffffffULL) : (((uint32_t)(ssa_v_0) & (uint32_t)(0xffULL))))))) << (uint64_t)(1ULL)))))));
    return (int64_t)(((uint64_t)(uint16_t)(((uint16_t)(ssa_mem_ld_1) & (uint16_t)(2ULL)))));
}

/*
 * Function : isspace @ 0x140143a90 (29 bytes)
 * Z3 Proof : VERIFIED_EQUIVALENT (14.16 ms, 33 SMT assertions)
 * Verified : win64_callee_saved_abi, x86_64_to_llvm_ir_equivalence
 */
int64_t __attribute__((ms_abi)) isspace(int64_t arg1, uint32_t arg2) {
    uint32_t __attribute__((unused)) ssa_v_0 = (uint32_t)(((uint32_t)(uint64_t)(((uint64_t)arg1))));
    uint64_t __attribute__((unused)) ssa_mem_ld = (uint64_t)((*(uint64_t*)(uintptr_t)(((uint64_t)(uintptr_t)&__nt_data_14034b150))));
    uint16_t __attribute__((unused)) ssa_mem_ld_1 = (uint16_t)((*(uint16_t*)(uintptr_t)(((uint64_t)(ssa_mem_ld) + (uint64_t)(((uint64_t)(((uint64_t)(int64_t)(int32_t)(((((uint32_t)(ssa_v_0) == (uint32_t)(0xffffffffffffffffULL))) ? (0xffffffffffffffffULL) : (((uint32_t)(ssa_v_0) & (uint32_t)(0xffULL))))))) << (uint64_t)(1ULL)))))));
    return (int64_t)(((uint64_t)(uint16_t)(((uint16_t)(ssa_mem_ld_1) & (uint16_t)(8ULL)))));
}

/*
 * Function : isupper @ 0x140143ac0 (29 bytes)
 * Z3 Proof : VERIFIED_EQUIVALENT (14.20 ms, 33 SMT assertions)
 * Verified : win64_callee_saved_abi, x86_64_to_llvm_ir_equivalence
 */
int64_t __attribute__((ms_abi)) isupper(int64_t arg1, uint32_t arg2) {
    uint32_t __attribute__((unused)) ssa_v_0 = (uint32_t)(((uint32_t)(uint64_t)(((uint64_t)arg1))));
    uint64_t __attribute__((unused)) ssa_mem_ld = (uint64_t)((*(uint64_t*)(uintptr_t)(((uint64_t)(uintptr_t)&__nt_data_14034b150))));
    uint16_t __attribute__((unused)) ssa_mem_ld_1 = (uint16_t)((*(uint16_t*)(uintptr_t)(((uint64_t)(ssa_mem_ld) + (uint64_t)(((uint64_t)(((uint64_t)(int64_t)(int32_t)(((((uint32_t)(ssa_v_0) == (uint32_t)(0xffffffffffffffffULL))) ? (0xffffffffffffffffULL) : (((uint32_t)(ssa_v_0) & (uint32_t)(0xffULL))))))) << (uint64_t)(1ULL)))))));
    return (int64_t)(((uint64_t)(uint16_t)(((uint16_t)(ssa_mem_ld_1) & (uint16_t)(1ULL)))));
}

/*
 * Function : sprintf_s @ 0x1401490e0 (29 bytes)
 * Z3 Proof : VERIFIED_EQUIVALENT (5.15 ms, 33 SMT assertions)
 * Verified : win64_callee_saved_abi, x86_64_to_llvm_ir_equivalence
 */
uint64_t __attribute__((ms_abi)) sprintf_s(uint64_t arg1, uint64_t arg2, uint64_t arg3, uint64_t arg4) {
    uint8_t __attribute__((aligned(16))) ssa_stack_frame[1024];
    (*(uint64_t*)(uintptr_t)((((uint64_t)(uintptr_t)ssa_stack_frame) + 792ULL))) = (uint64_t)(((uint64_t)arg3));
    (*(uint64_t*)(uintptr_t)((((uint64_t)(uintptr_t)ssa_stack_frame) + 800ULL))) = (uint64_t)(((uint64_t)arg4));
    uint64_t __attribute__((unused)) ssa_call_res = vsprintf_s(((uint64_t)arg1), ((uint64_t)arg2), ((uint64_t)arg3), (((uint64_t)(uintptr_t)ssa_stack_frame) + 800ULL));
    return (uint64_t)(ssa_call_res);
}

/*
 * Function : swprintf_s @ 0x140147de0 (29 bytes)
 * Z3 Proof : VERIFIED_EQUIVALENT (4.98 ms, 33 SMT assertions)
 * Verified : win64_callee_saved_abi, x86_64_to_llvm_ir_equivalence
 */
uint64_t __attribute__((ms_abi)) swprintf_s(uint64_t arg1, uint64_t arg2, uint64_t arg3, uint64_t arg4) {
    uint8_t __attribute__((aligned(16))) ssa_stack_frame[1024];
    (*(uint64_t*)(uintptr_t)((((uint64_t)(uintptr_t)ssa_stack_frame) + 792ULL))) = (uint64_t)(((uint64_t)arg3));
    (*(uint64_t*)(uintptr_t)((((uint64_t)(uintptr_t)ssa_stack_frame) + 800ULL))) = (uint64_t)(((uint64_t)arg4));
    uint64_t __attribute__((unused)) ssa_call_res = vswprintf_s(((uint64_t)arg1), ((uint64_t)arg2), ((uint64_t)arg3), (((uint64_t)(uintptr_t)ssa_stack_frame) + 800ULL));
    return (uint64_t)(ssa_call_res);
}

/*
 * Function : RtlAddAccessAllowedAce @ 0x140486970 (30 bytes)
 * Z3 Proof : VERIFIED_EQUIVALENT (4.33 ms, 33 SMT assertions)
 * Verified : win64_callee_saved_abi, x86_64_to_llvm_ir_equivalence
 */
uint64_t __attribute__((ms_abi)) RtlAddAccessAllowedAce(uint64_t arg1, uint64_t arg2, uint32_t arg3, uint64_t arg4) {
    uint64_t __attribute__((unused)) ssa_call_res = RtlpAddKnownAce(((uint64_t)arg1), ((uint64_t)arg2), 0ULL, ((uint64_t)(((uint64_t)arg3)) & (uint64_t)(0xffffffffULL)));
    return (uint64_t)(ssa_call_res);
}

/*
 * Function : rand @ 0x140144160 (30 bytes)
 * Z3 Proof : VERIFIED_EQUIVALENT (4.68 ms, 33 SMT assertions)
 * Verified : win64_callee_saved_abi, x86_64_to_llvm_ir_equivalence
 */
int32_t __attribute__((ms_abi)) rand(void) {
    uint32_t __attribute__((unused)) ssa_mem_ld = (uint32_t)((*(uint32_t*)(uintptr_t)(((uint64_t)(uintptr_t)&__nt_data_14034e510))));
    uint32_t __attribute__((unused)) ssa__27 = (uint32_t)(((uint32_t)(((uint32_t)(ssa_mem_ld) * (uint32_t)(0x343fdULL))) + (uint32_t)(0x269ec3ULL)));
    (*(uint32_t*)(uintptr_t)(((uint64_t)(uintptr_t)&__nt_data_14034e510))) = (uint32_t)(ssa__27);
    return (int32_t)(((uint64_t)(uint32_t)(((uint32_t)(((uint32_t)(ssa__27) >> (uint32_t)(0x10ULL))) & (uint32_t)(0x7fffULL)))));
}

/*
 * Function : RtlDrainNonVolatileFlush @ 0x1402484d0 (31 bytes)
 * Z3 Proof : VERIFIED_EQUIVALENT (3.98 ms, 33 SMT assertions)
 * Verified : win64_callee_saved_abi, x86_64_to_llvm_ir_equivalence
 */
NTSTATUS __attribute__((ms_abi)) RtlDrainNonVolatileFlush(uint8_t arg1) {
    uint64_t __attribute__((unused)) ssa_common_ret_op = 0;
    if (((uint64_t)(((uint64_t)(((uint64_t)arg1)) & (uint64_t)(1ULL))) == (uint64_t)(0ULL))) {
        ssa_common_ret_op = (uint64_t)(0xc000000dULL);
    } else {
        ssa_common_ret_op = (uint64_t)(0ULL);
    }
    return (NTSTATUS)(ssa_common_ret_op);
}

/*
 * Function : RtlEqualUnicodeString @ 0x1404e70f0 (31 bytes)
 * Z3 Proof : VERIFIED_EQUIVALENT (23.40 ms, 33 SMT assertions)
 * Verified : x86_64_to_llvm_ir_equivalence
 */
uint32_t __attribute__((ms_abi)) RtlEqualUnicodeString(PUNICODE_STRING arg1, PUNICODE_STRING arg2) {
    uint16_t __attribute__((unused)) ssa_mem_ld = (uint16_t)(arg1->Length);
    uint64_t __attribute__((unused)) ssa__41 = (uint64_t)(((uint64_t)(uint16_t)(ssa_mem_ld)));
    uint16_t __attribute__((unused)) ssa_mem_ld_1 = (uint16_t)(arg2->Length);
    return (uint32_t)(((((uint16_t)(ssa_mem_ld) == (uint16_t)(ssa_mem_ld_1))) ? (ssa__41) : (((uint64_t)(ssa__41) & (uint64_t)(0xff00ULL)))));
}
