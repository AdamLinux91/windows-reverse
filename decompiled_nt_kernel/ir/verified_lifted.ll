; ModuleID = '<string>'
source_filename = "<string>"
target triple = "x86_64-pc-windows-msvc"

define i64 @KeGetCurrentThread() local_unnamed_addr {
lift_init:
  %gs_val = tail call i64 @__nt_read_gs_i64(i64 392)
  ret i64 %gs_val
}

declare i64 @__nt_read_gs_i64(i64) local_unnamed_addr


; ModuleID = '<string>'
source_filename = "<string>"
target triple = "x86_64-pc-windows-msvc"

define range(i64 0, 4294967296) i64 @KeGetCurrentProcessorNumberEx(i64 %arg_rcx) local_unnamed_addr {
lift_init:
  %gs_val = tail call i64 @__nt_read_gs_i64(i64 32)
  %.27.not = icmp eq i64 %arg_rcx, 0
  br i1 %.27.not, label %bb_14001f57e, label %bb_14001f582

bb_14001f57e:                                     ; preds = %bb_14001f582, %lift_init
  %.39 = add i64 %gs_val, 36
  %.40 = inttoptr i64 %.39 to ptr
  %mem_ld = load i32, ptr %.40, align 4
  %.41 = zext i32 %mem_ld to i64
  ret i64 %.41

bb_14001f582:                                     ; preds = %lift_init
  %.46 = add i64 %gs_val, 208
  %.47 = inttoptr i64 %.46 to ptr
  %mem_ld.1 = load i8, ptr %.47, align 1
  %r_dx = zext i8 %mem_ld.1 to i16
  %.51 = inttoptr i64 %arg_rcx to ptr
  store i16 %r_dx, ptr %.51, align 2
  %.53 = add i64 %gs_val, 209
  %.54 = inttoptr i64 %.53 to ptr
  %mem_ld.2 = load i8, ptr %.54, align 1
  %.58 = add i64 %arg_rcx, 2
  %.59 = inttoptr i64 %.58 to ptr
  store i8 %mem_ld.2, ptr %.59, align 1
  %.61 = add i64 %arg_rcx, 3
  %.62 = inttoptr i64 %.61 to ptr
  store i8 0, ptr %.62, align 1
  br label %bb_14001f57e
}

declare i64 @__nt_read_gs_i64(i64) local_unnamed_addr


; ModuleID = '<string>'
source_filename = "<string>"
target triple = "x86_64-pc-windows-msvc"

; Function Attrs: nofree nosync nounwind memory(read, inaccessiblemem: none, target_mem0: none, target_mem1: none)
define range(i64 0, 4294967296) i64 @KeQueryPriorityThread(i64 %arg_rcx) local_unnamed_addr #0 {
lift_init:
  %addr_KiInitialProcess = tail call i64 @__nt_sym_addr_KiInitialProcess()
  %.25 = add i64 %arg_rcx, 544
  %.26 = inttoptr i64 %.25 to ptr
  %mem_ld = load i64, ptr %.26, align 4
  %.28 = icmp eq i64 %mem_ld, %addr_KiInitialProcess
  br i1 %.28, label %common.ret, label %bb_140033a00

common.ret:                                       ; preds = %lift_init, %bb_140033a00
  %common.ret.op = phi i64 [ %.48, %bb_140033a00 ], [ 1, %lift_init ]
  ret i64 %common.ret.op

bb_140033a00:                                     ; preds = %lift_init
  %.45 = add i64 %arg_rcx, 195
  %.46 = inttoptr i64 %.45 to ptr
  %mem_ld.1 = load i8, ptr %.46, align 1
  %.47 = sext i8 %mem_ld.1 to i32
  %.48 = zext i32 %.47 to i64
  br label %common.ret
}

; Function Attrs: nofree nosync nounwind memory(none)
declare i64 @__nt_sym_addr_KiInitialProcess() local_unnamed_addr #1

attributes #0 = { nofree nosync nounwind memory(read, inaccessiblemem: none, target_mem0: none, target_mem1: none) }
attributes #1 = { nofree nosync nounwind memory(none) }


; ModuleID = '<string>'
source_filename = "<string>"
target triple = "x86_64-pc-windows-msvc"

; Function Attrs: mustprogress nofree norecurse nosync nounwind willreturn memory(write, inaccessiblemem: none, target_mem0: none, target_mem1: none)
define noundef i64 @KeInitializeDpc(i64 %arg_rcx, i64 %arg_rdx, i64 %arg_r8) local_unnamed_addr #0 {
lift_init:
  %.34 = inttoptr i64 %arg_rcx to ptr
  store i32 275, ptr %.34, align 4
  %.36 = add i64 %arg_rcx, 56
  %.37 = inttoptr i64 %.36 to ptr
  store i64 0, ptr %.37, align 4
  %.39 = add i64 %arg_rcx, 16
  %.40 = inttoptr i64 %.39 to ptr
  store i64 0, ptr %.40, align 4
  %.42 = add i64 %arg_rcx, 24
  %.43 = inttoptr i64 %.42 to ptr
  store i64 %arg_rdx, ptr %.43, align 4
  %.45 = add i64 %arg_rcx, 32
  %.46 = inttoptr i64 %.45 to ptr
  store i64 %arg_r8, ptr %.46, align 4
  ret i64 0
}

attributes #0 = { mustprogress nofree norecurse nosync nounwind willreturn memory(write, inaccessiblemem: none, target_mem0: none, target_mem1: none) }


; ModuleID = '<string>'
source_filename = "<string>"
target triple = "x86_64-pc-windows-msvc"

; Function Attrs: mustprogress nofree norecurse nosync nounwind willreturn memory(write, inaccessiblemem: none, target_mem0: none, target_mem1: none)
define noundef i64 @KeSetImportanceDpc(i64 %arg_rcx, i64 %arg_rdx) local_unnamed_addr #0 {
lift_init:
  %r_dl = trunc i64 %arg_rdx to i8
  %.25 = add i64 %arg_rcx, 1
  %.26 = inttoptr i64 %.25 to ptr
  store i8 %r_dl, ptr %.26, align 1
  ret i64 0
}

attributes #0 = { mustprogress nofree norecurse nosync nounwind willreturn memory(write, inaccessiblemem: none, target_mem0: none, target_mem1: none) }


; ModuleID = '<string>'
source_filename = "<string>"
target triple = "x86_64-pc-windows-msvc"

define i64 @KeSetTargetProcessorDpc(i64 %arg_rcx, i64 %arg_rdx) local_unnamed_addr {
lift_init:
  %stack_frame = alloca [1024 x i8], align 16
  %.26 = getelementptr inbounds nuw i8, ptr %stack_frame, i64 776
  store i64 0, ptr %.26, align 8
  %.30 = getelementptr inbounds nuw i8, ptr %stack_frame, i64 792
  store i64 0, ptr %.30, align 8
  %.35 = getelementptr inbounds nuw i8, ptr %stack_frame, i64 760
  store i64 0, ptr %.35, align 8
  %addr_KeForceGroupAwareness = tail call i64 @__nt_sym_addr_KeForceGroupAwareness()
  %.52 = inttoptr i64 %addr_KeForceGroupAwareness to ptr
  %mem_ld = load i8, ptr %.52, align 1
  %.54 = icmp eq i8 %mem_ld, 0
  %r_dl = trunc i64 %arg_rdx to i8
  br i1 %.54, label %bb_140200bcc, label %bb_140200bbe

bb_140200bbe:                                     ; preds = %lift_init
  %addr_KiActiveGroups = tail call i64 @__nt_sym_addr_KiActiveGroups()
  %.77 = inttoptr i64 %addr_KiActiveGroups to ptr
  %mem_ld.1 = load i16, ptr %.77, align 2
  %.82 = add i16 %mem_ld.1, -1
  br label %bb_140200bcc

bb_140200bcc:                                     ; preds = %lift_init, %bb_140200bbe
  %reg_rbx.0.off0 = phi i16 [ %.82, %bb_140200bbe ], [ 0, %lift_init ]
  %.112 = zext i16 %reg_rbx.0.off0 to i64
  %call_res = tail call i64 @KeQueryActiveProcessorCountEx(i64 %.112, i64 %arg_rdx, i64 0, i64 0)
  %.115 = sext i8 %r_dl to i32
  %r_eax = trunc i64 %call_res to i32
  %.123.not = icmp ult i32 %.115, %r_eax
  br i1 %.123.not, label %bb_140200bdc, label %bb_140200bf8

bb_140200bdc:                                     ; preds = %bb_140200bcc
  %.138 = getelementptr inbounds nuw i8, ptr %stack_frame, i64 784
  %.139 = ptrtoint ptr %.138 to i64
  store i16 %reg_rbx.0.off0, ptr %.138, align 16
  %.147 = getelementptr inbounds nuw i8, ptr %stack_frame, i64 786
  store i8 %r_dl, ptr %.147, align 2
  %.150 = getelementptr inbounds nuw i8, ptr %stack_frame, i64 787
  store i8 0, ptr %.150, align 1
  %call_res.1 = call i64 @KeSetTargetProcessorDpcEx(i64 %arg_rcx, i64 %.139, i64 0, i64 0)
  br label %bb_140200bf8

bb_140200bf8:                                     ; preds = %bb_140200bdc, %bb_140200bcc
  %reg_rax.0 = phi i64 [ %call_res, %bb_140200bcc ], [ %call_res.1, %bb_140200bdc ]
  ret i64 %reg_rax.0
}

; Function Attrs: nofree nosync nounwind memory(none)
declare i64 @__nt_sym_addr_KeForceGroupAwareness() local_unnamed_addr #0

; Function Attrs: nofree nosync nounwind memory(none)
declare i64 @__nt_sym_addr_KiActiveGroups() local_unnamed_addr #0

declare i64 @KeQueryActiveProcessorCountEx(i64, i64, i64, i64) local_unnamed_addr

declare i64 @KeSetTargetProcessorDpcEx(i64, i64, i64, i64) local_unnamed_addr

attributes #0 = { nofree nosync nounwind memory(none) }


; ModuleID = '<string>'
source_filename = "<string>"
target triple = "x86_64-pc-windows-msvc"

; Function Attrs: nofree norecurse nosync nounwind memory(read, inaccessiblemem: none, target_mem0: none, target_mem1: none)
define i64 @KeCheckForZeroPage(i64 %arg_rcx) local_unnamed_addr #0 {
lift_init:
  %.24 = add i64 %arg_rcx, 4096
  br label %bb_140166a90

bb_140166a90:                                     ; preds = %bb_140166a90, %lift_init
  %reg_rcx.0 = phi i64 [ %arg_rcx, %lift_init ], [ %.194, %bb_140166a90 ]
  %reg_rax.0 = phi i64 [ 0, %lift_init ], [ %.186, %bb_140166a90 ]
  %.35 = inttoptr i64 %reg_rcx.0 to ptr
  %mem_ld = load i64, ptr %.35, align 4
  %.36 = or i64 %mem_ld, %reg_rax.0
  %.44 = add i64 %reg_rcx.0, 8
  %.45 = inttoptr i64 %.44 to ptr
  %mem_ld.1 = load i64, ptr %.45, align 4
  %.46 = or i64 %.36, %mem_ld.1
  %.54 = add i64 %reg_rcx.0, 16
  %.55 = inttoptr i64 %.54 to ptr
  %mem_ld.2 = load i64, ptr %.55, align 4
  %.56 = or i64 %.46, %mem_ld.2
  %.64 = add i64 %reg_rcx.0, 24
  %.65 = inttoptr i64 %.64 to ptr
  %mem_ld.3 = load i64, ptr %.65, align 4
  %.66 = or i64 %.56, %mem_ld.3
  %.74 = add i64 %reg_rcx.0, 32
  %.75 = inttoptr i64 %.74 to ptr
  %mem_ld.4 = load i64, ptr %.75, align 4
  %.76 = or i64 %.66, %mem_ld.4
  %.84 = add i64 %reg_rcx.0, 40
  %.85 = inttoptr i64 %.84 to ptr
  %mem_ld.5 = load i64, ptr %.85, align 4
  %.86 = or i64 %.76, %mem_ld.5
  %.94 = add i64 %reg_rcx.0, 48
  %.95 = inttoptr i64 %.94 to ptr
  %mem_ld.6 = load i64, ptr %.95, align 4
  %.96 = or i64 %.86, %mem_ld.6
  %.104 = add i64 %reg_rcx.0, 56
  %.105 = inttoptr i64 %.104 to ptr
  %mem_ld.7 = load i64, ptr %.105, align 4
  %.106 = or i64 %.96, %mem_ld.7
  %.114 = add i64 %reg_rcx.0, 64
  %.115 = inttoptr i64 %.114 to ptr
  %mem_ld.8 = load i64, ptr %.115, align 4
  %.116 = or i64 %.106, %mem_ld.8
  %.124 = add i64 %reg_rcx.0, 72
  %.125 = inttoptr i64 %.124 to ptr
  %mem_ld.9 = load i64, ptr %.125, align 4
  %.126 = or i64 %.116, %mem_ld.9
  %.134 = add i64 %reg_rcx.0, 80
  %.135 = inttoptr i64 %.134 to ptr
  %mem_ld.10 = load i64, ptr %.135, align 4
  %.136 = or i64 %.126, %mem_ld.10
  %.144 = add i64 %reg_rcx.0, 88
  %.145 = inttoptr i64 %.144 to ptr
  %mem_ld.11 = load i64, ptr %.145, align 4
  %.146 = or i64 %.136, %mem_ld.11
  %.154 = add i64 %reg_rcx.0, 96
  %.155 = inttoptr i64 %.154 to ptr
  %mem_ld.12 = load i64, ptr %.155, align 4
  %.156 = or i64 %.146, %mem_ld.12
  %.164 = add i64 %reg_rcx.0, 104
  %.165 = inttoptr i64 %.164 to ptr
  %mem_ld.13 = load i64, ptr %.165, align 4
  %.166 = or i64 %.156, %mem_ld.13
  %.174 = add i64 %reg_rcx.0, 112
  %.175 = inttoptr i64 %.174 to ptr
  %mem_ld.14 = load i64, ptr %.175, align 4
  %.176 = or i64 %.166, %mem_ld.14
  %.184 = add i64 %reg_rcx.0, 120
  %.185 = inttoptr i64 %.184 to ptr
  %mem_ld.15 = load i64, ptr %.185, align 4
  %.186 = or i64 %.176, %mem_ld.15
  %.194 = add i64 %reg_rcx.0, 128
  %.201 = icmp ult i64 %.194, %.24
  br i1 %.201, label %bb_140166a90, label %bb_140166adb

bb_140166adb:                                     ; preds = %bb_140166a90
  ret i64 %.186
}

attributes #0 = { nofree norecurse nosync nounwind memory(read, inaccessiblemem: none, target_mem0: none, target_mem1: none) }


; ModuleID = '<string>'
source_filename = "<string>"
target triple = "x86_64-pc-windows-msvc"

define i64 @KeSetCoalescableTimer(i64 %arg_rcx, i64 %arg_rdx, i64 %arg_r8, i64 %arg_r9) local_unnamed_addr {
lift_init:
  %r_r8d = trunc i64 %arg_r8 to i32
  %r_r9d = trunc i64 %arg_r9 to i32
  %.58 = icmp eq i32 %r_r9d, 0
  br i1 %.58, label %bb_14003787e, label %bb_140037854

bb_140037854:                                     ; preds = %lift_init
  %.69 = and i64 %arg_r9, 4294967295
  %.71 = mul nuw nsw i64 %.69, 10000
  %.78 = icmp samesign ugt i64 %.69, 1651
  %extract.t65 = trunc i64 %.71 to i32
  br i1 %.78, label %bb_14003789c, label %bb_140037867

bb_140037867:                                     ; preds = %bb_1400378b7, %bb_1400378c7, %bb_140037854
  %reg_r9.1.off0 = phi i32 [ %extract.t65, %bb_140037854 ], [ 16515072, %bb_1400378c7 ], [ 16515072, %bb_1400378b7 ]
  %reg_r11.1.off0 = phi i32 [ %r_r8d, %bb_140037854 ], [ %.355, %bb_1400378c7 ], [ 0, %bb_1400378b7 ]
  %reg_rdx.1 = phi i64 [ %arg_rdx, %bb_140037854 ], [ %reg_r8.0, %bb_1400378c7 ], [ %reg_r8.0, %bb_1400378b7 ]
  %0 = tail call i32 @llvm.umin.i32(i32 %reg_r9.1.off0, i32 16515072)
  %sh.diff = lshr i32 %0, 16
  %tr.sh.diff = trunc nuw i32 %sh.diff to i8
  %.131 = and i8 %tr.sh.diff, -4
  br label %bb_14003787e

bb_14003787e:                                     ; preds = %lift_init, %bb_140037867
  %reg_r9.0 = phi i64 [ 0, %bb_140037867 ], [ %arg_r9, %lift_init ]
  %reg_r10.0.off0 = phi i8 [ %.131, %bb_140037867 ], [ 0, %lift_init ]
  %reg_r11.0.off0 = phi i32 [ %reg_r11.1.off0, %bb_140037867 ], [ %r_r8d, %lift_init ]
  %reg_rdx.0 = phi i64 [ %reg_rdx.1, %bb_140037867 ], [ %arg_rdx, %lift_init ]
  %.155 = zext i8 %reg_r10.0.off0 to i64
  %.156 = or disjoint i64 %reg_r9.0, %.155
  %.158 = zext i32 %reg_r11.0.off0 to i64
  %call_res = tail call i64 @KiSetTimerEx(i64 %arg_rcx, i64 %reg_rdx.0, i64 %.158, i64 %.156)
  ret i64 %call_res

bb_14003789c:                                     ; preds = %bb_140037854
  %.188 = add nsw i64 %.71, -16515072
  %.205 = icmp sgt i64 %arg_rdx, -1
  br i1 %.205, label %bb_1400378f3, label %bb_1400378a8

bb_1400378a8:                                     ; preds = %bb_14003789c
  %.216 = sub i64 %arg_rdx, %.188
  %.224 = xor i64 %.216, %arg_rdx
  %.230 = sub nsw i64 16515072, %.71
  %.238 = xor i64 %.216, %.230
  %1 = and i64 %.224, %.238
  %.240.not = icmp slt i64 %1, 0
  %spec.select = select i1 %.240.not, i64 -9223372036854775808, i64 %.216
  br label %bb_1400378b7

bb_1400378b7:                                     ; preds = %bb_1400378a8, %bb_1400378f3
  %reg_r8.0 = phi i64 [ %.382, %bb_1400378f3 ], [ %spec.select, %bb_1400378a8 ]
  %.253.not = icmp eq i32 %r_r8d, 0
  br i1 %.253.not, label %bb_140037867, label %bb_1400378c7

bb_1400378c7:                                     ; preds = %bb_1400378b7
  %.271 = sext i64 %.188 to i128
  %.272 = mul nsw i128 %.271, 3777893186295716171
  %.274 = lshr i128 %.272, 64
  %.275 = trunc nuw i128 %.274 to i64
  %.289 = ashr i64 %.275, 11
  %.300 = lshr i64 %.289, 63
  %.308 = add nsw i64 %.300, %.289
  %r_ecx = trunc i64 %.308 to i32
  %.355 = tail call i32 @llvm.uadd.sat.i32(i32 %r_r8d, i32 %r_ecx)
  br label %bb_140037867

bb_1400378f3:                                     ; preds = %bb_14003789c
  %.361 = add i64 %.188, %arg_rdx
  %2 = sub nsw i64 16515056, %.71
  %3 = and i64 %.361, %2
  %.374 = icmp slt i64 %3, 0
  %.382 = select i1 %.374, i64 9223372036854775807, i64 %.361
  br label %bb_1400378b7
}

declare i64 @KiSetTimerEx(i64, i64, i64, i64) local_unnamed_addr

; Function Attrs: nocallback nocreateundeforpoison nofree nosync nounwind speculatable willreturn memory(none)
declare i32 @llvm.uadd.sat.i32(i32, i32) #0

; Function Attrs: nocallback nocreateundeforpoison nofree nosync nounwind speculatable willreturn memory(none)
declare i32 @llvm.umin.i32(i32, i32) #0

attributes #0 = { nocallback nocreateundeforpoison nofree nosync nounwind speculatable willreturn memory(none) }


; ModuleID = '<string>'
source_filename = "<string>"
target triple = "x86_64-pc-windows-msvc"

; Function Attrs: mustprogress nofree norecurse nosync nounwind willreturn memory(read, inaccessiblemem: none, target_mem0: none, target_mem1: none)
define range(i64 0, 4294967296) i64 @KeReadStateEvent(i64 %arg_rcx) local_unnamed_addr #0 {
lift_init:
  %.24 = add i64 %arg_rcx, 4
  %.25 = inttoptr i64 %.24 to ptr
  %mem_ld = load i32, ptr %.25, align 4
  %.26 = zext i32 %mem_ld to i64
  ret i64 %.26
}

attributes #0 = { mustprogress nofree norecurse nosync nounwind willreturn memory(read, inaccessiblemem: none, target_mem0: none, target_mem1: none) }


; ModuleID = '<string>'
source_filename = "<string>"
target triple = "x86_64-pc-windows-msvc"

; Function Attrs: mustprogress nofree norecurse nosync nounwind willreturn memory(read, inaccessiblemem: none, target_mem0: none, target_mem1: none)
define range(i64 0, 4294967296) i64 @KeReadStateMutex(i64 %arg_rcx) local_unnamed_addr #0 {
lift_init:
  %.24 = add i64 %arg_rcx, 4
  %.25 = inttoptr i64 %.24 to ptr
  %mem_ld = load i32, ptr %.25, align 4
  %.26 = zext i32 %mem_ld to i64
  ret i64 %.26
}

attributes #0 = { mustprogress nofree norecurse nosync nounwind willreturn memory(read, inaccessiblemem: none, target_mem0: none, target_mem1: none) }


; ModuleID = '<string>'
source_filename = "<string>"
target triple = "x86_64-pc-windows-msvc"

; Function Attrs: mustprogress nofree norecurse nosync nounwind willreturn memory(read, inaccessiblemem: none, target_mem0: none, target_mem1: none)
define range(i64 0, 4294967296) i64 @KeReadStateSemaphore(i64 %arg_rcx) local_unnamed_addr #0 {
lift_init:
  %.24 = add i64 %arg_rcx, 4
  %.25 = inttoptr i64 %.24 to ptr
  %mem_ld = load i32, ptr %.25, align 4
  %.26 = zext i32 %mem_ld to i64
  ret i64 %.26
}

attributes #0 = { mustprogress nofree norecurse nosync nounwind willreturn memory(read, inaccessiblemem: none, target_mem0: none, target_mem1: none) }


; ModuleID = '<string>'
source_filename = "<string>"
target triple = "x86_64-pc-windows-msvc"

; Function Attrs: mustprogress nofree norecurse nosync nounwind willreturn memory(read, inaccessiblemem: none, target_mem0: none, target_mem1: none)
define range(i64 0, 256) i64 @KeReadStateTimer(i64 %arg_rcx) local_unnamed_addr #0 {
lift_init:
  %.24 = add i64 %arg_rcx, 4
  %.25 = inttoptr i64 %.24 to ptr
  %mem_ld = load i8, ptr %.25, align 1
  %.28 = zext i8 %mem_ld to i64
  ret i64 %.28
}

attributes #0 = { mustprogress nofree norecurse nosync nounwind willreturn memory(read, inaccessiblemem: none, target_mem0: none, target_mem1: none) }


; ModuleID = '<string>'
source_filename = "<string>"
target triple = "x86_64-pc-windows-msvc"

define i64 @PsGetCurrentProcess() local_unnamed_addr {
lift_init:
  %gs_val = tail call i64 @__nt_read_gs_i64(i64 392)
  %.25 = add i64 %gs_val, 184
  %.26 = inttoptr i64 %.25 to ptr
  %mem_ld = load i64, ptr %.26, align 4
  ret i64 %mem_ld
}

declare i64 @__nt_read_gs_i64(i64) local_unnamed_addr


; ModuleID = '<string>'
source_filename = "<string>"
target triple = "x86_64-pc-windows-msvc"

define i64 @PsGetCurrentThread() local_unnamed_addr {
lift_init:
  %gs_val = tail call i64 @__nt_read_gs_i64(i64 392)
  ret i64 %gs_val
}

declare i64 @__nt_read_gs_i64(i64) local_unnamed_addr


; ModuleID = '<string>'
source_filename = "<string>"
target triple = "x86_64-pc-windows-msvc"

; Function Attrs: mustprogress nofree norecurse nosync nounwind willreturn memory(read, inaccessiblemem: none, target_mem0: none, target_mem1: none)
define i64 @PsGetProcessId(i64 %arg_rcx) local_unnamed_addr #0 {
lift_init:
  %.24 = add i64 %arg_rcx, 736
  %.25 = inttoptr i64 %.24 to ptr
  %mem_ld = load i64, ptr %.25, align 4
  ret i64 %mem_ld
}

attributes #0 = { mustprogress nofree norecurse nosync nounwind willreturn memory(read, inaccessiblemem: none, target_mem0: none, target_mem1: none) }


; ModuleID = '<string>'
source_filename = "<string>"
target triple = "x86_64-pc-windows-msvc"

; Function Attrs: mustprogress nofree norecurse nosync nounwind willreturn memory(read, inaccessiblemem: none, target_mem0: none, target_mem1: none)
define i64 @PsGetProcessInheritedFromUniqueProcessId(i64 %arg_rcx) local_unnamed_addr #0 {
lift_init:
  %.24 = add i64 %arg_rcx, 992
  %.25 = inttoptr i64 %.24 to ptr
  %mem_ld = load i64, ptr %.25, align 4
  ret i64 %mem_ld
}

attributes #0 = { mustprogress nofree norecurse nosync nounwind willreturn memory(read, inaccessiblemem: none, target_mem0: none, target_mem1: none) }


; ModuleID = '<string>'
source_filename = "<string>"
target triple = "x86_64-pc-windows-msvc"

; Function Attrs: mustprogress nofree norecurse nosync nounwind willreturn memory(none)
define i64 @PsGetProcessImageFileName(i64 %arg_rcx) local_unnamed_addr #0 {
lift_init:
  %.24 = add i64 %arg_rcx, 1104
  ret i64 %.24
}

attributes #0 = { mustprogress nofree norecurse nosync nounwind willreturn memory(none) }


; ModuleID = '<string>'
source_filename = "<string>"
target triple = "x86_64-pc-windows-msvc"

; Function Attrs: mustprogress nofree norecurse nosync nounwind willreturn memory(read, inaccessiblemem: none, target_mem0: none, target_mem1: none)
define range(i64 0, 4294967296) i64 @PsGetProcessExitStatus(i64 %arg_rcx) local_unnamed_addr #0 {
lift_init:
  %.24 = add i64 %arg_rcx, 1572
  %.25 = inttoptr i64 %.24 to ptr
  %mem_ld = load i32, ptr %.25, align 4
  %.26 = zext i32 %mem_ld to i64
  ret i64 %.26
}

attributes #0 = { mustprogress nofree norecurse nosync nounwind willreturn memory(read, inaccessiblemem: none, target_mem0: none, target_mem1: none) }


; ModuleID = '<string>'
source_filename = "<string>"
target triple = "x86_64-pc-windows-msvc"

; Function Attrs: mustprogress nofree norecurse nosync nounwind willreturn memory(read, inaccessiblemem: none, target_mem0: none, target_mem1: none)
define i64 @PsGetProcessPeb(i64 %arg_rcx) local_unnamed_addr #0 {
lift_init:
  %.24 = add i64 %arg_rcx, 1016
  %.25 = inttoptr i64 %.24 to ptr
  %mem_ld = load i64, ptr %.25, align 4
  ret i64 %mem_ld
}

attributes #0 = { mustprogress nofree norecurse nosync nounwind willreturn memory(read, inaccessiblemem: none, target_mem0: none, target_mem1: none) }


; ModuleID = '<string>'
source_filename = "<string>"
target triple = "x86_64-pc-windows-msvc"

; Function Attrs: mustprogress nofree norecurse nosync nounwind willreturn memory(read, inaccessiblemem: none, target_mem0: none, target_mem1: none)
define i64 @PsGetProcessSectionBaseAddress(i64 %arg_rcx) local_unnamed_addr #0 {
lift_init:
  %.24 = add i64 %arg_rcx, 960
  %.25 = inttoptr i64 %.24 to ptr
  %mem_ld = load i64, ptr %.25, align 4
  ret i64 %mem_ld
}

attributes #0 = { mustprogress nofree norecurse nosync nounwind willreturn memory(read, inaccessiblemem: none, target_mem0: none, target_mem1: none) }


; ModuleID = '<string>'
source_filename = "<string>"
target triple = "x86_64-pc-windows-msvc"

; Function Attrs: mustprogress nofree norecurse nosync nounwind willreturn memory(read, inaccessiblemem: none, target_mem0: none, target_mem1: none)
define range(i64 0, 1048322) i64 @PsIsSystemProcess(i64 %arg_rcx) local_unnamed_addr #0 {
lift_init:
  %.24 = add i64 %arg_rcx, 1740
  %.25 = inttoptr i64 %.24 to ptr
  %mem_ld = load i32, ptr %.25, align 4
  %.30 = lshr i32 %mem_ld, 12
  %.494 = and i32 %.30, 1048321
  %.49 = zext nneg i32 %.494 to i64
  ret i64 %.49
}

attributes #0 = { mustprogress nofree norecurse nosync nounwind willreturn memory(read, inaccessiblemem: none, target_mem0: none, target_mem1: none) }


; ModuleID = '<string>'
source_filename = "<string>"
target triple = "x86_64-pc-windows-msvc"

; Function Attrs: mustprogress nofree norecurse nosync nounwind willreturn memory(read, inaccessiblemem: none, target_mem0: none, target_mem1: none)
define range(i64 0, 2) i64 @PsIsProtectedProcess(i64 %arg_rcx) local_unnamed_addr #0 {
lift_init:
  %.24 = add i64 %arg_rcx, 1738
  %.25 = inttoptr i64 %.24 to ptr
  %mem_ld = load i8, ptr %.25, align 1
  %.26 = and i8 %mem_ld, 7
  %.27 = icmp ne i8 %.26, 0
  %.45 = zext i1 %.27 to i64
  ret i64 %.45
}

attributes #0 = { mustprogress nofree norecurse nosync nounwind willreturn memory(read, inaccessiblemem: none, target_mem0: none, target_mem1: none) }


; ModuleID = '<string>'
source_filename = "<string>"
target triple = "x86_64-pc-windows-msvc"

; Function Attrs: mustprogress nofree norecurse nosync nounwind willreturn memory(read, inaccessiblemem: none, target_mem0: none, target_mem1: none)
define range(i64 0, 2) i64 @PsIsProtectedProcessLight(i64 %arg_rcx) local_unnamed_addr #0 {
lift_init:
  %.24 = add i64 %arg_rcx, 1738
  %.25 = inttoptr i64 %.24 to ptr
  %mem_ld = load i8, ptr %.25, align 1
  %.39 = and i8 %mem_ld, 7
  %.52 = icmp eq i8 %.39, 1
  %.71 = zext i1 %.52 to i64
  ret i64 %.71
}

attributes #0 = { mustprogress nofree norecurse nosync nounwind willreturn memory(read, inaccessiblemem: none, target_mem0: none, target_mem1: none) }


; ModuleID = '<string>'
source_filename = "<string>"
target triple = "x86_64-pc-windows-msvc"

; Function Attrs: mustprogress nofree norecurse nosync nounwind willreturn memory(read, inaccessiblemem: none, target_mem0: none, target_mem1: none)
define i64 @PsGetThreadId(i64 %arg_rcx) local_unnamed_addr #0 {
lift_init:
  %.24 = add i64 %arg_rcx, 1600
  %.25 = inttoptr i64 %.24 to ptr
  %mem_ld = load i64, ptr %.25, align 4
  ret i64 %mem_ld
}

attributes #0 = { mustprogress nofree norecurse nosync nounwind willreturn memory(read, inaccessiblemem: none, target_mem0: none, target_mem1: none) }


; ModuleID = '<string>'
source_filename = "<string>"
target triple = "x86_64-pc-windows-msvc"

; Function Attrs: mustprogress nofree norecurse nosync nounwind willreturn memory(read, inaccessiblemem: none, target_mem0: none, target_mem1: none)
define i64 @PsGetThreadProcess(i64 %arg_rcx) local_unnamed_addr #0 {
lift_init:
  %.24 = add i64 %arg_rcx, 544
  %.25 = inttoptr i64 %.24 to ptr
  %mem_ld = load i64, ptr %.25, align 4
  ret i64 %mem_ld
}

attributes #0 = { mustprogress nofree norecurse nosync nounwind willreturn memory(read, inaccessiblemem: none, target_mem0: none, target_mem1: none) }


; ModuleID = '<string>'
source_filename = "<string>"
target triple = "x86_64-pc-windows-msvc"

; Function Attrs: mustprogress nofree norecurse nosync nounwind willreturn memory(read, inaccessiblemem: none, target_mem0: none, target_mem1: none)
define i64 @PsGetThreadProcessId(i64 %arg_rcx) local_unnamed_addr #0 {
lift_init:
  %.24 = add i64 %arg_rcx, 1592
  %.25 = inttoptr i64 %.24 to ptr
  %mem_ld = load i64, ptr %.25, align 4
  ret i64 %mem_ld
}

attributes #0 = { mustprogress nofree norecurse nosync nounwind willreturn memory(read, inaccessiblemem: none, target_mem0: none, target_mem1: none) }


; ModuleID = '<string>'
source_filename = "<string>"
target triple = "x86_64-pc-windows-msvc"

; Function Attrs: mustprogress nofree norecurse nosync nounwind willreturn memory(read, inaccessiblemem: none, target_mem0: none, target_mem1: none)
define i64 @PsGetThreadTeb(i64 %arg_rcx) local_unnamed_addr #0 {
lift_init:
  %.24 = add i64 %arg_rcx, 240
  %.25 = inttoptr i64 %.24 to ptr
  %mem_ld = load i64, ptr %.25, align 4
  ret i64 %mem_ld
}

attributes #0 = { mustprogress nofree norecurse nosync nounwind willreturn memory(read, inaccessiblemem: none, target_mem0: none, target_mem1: none) }


; ModuleID = '<string>'
source_filename = "<string>"
target triple = "x86_64-pc-windows-msvc"

; Function Attrs: mustprogress nofree norecurse nosync nounwind willreturn memory(read, inaccessiblemem: none, target_mem0: none, target_mem1: none)
define range(i64 0, 4194050) i64 @PsIsSystemThread(i64 %arg_rcx) local_unnamed_addr #0 {
lift_init:
  %.24 = add i64 %arg_rcx, 116
  %.25 = inttoptr i64 %.24 to ptr
  %mem_ld = load i32, ptr %.25, align 4
  %.30 = lshr i32 %mem_ld, 10
  %.494 = and i32 %.30, 4194049
  %.49 = zext nneg i32 %.494 to i64
  ret i64 %.49
}

attributes #0 = { mustprogress nofree norecurse nosync nounwind willreturn memory(read, inaccessiblemem: none, target_mem0: none, target_mem1: none) }


; ModuleID = '<string>'
source_filename = "<string>"
target triple = "x86_64-pc-windows-msvc"

; Function Attrs: mustprogress nofree norecurse nosync nounwind willreturn memory(none)
define range(i64 48, 36028797018964009) i64 @MmSizeOfMdl(i64 %arg_rcx, i64 %arg_rdx) local_unnamed_addr #0 {
lift_init:
  %.32 = and i64 %arg_rcx, 4095
  %.34 = add i64 %arg_rdx, 4095
  %.36 = add i64 %.34, %.32
  %0 = lshr i64 %.36, 9
  %.60 = and i64 %0, 36028797018963960
  %.62 = add nuw nsw i64 %.60, 48
  ret i64 %.62
}

attributes #0 = { mustprogress nofree norecurse nosync nounwind willreturn memory(none) }


; ModuleID = '<string>'
source_filename = "<string>"
target triple = "x86_64-pc-windows-msvc"

; Function Attrs: mustprogress nofree norecurse nosync nounwind willreturn memory(none)
define noundef i64 @MmGetMaximumFileSectionSize() local_unnamed_addr #0 {
lift_init:
  ret i64 18014398509477888
}

attributes #0 = { mustprogress nofree norecurse nosync nounwind willreturn memory(none) }


; ModuleID = '<string>'
source_filename = "<string>"
target triple = "x86_64-pc-windows-msvc"

define i64 @MmIsAddressValid(i64 %arg_rcx) local_unnamed_addr {
lift_init:
  %call_res = tail call i64 @MiIsAddressValid(i64 %arg_rcx)
  ret i64 %call_res
}

declare i64 @MiIsAddressValid(i64) local_unnamed_addr


; ModuleID = '<string>'
source_filename = "<string>"
target triple = "x86_64-pc-windows-msvc"

define i64 @MmFreePagesFromMdl(i64 %arg_rcx) local_unnamed_addr {
lift_init:
  %call_res = tail call i64 @MiFreePagesFromMdl(i64 %arg_rcx, i64 0, i64 0, i64 0)
  ret i64 %call_res
}

declare i64 @MiFreePagesFromMdl(i64, i64, i64, i64) local_unnamed_addr


; ModuleID = '<string>'
source_filename = "<string>"
target triple = "x86_64-pc-windows-msvc"

; Function Attrs: nofree nosync nounwind memory(read, inaccessiblemem: none, target_mem0: none, target_mem1: none)
define i64 @ObGetObjectType(i64 %arg_rcx) local_unnamed_addr #0 {
lift_init:
  %.26 = add i64 %arg_rcx, -24
  %.27 = inttoptr i64 %.26 to ptr
  %mem_ld = load i8, ptr %.27, align 1
  %0 = trunc i64 %arg_rcx to i32
  %1 = add i32 %0, 65488
  %2 = lshr i32 %1, 8
  %.41 = and i32 %2, 255
  %3 = zext i8 %mem_ld to i32
  %.4410 = xor i32 %.41, %3
  %addr_ObHeaderCookie = tail call i64 @__nt_sym_addr_ObHeaderCookie()
  %.52 = inttoptr i64 %addr_ObHeaderCookie to ptr
  %mem_ld.1 = load i8, ptr %.52, align 1
  %4 = zext i8 %mem_ld.1 to i32
  %.5611 = xor i32 %.4410, %4
  %addr_ObTypeIndexTable = tail call i64 @__nt_sym_addr_ObTypeIndexTable()
  %5 = shl nuw nsw i32 %.5611, 3
  %.65 = zext nneg i32 %5 to i64
  %.66 = add i64 %addr_ObTypeIndexTable, %.65
  %.67 = inttoptr i64 %.66 to ptr
  %mem_ld.2 = load i64, ptr %.67, align 4
  ret i64 %mem_ld.2
}

; Function Attrs: nofree nosync nounwind memory(none)
declare i64 @__nt_sym_addr_ObHeaderCookie() local_unnamed_addr #1

; Function Attrs: nofree nosync nounwind memory(none)
declare i64 @__nt_sym_addr_ObTypeIndexTable() local_unnamed_addr #1

attributes #0 = { nofree nosync nounwind memory(read, inaccessiblemem: none, target_mem0: none, target_mem1: none) }
attributes #1 = { nofree nosync nounwind memory(none) }


; ModuleID = '<string>'
source_filename = "<string>"
target triple = "x86_64-pc-windows-msvc"

; Function Attrs: mustprogress nofree norecurse nosync nounwind willreturn memory(none)
define noundef i64 @ObGetFilterVersion() local_unnamed_addr #0 {
lift_init:
  ret i64 256
}

attributes #0 = { mustprogress nofree norecurse nosync nounwind willreturn memory(none) }


; ModuleID = '<string>'
source_filename = "<string>"
target triple = "x86_64-pc-windows-msvc"

; Function Attrs: nofree nosync nounwind memory(read, inaccessiblemem: none, target_mem0: none, target_mem1: none)
define i64 @ObQueryNameInfo(i64 %arg_rcx) local_unnamed_addr #0 {
lift_init:
  %.24 = add i64 %arg_rcx, -22
  %.25 = inttoptr i64 %.24 to ptr
  %mem_ld = load i8, ptr %.25, align 1
  %.45 = and i8 %mem_ld, 2
  %.46 = icmp eq i8 %.45, 0
  br i1 %.46, label %bb_14056619f, label %bb_14056618b

bb_14056618b:                                     ; preds = %lift_init
  %.31 = add i64 %arg_rcx, -48
  %addr_ObpInfoMaskToOffset = tail call i64 @__nt_sym_addr_ObpInfoMaskToOffset()
  %0 = and i8 %mem_ld, 3
  %.68 = zext nneg i8 %0 to i64
  %.70 = add i64 %addr_ObpInfoMaskToOffset, %.68
  %.71 = inttoptr i64 %.70 to ptr
  %mem_ld.1 = load i8, ptr %.71, align 1
  %.73 = zext i8 %mem_ld.1 to i64
  %.75 = sub i64 %.31, %.73
  br label %bb_14056619f

bb_14056619f:                                     ; preds = %lift_init, %bb_14056618b
  %reg_rcx.0 = phi i64 [ %.75, %bb_14056618b ], [ 0, %lift_init ]
  ret i64 %reg_rcx.0
}

; Function Attrs: nofree nosync nounwind memory(none)
declare i64 @__nt_sym_addr_ObpInfoMaskToOffset() local_unnamed_addr #1

attributes #0 = { nofree nosync nounwind memory(read, inaccessiblemem: none, target_mem0: none, target_mem1: none) }
attributes #1 = { nofree nosync nounwind memory(none) }


; ModuleID = '<string>'
source_filename = "<string>"
target triple = "x86_64-pc-windows-msvc"

define i64 @ObDereferenceObject(i64 %arg_rcx) local_unnamed_addr {
lift_init:
  %call_res = tail call i64 @ObfDereferenceObjectWithTag(i64 %arg_rcx, i64 1953261124)
  ret i64 %call_res
}

declare i64 @ObfDereferenceObjectWithTag(i64, i64) local_unnamed_addr


; ModuleID = '<string>'
source_filename = "<string>"
target triple = "x86_64-pc-windows-msvc"

define i64 @ObIsKernelHandle(i64 %arg_rcx) local_unnamed_addr {
lift_init:
  %call_res = tail call i64 @ObpIsKernelHandle(i64 %arg_rcx, i64 0)
  ret i64 %call_res
}

declare i64 @ObpIsKernelHandle(i64, i64) local_unnamed_addr


; ModuleID = '<string>'
source_filename = "<string>"
target triple = "x86_64-pc-windows-msvc"

; Function Attrs: nofree norecurse nosync nounwind memory(read, inaccessiblemem: none, target_mem0: none, target_mem1: none)
define i64 @IoGetAttachedDevice(i64 %arg_rcx) local_unnamed_addr #0 {
lift_init:
  %.24 = add i64 %arg_rcx, 24
  %.25 = inttoptr i64 %.24 to ptr
  %mem_ld = load i64, ptr %.25, align 4
  %.28 = icmp eq i64 %mem_ld, 0
  br i1 %.28, label %bb_140055be5, label %bb_140055bd9

bb_140055bd9:                                     ; preds = %lift_init, %bb_140055bd9
  %reg_rax.0 = phi i64 [ %mem_ld.1, %bb_140055bd9 ], [ %mem_ld, %lift_init ]
  %.40 = add i64 %reg_rax.0, 24
  %.41 = inttoptr i64 %.40 to ptr
  %mem_ld.1 = load i64, ptr %.41, align 4
  %.44.not = icmp eq i64 %mem_ld.1, 0
  br i1 %.44.not, label %bb_140055be5, label %bb_140055bd9

bb_140055be5:                                     ; preds = %bb_140055bd9, %lift_init
  %reg_rcx.0 = phi i64 [ %arg_rcx, %lift_init ], [ %reg_rax.0, %bb_140055bd9 ]
  ret i64 %reg_rcx.0
}

attributes #0 = { nofree norecurse nosync nounwind memory(read, inaccessiblemem: none, target_mem0: none, target_mem1: none) }


; ModuleID = '<string>'
source_filename = "<string>"
target triple = "x86_64-pc-windows-msvc"

; Function Attrs: mustprogress nofree norecurse nosync nounwind willreturn memory(read, inaccessiblemem: none, target_mem0: none, target_mem1: none)
define range(i64 0, 7) i64 @IoGetIoPriorityHint(i64 %arg_rcx) local_unnamed_addr #0 {
lift_init:
  %.24 = add i64 %arg_rcx, 16
  %.25 = inttoptr i64 %.24 to ptr
  %mem_ld = load i32, ptr %.25, align 4
  %.30 = lshr i32 %mem_ld, 17
  %.39 = and i32 %.30, 7
  %.40.not = icmp eq i32 %.39, 0
  br i1 %.40.not, label %bb_14005a00b, label %bb_14005a011

bb_14005a00b:                                     ; preds = %bb_14005a024, %lift_init
  br label %bb_14005a010

bb_14005a010:                                     ; preds = %bb_14005a024, %bb_14005a018, %bb_14005a011, %bb_14005a00b
  %reg_rax.0.shrunk = phi i32 [ %.61, %bb_14005a011 ], [ %.61, %bb_14005a018 ], [ %.61, %bb_14005a024 ], [ 2, %bb_14005a00b ]
  %reg_rax.0 = zext nneg i32 %reg_rax.0.shrunk to i64
  ret i64 %reg_rax.0

bb_14005a011:                                     ; preds = %lift_init
  %.61 = add nsw i32 %.39, -1
  %.80 = icmp samesign ugt i32 %.39, 2
  br i1 %.80, label %bb_14005a010, label %bb_14005a018

bb_14005a018:                                     ; preds = %bb_14005a011
  %.97 = add i64 %arg_rcx, 152
  %.98 = inttoptr i64 %.97 to ptr
  %mem_ld.1 = load i64, ptr %.98, align 4
  %.101 = icmp eq i64 %mem_ld.1, 0
  br i1 %.101, label %bb_14005a010, label %bb_14005a024

bb_14005a024:                                     ; preds = %bb_14005a018
  %.112 = add i64 %mem_ld.1, 1800
  %.113 = inttoptr i64 %.112 to ptr
  %mem_ld.2 = load i32, ptr %.113, align 4
  %.115 = icmp eq i32 %mem_ld.2, 0
  br i1 %.115, label %bb_14005a010, label %bb_14005a00b
}

attributes #0 = { mustprogress nofree norecurse nosync nounwind willreturn memory(read, inaccessiblemem: none, target_mem0: none, target_mem1: none) }


; ModuleID = '<string>'
source_filename = "<string>"
target triple = "x86_64-pc-windows-msvc"

define i64 @IoGetCurrentProcess() local_unnamed_addr {
lift_init:
  %gs_val = tail call i64 @__nt_read_gs_i64(i64 392)
  %.25 = add i64 %gs_val, 184
  %.26 = inttoptr i64 %.25 to ptr
  %mem_ld = load i64, ptr %.26, align 4
  ret i64 %mem_ld
}

declare i64 @__nt_read_gs_i64(i64) local_unnamed_addr


; ModuleID = '<string>'
source_filename = "<string>"
target triple = "x86_64-pc-windows-msvc"

; Function Attrs: mustprogress nofree norecurse nosync nounwind willreturn memory(read, inaccessiblemem: none, target_mem0: none, target_mem1: none)
define i64 @IoThreadToProcess(i64 %arg_rcx) local_unnamed_addr #0 {
lift_init:
  %.24 = add i64 %arg_rcx, 544
  %.25 = inttoptr i64 %.24 to ptr
  %mem_ld = load i64, ptr %.25, align 4
  ret i64 %mem_ld
}

attributes #0 = { mustprogress nofree norecurse nosync nounwind willreturn memory(read, inaccessiblemem: none, target_mem0: none, target_mem1: none) }


; ModuleID = '<string>'
source_filename = "<string>"
target triple = "x86_64-pc-windows-msvc"

define range(i64 0, -255) i64 @IoIs32bitProcess(i64 %arg_rcx) local_unnamed_addr {
lift_init:
  %.25 = icmp eq i64 %arg_rcx, 0
  br i1 %.25, label %bb_140022e57, label %bb_140022e25

bb_140022e25:                                     ; preds = %lift_init
  %.36 = add i64 %arg_rcx, 64
  %.37 = inttoptr i64 %.36 to ptr
  %mem_ld = load i8, ptr %.37, align 1
  %.39.not = icmp eq i8 %mem_ld, 1
  br i1 %.39.not, label %bb_140022e2b, label %bb_140022e54

bb_140022e2b:                                     ; preds = %bb_140022e25
  %.57 = add i64 %arg_rcx, 16
  %.58 = inttoptr i64 %.57 to ptr
  %mem_ld.1 = load i32, ptr %.58, align 4
  %.61 = add i64 %arg_rcx, 152
  %.62 = inttoptr i64 %.61 to ptr
  %mem_ld.2 = load i64, ptr %.62, align 4
  %0 = and i32 %mem_ld.1, 8192
  %.67.not = icmp eq i32 %0, 0
  br i1 %.67.not, label %bb_140022e9a, label %bb_140022e3b

bb_140022e3b:                                     ; preds = %bb_140022e2b
  %.75 = add i64 %arg_rcx, 88
  %.76 = inttoptr i64 %.75 to ptr
  %mem_ld.3 = load i64, ptr %.76, align 4
  %.78 = and i64 %mem_ld.3, -7
  br label %bb_140022e43

bb_140022e43:                                     ; preds = %bb_14017f74a, %bb_140022eab, %bb_140022e3b
  %reg_rax.1 = phi i64 [ %mem_ld.12, %bb_14017f74a ], [ %mem_ld.10, %bb_140022eab ], [ %.78, %bb_140022e3b ]
  %.88 = icmp eq i64 %reg_rax.1, 0
  br i1 %.88, label %bb_140022e54, label %bb_140022e48

bb_140022e48:                                     ; preds = %bb_140022e43
  %.99 = add i64 %reg_rax.1, 1064
  %.100 = inttoptr i64 %.99 to ptr
  %mem_ld.4 = load i64, ptr %.100, align 4
  %.103.not = icmp eq i64 %mem_ld.4, 0
  br i1 %.103.not, label %bb_140022e54, label %bb_140022eb4

common.ret:                                       ; preds = %bb_140022e85, %bb_140022e85, %bb_140022eb4, %bb_140022eb4, %bb_140022e54
  %common.ret.op = phi i64 [ %.116, %bb_140022e54 ], [ 257, %bb_140022e85 ], [ 257, %bb_140022eb4 ], [ 257, %bb_140022eb4 ], [ 257, %bb_140022e85 ]
  ret i64 %common.ret.op

bb_140022e54:                                     ; preds = %bb_140022e9f, %bb_140022e85, %bb_140022eb4, %bb_140022e9a, %bb_140022e69, %bb_140022e57, %bb_140022e48, %bb_140022e43, %bb_140022e25
  %reg_rax.0 = phi i64 [ %gs_val, %bb_140022e57 ], [ 0, %bb_140022e69 ], [ 452, %bb_140022e85 ], [ 0, %bb_140022e25 ], [ %.59, %bb_140022e9a ], [ 0, %bb_140022e9f ], [ 0, %bb_140022e43 ], [ 452, %bb_140022eb4 ], [ %reg_rax.1, %bb_140022e48 ]
  %.116 = and i64 %reg_rax.0, -256
  br label %common.ret

bb_140022e57:                                     ; preds = %lift_init
  %gs_val = tail call i64 @__nt_read_gs_i64(i64 392)
  %.131 = add i64 %gs_val, 562
  %.132 = inttoptr i64 %.131 to ptr
  %mem_ld.5 = load i8, ptr %.132, align 1
  %.134.not = icmp eq i8 %mem_ld.5, 1
  br i1 %.134.not, label %bb_140022e69, label %bb_140022e54

bb_140022e69:                                     ; preds = %bb_140022e57
  %gs_val.1 = tail call i64 @__nt_read_gs_i64(i64 392)
  %.154 = add i64 %gs_val.1, 184
  %.155 = inttoptr i64 %.154 to ptr
  %mem_ld.6 = load i64, ptr %.155, align 4
  %.157 = add i64 %mem_ld.6, 1064
  %.158 = inttoptr i64 %.157 to ptr
  %mem_ld.7 = load i64, ptr %.158, align 4
  %.161 = icmp eq i64 %mem_ld.7, 0
  br i1 %.161, label %bb_140022e54, label %bb_140022e85

bb_140022e85:                                     ; preds = %bb_140022e69
  %.172 = add i64 %mem_ld.7, 8
  %.173 = inttoptr i64 %.172 to ptr
  %mem_ld.8 = load i16, ptr %.173, align 2
  switch i16 %mem_ld.8, label %bb_140022e54 [
    i16 332, label %common.ret
    i16 452, label %common.ret
  ]

bb_140022e9a:                                     ; preds = %bb_140022e2b
  %.59 = zext i32 %mem_ld.1 to i64
  %.207 = icmp eq i64 %mem_ld.2, 0
  br i1 %.207, label %bb_140022e54, label %bb_140022e9f

bb_140022e9f:                                     ; preds = %bb_140022e9a
  %.218 = add i64 %arg_rcx, 70
  %.219 = inttoptr i64 %.218 to ptr
  %mem_ld.9 = load i8, ptr %.219, align 1
  switch i8 %mem_ld.9, label %bb_140022e54 [
    i8 0, label %bb_140022eab
    i8 1, label %bb_14017f74a
  ]

bb_140022eab:                                     ; preds = %bb_140022e9f
  %.236 = add i64 %mem_ld.2, 544
  %.237 = inttoptr i64 %.236 to ptr
  %mem_ld.10 = load i64, ptr %.237, align 4
  br label %bb_140022e43

bb_140022eb4:                                     ; preds = %bb_140022e48
  %.240 = add i64 %mem_ld.4, 8
  %.241 = inttoptr i64 %.240 to ptr
  %mem_ld.11 = load i16, ptr %.241, align 2
  switch i16 %mem_ld.11, label %bb_140022e54 [
    i16 332, label %common.ret
    i16 452, label %common.ret
  ]

bb_14017f74a:                                     ; preds = %bb_140022e9f
  %.285 = add i64 %mem_ld.2, 184
  %.286 = inttoptr i64 %.285 to ptr
  %mem_ld.12 = load i64, ptr %.286, align 4
  br label %bb_140022e43
}

declare i64 @__nt_read_gs_i64(i64) local_unnamed_addr


; ModuleID = '<string>'
source_filename = "<string>"
target triple = "x86_64-pc-windows-msvc"

define range(i64 0, 4294967296) i64 @PiUEventHashStringIntoBucket(i64 %arg_rcx) local_unnamed_addr {
lift_init:
  %stack_frame = alloca [1024 x i8], align 16
  %.39 = getelementptr inbounds nuw i8, ptr %stack_frame, i64 784
  store i32 0, ptr %.39, align 16
  %.54 = getelementptr inbounds nuw i8, ptr %stack_frame, i64 744
  %.55 = ptrtoint ptr %.54 to i64
  %call_res = call i64 @RtlInitUnicodeString(i64 %.55, i64 %arg_rcx)
  %.60 = ptrtoint ptr %.39 to i64
  %call_res.1 = call i64 @RtlHashUnicodeString(i64 %.55, i64 1, i64 0, i64 %.60)
  %mem_ld.1 = load i32, ptr %.39, align 16
  %.84 = zext i32 %mem_ld.1 to i64
  %.90 = mul nuw nsw i64 %.84, 1321528399
  %sum.shift = lshr i64 %.90, 34
  %.10020 = trunc nuw nsw i64 %sum.shift to i32
  %.109.neg = mul i32 %.10020, -13
  %.112 = add i32 %.109.neg, %mem_ld.1
  %.125 = zext i32 %.112 to i64
  ret i64 %.125
}

declare i64 @RtlInitUnicodeString(i64, i64) local_unnamed_addr

declare i64 @RtlHashUnicodeString(i64, i64, i64, i64) local_unnamed_addr


; ModuleID = '<string>'
source_filename = "<string>"
target triple = "x86_64-pc-windows-msvc"

; Function Attrs: mustprogress nofree norecurse nosync nounwind willreturn memory(read, inaccessiblemem: none, target_mem0: none, target_mem1: none)
define range(i64 0, 65536) i64 @ExQueryDepthSList(i64 %arg_rcx) local_unnamed_addr #0 {
lift_init:
  %.24 = inttoptr i64 %arg_rcx to ptr
  %mem_ld = load i16, ptr %.24, align 2
  %.26 = zext i16 %mem_ld to i64
  ret i64 %.26
}

attributes #0 = { mustprogress nofree norecurse nosync nounwind willreturn memory(read, inaccessiblemem: none, target_mem0: none, target_mem1: none) }


; ModuleID = '<string>'
source_filename = "<string>"
target triple = "x86_64-pc-windows-msvc"

; Function Attrs: mustprogress nofree norecurse nosync nounwind willreturn memory(write, inaccessiblemem: none, target_mem0: none, target_mem1: none)
define noundef i64 @ExInitializeRundownProtection(i64 %arg_rcx) local_unnamed_addr #0 {
lift_init:
  %.24 = inttoptr i64 %arg_rcx to ptr
  store i64 0, ptr %.24, align 4
  ret i64 0
}

attributes #0 = { mustprogress nofree norecurse nosync nounwind willreturn memory(write, inaccessiblemem: none, target_mem0: none, target_mem1: none) }


; ModuleID = '<string>'
source_filename = "<string>"
target triple = "x86_64-pc-windows-msvc"

; Function Attrs: mustprogress nofree norecurse nosync nounwind willreturn memory(readwrite, inaccessiblemem: none, target_mem0: none, target_mem1: none)
define i64 @ExReInitializeRundownProtection(i64 %arg_rcx) local_unnamed_addr #0 {
lift_init:
  %.32 = inttoptr i64 %arg_rcx to ptr
  %mem_ld = load i64, ptr %.32, align 4
  store i64 0, ptr %.32, align 4
  ret i64 %mem_ld
}

attributes #0 = { mustprogress nofree norecurse nosync nounwind willreturn memory(readwrite, inaccessiblemem: none, target_mem0: none, target_mem1: none) }


; ModuleID = '<string>'
source_filename = "<string>"
target triple = "x86_64-pc-windows-msvc"

; Function Attrs: nofree norecurse nosync nounwind memory(readwrite, inaccessiblemem: none, target_mem0: none, target_mem1: none)
define range(i64 0, 65536) i64 @RtlInitUnicodeString(i64 %arg_rcx, i64 %arg_rdx) local_unnamed_addr #0 {
lift_init:
  %.33 = add i64 %arg_rcx, 8
  %.34 = inttoptr i64 %.33 to ptr
  store i64 %arg_rdx, ptr %.34, align 4
  %.36 = inttoptr i64 %arg_rcx to ptr
  store i32 0, ptr %.36, align 4
  %.39.not = icmp eq i64 %arg_rdx, 0
  br i1 %.39.not, label %common.ret, label %bb_140055c32

common.ret:                                       ; preds = %lift_init, %bb_140055c3c
  %common.ret.op = phi i64 [ %.151, %bb_140055c3c ], [ 0, %lift_init ]
  ret i64 %common.ret.op

bb_140055c32:                                     ; preds = %lift_init, %bb_140055c32
  %reg_rax.0 = phi i64 [ %.64, %bb_140055c32 ], [ -1, %lift_init ]
  %.64 = add i64 %reg_rax.0, 1
  %.79 = shl i64 %.64, 1
  %.80 = add i64 %.79, %arg_rdx
  %.81 = inttoptr i64 %.80 to ptr
  %mem_ld = load i16, ptr %.81, align 2
  %.83.not = icmp eq i16 %mem_ld, 0
  br i1 %.83.not, label %bb_140055c3c, label %bb_140055c32

bb_140055c3c:                                     ; preds = %bb_140055c32
  %.120 = icmp ugt i64 %.79, 65533
  %extract.t = trunc i64 %.79 to i16
  %spec.select = select i1 %.120, i16 -4, i16 %extract.t
  store i16 %spec.select, ptr %.36, align 2
  %.136 = add i16 %spec.select, 2
  %.151 = zext i16 %.136 to i64
  %.154 = add i64 %arg_rcx, 2
  %.155 = inttoptr i64 %.154 to ptr
  store i16 %.136, ptr %.155, align 2
  br label %common.ret
}

attributes #0 = { nofree norecurse nosync nounwind memory(readwrite, inaccessiblemem: none, target_mem0: none, target_mem1: none) }


; ModuleID = '<string>'
source_filename = "<string>"
target triple = "x86_64-pc-windows-msvc"

; Function Attrs: nofree norecurse nosync nounwind memory(readwrite, inaccessiblemem: none, target_mem0: none, target_mem1: none)
define range(i64 0, 65536) i64 @RtlInitAnsiString(i64 %arg_rcx, i64 %arg_rdx) local_unnamed_addr #0 {
lift_init:
  %.33 = add i64 %arg_rcx, 8
  %.34 = inttoptr i64 %.33 to ptr
  store i64 %arg_rdx, ptr %.34, align 4
  %.36 = inttoptr i64 %arg_rcx to ptr
  store i32 0, ptr %.36, align 4
  %.39.not = icmp eq i64 %arg_rdx, 0
  br i1 %.39.not, label %common.ret, label %bb_14004c814

common.ret:                                       ; preds = %lift_init, %bb_14004c81d
  %common.ret.op = phi i64 [ %.141, %bb_14004c81d ], [ 0, %lift_init ]
  ret i64 %common.ret.op

bb_14004c814:                                     ; preds = %lift_init, %bb_14004c814
  %reg_rax.0 = phi i64 [ %.64, %bb_14004c814 ], [ -1, %lift_init ]
  %.64 = add i64 %reg_rax.0, 1
  %.79 = add i64 %.64, %arg_rdx
  %.80 = inttoptr i64 %.79 to ptr
  %mem_ld = load i8, ptr %.80, align 1
  %.82.not = icmp eq i8 %mem_ld, 0
  br i1 %.82.not, label %bb_14004c81d, label %bb_14004c814

bb_14004c81d:                                     ; preds = %bb_14004c814
  %.120 = tail call i64 @llvm.umin.i64(i64 %.64, i64 65534)
  %r_ax = trunc nuw i64 %.120 to i16
  store i16 %r_ax, ptr %.36, align 2
  %.125 = add nuw i16 %r_ax, 1
  %.141 = zext i16 %.125 to i64
  %.144 = add i64 %arg_rcx, 2
  %.145 = inttoptr i64 %.144 to ptr
  store i16 %.125, ptr %.145, align 2
  br label %common.ret
}

; Function Attrs: nocallback nocreateundeforpoison nofree nosync nounwind speculatable willreturn memory(none)
declare i64 @llvm.umin.i64(i64, i64) #1

attributes #0 = { nofree norecurse nosync nounwind memory(readwrite, inaccessiblemem: none, target_mem0: none, target_mem1: none) }
attributes #1 = { nocallback nocreateundeforpoison nofree nosync nounwind speculatable willreturn memory(none) }


; ModuleID = '<string>'
source_filename = "<string>"
target triple = "x86_64-pc-windows-msvc"

; Function Attrs: mustprogress nofree norecurse nosync nounwind willreturn memory(write, inaccessiblemem: none, target_mem0: none, target_mem1: none)
define noundef i64 @RtlInitializeBitMap(i64 %arg_rcx, i64 %arg_rdx, i64 %arg_r8) local_unnamed_addr #0 {
lift_init:
  %r_r8d = trunc i64 %arg_r8 to i32
  %.26 = inttoptr i64 %arg_rcx to ptr
  store i32 %r_r8d, ptr %.26, align 4
  %.28 = add i64 %arg_rcx, 8
  %.29 = inttoptr i64 %.28 to ptr
  store i64 %arg_rdx, ptr %.29, align 4
  ret i64 0
}

attributes #0 = { mustprogress nofree norecurse nosync nounwind willreturn memory(write, inaccessiblemem: none, target_mem0: none, target_mem1: none) }


; ModuleID = '<string>'
source_filename = "<string>"
target triple = "x86_64-pc-windows-msvc"

; Function Attrs: mustprogress nofree norecurse nosync nounwind willreturn memory(read, inaccessiblemem: none, target_mem0: none, target_mem1: none)
define range(i64 0, -254) i64 @RtlTestBit(i64 %arg_rcx, i64 %arg_rdx) local_unnamed_addr #0 {
lift_init:
  %.25 = add i64 %arg_rcx, 8
  %.26 = inttoptr i64 %.25 to ptr
  %mem_ld = load i64, ptr %.26, align 4
  %r_edx = trunc i64 %arg_rdx to i32
  %0 = ashr i32 %r_edx, 5
  %.29 = sext i32 %0 to i64
  %.30 = shl nsw i64 %.29, 2
  %.31 = add i64 %mem_ld, %.30
  %.32 = inttoptr i64 %.31 to ptr
  %bt_word = load i32, ptr %.32, align 4
  %.34 = and i32 %r_edx, 31
  %.35 = lshr i32 %bt_word, %.34
  %1 = trunc i32 %.35 to i8
  %.42 = and i8 %1, 1
  %.44 = and i64 %mem_ld, -256
  %.45 = zext nneg i8 %.42 to i64
  %.46 = or disjoint i64 %.44, %.45
  ret i64 %.46
}

attributes #0 = { mustprogress nofree norecurse nosync nounwind willreturn memory(read, inaccessiblemem: none, target_mem0: none, target_mem1: none) }


; ModuleID = '<string>'
source_filename = "<string>"
target triple = "x86_64-pc-windows-msvc"

; Function Attrs: mustprogress nofree norecurse nosync nounwind willreturn memory(readwrite, inaccessiblemem: none, target_mem0: none, target_mem1: none)
define i64 @RtlSetBit(i64 %arg_rcx, i64 %arg_rdx) local_unnamed_addr #0 {
lift_init:
  %.25 = add i64 %arg_rcx, 8
  %.26 = inttoptr i64 %.25 to ptr
  %mem_ld = load i64, ptr %.26, align 4
  %r_edx = trunc i64 %arg_rdx to i32
  %0 = ashr i32 %r_edx, 5
  %.29 = sext i32 %0 to i64
  %.30 = shl nsw i64 %.29, 2
  %.31 = add i64 %mem_ld, %.30
  %.32 = inttoptr i64 %.31 to ptr
  %bt_word = load i32, ptr %.32, align 4
  %.34 = and i32 %r_edx, 31
  %.38 = shl nuw i32 1, %.34
  %.39 = or i32 %bt_word, %.38
  store i32 %.39, ptr %.32, align 4
  ret i64 %mem_ld
}

attributes #0 = { mustprogress nofree norecurse nosync nounwind willreturn memory(readwrite, inaccessiblemem: none, target_mem0: none, target_mem1: none) }


; ModuleID = '<string>'
source_filename = "<string>"
target triple = "x86_64-pc-windows-msvc"

; Function Attrs: mustprogress nofree norecurse nosync nounwind willreturn memory(readwrite, inaccessiblemem: none, target_mem0: none, target_mem1: none)
define i64 @RtlClearBit(i64 %arg_rcx, i64 %arg_rdx) local_unnamed_addr #0 {
lift_init:
  %.25 = add i64 %arg_rcx, 8
  %.26 = inttoptr i64 %.25 to ptr
  %mem_ld = load i64, ptr %.26, align 4
  %r_edx = trunc i64 %arg_rdx to i32
  %0 = ashr i32 %r_edx, 5
  %.29 = sext i32 %0 to i64
  %.30 = shl nsw i64 %.29, 2
  %.31 = add i64 %mem_ld, %.30
  %.32 = inttoptr i64 %.31 to ptr
  %bt_word = load i32, ptr %.32, align 4
  %.34 = and i32 %r_edx, 31
  %.38 = shl nuw i32 1, %.34
  %.39 = xor i32 %.38, -1
  %.40 = and i32 %bt_word, %.39
  store i32 %.40, ptr %.32, align 4
  ret i64 %mem_ld
}

attributes #0 = { mustprogress nofree norecurse nosync nounwind willreturn memory(readwrite, inaccessiblemem: none, target_mem0: none, target_mem1: none) }


; ModuleID = '<string>'
source_filename = "<string>"
target triple = "x86_64-pc-windows-msvc"

; Function Attrs: mustprogress nofree norecurse nosync nounwind willreturn memory(none)
define range(i64 -9223372036854775807, -9223372036854775808) i64 @RtlExtendedMagicDivide(i64 %arg_rcx, i64 %arg_rdx, i64 %arg_r8) local_unnamed_addr #0 {
lift_init:
  %.31 = icmp slt i64 %arg_rcx, 0
  %.80.sink = tail call i64 @llvm.abs.i64(i64 %arg_rcx, i1 false)
  %.94 = zext i64 %arg_rdx to i128
  %.95 = zext i64 %.80.sink to i128
  %.96 = mul nuw nsw i128 %.94, %.95
  %.98 = lshr i128 %.96, 64
  %.99 = trunc nuw nsw i128 %.98 to i64
  %.55 = and i64 %arg_r8, 63
  %.56 = lshr i64 %.99, %.55
  %.103 = sub nsw i64 0, %.56
  %spec.select = select i1 %.31, i64 %.103, i64 %.56
  ret i64 %spec.select
}

; Function Attrs: nocallback nofree nosync nounwind speculatable willreturn memory(none)
declare i64 @llvm.abs.i64(i64, i1 immarg) #1

attributes #0 = { mustprogress nofree norecurse nosync nounwind willreturn memory(none) }
attributes #1 = { nocallback nofree nosync nounwind speculatable willreturn memory(none) }


; ModuleID = '<string>'
source_filename = "<string>"
target triple = "x86_64-pc-windows-msvc"

; Function Attrs: mustprogress nofree norecurse nosync nounwind willreturn memory(write, inaccessiblemem: none, target_mem0: none, target_mem1: none)
define range(i64 116444736000000000, 159394408950000001) i64 @RtlSecondsSince1970ToTime(i64 %arg_rcx, i64 %arg_rdx) local_unnamed_addr #0 {
lift_init:
  %stack_frame.sroa.0.784.insert.ext = and i64 %arg_rcx, 4294967295
  %0 = mul nuw nsw i64 %stack_frame.sroa.0.784.insert.ext, 10000000
  %.61 = add nuw nsw i64 %0, 116444736000000000
  %.63 = inttoptr i64 %arg_rdx to ptr
  store i64 %.61, ptr %.63, align 4
  ret i64 %.61
}

attributes #0 = { mustprogress nofree norecurse nosync nounwind willreturn memory(write, inaccessiblemem: none, target_mem0: none, target_mem1: none) }


; ModuleID = '<string>'
source_filename = "<string>"
target triple = "x86_64-pc-windows-msvc"

; Function Attrs: mustprogress nofree norecurse nosync nounwind willreturn memory(write, inaccessiblemem: none, target_mem0: none, target_mem1: none)
define range(i64 119600064000000000, 162549736950000001) i64 @RtlSecondsSince1980ToTime(i64 %arg_rcx, i64 %arg_rdx) local_unnamed_addr #0 {
lift_init:
  %stack_frame.sroa.0.784.insert.ext = and i64 %arg_rcx, 4294967295
  %0 = mul nuw nsw i64 %stack_frame.sroa.0.784.insert.ext, 10000000
  %.61 = add nuw nsw i64 %0, 119600064000000000
  %.63 = inttoptr i64 %arg_rdx to ptr
  store i64 %.61, ptr %.63, align 4
  ret i64 %.61
}

attributes #0 = { mustprogress nofree norecurse nosync nounwind willreturn memory(write, inaccessiblemem: none, target_mem0: none, target_mem1: none) }


; ModuleID = '<string>'
source_filename = "<string>"
target triple = "x86_64-pc-windows-msvc"

define range(i64 1, -255) i64 @RtlTimeToSecondsSince1970(i64 %arg_rcx, i64 %arg_rdx) local_unnamed_addr {
lift_init:
  %.39 = inttoptr i64 %arg_rcx to ptr
  %mem_ld = load i64, ptr %.39, align 4
  %call_res = tail call i64 @RtlExtendedMagicDivide(i64 %mem_ld, i64 -2972493582642298179, i64 23)
  %.50 = add i64 %call_res, -11644473600
  %.76 = icmp ult i64 %.50, 4294967296
  br i1 %.76, label %bb_1402480af, label %bb_1402480ab

bb_1402480ab:                                     ; preds = %lift_init
  %.88 = and i64 %.50, -256
  br label %bb_1402480b4

bb_1402480af:                                     ; preds = %lift_init
  %r_eax = trunc nuw i64 %.50 to i32
  %.99 = inttoptr i64 %arg_rdx to ptr
  store i32 %r_eax, ptr %.99, align 4
  %.102 = and i64 %.50, 4294967040
  %.104 = or disjoint i64 %.102, 1
  br label %bb_1402480b4

bb_1402480b4:                                     ; preds = %bb_1402480af, %bb_1402480ab
  %reg_rax.0 = phi i64 [ %.104, %bb_1402480af ], [ %.88, %bb_1402480ab ]
  ret i64 %reg_rax.0
}

declare i64 @RtlExtendedMagicDivide(i64, i64, i64) local_unnamed_addr


; ModuleID = '<string>'
source_filename = "<string>"
target triple = "x86_64-pc-windows-msvc"

; Function Attrs: mustprogress nofree norecurse nosync nounwind willreturn memory(read, inaccessiblemem: none, target_mem0: none, target_mem1: none)
define range(i64 0, 268435202) i64 @SeTokenIsRestricted(i64 %arg_rcx) local_unnamed_addr #0 {
lift_init:
  %.24 = add i64 %arg_rcx, 200
  %.25 = inttoptr i64 %.24 to ptr
  %mem_ld = load i32, ptr %.25, align 4
  %.30 = lshr i32 %mem_ld, 4
  %.494 = and i32 %.30, 268435201
  %.49 = zext nneg i32 %.494 to i64
  ret i64 %.49
}

attributes #0 = { mustprogress nofree norecurse nosync nounwind willreturn memory(read, inaccessiblemem: none, target_mem0: none, target_mem1: none) }


; ModuleID = '<string>'
source_filename = "<string>"
target triple = "x86_64-pc-windows-msvc"

define i64 @SeTokenIsAdmin(i64 %arg_rcx) local_unnamed_addr {
lift_init:
  %.51 = add i64 %arg_rcx, 200
  %.52 = inttoptr i64 %.51 to ptr
  %mem_ld = load i32, ptr %.52, align 4
  %0 = and i32 %mem_ld, 16384
  %.59.not = icmp eq i32 %0, 0
  br i1 %.59.not, label %bb_1404582ae, label %bb_140458303

bb_1404582ae:                                     ; preds = %lift_init
  %.66 = add i64 %arg_rcx, 192
  %.67 = inttoptr i64 %.66 to ptr
  %mem_ld.1 = load i32, ptr %.67, align 4
  %.69 = icmp eq i32 %mem_ld.1, 2
  br i1 %.69, label %bb_1405f9ce6, label %bb_1404582bb

bb_1404582bb:                                     ; preds = %bb_1405f9ce6, %bb_1404582ae
  %addr_SeAliasAdminsSid = tail call i64 @__nt_sym_addr_SeAliasAdminsSid()
  %.86 = inttoptr i64 %addr_SeAliasAdminsSid to ptr
  %mem_ld.2 = load i64, ptr %.86, align 4
  %call_res = tail call i64 @SepSidInToken(i64 %arg_rcx, i64 0, i64 %mem_ld.2, i64 0, i64 0, i64 0)
  %r_al = trunc i64 %call_res to i8
  %.125 = icmp eq i8 %r_al, 0
  br i1 %.125, label %bb_1404582f0, label %bb_1404582e0

bb_1404582e0:                                     ; preds = %bb_1404582bb
  %call_res.1 = tail call i64 @SeTokenIsRestricted(i64 %arg_rcx)
  %r_al.3 = trunc i64 %call_res.1 to i8
  %.139.not = icmp eq i8 %r_al.3, 0
  br i1 %.139.not, label %bb_1404582f0, label %bb_1405f9cf8

bb_1404582f0:                                     ; preds = %bb_1405f9cf8, %bb_1404582e0, %bb_1404582bb
  %reg_rdi.0.in.off0 = phi i8 [ 0, %bb_1404582bb ], [ %extract.t44, %bb_1405f9cf8 ], [ %r_al, %bb_1404582e0 ]
  %reg_rax.0 = phi i64 [ %call_res, %bb_1404582bb ], [ %call_res.2, %bb_1405f9cf8 ], [ %call_res.1, %bb_1404582e0 ]
  %.152 = and i64 %reg_rax.0, -256
  %.153 = zext i8 %reg_rdi.0.in.off0 to i64
  %.154 = or disjoint i64 %.152, %.153
  br label %bb_1404582f3

bb_1404582f3:                                     ; preds = %bb_140458303, %bb_1404582f0
  %reg_rax.1 = phi i64 [ %.188, %bb_140458303 ], [ %.154, %bb_1404582f0 ]
  ret i64 %reg_rax.1

bb_140458303:                                     ; preds = %bb_1405f9ce6, %lift_init
  %1 = and i32 %mem_ld, -256
  %.188 = zext i32 %1 to i64
  br label %bb_1404582f3

bb_1405f9ce6:                                     ; preds = %bb_1404582ae
  %.199 = add i64 %arg_rcx, 196
  %.200 = inttoptr i64 %.199 to ptr
  %mem_ld.8 = load i32, ptr %.200, align 4
  %.201 = add i32 %mem_ld.8, -2
  %2 = sub i32 1, %mem_ld.8
  %3 = and i32 %2, %mem_ld.8
  %4 = xor i32 %3, %.201
  %.218 = icmp slt i32 %4, 0
  br i1 %.218, label %bb_140458303, label %bb_1404582bb

bb_1405f9cf8:                                     ; preds = %bb_1404582e0
  %call_res.2 = tail call i64 @SepSidInToken(i64 %arg_rcx, i64 0, i64 %mem_ld.2, i64 0, i64 1, i64 0)
  %extract.t44 = trunc i64 %call_res.2 to i8
  br label %bb_1404582f0
}

; Function Attrs: nofree nosync nounwind memory(none)
declare i64 @__nt_sym_addr_SeAliasAdminsSid() local_unnamed_addr #0

declare i64 @SepSidInToken(i64, i64, i64, i64, i64, i64) local_unnamed_addr

declare i64 @SeTokenIsRestricted(i64) local_unnamed_addr

attributes #0 = { nofree nosync nounwind memory(none) }


; ModuleID = '<string>'
source_filename = "<string>"
target triple = "x86_64-pc-windows-msvc"

; Function Attrs: mustprogress nofree norecurse nosync nounwind willreturn memory(readwrite, inaccessiblemem: none, target_mem0: none, target_mem1: none)
define noundef i64 @SeQueryAuthenticationIdToken(i64 %arg_rcx, i64 %arg_rdx) local_unnamed_addr #0 {
lift_init:
  %.25 = add i64 %arg_rcx, 24
  %.26 = inttoptr i64 %.25 to ptr
  %mem_ld = load i64, ptr %.26, align 4
  %.28 = inttoptr i64 %arg_rdx to ptr
  store i64 %mem_ld, ptr %.28, align 4
  ret i64 0
}

attributes #0 = { mustprogress nofree norecurse nosync nounwind willreturn memory(readwrite, inaccessiblemem: none, target_mem0: none, target_mem1: none) }


; ModuleID = '<string>'
source_filename = "<string>"
target triple = "x86_64-pc-windows-msvc"

define i64 @KdRefreshDebuggerNotPresent() local_unnamed_addr {
lift_init:
  %stack_frame = alloca [1024 x i8], align 16
  %.24 = getelementptr inbounds nuw i8, ptr %stack_frame, i64 776
  store i64 0, ptr %.24, align 8
  %.29 = getelementptr inbounds nuw i8, ptr %stack_frame, i64 760
  store i64 0, ptr %.29, align 8
  %addr_KdPitchDebugger = tail call i64 @__nt_sym_addr_KdPitchDebugger()
  %.46 = inttoptr i64 %addr_KdPitchDebugger to ptr
  %mem_ld = load i8, ptr %.46, align 1
  %.48 = icmp eq i8 %mem_ld, 0
  br i1 %.48, label %bb_1401d8d30, label %bb_140141f09

bb_140141f09:                                     ; preds = %lift_init, %bb_1401d8d49
  %reg_rax.0 = phi i64 [ %.187, %bb_1401d8d49 ], [ 1, %lift_init ]
  ret i64 %reg_rax.0

bb_1401d8d30:                                     ; preds = %lift_init
  %addr___nt_rodata_1401715d0 = tail call i64 @__nt_sym_addr___nt_rodata_1401715d0()
  %.107 = getelementptr inbounds nuw i8, ptr %stack_frame, i64 752
  store i64 %addr___nt_rodata_1401715d0, ptr %.107, align 16
  br label %bb_1401d8d40

bb_1401d8d40:                                     ; preds = %bb_1401d8d40, %bb_1401d8d30
  %reg_rax.1 = phi i64 [ -1, %bb_1401d8d30 ], [ %.112, %bb_1401d8d40 ]
  %.112 = add i64 %reg_rax.1, 1
  %.127 = add i64 %.112, %addr___nt_rodata_1401715d0
  %.128 = inttoptr i64 %.127 to ptr
  %mem_ld.3 = load i8, ptr %.128, align 1
  %.130.not = icmp eq i8 %mem_ld.3, 0
  br i1 %.130.not, label %bb_1401d8d49, label %bb_1401d8d40

bb_1401d8d49:                                     ; preds = %bb_1401d8d40
  %r_ax = trunc i64 %.112 to i16
  %.157 = getelementptr inbounds nuw i8, ptr %stack_frame, i64 744
  store i16 %r_ax, ptr %.157, align 8
  %call_res = tail call i64 @KdEnterDebugger(i64 0)
  %.163 = ptrtoint ptr %.157 to i64
  %.167 = and i64 %call_res, 255
  %call_res.1 = call i64 @KdpPrintString(i64 %.163)
  %addr_KdDebuggerNotPresent = tail call i64 @__nt_sym_addr_KdDebuggerNotPresent()
  %.171 = inttoptr i64 %addr_KdDebuggerNotPresent to ptr
  %mem_ld.4 = load i8, ptr %.171, align 1
  %.174 = zext i8 %mem_ld.4 to i64
  %call_res.2 = call i64 @KdExitDebugger(i64 %.167)
  %.185 = and i64 %call_res.2, -256
  %.187 = or disjoint i64 %.185, %.174
  br label %bb_140141f09
}

; Function Attrs: nofree nosync nounwind memory(none)
declare i64 @__nt_sym_addr_KdPitchDebugger() local_unnamed_addr #0

; Function Attrs: nofree nosync nounwind memory(none)
declare i64 @__nt_sym_addr___nt_rodata_1401715d0() local_unnamed_addr #0

declare i64 @KdEnterDebugger(i64) local_unnamed_addr

declare i64 @KdpPrintString(i64) local_unnamed_addr

; Function Attrs: nofree nosync nounwind memory(none)
declare i64 @__nt_sym_addr_KdDebuggerNotPresent() local_unnamed_addr #0

declare i64 @KdExitDebugger(i64) local_unnamed_addr

attributes #0 = { nofree nosync nounwind memory(none) }


; ModuleID = '<string>'
source_filename = "<string>"
target triple = "x86_64-pc-windows-msvc"

; Function Attrs: mustprogress nofree norecurse nosync nounwind willreturn memory(none)
define noundef i64 @KeRestoreFloatingPointState() local_unnamed_addr #0 {
lift_init:
  ret i64 0
}

attributes #0 = { mustprogress nofree norecurse nosync nounwind willreturn memory(none) }


; ModuleID = '<string>'
source_filename = "<string>"
target triple = "x86_64-pc-windows-msvc"

; Function Attrs: mustprogress nofree norecurse nosync nounwind willreturn memory(readwrite, inaccessiblemem: none, target_mem0: none, target_mem1: none)
define noundef i64 @KeSignalCallDpcDone(i64 %arg_rcx) local_unnamed_addr #0 {
lift_init:
  %.24 = inttoptr i64 %arg_rcx to ptr
  %mem_ld = load i32, ptr %.24, align 4
  %.26 = add i32 %mem_ld, -1
  store i32 %.26, ptr %.24, align 4
  ret i64 0
}

attributes #0 = { mustprogress nofree norecurse nosync nounwind willreturn memory(readwrite, inaccessiblemem: none, target_mem0: none, target_mem1: none) }


; ModuleID = '<string>'
source_filename = "<string>"
target triple = "x86_64-pc-windows-msvc"

; Function Attrs: mustprogress nofree norecurse nosync nounwind willreturn memory(write, inaccessiblemem: none, target_mem0: none, target_mem1: none)
define noundef i64 @HalExamineMBR(i64 %arg_rcx, i64 %arg_rdx, i64 %arg_r8, i64 %arg_r9) local_unnamed_addr #0 {
lift_init:
  %.27 = inttoptr i64 %arg_r9 to ptr
  store i64 0, ptr %.27, align 4
  ret i64 0
}

attributes #0 = { mustprogress nofree norecurse nosync nounwind willreturn memory(write, inaccessiblemem: none, target_mem0: none, target_mem1: none) }


; ModuleID = '<string>'
source_filename = "<string>"
target triple = "x86_64-pc-windows-msvc"

; Function Attrs: nofree nosync nounwind memory(readwrite, inaccessiblemem: none, target_mem0: none, target_mem1: none)
define noundef i64 @KeClockTimerPowerChange() local_unnamed_addr #0 {
lift_init:
  %addr_KiClockStats = tail call i64 @__nt_sym_addr_KiClockStats()
  %.23 = inttoptr i64 %addr_KiClockStats to ptr
  %mem_ld = load i32, ptr %.23, align 4
  %.25 = add i32 %mem_ld, 1
  store i32 %.25, ptr %.23, align 4
  ret i64 0
}

; Function Attrs: nofree nosync nounwind memory(none)
declare i64 @__nt_sym_addr_KiClockStats() local_unnamed_addr #1

attributes #0 = { nofree nosync nounwind memory(readwrite, inaccessiblemem: none, target_mem0: none, target_mem1: none) }
attributes #1 = { nofree nosync nounwind memory(none) }


; ModuleID = '<string>'
source_filename = "<string>"
target triple = "x86_64-pc-windows-msvc"

; Function Attrs: nofree nosync nounwind memory(read, inaccessiblemem: none, target_mem0: none, target_mem1: none)
define range(i64 0, 256) i64 @KeForceEnableNx() local_unnamed_addr #0 {
lift_init:
  %addr_KiNxForceEnable = tail call i64 @__nt_sym_addr_KiNxForceEnable()
  %.23 = inttoptr i64 %addr_KiNxForceEnable to ptr
  %mem_ld = load i8, ptr %.23, align 1
  %.26 = zext i8 %mem_ld to i64
  ret i64 %.26
}

; Function Attrs: nofree nosync nounwind memory(none)
declare i64 @__nt_sym_addr_KiNxForceEnable() local_unnamed_addr #1

attributes #0 = { nofree nosync nounwind memory(read, inaccessiblemem: none, target_mem0: none, target_mem1: none) }
attributes #1 = { nofree nosync nounwind memory(none) }


; ModuleID = '<string>'
source_filename = "<string>"
target triple = "x86_64-pc-windows-msvc"

; Function Attrs: nofree nosync nounwind memory(read, inaccessiblemem: none, target_mem0: none, target_mem1: none)
define range(i64 0, 4294967296) i64 @KeGetClockOwner() local_unnamed_addr #0 {
lift_init:
  %addr_KiClockTimerOwner = tail call i64 @__nt_sym_addr_KiClockTimerOwner()
  %.23 = inttoptr i64 %addr_KiClockTimerOwner to ptr
  %mem_ld = load i32, ptr %.23, align 4
  %.24 = zext i32 %mem_ld to i64
  ret i64 %.24
}

; Function Attrs: nofree nosync nounwind memory(none)
declare i64 @__nt_sym_addr_KiClockTimerOwner() local_unnamed_addr #1

attributes #0 = { nofree nosync nounwind memory(read, inaccessiblemem: none, target_mem0: none, target_mem1: none) }
attributes #1 = { nofree nosync nounwind memory(none) }


; ModuleID = '<string>'
source_filename = "<string>"
target triple = "x86_64-pc-windows-msvc"

; Function Attrs: nofree nosync nounwind memory(read, inaccessiblemem: none, target_mem0: none, target_mem1: none)
define range(i64 0, 4294967296) i64 @KeGetRecommendedSharedDataAlignment() local_unnamed_addr #0 {
lift_init:
  %addr_KeLargestCacheLine = tail call i64 @__nt_sym_addr_KeLargestCacheLine()
  %.23 = inttoptr i64 %addr_KeLargestCacheLine to ptr
  %mem_ld = load i32, ptr %.23, align 4
  %.24 = zext i32 %mem_ld to i64
  ret i64 %.24
}

; Function Attrs: nofree nosync nounwind memory(none)
declare i64 @__nt_sym_addr_KeLargestCacheLine() local_unnamed_addr #1

attributes #0 = { nofree nosync nounwind memory(read, inaccessiblemem: none, target_mem0: none, target_mem1: none) }
attributes #1 = { nofree nosync nounwind memory(none) }


; ModuleID = '<string>'
source_filename = "<string>"
target triple = "x86_64-pc-windows-msvc"

; Function Attrs: nofree nosync nounwind memory(read, inaccessiblemem: none, target_mem0: none, target_mem1: none)
define range(i64 0, 4294967296) i64 @KeQueryTimeIncrement() local_unnamed_addr #0 {
lift_init:
  %addr_KeMaximumIncrement = tail call i64 @__nt_sym_addr_KeMaximumIncrement()
  %.23 = inttoptr i64 %addr_KeMaximumIncrement to ptr
  %mem_ld = load i32, ptr %.23, align 4
  %.24 = zext i32 %mem_ld to i64
  ret i64 %.24
}

; Function Attrs: nofree nosync nounwind memory(none)
declare i64 @__nt_sym_addr_KeMaximumIncrement() local_unnamed_addr #1

attributes #0 = { nofree nosync nounwind memory(read, inaccessiblemem: none, target_mem0: none, target_mem1: none) }
attributes #1 = { nofree nosync nounwind memory(none) }


; ModuleID = '<string>'
source_filename = "<string>"
target triple = "x86_64-pc-windows-msvc"

; Function Attrs: nofree nosync nounwind memory(write, inaccessiblemem: none, target_mem0: none, target_mem1: none)
define noundef i64 @KeSetLastBranchRecordInUse(i64 %arg_rcx) local_unnamed_addr #0 {
lift_init:
  %r_cl = trunc i64 %arg_rcx to i8
  %addr_KiLastBranchRecordInUse = tail call i64 @__nt_sym_addr_KiLastBranchRecordInUse()
  %.24 = inttoptr i64 %addr_KiLastBranchRecordInUse to ptr
  store i8 %r_cl, ptr %.24, align 1
  ret i64 0
}

; Function Attrs: nofree nosync nounwind memory(none)
declare i64 @__nt_sym_addr_KiLastBranchRecordInUse() local_unnamed_addr #1

attributes #0 = { nofree nosync nounwind memory(write, inaccessiblemem: none, target_mem0: none, target_mem1: none) }
attributes #1 = { nofree nosync nounwind memory(none) }


; ModuleID = '<string>'
source_filename = "<string>"
target triple = "x86_64-pc-windows-msvc"

; Function Attrs: nofree nosync nounwind memory(write, inaccessiblemem: none, target_mem0: none, target_mem1: none)
define noundef i64 @KeSetProfileIrql(i64 %arg_rcx) local_unnamed_addr #0 {
lift_init:
  %r_cl = trunc i64 %arg_rcx to i8
  %addr_KiProfileIrql = tail call i64 @__nt_sym_addr_KiProfileIrql()
  %.24 = inttoptr i64 %addr_KiProfileIrql to ptr
  store i8 %r_cl, ptr %.24, align 1
  ret i64 0
}

; Function Attrs: nofree nosync nounwind memory(none)
declare i64 @__nt_sym_addr_KiProfileIrql() local_unnamed_addr #1

attributes #0 = { nofree nosync nounwind memory(write, inaccessiblemem: none, target_mem0: none, target_mem1: none) }
attributes #1 = { nofree nosync nounwind memory(none) }


; ModuleID = '<string>'
source_filename = "<string>"
target triple = "x86_64-pc-windows-msvc"

; Function Attrs: nofree nosync nounwind memory(read, inaccessiblemem: none, target_mem0: none, target_mem1: none)
define range(i64 0, 4294967296) i64 @KeSystemFullyCacheCoherent() local_unnamed_addr #0 {
lift_init:
  %addr_KiSystemFullyCoherent = tail call i64 @__nt_sym_addr_KiSystemFullyCoherent()
  %.23 = inttoptr i64 %addr_KiSystemFullyCoherent to ptr
  %mem_ld = load i32, ptr %.23, align 4
  %.24 = zext i32 %mem_ld to i64
  ret i64 %.24
}

; Function Attrs: nofree nosync nounwind memory(none)
declare i64 @__nt_sym_addr_KiSystemFullyCoherent() local_unnamed_addr #1

attributes #0 = { nofree nosync nounwind memory(read, inaccessiblemem: none, target_mem0: none, target_mem1: none) }
attributes #1 = { nofree nosync nounwind memory(none) }


; ModuleID = '<string>'
source_filename = "<string>"
target triple = "x86_64-pc-windows-msvc"

; Function Attrs: nofree nosync nounwind memory(write, inaccessiblemem: none, target_mem0: none, target_mem1: none)
define noundef i64 @InbvInstallDisplayStringFilter(i64 %arg_rcx) local_unnamed_addr #0 {
lift_init:
  %addr___nt_data_14037f730 = tail call i64 @__nt_sym_addr___nt_data_14037f730()
  %.24 = inttoptr i64 %addr___nt_data_14037f730 to ptr
  store i64 %arg_rcx, ptr %.24, align 4
  ret i64 0
}

; Function Attrs: nofree nosync nounwind memory(none)
declare i64 @__nt_sym_addr___nt_data_14037f730() local_unnamed_addr #1

attributes #0 = { nofree nosync nounwind memory(write, inaccessiblemem: none, target_mem0: none, target_mem1: none) }
attributes #1 = { nofree nosync nounwind memory(none) }


; ModuleID = '<string>'
source_filename = "<string>"
target triple = "x86_64-pc-windows-msvc"

; Function Attrs: nofree nosync nounwind memory(read, inaccessiblemem: none, target_mem0: none, target_mem1: none)
define range(i64 0, 65536) i64 @KeQueryActiveGroupCount() local_unnamed_addr #0 {
lift_init:
  %addr_KiActiveGroups = tail call i64 @__nt_sym_addr_KiActiveGroups()
  %.23 = inttoptr i64 %addr_KiActiveGroups to ptr
  %mem_ld = load i16, ptr %.23, align 2
  %.25 = zext i16 %mem_ld to i64
  ret i64 %.25
}

; Function Attrs: nofree nosync nounwind memory(none)
declare i64 @__nt_sym_addr_KiActiveGroups() local_unnamed_addr #1

attributes #0 = { nofree nosync nounwind memory(read, inaccessiblemem: none, target_mem0: none, target_mem1: none) }
attributes #1 = { nofree nosync nounwind memory(none) }


; ModuleID = '<string>'
source_filename = "<string>"
target triple = "x86_64-pc-windows-msvc"

; Function Attrs: nofree nosync nounwind memory(read, inaccessiblemem: none, target_mem0: none, target_mem1: none)
define i64 @KeQueryActiveProcessors() local_unnamed_addr #0 {
lift_init:
  %addr___nt_data_1403f93e8 = tail call i64 @__nt_sym_addr___nt_data_1403f93e8()
  %.23 = inttoptr i64 %addr___nt_data_1403f93e8 to ptr
  %mem_ld = load i64, ptr %.23, align 4
  ret i64 %mem_ld
}

; Function Attrs: nofree nosync nounwind memory(none)
declare i64 @__nt_sym_addr___nt_data_1403f93e8() local_unnamed_addr #1

attributes #0 = { nofree nosync nounwind memory(read, inaccessiblemem: none, target_mem0: none, target_mem1: none) }
attributes #1 = { nofree nosync nounwind memory(none) }


; ModuleID = '<string>'
source_filename = "<string>"
target triple = "x86_64-pc-windows-msvc"

; Function Attrs: nofree nosync nounwind memory(read, inaccessiblemem: none, target_mem0: none, target_mem1: none)
define range(i64 0, 65536) i64 @KeQueryMaximumGroupCount() local_unnamed_addr #0 {
lift_init:
  %addr_KiMaximumGroups = tail call i64 @__nt_sym_addr_KiMaximumGroups()
  %.23 = inttoptr i64 %addr_KiMaximumGroups to ptr
  %mem_ld = load i16, ptr %.23, align 2
  %.25 = zext i16 %mem_ld to i64
  ret i64 %.25
}

; Function Attrs: nofree nosync nounwind memory(none)
declare i64 @__nt_sym_addr_KiMaximumGroups() local_unnamed_addr #1

attributes #0 = { nofree nosync nounwind memory(read, inaccessiblemem: none, target_mem0: none, target_mem1: none) }
attributes #1 = { nofree nosync nounwind memory(none) }


; ModuleID = '<string>'
source_filename = "<string>"
target triple = "x86_64-pc-windows-msvc"

; Function Attrs: nofree nosync nounwind memory(read, inaccessiblemem: none, target_mem0: none, target_mem1: none)
define range(i64 0, 65536) i64 @KeQueryHighestNodeNumber() local_unnamed_addr #0 {
lift_init:
  %addr_KeNumberNodes = tail call i64 @__nt_sym_addr_KeNumberNodes()
  %.23 = inttoptr i64 %addr_KeNumberNodes to ptr
  %mem_ld = load i16, ptr %.23, align 2
  %.28 = add i16 %mem_ld, -1
  %.44 = zext i16 %.28 to i64
  ret i64 %.44
}

; Function Attrs: nofree nosync nounwind memory(none)
declare i64 @__nt_sym_addr_KeNumberNodes() local_unnamed_addr #1

attributes #0 = { nofree nosync nounwind memory(read, inaccessiblemem: none, target_mem0: none, target_mem1: none) }
attributes #1 = { nofree nosync nounwind memory(none) }


; ModuleID = '<string>'
source_filename = "<string>"
target triple = "x86_64-pc-windows-msvc"

; Function Attrs: nofree nosync nounwind memory(write, inaccessiblemem: none, target_mem0: none, target_mem1: none)
define noundef i64 @KeReportCacheIncoherentDevice() local_unnamed_addr #0 {
lift_init:
  %addr_KiSystemFullyCoherent = tail call i64 @__nt_sym_addr_KiSystemFullyCoherent()
  %.23 = inttoptr i64 %addr_KiSystemFullyCoherent to ptr
  store i32 0, ptr %.23, align 4
  ret i64 0
}

; Function Attrs: nofree nosync nounwind memory(none)
declare i64 @__nt_sym_addr_KiSystemFullyCoherent() local_unnamed_addr #1

attributes #0 = { nofree nosync nounwind memory(write, inaccessiblemem: none, target_mem0: none, target_mem1: none) }
attributes #1 = { nofree nosync nounwind memory(none) }


; ModuleID = '<string>'
source_filename = "<string>"
target triple = "x86_64-pc-windows-msvc"

; Function Attrs: nofree nosync nounwind memory(readwrite, inaccessiblemem: none, target_mem0: none, target_mem1: none)
define range(i64 0, 256) i64 @InbvEnableDisplayString(i64 %arg_rcx) local_unnamed_addr #0 {
lift_init:
  %addr___nt_data_14037f728 = tail call i64 @__nt_sym_addr___nt_data_14037f728()
  %.24 = inttoptr i64 %addr___nt_data_14037f728 to ptr
  %mem_ld = load i8, ptr %.24, align 1
  %.27 = zext i8 %mem_ld to i64
  %r_cl = trunc i64 %arg_rcx to i8
  store i8 %r_cl, ptr %.24, align 1
  ret i64 %.27
}

; Function Attrs: nofree nosync nounwind memory(none)
declare i64 @__nt_sym_addr___nt_data_14037f728() local_unnamed_addr #1

attributes #0 = { nofree nosync nounwind memory(readwrite, inaccessiblemem: none, target_mem0: none, target_mem1: none) }
attributes #1 = { nofree nosync nounwind memory(none) }


; ModuleID = '<string>'
source_filename = "<string>"
target triple = "x86_64-pc-windows-msvc"

; Function Attrs: mustprogress nofree norecurse nosync nounwind willreturn memory(write, inaccessiblemem: none, target_mem0: none, target_mem1: none)
define noundef i64 @KeInitializeEnumerationContextFromAffinity(i64 %arg_rcx, i64 %arg_rdx, i64 %arg_r8) local_unnamed_addr #0 {
lift_init:
  %.26 = inttoptr i64 %arg_rcx to ptr
  store i64 0, ptr %.26, align 4
  %r_dx = trunc i64 %arg_rdx to i16
  %.36 = add i64 %arg_rcx, 16
  %.37 = inttoptr i64 %.36 to ptr
  store i16 %r_dx, ptr %.37, align 2
  %.39 = add i64 %arg_rcx, 8
  %.40 = inttoptr i64 %.39 to ptr
  store i64 %arg_r8, ptr %.40, align 4
  ret i64 0
}

attributes #0 = { mustprogress nofree norecurse nosync nounwind willreturn memory(write, inaccessiblemem: none, target_mem0: none, target_mem1: none) }


; ModuleID = '<string>'
source_filename = "<string>"
target triple = "x86_64-pc-windows-msvc"

define range(i64 0, 65538) i64 @KeIsExecutingDpc() local_unnamed_addr {
lift_init:
  %gs_val = tail call i32 @__nt_read_gs_i32(i64 12268)
  %.26 = and i32 %gs_val, 65537
  %.33 = zext nneg i32 %.26 to i64
  ret i64 %.33
}

declare i32 @__nt_read_gs_i32(i64) local_unnamed_addr


; ModuleID = '<string>'
source_filename = "<string>"
target triple = "x86_64-pc-windows-msvc"

; Function Attrs: nofree nosync nounwind memory(read, inaccessiblemem: none, target_mem0: none, target_mem1: none)
define i64 @KeQueryPrcbAddress(i64 %arg_rcx) local_unnamed_addr #0 {
lift_init:
  %addr_KiProcessorBlock = tail call i64 @__nt_sym_addr_KiProcessorBlock()
  %.24 = shl i64 %arg_rcx, 3
  %.27 = and i64 %.24, 34359738360
  %.28 = add i64 %addr_KiProcessorBlock, %.27
  %.29 = inttoptr i64 %.28 to ptr
  %mem_ld = load i64, ptr %.29, align 4
  ret i64 %mem_ld
}

; Function Attrs: nofree nosync nounwind memory(none)
declare i64 @__nt_sym_addr_KiProcessorBlock() local_unnamed_addr #1

attributes #0 = { nofree nosync nounwind memory(read, inaccessiblemem: none, target_mem0: none, target_mem1: none) }
attributes #1 = { nofree nosync nounwind memory(none) }


; ModuleID = '<string>'
source_filename = "<string>"
target triple = "x86_64-pc-windows-msvc"

; Function Attrs: mustprogress nofree norecurse nosync nounwind willreturn memory(readwrite, inaccessiblemem: none, target_mem0: none, target_mem1: none)
define range(i64 0, 4294967296) i64 @KeQueryRuntimeThread(i64 %arg_rcx, i64 %arg_rdx) local_unnamed_addr #0 {
lift_init:
  %.25 = add i64 %arg_rcx, 732
  %.26 = inttoptr i64 %.25 to ptr
  %mem_ld = load i32, ptr %.26, align 4
  %.29 = inttoptr i64 %arg_rdx to ptr
  store i32 %mem_ld, ptr %.29, align 4
  %.31 = add i64 %arg_rcx, 652
  %.32 = inttoptr i64 %.31 to ptr
  %mem_ld.1 = load i32, ptr %.32, align 4
  %.33 = zext i32 %mem_ld.1 to i64
  ret i64 %.33
}

attributes #0 = { mustprogress nofree norecurse nosync nounwind willreturn memory(readwrite, inaccessiblemem: none, target_mem0: none, target_mem1: none) }


; ModuleID = '<string>'
source_filename = "<string>"
target triple = "x86_64-pc-windows-msvc"

; Function Attrs: mustprogress nofree norecurse nosync nounwind willreturn memory(read, inaccessiblemem: none, target_mem0: none, target_mem1: none)
define range(i64 0, 2) i64 @KeIsWaitListEmpty(i64 %arg_rcx) local_unnamed_addr #0 {
lift_init:
  %.24 = add i64 %arg_rcx, 8
  %.50 = inttoptr i64 %.24 to ptr
  %mem_ld.1 = load i64, ptr %.50, align 4
  %.52 = icmp eq i64 %.24, %mem_ld.1
  %.71 = zext i1 %.52 to i64
  ret i64 %.71
}

attributes #0 = { mustprogress nofree norecurse nosync nounwind willreturn memory(read, inaccessiblemem: none, target_mem0: none, target_mem1: none) }


; ModuleID = '<string>'
source_filename = "<string>"
target triple = "x86_64-pc-windows-msvc"

; Function Attrs: mustprogress nofree norecurse nosync nounwind willreturn memory(read, inaccessiblemem: none, target_mem0: none, target_mem1: none)
define range(i64 0, -255) i64 @KeTestSpinLock(i64 %arg_rcx) local_unnamed_addr #0 {
lift_init:
  %.24 = inttoptr i64 %arg_rcx to ptr
  %mem_ld = load i64, ptr %.24, align 4
  %.27.not = icmp eq i64 %mem_ld, 0
  %.48 = and i64 %mem_ld, -256
  %common.ret.op = select i1 %.27.not, i64 1, i64 %.48
  ret i64 %common.ret.op
}

attributes #0 = { mustprogress nofree norecurse nosync nounwind willreturn memory(read, inaccessiblemem: none, target_mem0: none, target_mem1: none) }


; ModuleID = '<string>'
source_filename = "<string>"
target triple = "x86_64-pc-windows-msvc"

define i64 @KeEnterCriticalRegion() local_unnamed_addr {
lift_init:
  %gs_val = tail call i64 @__nt_read_gs_i64(i64 392)
  %.25 = add i64 %gs_val, 484
  %.26 = inttoptr i64 %.25 to ptr
  %mem_ld = load i16, ptr %.26, align 2
  %.28 = add i16 %mem_ld, -1
  store i16 %.28, ptr %.26, align 2
  ret i64 %gs_val
}

declare i64 @__nt_read_gs_i64(i64) local_unnamed_addr


; ModuleID = '<string>'
source_filename = "<string>"
target triple = "x86_64-pc-windows-msvc"

define i64 @KeEnterGuardedRegion() local_unnamed_addr {
lift_init:
  %gs_val = tail call i64 @__nt_read_gs_i64(i64 392)
  %.25 = add i64 %gs_val, 486
  %.26 = inttoptr i64 %.25 to ptr
  %mem_ld = load i16, ptr %.26, align 2
  %.28 = add i16 %mem_ld, -1
  store i16 %.28, ptr %.26, align 2
  ret i64 %gs_val
}

declare i64 @__nt_read_gs_i64(i64) local_unnamed_addr


; ModuleID = '<string>'
source_filename = "<string>"
target triple = "x86_64-pc-windows-msvc"

; Function Attrs: nofree nosync nounwind memory(readwrite, inaccessiblemem: none, target_mem0: none, target_mem1: none)
define range(i64 0, 4294967296) i64 @KeGetClockTimerResolution(i64 %arg_rcx, i64 %arg_rdx) local_unnamed_addr #0 {
lift_init:
  %addr_KeTimeIncrement = tail call i64 @__nt_sym_addr_KeTimeIncrement()
  %.25 = inttoptr i64 %addr_KeTimeIncrement to ptr
  %mem_ld = load i32, ptr %.25, align 4
  %.28 = inttoptr i64 %arg_rdx to ptr
  store i32 %mem_ld, ptr %.28, align 4
  %addr_KiLastRequestedTimeIncrement = tail call i64 @__nt_sym_addr_KiLastRequestedTimeIncrement()
  %.30 = inttoptr i64 %addr_KiLastRequestedTimeIncrement to ptr
  %mem_ld.1 = load i32, ptr %.30, align 4
  %.31 = zext i32 %mem_ld.1 to i64
  %.33 = inttoptr i64 %arg_rcx to ptr
  store i32 %mem_ld.1, ptr %.33, align 4
  ret i64 %.31
}

; Function Attrs: nofree nosync nounwind memory(none)
declare i64 @__nt_sym_addr_KeTimeIncrement() local_unnamed_addr #1

; Function Attrs: nofree nosync nounwind memory(none)
declare i64 @__nt_sym_addr_KiLastRequestedTimeIncrement() local_unnamed_addr #1

attributes #0 = { nofree nosync nounwind memory(readwrite, inaccessiblemem: none, target_mem0: none, target_mem1: none) }
attributes #1 = { nofree nosync nounwind memory(none) }


; ModuleID = '<string>'
source_filename = "<string>"
target triple = "x86_64-pc-windows-msvc"

; Function Attrs: mustprogress nofree norecurse nosync nounwind willreturn memory(read, inaccessiblemem: none, target_mem0: none, target_mem1: none)
define i64 @KeQueryGroupAffinityEx(i64 %arg_rcx, i64 %arg_rdx) local_unnamed_addr #0 {
lift_init:
  %r_dx = trunc i64 %arg_rdx to i16
  %.25 = inttoptr i64 %arg_rcx to ptr
  %mem_ld = load i16, ptr %.25, align 2
  %.31.not = icmp ugt i16 %mem_ld, %r_dx
  br i1 %.31.not, label %bb_1401fc3f5, label %common.ret

common.ret:                                       ; preds = %lift_init, %bb_1401fc3f5
  %common.ret.op = phi i64 [ %mem_ld.1, %bb_1401fc3f5 ], [ 0, %lift_init ]
  ret i64 %common.ret.op

bb_1401fc3f5:                                     ; preds = %lift_init
  %.46 = shl i64 %arg_rdx, 3
  %.48 = and i64 %.46, 524280
  %.49 = add i64 %arg_rcx, 8
  %.50 = add i64 %.49, %.48
  %.51 = inttoptr i64 %.50 to ptr
  %mem_ld.1 = load i64, ptr %.51, align 4
  br label %common.ret
}

attributes #0 = { mustprogress nofree norecurse nosync nounwind willreturn memory(read, inaccessiblemem: none, target_mem0: none, target_mem1: none) }


; ModuleID = '<string>'
source_filename = "<string>"
target triple = "x86_64-pc-windows-msvc"

; Function Attrs: mustprogress nofree norecurse nosync nounwind willreturn memory(readwrite, inaccessiblemem: none, target_mem0: none, target_mem1: none)
define i64 @KeInitializeEnumerationContext(i64 %arg_rcx, i64 %arg_rdx) local_unnamed_addr #0 {
lift_init:
  %.33 = add i64 %arg_rcx, 16
  %.34 = inttoptr i64 %.33 to ptr
  store i16 0, ptr %.34, align 2
  %.36 = add i64 %arg_rdx, 8
  %.37 = inttoptr i64 %.36 to ptr
  %mem_ld = load i64, ptr %.37, align 4
  %.39 = add i64 %arg_rcx, 8
  %.40 = inttoptr i64 %.39 to ptr
  store i64 %mem_ld, ptr %.40, align 4
  %.42 = inttoptr i64 %arg_rcx to ptr
  store i64 %arg_rdx, ptr %.42, align 4
  ret i64 %mem_ld
}

attributes #0 = { mustprogress nofree norecurse nosync nounwind willreturn memory(readwrite, inaccessiblemem: none, target_mem0: none, target_mem1: none) }


; ModuleID = '<string>'
source_filename = "<string>"
target triple = "x86_64-pc-windows-msvc"

define range(i64 0, -254) i64 @KeAreApcsDisabled() local_unnamed_addr {
lift_init:
  %gs_val = tail call i64 @__nt_read_gs_i64(i64 392)
  %.25 = add i64 %gs_val, 484
  %.26 = inttoptr i64 %.25 to ptr
  %mem_ld = load i32, ptr %.26, align 4
  %.28 = icmp ne i32 %mem_ld, 0
  %.47 = and i64 %gs_val, -256
  %.48 = zext i1 %.28 to i64
  %.49 = or disjoint i64 %.47, %.48
  ret i64 %.49
}

declare i64 @__nt_read_gs_i64(i64) local_unnamed_addr


; ModuleID = '<string>'
source_filename = "<string>"
target triple = "x86_64-pc-windows-msvc"

; Function Attrs: mustprogress nofree norecurse nosync nounwind willreturn memory(none)
define noundef i64 @KeCountSetBitsAffinityEx(i64 %arg_rcx) local_unnamed_addr #0 {
lift_init:
  ret i64 0
}

attributes #0 = { mustprogress nofree norecurse nosync nounwind willreturn memory(none) }


; ModuleID = '<string>'
source_filename = "<string>"
target triple = "x86_64-pc-windows-msvc"

; Function Attrs: mustprogress nofree norecurse nosync nounwind willreturn memory(readwrite, inaccessiblemem: none, target_mem0: none, target_mem1: none)
define range(i64 0, 65536) i64 @KeInitializeEnumerationContextFromGroup(i64 %arg_rcx, i64 %arg_rdx) local_unnamed_addr #0 {
lift_init:
  %.25 = inttoptr i64 %arg_rdx to ptr
  %mem_ld = load i64, ptr %.25, align 4
  %.27 = add i64 %arg_rdx, 8
  %.28 = inttoptr i64 %.27 to ptr
  %mem_ld.1 = load i16, ptr %.28, align 2
  %.30 = zext i16 %mem_ld.1 to i64
  %.32 = inttoptr i64 %arg_rcx to ptr
  store i64 0, ptr %.32, align 4
  %.42 = add i64 %arg_rcx, 16
  %.43 = inttoptr i64 %.42 to ptr
  store i16 %mem_ld.1, ptr %.43, align 2
  %.45 = add i64 %arg_rcx, 8
  %.46 = inttoptr i64 %.45 to ptr
  store i64 %mem_ld, ptr %.46, align 4
  ret i64 %.30
}

attributes #0 = { mustprogress nofree norecurse nosync nounwind willreturn memory(readwrite, inaccessiblemem: none, target_mem0: none, target_mem1: none) }


; ModuleID = '<string>'
source_filename = "<string>"
target triple = "x86_64-pc-windows-msvc"

define range(i64 0, -254) i64 @KeIsAttachedProcess() local_unnamed_addr {
lift_init:
  %gs_val = tail call i64 @__nt_read_gs_i64(i64 392)
  %.25 = add i64 %gs_val, 586
  %.26 = inttoptr i64 %.25 to ptr
  %mem_ld = load i8, ptr %.26, align 1
  %.28 = icmp eq i8 %mem_ld, 1
  %.46 = and i64 %gs_val, -256
  %.47 = zext i1 %.28 to i64
  %.48 = or disjoint i64 %.46, %.47
  ret i64 %.48
}

declare i64 @__nt_read_gs_i64(i64) local_unnamed_addr


; ModuleID = '<string>'
source_filename = "<string>"
target triple = "x86_64-pc-windows-msvc"

define i64 @KeSetHeteroCpuPolicyThread(i64 %arg_rcx, i64 %arg_rdx, i64 %arg_r8) local_unnamed_addr {
lift_init:
  %.40 = and i64 %arg_r8, 4294967295
  %call_res = tail call i64 @KiSetHeteroPolicyThread(i64 %arg_rcx, i64 %arg_rdx, i64 0, i64 %.40)
  ret i64 %call_res
}

declare i64 @KiSetHeteroPolicyThread(i64, i64, i64, i64) local_unnamed_addr


; ModuleID = '<string>'
source_filename = "<string>"
target triple = "x86_64-pc-windows-msvc"

define range(i64 0, 65536) i64 @KeGetCurrentNodeNumber() local_unnamed_addr {
lift_init:
  %gs_val = tail call i64 @__nt_read_gs_i64(i64 32)
  %.25 = add i64 %gs_val, 192
  %.26 = inttoptr i64 %.25 to ptr
  %mem_ld = load i64, ptr %.26, align 4
  %.28 = add i64 %mem_ld, 146
  %.29 = inttoptr i64 %.28 to ptr
  %mem_ld.1 = load i16, ptr %.29, align 2
  %.31 = zext i16 %mem_ld.1 to i64
  ret i64 %.31
}

declare i64 @__nt_read_gs_i64(i64) local_unnamed_addr


; ModuleID = '<string>'
source_filename = "<string>"
target triple = "x86_64-pc-windows-msvc"

; Function Attrs: mustprogress nofree norecurse nosync nounwind willreturn memory(write, inaccessiblemem: none, target_mem0: none, target_mem1: none)
define noundef i64 @KeInitializeThreadedDpc(i64 %arg_rcx, i64 %arg_rdx, i64 %arg_r8) local_unnamed_addr #0 {
lift_init:
  %.34 = inttoptr i64 %arg_rcx to ptr
  store i32 282, ptr %.34, align 4
  %.36 = add i64 %arg_rcx, 56
  %.37 = inttoptr i64 %.36 to ptr
  store i64 0, ptr %.37, align 4
  %.39 = add i64 %arg_rcx, 16
  %.40 = inttoptr i64 %.39 to ptr
  store i64 0, ptr %.40, align 4
  %.42 = add i64 %arg_rcx, 24
  %.43 = inttoptr i64 %.42 to ptr
  store i64 %arg_rdx, ptr %.43, align 4
  %.45 = add i64 %arg_rcx, 32
  %.46 = inttoptr i64 %.45 to ptr
  store i64 %arg_r8, ptr %.46, align 4
  ret i64 0
}

attributes #0 = { mustprogress nofree norecurse nosync nounwind willreturn memory(write, inaccessiblemem: none, target_mem0: none, target_mem1: none) }


; ModuleID = '<string>'
source_filename = "<string>"
target triple = "x86_64-pc-windows-msvc"

define i64 @KeSetTimer(i64 %arg_rcx, i64 %arg_rdx, i64 %arg_r8) local_unnamed_addr {
lift_init:
  %call_res = tail call i64 @KiSetTimerEx(i64 %arg_rcx, i64 %arg_rdx, i64 0, i64 0)
  ret i64 %call_res
}

declare i64 @KiSetTimerEx(i64, i64, i64, i64) local_unnamed_addr


; ModuleID = '<string>'
source_filename = "<string>"
target triple = "x86_64-pc-windows-msvc"

; Function Attrs: nofree nosync nounwind memory(readwrite, inaccessiblemem: none, target_mem0: none, target_mem1: none)
define range(i64 1, 0) i64 @KeAddProcessorGroupAffinity(i64 %arg_rcx, i64 %arg_rdx) local_unnamed_addr #0 {
lift_init:
  %addr_KiProcessorIndexToNumberMappingTable = tail call i64 @__nt_sym_addr_KiProcessorIndexToNumberMappingTable()
  %.25 = shl i64 %arg_rdx, 2
  %.28 = and i64 %.25, 17179869180
  %.29 = add i64 %addr_KiProcessorIndexToNumberMappingTable, %.28
  %.30 = inttoptr i64 %.29 to ptr
  %mem_ld = load i32, ptr %.30, align 4
  %.33 = inttoptr i64 %arg_rcx to ptr
  %mem_ld.1 = load i64, ptr %.33, align 4
  %.35 = and i32 %mem_ld, 63
  %.42 = zext nneg i32 %.35 to i64
  %.48 = shl nuw i64 1, %.42
  %.49 = or i64 %.48, %mem_ld.1
  store i64 %.49, ptr %.33, align 4
  ret i64 %.49
}

; Function Attrs: nofree nosync nounwind memory(none)
declare i64 @__nt_sym_addr_KiProcessorIndexToNumberMappingTable() local_unnamed_addr #1

attributes #0 = { nofree nosync nounwind memory(readwrite, inaccessiblemem: none, target_mem0: none, target_mem1: none) }
attributes #1 = { nofree nosync nounwind memory(none) }


; ModuleID = '<string>'
source_filename = "<string>"
target triple = "x86_64-pc-windows-msvc"

; Function Attrs: mustprogress nofree norecurse nosync nounwind willreturn memory(write, inaccessiblemem: none, target_mem0: none, target_mem1: none)
define i64 @KeInitializeSemaphore(i64 %arg_rcx, i64 %arg_rdx, i64 %arg_r8) local_unnamed_addr #0 {
lift_init:
  %.26 = add i64 %arg_rcx, 8
  %.28 = inttoptr i64 %arg_rcx to ptr
  store i8 5, ptr %.28, align 1
  %.30 = add i64 %arg_rcx, 2
  %.31 = inttoptr i64 %.30 to ptr
  store i8 8, ptr %.31, align 1
  %r_edx = trunc i64 %arg_rdx to i32
  %.33 = add i64 %arg_rcx, 4
  %.34 = inttoptr i64 %.33 to ptr
  store i32 %r_edx, ptr %.34, align 4
  %.36 = add i64 %arg_rcx, 16
  %.37 = inttoptr i64 %.36 to ptr
  store i64 %.26, ptr %.37, align 4
  %.39 = inttoptr i64 %.26 to ptr
  store i64 %.26, ptr %.39, align 4
  %r_r8d = trunc i64 %arg_r8 to i32
  %.41 = add i64 %arg_rcx, 24
  %.42 = inttoptr i64 %.41 to ptr
  store i32 %r_r8d, ptr %.42, align 4
  ret i64 %.26
}

attributes #0 = { mustprogress nofree norecurse nosync nounwind willreturn memory(write, inaccessiblemem: none, target_mem0: none, target_mem1: none) }


; ModuleID = '<string>'
source_filename = "<string>"
target triple = "x86_64-pc-windows-msvc"

; Function Attrs: nofree nosync nounwind memory(readwrite, inaccessiblemem: none, target_mem0: none, target_mem1: none)
define range(i64 0, -1) i64 @KeRemoveProcessorGroupAffinity(i64 %arg_rcx, i64 %arg_rdx) local_unnamed_addr #0 {
lift_init:
  %addr_KiProcessorIndexToNumberMappingTable = tail call i64 @__nt_sym_addr_KiProcessorIndexToNumberMappingTable()
  %.25 = shl i64 %arg_rdx, 2
  %.28 = and i64 %.25, 17179869180
  %.29 = add i64 %addr_KiProcessorIndexToNumberMappingTable, %.28
  %.30 = inttoptr i64 %.29 to ptr
  %mem_ld = load i32, ptr %.30, align 4
  %.33 = inttoptr i64 %arg_rcx to ptr
  %mem_ld.1 = load i64, ptr %.33, align 4
  %.35 = and i32 %mem_ld, 63
  %.42 = zext nneg i32 %.35 to i64
  %.48 = shl nuw i64 1, %.42
  %.49 = xor i64 %.48, -1
  %.50 = and i64 %mem_ld.1, %.49
  store i64 %.50, ptr %.33, align 4
  ret i64 %.50
}

; Function Attrs: nofree nosync nounwind memory(none)
declare i64 @__nt_sym_addr_KiProcessorIndexToNumberMappingTable() local_unnamed_addr #1

attributes #0 = { nofree nosync nounwind memory(readwrite, inaccessiblemem: none, target_mem0: none, target_mem1: none) }
attributes #1 = { nofree nosync nounwind memory(none) }


; ModuleID = '<string>'
source_filename = "<string>"
target triple = "x86_64-pc-windows-msvc"

; Function Attrs: mustprogress nofree norecurse nosync nounwind willreturn memory(write, inaccessiblemem: none, target_mem0: none, target_mem1: none)
define i64 @KeInitializeEvent(i64 %arg_rcx, i64 %arg_rdx, i64 %arg_r8) local_unnamed_addr #0 {
lift_init:
  %0 = trunc i64 %arg_r8 to i32
  %.26 = and i32 %0, 255
  %.29 = add i64 %arg_rcx, 4
  %.30 = inttoptr i64 %.29 to ptr
  store i32 %.26, ptr %.30, align 4
  %.32 = add i64 %arg_rcx, 8
  %r_dl = trunc i64 %arg_rdx to i8
  %.34 = inttoptr i64 %arg_rcx to ptr
  store i8 %r_dl, ptr %.34, align 1
  %.36 = add i64 %arg_rcx, 1
  %.37 = inttoptr i64 %.36 to ptr
  store i16 1536, ptr %.37, align 2
  %.39 = add i64 %arg_rcx, 16
  %.40 = inttoptr i64 %.39 to ptr
  store i64 %.32, ptr %.40, align 4
  %.42 = inttoptr i64 %.32 to ptr
  store i64 %.32, ptr %.42, align 4
  ret i64 %.32
}

attributes #0 = { mustprogress nofree norecurse nosync nounwind willreturn memory(write, inaccessiblemem: none, target_mem0: none, target_mem1: none) }


; ModuleID = '<string>'
source_filename = "<string>"
target triple = "x86_64-pc-windows-msvc"

define range(i64 0, 4294967296) i64 @KeQueryActiveProcessorAffinity() local_unnamed_addr {
lift_init:
  %addr_KeActiveProcessors = tail call i64 @__nt_sym_addr_KeActiveProcessors()
  %call_res = tail call i64 @KeCopyAffinityEx(i64 0, i64 %addr_KeActiveProcessors, i64 0, i64 0)
  %addr_KeNumberProcessors = tail call i64 @__nt_sym_addr_KeNumberProcessors()
  %.39 = inttoptr i64 %addr_KeNumberProcessors to ptr
  %mem_ld = load i32, ptr %.39, align 4
  %.40 = zext i32 %mem_ld to i64
  ret i64 %.40
}

; Function Attrs: nofree nosync nounwind memory(none)
declare i64 @__nt_sym_addr_KeActiveProcessors() local_unnamed_addr #0

declare i64 @KeCopyAffinityEx(i64, i64, i64, i64) local_unnamed_addr

; Function Attrs: nofree nosync nounwind memory(none)
declare i64 @__nt_sym_addr_KeNumberProcessors() local_unnamed_addr #0

attributes #0 = { nofree nosync nounwind memory(none) }


; ModuleID = '<string>'
source_filename = "<string>"
target triple = "x86_64-pc-windows-msvc"

; Function Attrs: nofree nosync nounwind memory(read, inaccessiblemem: none, target_mem0: none, target_mem1: none)
define i64 @KeQueryGroupAffinity(i64 %arg_rcx) local_unnamed_addr #0 {
lift_init:
  %r_cx = trunc i64 %arg_rcx to i16
  %addr_KeActiveProcessors = tail call i64 @__nt_sym_addr_KeActiveProcessors()
  %.24 = inttoptr i64 %addr_KeActiveProcessors to ptr
  %mem_ld = load i16, ptr %.24, align 2
  %.30.not = icmp ugt i16 %mem_ld, %r_cx
  br i1 %.30.not, label %bb_14004cbb9, label %common.ret

common.ret:                                       ; preds = %lift_init, %bb_14004cbb9
  %common.ret.op = phi i64 [ %mem_ld.1, %bb_14004cbb9 ], [ 0, %lift_init ]
  ret i64 %common.ret.op

bb_14004cbb9:                                     ; preds = %lift_init
  %addr___nt_rodata_1403f93e8 = tail call i64 @__nt_sym_addr___nt_rodata_1403f93e8()
  %.45 = shl i64 %arg_rcx, 3
  %.48 = and i64 %.45, 524280
  %.49 = add i64 %addr___nt_rodata_1403f93e8, %.48
  %.50 = inttoptr i64 %.49 to ptr
  %mem_ld.1 = load i64, ptr %.50, align 4
  br label %common.ret
}

; Function Attrs: nofree nosync nounwind memory(none)
declare i64 @__nt_sym_addr_KeActiveProcessors() local_unnamed_addr #1

; Function Attrs: nofree nosync nounwind memory(none)
declare i64 @__nt_sym_addr___nt_rodata_1403f93e8() local_unnamed_addr #1

attributes #0 = { nofree nosync nounwind memory(read, inaccessiblemem: none, target_mem0: none, target_mem1: none) }
attributes #1 = { nofree nosync nounwind memory(none) }


; ModuleID = '<string>'
source_filename = "<string>"
target triple = "x86_64-pc-windows-msvc"

; Function Attrs: mustprogress nofree norecurse nosync nounwind willreturn memory(write, inaccessiblemem: none, target_mem0: none, target_mem1: none)
define i64 @KeInitializeTimer(i64 %arg_rcx) local_unnamed_addr #0 {
lift_init:
  %.32 = add i64 %arg_rcx, 8
  %.34 = inttoptr i64 %arg_rcx to ptr
  store i64 8, ptr %.34, align 4
  %.38 = add i64 %arg_rcx, 16
  %.39 = inttoptr i64 %.38 to ptr
  store i64 %.32, ptr %.39, align 4
  %.41 = inttoptr i64 %.32 to ptr
  store i64 %.32, ptr %.41, align 4
  %.43 = add i64 %arg_rcx, 24
  %.44 = inttoptr i64 %.43 to ptr
  store i64 0, ptr %.44, align 4
  %.46 = add i64 %arg_rcx, 56
  %.47 = inttoptr i64 %.46 to ptr
  store i64 0, ptr %.47, align 4
  ret i64 %.32
}

attributes #0 = { mustprogress nofree norecurse nosync nounwind willreturn memory(write, inaccessiblemem: none, target_mem0: none, target_mem1: none) }


; ModuleID = '<string>'
source_filename = "<string>"
target triple = "x86_64-pc-windows-msvc"

; Function Attrs: nofree nosync nounwind memory(read, inaccessiblemem: none, target_mem0: none, target_mem1: none)
define range(i64 0, 4294967296) i64 @KeQueryHeteroCpuPolicyThread(i64 %arg_rcx, i64 %arg_rdx) local_unnamed_addr #0 {
lift_init:
  %r_edx = trunc i64 %arg_rdx to i32
  %.26 = icmp eq i32 %r_edx, 0
  br i1 %.26, label %bb_1401fbf57, label %bb_1401fbf44

bb_1401fbf44:                                     ; preds = %lift_init
  %.37 = add i64 %arg_rcx, 126
  %.38 = inttoptr i64 %.37 to ptr
  %mem_ld = load i8, ptr %.38, align 1
  %0 = and i8 %mem_ld, 127
  %.52.not = icmp eq i8 %0, 8
  br i1 %.52.not, label %bb_1401fbf50, label %bb_1401fbf5b

common.ret:                                       ; preds = %bb_1401fbf5b, %bb_1401fbf50
  %common.ret.op = phi i64 [ %.71, %bb_1401fbf50 ], [ %reg_rax.0, %bb_1401fbf5b ]
  ret i64 %common.ret.op

bb_1401fbf50:                                     ; preds = %bb_1401fbf44
  %addr_KiDefaultHeteroCpuPolicy = tail call i64 @__nt_sym_addr_KiDefaultHeteroCpuPolicy()
  %.70 = inttoptr i64 %addr_KiDefaultHeteroCpuPolicy to ptr
  %mem_ld.1 = load i32, ptr %.70, align 4
  %.71 = zext i32 %mem_ld.1 to i64
  br label %common.ret

bb_1401fbf57:                                     ; preds = %lift_init
  %.76 = add i64 %arg_rcx, 125
  %.77 = inttoptr i64 %.76 to ptr
  %mem_ld.2 = load i8, ptr %.77, align 1
  br label %bb_1401fbf5b

bb_1401fbf5b:                                     ; preds = %bb_1401fbf57, %bb_1401fbf44
  %reg_rax.0.in = phi i8 [ %mem_ld.2, %bb_1401fbf57 ], [ %0, %bb_1401fbf44 ]
  %reg_rax.0 = zext i8 %reg_rax.0.in to i64
  br label %common.ret
}

; Function Attrs: nofree nosync nounwind memory(none)
declare i64 @__nt_sym_addr_KiDefaultHeteroCpuPolicy() local_unnamed_addr #1

attributes #0 = { nofree nosync nounwind memory(read, inaccessiblemem: none, target_mem0: none, target_mem1: none) }
attributes #1 = { nofree nosync nounwind memory(none) }


; ModuleID = '<string>'
source_filename = "<string>"
target triple = "x86_64-pc-windows-msvc"

; Function Attrs: mustprogress nofree norecurse nosync nounwind willreturn memory(write, inaccessiblemem: none, target_mem0: none, target_mem1: none)
define i64 @KeInitializeTimerEx(i64 %arg_rcx, i64 %arg_rdx) local_unnamed_addr #0 {
lift_init:
  %.33 = add i64 %arg_rcx, 8
  %.35 = inttoptr i64 %arg_rcx to ptr
  store i64 0, ptr %.35, align 4
  %r_dl = trunc i64 %arg_rdx to i8
  %.37 = add i8 %r_dl, 8
  store i8 %.37, ptr %.35, align 4
  %.57 = add i64 %arg_rcx, 16
  %.58 = inttoptr i64 %.57 to ptr
  store i64 %.33, ptr %.58, align 4
  %.60 = inttoptr i64 %.33 to ptr
  store i64 %.33, ptr %.60, align 4
  %.62 = add i64 %arg_rcx, 24
  %.63 = inttoptr i64 %.62 to ptr
  store i64 0, ptr %.63, align 4
  %.65 = add i64 %arg_rcx, 56
  %.66 = inttoptr i64 %.65 to ptr
  store i64 0, ptr %.66, align 4
  ret i64 %.33
}

attributes #0 = { mustprogress nofree norecurse nosync nounwind willreturn memory(write, inaccessiblemem: none, target_mem0: none, target_mem1: none) }


; ModuleID = '<string>'
source_filename = "<string>"
target triple = "x86_64-pc-windows-msvc"

define i64 @KeExpandKernelStackAndCalloutEx(i64 %arg_rcx, i64 %arg_rdx, i64 %arg_r8, i64 %arg_r9) local_unnamed_addr {
lift_init:
  %r_r9b = trunc i64 %arg_r9 to i8
  %.50.not = icmp eq i8 %r_r9b, 0
  %.92 = select i1 %.50.not, i64 0, i64 2
  %call_res = tail call i64 @KeExpandKernelStackAndCalloutInternal(i64 %arg_rcx, i64 %arg_rdx, i64 %arg_r8, i64 %.92)
  ret i64 %call_res
}

declare i64 @KeExpandKernelStackAndCalloutInternal(i64, i64, i64, i64) local_unnamed_addr


; ModuleID = '<string>'
source_filename = "<string>"
target triple = "x86_64-pc-windows-msvc"

; Function Attrs: mustprogress nofree norecurse nosync nounwind willreturn memory(readwrite, inaccessiblemem: none, target_mem0: none, target_mem1: none)
define i64 @KeInitializeDeviceQueue(i64 %arg_rcx) local_unnamed_addr #0 {
lift_init:
  %.24 = add i64 %arg_rcx, 8
  %.26 = inttoptr i64 %arg_rcx to ptr
  store i32 2621460, ptr %.26, align 4
  %.28 = add i64 %arg_rcx, 16
  %.29 = inttoptr i64 %.28 to ptr
  store i64 %.24, ptr %.29, align 4
  %.31 = inttoptr i64 %.24 to ptr
  store i64 %.24, ptr %.31, align 4
  %.33 = add i64 %arg_rcx, 24
  %.34 = inttoptr i64 %.33 to ptr
  store i64 0, ptr %.34, align 4
  %.45 = add i64 %arg_rcx, 32
  %.46 = inttoptr i64 %.45 to ptr
  store i8 0, ptr %.46, align 1
  %mem_ld.1 = load i64, ptr %.46, align 4
  %.50 = and i64 %mem_ld.1, 255
  store i64 %.50, ptr %.46, align 4
  ret i64 %.24
}

attributes #0 = { mustprogress nofree norecurse nosync nounwind willreturn memory(readwrite, inaccessiblemem: none, target_mem0: none, target_mem1: none) }


; ModuleID = '<string>'
source_filename = "<string>"
target triple = "x86_64-pc-windows-msvc"

; Function Attrs: mustprogress nofree norecurse nosync nounwind willreturn memory(read, inaccessiblemem: none, target_mem0: none, target_mem1: none)
define range(i64 0, 4294967296) i64 @PsGetSiloMonitorContextSlot(i64 %arg_rcx) local_unnamed_addr #0 {
lift_init:
  %.24 = add i64 %arg_rcx, 20
  %.25 = inttoptr i64 %.24 to ptr
  %mem_ld = load i32, ptr %.25, align 4
  %.26 = zext i32 %mem_ld to i64
  ret i64 %.26
}

attributes #0 = { mustprogress nofree norecurse nosync nounwind willreturn memory(read, inaccessiblemem: none, target_mem0: none, target_mem1: none) }


; ModuleID = '<string>'
source_filename = "<string>"
target triple = "x86_64-pc-windows-msvc"

; Function Attrs: mustprogress nofree norecurse nosync nounwind willreturn memory(none)
define i64 @PsGetJobLock(i64 %arg_rcx) local_unnamed_addr #0 {
lift_init:
  %.24 = add i64 %arg_rcx, 56
  ret i64 %.24
}

attributes #0 = { mustprogress nofree norecurse nosync nounwind willreturn memory(none) }


; ModuleID = '<string>'
source_filename = "<string>"
target triple = "x86_64-pc-windows-msvc"

; Function Attrs: mustprogress nofree norecurse nosync nounwind willreturn memory(read, inaccessiblemem: none, target_mem0: none, target_mem1: none)
define range(i64 0, 4294967296) i64 @PsGetJobUIRestrictionsClass(i64 %arg_rcx) local_unnamed_addr #0 {
lift_init:
  %.24 = add i64 %arg_rcx, 448
  %.25 = inttoptr i64 %.24 to ptr
  %mem_ld = load i32, ptr %.25, align 4
  %.26 = zext i32 %mem_ld to i64
  ret i64 %.26
}

attributes #0 = { mustprogress nofree norecurse nosync nounwind willreturn memory(read, inaccessiblemem: none, target_mem0: none, target_mem1: none) }


; ModuleID = '<string>'
source_filename = "<string>"
target triple = "x86_64-pc-windows-msvc"

; Function Attrs: mustprogress nofree norecurse nosync nounwind willreturn memory(read, inaccessiblemem: none, target_mem0: none, target_mem1: none)
define range(i64 0, 256) i64 @PsGetProcessPriorityClass(i64 %arg_rcx) local_unnamed_addr #0 {
lift_init:
  %.24 = add i64 %arg_rcx, 1119
  %.25 = inttoptr i64 %.24 to ptr
  %mem_ld = load i8, ptr %.25, align 1
  %.28 = zext i8 %mem_ld to i64
  ret i64 %.28
}

attributes #0 = { mustprogress nofree norecurse nosync nounwind willreturn memory(read, inaccessiblemem: none, target_mem0: none, target_mem1: none) }


; ModuleID = '<string>'
source_filename = "<string>"
target triple = "x86_64-pc-windows-msvc"

; Function Attrs: mustprogress nofree norecurse nosync nounwind willreturn memory(read, inaccessiblemem: none, target_mem0: none, target_mem1: none)
define range(i64 0, 256) i64 @PsGetProcessProtection(i64 %arg_rcx) local_unnamed_addr #0 {
lift_init:
  %.24 = add i64 %arg_rcx, 1738
  %.25 = inttoptr i64 %.24 to ptr
  %mem_ld = load i8, ptr %.25, align 1
  %.28 = zext i8 %mem_ld to i64
  ret i64 %.28
}

attributes #0 = { mustprogress nofree norecurse nosync nounwind willreturn memory(read, inaccessiblemem: none, target_mem0: none, target_mem1: none) }


; ModuleID = '<string>'
source_filename = "<string>"
target triple = "x86_64-pc-windows-msvc"

; Function Attrs: mustprogress nofree norecurse nosync nounwind willreturn memory(write, inaccessiblemem: none, target_mem0: none, target_mem1: none)
define noundef i64 @PsSetProcessPriorityClass(i64 %arg_rcx, i64 %arg_rdx) local_unnamed_addr #0 {
lift_init:
  %r_dl = trunc i64 %arg_rdx to i8
  %.25 = add i64 %arg_rcx, 1119
  %.26 = inttoptr i64 %.25 to ptr
  store i8 %r_dl, ptr %.26, align 1
  ret i64 0
}

attributes #0 = { mustprogress nofree norecurse nosync nounwind willreturn memory(write, inaccessiblemem: none, target_mem0: none, target_mem1: none) }


; ModuleID = '<string>'
source_filename = "<string>"
target triple = "x86_64-pc-windows-msvc"

; Function Attrs: mustprogress nofree norecurse nosync nounwind willreturn memory(read, inaccessiblemem: none, target_mem0: none, target_mem1: none)
define i64 @PsGetProcessCreateTimeQuadPart(i64 %arg_rcx) local_unnamed_addr #0 {
lift_init:
  %.24 = add i64 %arg_rcx, 776
  %.25 = inttoptr i64 %.24 to ptr
  %mem_ld = load i64, ptr %.25, align 4
  ret i64 %mem_ld
}

attributes #0 = { mustprogress nofree norecurse nosync nounwind willreturn memory(read, inaccessiblemem: none, target_mem0: none, target_mem1: none) }


; ModuleID = '<string>'
source_filename = "<string>"
target triple = "x86_64-pc-windows-msvc"

; Function Attrs: mustprogress nofree norecurse nosync nounwind willreturn memory(read, inaccessiblemem: none, target_mem0: none, target_mem1: none)
define i64 @PsGetProcessDebugPort(i64 %arg_rcx) local_unnamed_addr #0 {
lift_init:
  %.24 = add i64 %arg_rcx, 1056
  %.25 = inttoptr i64 %.24 to ptr
  %mem_ld = load i64, ptr %.25, align 4
  ret i64 %mem_ld
}

attributes #0 = { mustprogress nofree norecurse nosync nounwind willreturn memory(read, inaccessiblemem: none, target_mem0: none, target_mem1: none) }


; ModuleID = '<string>'
source_filename = "<string>"
target triple = "x86_64-pc-windows-msvc"

; Function Attrs: mustprogress nofree norecurse nosync nounwind willreturn memory(read, inaccessiblemem: none, target_mem0: none, target_mem1: none)
define i64 @PsGetProcessDxgProcess(i64 %arg_rcx) local_unnamed_addr #0 {
lift_init:
  %.24 = add i64 %arg_rcx, 1976
  %.25 = inttoptr i64 %.24 to ptr
  %mem_ld = load i64, ptr %.25, align 4
  ret i64 %mem_ld
}

attributes #0 = { mustprogress nofree norecurse nosync nounwind willreturn memory(read, inaccessiblemem: none, target_mem0: none, target_mem1: none) }


; ModuleID = '<string>'
source_filename = "<string>"
target triple = "x86_64-pc-windows-msvc"

; Function Attrs: mustprogress nofree norecurse nosync nounwind willreturn memory(read, inaccessiblemem: none, target_mem0: none, target_mem1: none)
define i64 @PsGetProcessJob(i64 %arg_rcx) local_unnamed_addr #0 {
lift_init:
  %.24 = add i64 %arg_rcx, 944
  %.25 = inttoptr i64 %.24 to ptr
  %mem_ld = load i64, ptr %.25, align 4
  ret i64 %mem_ld
}

attributes #0 = { mustprogress nofree norecurse nosync nounwind willreturn memory(read, inaccessiblemem: none, target_mem0: none, target_mem1: none) }


; ModuleID = '<string>'
source_filename = "<string>"
target triple = "x86_64-pc-windows-msvc"

; Function Attrs: mustprogress nofree norecurse nosync nounwind willreturn memory(read, inaccessiblemem: none, target_mem0: none, target_mem1: none)
define i64 @PsGetProcessSecurityPort(i64 %arg_rcx) local_unnamed_addr #0 {
lift_init:
  %.24 = add i64 %arg_rcx, 1120
  %.25 = inttoptr i64 %.24 to ptr
  %mem_ld = load i64, ptr %.25, align 4
  ret i64 %mem_ld
}

attributes #0 = { mustprogress nofree norecurse nosync nounwind willreturn memory(read, inaccessiblemem: none, target_mem0: none, target_mem1: none) }


; ModuleID = '<string>'
source_filename = "<string>"
target triple = "x86_64-pc-windows-msvc"

; Function Attrs: mustprogress nofree norecurse nosync nounwind willreturn memory(read, inaccessiblemem: none, target_mem0: none, target_mem1: none)
define i64 @PsGetProcessSequenceNumber(i64 %arg_rcx) local_unnamed_addr #0 {
lift_init:
  %.24 = add i64 %arg_rcx, 1872
  %.25 = inttoptr i64 %.24 to ptr
  %mem_ld = load i64, ptr %.25, align 4
  ret i64 %mem_ld
}

attributes #0 = { mustprogress nofree norecurse nosync nounwind willreturn memory(read, inaccessiblemem: none, target_mem0: none, target_mem1: none) }


; ModuleID = '<string>'
source_filename = "<string>"
target triple = "x86_64-pc-windows-msvc"

; Function Attrs: mustprogress nofree norecurse nosync nounwind willreturn memory(read, inaccessiblemem: none, target_mem0: none, target_mem1: none)
define i64 @PsGetProcessServerSilo(i64 %arg_rcx) local_unnamed_addr #0 {
lift_init:
  %.24 = add i64 %arg_rcx, 1728
  %.25 = inttoptr i64 %.24 to ptr
  %mem_ld = load i64, ptr %.25, align 4
  ret i64 %mem_ld
}

attributes #0 = { mustprogress nofree norecurse nosync nounwind willreturn memory(read, inaccessiblemem: none, target_mem0: none, target_mem1: none) }


; ModuleID = '<string>'
source_filename = "<string>"
target triple = "x86_64-pc-windows-msvc"

; Function Attrs: mustprogress nofree norecurse nosync nounwind willreturn memory(read, inaccessiblemem: none, target_mem0: none, target_mem1: none)
define i64 @PsGetProcessWin32Process(i64 %arg_rcx) local_unnamed_addr #0 {
lift_init:
  %.24 = add i64 %arg_rcx, 936
  %.25 = inttoptr i64 %.24 to ptr
  %mem_ld = load i64, ptr %.25, align 4
  ret i64 %mem_ld
}

attributes #0 = { mustprogress nofree norecurse nosync nounwind willreturn memory(read, inaccessiblemem: none, target_mem0: none, target_mem1: none) }


; ModuleID = '<string>'
source_filename = "<string>"
target triple = "x86_64-pc-windows-msvc"

; Function Attrs: mustprogress nofree norecurse nosync nounwind willreturn memory(read, inaccessiblemem: none, target_mem0: none, target_mem1: none)
define i64 @PsGetProcessWin32WindowStation(i64 %arg_rcx) local_unnamed_addr #0 {
lift_init:
  %.24 = add i64 %arg_rcx, 984
  %.25 = inttoptr i64 %.24 to ptr
  %mem_ld = load i64, ptr %.25, align 4
  ret i64 %mem_ld
}

attributes #0 = { mustprogress nofree norecurse nosync nounwind willreturn memory(read, inaccessiblemem: none, target_mem0: none, target_mem1: none) }


; ModuleID = '<string>'
source_filename = "<string>"
target triple = "x86_64-pc-windows-msvc"

; Function Attrs: mustprogress nofree norecurse nosync nounwind willreturn memory(read, inaccessiblemem: none, target_mem0: none, target_mem1: none)
define i64 @PsGetThreadCreateTime(i64 %arg_rcx) local_unnamed_addr #0 {
lift_init:
  %.24 = add i64 %arg_rcx, 1520
  %.25 = inttoptr i64 %.24 to ptr
  %mem_ld = load i64, ptr %.25, align 4
  ret i64 %mem_ld
}

attributes #0 = { mustprogress nofree norecurse nosync nounwind willreturn memory(read, inaccessiblemem: none, target_mem0: none, target_mem1: none) }


; ModuleID = '<string>'
source_filename = "<string>"
target triple = "x86_64-pc-windows-msvc"

; Function Attrs: mustprogress nofree norecurse nosync nounwind willreturn memory(read, inaccessiblemem: none, target_mem0: none, target_mem1: none)
define i64 @PsGetThreadWin32Thread(i64 %arg_rcx) local_unnamed_addr #0 {
lift_init:
  %.24 = add i64 %arg_rcx, 456
  %.25 = inttoptr i64 %.24 to ptr
  %mem_ld = load i64, ptr %.25, align 4
  ret i64 %mem_ld
}

attributes #0 = { mustprogress nofree norecurse nosync nounwind willreturn memory(read, inaccessiblemem: none, target_mem0: none, target_mem1: none) }


; ModuleID = '<string>'
source_filename = "<string>"
target triple = "x86_64-pc-windows-msvc"

; Function Attrs: mustprogress nofree norecurse nosync nounwind willreturn memory(readwrite, inaccessiblemem: none, target_mem0: none, target_mem1: none)
define noundef i64 @PsReferenceKernelStack(i64 %arg_rcx) local_unnamed_addr #0 {
lift_init:
  %.24 = add i64 %arg_rcx, 1904
  %.25 = inttoptr i64 %.24 to ptr
  %mem_ld = load i32, ptr %.25, align 4
  %.27 = add i32 %mem_ld, 1
  store i32 %.27, ptr %.25, align 4
  ret i64 0
}

attributes #0 = { mustprogress nofree norecurse nosync nounwind willreturn memory(readwrite, inaccessiblemem: none, target_mem0: none, target_mem1: none) }


; ModuleID = '<string>'
source_filename = "<string>"
target triple = "x86_64-pc-windows-msvc"

; Function Attrs: mustprogress nofree norecurse nosync nounwind willreturn memory(write, inaccessiblemem: none, target_mem0: none, target_mem1: none)
define noundef i64 @PsSetProcessDxgProcess(i64 %arg_rcx, i64 %arg_rdx) local_unnamed_addr #0 {
lift_init:
  %.25 = add i64 %arg_rcx, 1976
  %.26 = inttoptr i64 %.25 to ptr
  store i64 %arg_rdx, ptr %.26, align 4
  ret i64 0
}

attributes #0 = { mustprogress nofree norecurse nosync nounwind willreturn memory(write, inaccessiblemem: none, target_mem0: none, target_mem1: none) }


; ModuleID = '<string>'
source_filename = "<string>"
target triple = "x86_64-pc-windows-msvc"

; Function Attrs: mustprogress nofree norecurse nosync nounwind willreturn memory(write, inaccessiblemem: none, target_mem0: none, target_mem1: none)
define noundef i64 @PsSetProcessWindowStation(i64 %arg_rcx, i64 %arg_rdx) local_unnamed_addr #0 {
lift_init:
  %.25 = add i64 %arg_rcx, 984
  %.26 = inttoptr i64 %.25 to ptr
  store i64 %arg_rdx, ptr %.26, align 4
  ret i64 0
}

attributes #0 = { mustprogress nofree norecurse nosync nounwind willreturn memory(write, inaccessiblemem: none, target_mem0: none, target_mem1: none) }


; ModuleID = '<string>'
source_filename = "<string>"
target triple = "x86_64-pc-windows-msvc"

; Function Attrs: mustprogress nofree norecurse nosync nounwind willreturn memory(read, inaccessiblemem: none, target_mem0: none, target_mem1: none)
define range(i64 0, 2) i64 @PsIsThreadTerminating(i64 %arg_rcx) local_unnamed_addr #0 {
lift_init:
  %.24 = add i64 %arg_rcx, 1744
  %.25 = inttoptr i64 %.24 to ptr
  %mem_ld = load i32, ptr %.25, align 4
  %.28 = and i32 %mem_ld, 1
  %.35 = zext nneg i32 %.28 to i64
  ret i64 %.35
}

attributes #0 = { mustprogress nofree norecurse nosync nounwind willreturn memory(read, inaccessiblemem: none, target_mem0: none, target_mem1: none) }


; ModuleID = '<string>'
source_filename = "<string>"
target triple = "x86_64-pc-windows-msvc"

; Function Attrs: mustprogress nofree norecurse nosync nounwind willreturn memory(none)
define noundef i64 @PsLookupThreadByThreadId() local_unnamed_addr #0 {
lift_init:
  ret i64 0
}

attributes #0 = { mustprogress nofree norecurse nosync nounwind willreturn memory(none) }


; ModuleID = '<string>'
source_filename = "<string>"
target triple = "x86_64-pc-windows-msvc"

; Function Attrs: mustprogress nofree norecurse nosync nounwind willreturn memory(write, inaccessiblemem: none, target_mem0: none, target_mem1: none)
define noundef i64 @PsSetProcessSecurityPort(i64 %arg_rcx, i64 %arg_rdx) local_unnamed_addr #0 {
lift_init:
  %.25 = add i64 %arg_rcx, 1120
  %.26 = inttoptr i64 %.25 to ptr
  store i64 %arg_rdx, ptr %.26, align 4
  ret i64 0
}

attributes #0 = { mustprogress nofree norecurse nosync nounwind willreturn memory(write, inaccessiblemem: none, target_mem0: none, target_mem1: none) }


; ModuleID = '<string>'
source_filename = "<string>"
target triple = "x86_64-pc-windows-msvc"

; Function Attrs: nofree nosync nounwind memory(read, inaccessiblemem: none, target_mem0: none, target_mem1: none)
define range(i64 0, 2) i64 @PsIsDiskCountersEnabled() local_unnamed_addr #0 {
lift_init:
  %addr_PsDisableDiskCounters = tail call i64 @__nt_sym_addr_PsDisableDiskCounters()
  %.23 = inttoptr i64 %addr_PsDisableDiskCounters to ptr
  %mem_ld = load i32, ptr %.23, align 4
  %.25 = icmp eq i32 %mem_ld, 0
  %.44 = zext i1 %.25 to i64
  ret i64 %.44
}

; Function Attrs: nofree nosync nounwind memory(none)
declare i64 @__nt_sym_addr_PsDisableDiskCounters() local_unnamed_addr #1

attributes #0 = { nofree nosync nounwind memory(read, inaccessiblemem: none, target_mem0: none, target_mem1: none) }
attributes #1 = { nofree nosync nounwind memory(none) }


; ModuleID = '<string>'
source_filename = "<string>"
target triple = "x86_64-pc-windows-msvc"

; Function Attrs: mustprogress nofree norecurse nosync nounwind willreturn memory(none)
define range(i64 0, 2) i64 @PsIsHostSilo(i64 %arg_rcx) local_unnamed_addr #0 {
lift_init:
  %.25.not = icmp eq i64 %arg_rcx, 0
  %. = zext i1 %.25.not to i64
  ret i64 %.
}

attributes #0 = { mustprogress nofree norecurse nosync nounwind willreturn memory(none) }


; ModuleID = '<string>'
source_filename = "<string>"
target triple = "x86_64-pc-windows-msvc"

; Function Attrs: mustprogress nofree norecurse nosync nounwind willreturn memory(read, inaccessiblemem: none, target_mem0: none, target_mem1: none)
define range(i64 0, 1073741570) i64 @PsGetProcessExitProcessCalled(i64 %arg_rcx) local_unnamed_addr #0 {
lift_init:
  %.24 = add i64 %arg_rcx, 772
  %.25 = inttoptr i64 %.24 to ptr
  %mem_ld = load i32, ptr %.25, align 4
  %.30 = lshr i32 %mem_ld, 2
  %.494 = and i32 %.30, 1073741569
  %.49 = zext nneg i32 %.494 to i64
  ret i64 %.49
}

attributes #0 = { mustprogress nofree norecurse nosync nounwind willreturn memory(read, inaccessiblemem: none, target_mem0: none, target_mem1: none) }


; ModuleID = '<string>'
source_filename = "<string>"
target triple = "x86_64-pc-windows-msvc"

; Function Attrs: mustprogress nofree norecurse nosync nounwind willreturn memory(read, inaccessiblemem: none, target_mem0: none, target_mem1: none)
define range(i64 0, 2) i64 @PsGetThreadHardErrorsAreDisabled(i64 %arg_rcx) local_unnamed_addr #0 {
lift_init:
  %.24 = add i64 %arg_rcx, 1744
  %.25 = inttoptr i64 %.24 to ptr
  %mem_ld = load i8, ptr %.25, align 1
  %.32 = lshr i8 %mem_ld, 4
  %.44 = and i8 %.32, 1
  %.53 = zext nneg i8 %.44 to i64
  ret i64 %.53
}

attributes #0 = { mustprogress nofree norecurse nosync nounwind willreturn memory(read, inaccessiblemem: none, target_mem0: none, target_mem1: none) }


; ModuleID = '<string>'
source_filename = "<string>"
target triple = "x86_64-pc-windows-msvc"

; Function Attrs: mustprogress nofree norecurse nosync nounwind willreturn memory(read, inaccessiblemem: none, target_mem0: none, target_mem1: none)
define range(i64 0, 2) i64 @PsIsProcessBeingDebugged(i64 %arg_rcx) local_unnamed_addr #0 {
lift_init:
  %.24 = add i64 %arg_rcx, 1056
  %.25 = inttoptr i64 %.24 to ptr
  %mem_ld = load i64, ptr %.25, align 4
  %.27 = icmp ne i64 %mem_ld, 0
  %.47 = zext i1 %.27 to i64
  ret i64 %.47
}

attributes #0 = { mustprogress nofree norecurse nosync nounwind willreturn memory(read, inaccessiblemem: none, target_mem0: none, target_mem1: none) }


; ModuleID = '<string>'
source_filename = "<string>"
target triple = "x86_64-pc-windows-msvc"

; Function Attrs: mustprogress nofree norecurse nosync nounwind willreturn memory(read, inaccessiblemem: none, target_mem0: none, target_mem1: none)
define range(i64 0, 16776962) i64 @PsIsProcessCommitRelinquished(i64 %arg_rcx) local_unnamed_addr #0 {
lift_init:
  %.24 = add i64 %arg_rcx, 1740
  %.25 = inttoptr i64 %.24 to ptr
  %mem_ld = load i32, ptr %.25, align 4
  %.30 = lshr i32 %mem_ld, 8
  %.494 = and i32 %.30, 16776961
  %.49 = zext nneg i32 %.494 to i64
  ret i64 %.49
}

attributes #0 = { mustprogress nofree norecurse nosync nounwind willreturn memory(read, inaccessiblemem: none, target_mem0: none, target_mem1: none) }


; ModuleID = '<string>'
source_filename = "<string>"
target triple = "x86_64-pc-windows-msvc"

; Function Attrs: mustprogress nofree norecurse nosync nounwind willreturn memory(read, inaccessiblemem: none, target_mem0: none, target_mem1: none)
define range(i64 0, 536870658) i64 @PsIsThreadImpersonating(i64 %arg_rcx) local_unnamed_addr #0 {
lift_init:
  %.24 = add i64 %arg_rcx, 1744
  %.25 = inttoptr i64 %.24 to ptr
  %mem_ld = load i32, ptr %.25, align 4
  %.30 = lshr i32 %mem_ld, 3
  %.494 = and i32 %.30, 536870657
  %.49 = zext nneg i32 %.494 to i64
  ret i64 %.49
}

attributes #0 = { mustprogress nofree norecurse nosync nounwind willreturn memory(read, inaccessiblemem: none, target_mem0: none, target_mem1: none) }


; ModuleID = '<string>'
source_filename = "<string>"
target triple = "x86_64-pc-windows-msvc"

; Function Attrs: mustprogress nofree norecurse nosync nounwind willreturn memory(read, inaccessiblemem: none, target_mem0: none, target_mem1: none)
define range(i64 0, 130818) i64 @PsIsWin32KFilterAuditEnabledForProcess(i64 %arg_rcx) local_unnamed_addr #0 {
lift_init:
  %.24 = add i64 %arg_rcx, 2088
  %.25 = inttoptr i64 %.24 to ptr
  %mem_ld = load i32, ptr %.25, align 4
  %.30 = lshr i32 %mem_ld, 15
  %.494 = and i32 %.30, 130817
  %.49 = zext nneg i32 %.494 to i64
  ret i64 %.49
}

attributes #0 = { mustprogress nofree norecurse nosync nounwind willreturn memory(read, inaccessiblemem: none, target_mem0: none, target_mem1: none) }


; ModuleID = '<string>'
source_filename = "<string>"
target triple = "x86_64-pc-windows-msvc"

; Function Attrs: mustprogress nofree norecurse nosync nounwind willreturn memory(read, inaccessiblemem: none, target_mem0: none, target_mem1: none)
define range(i64 0, 261890) i64 @PsIsWin32KFilterEnabledForProcess(i64 %arg_rcx) local_unnamed_addr #0 {
lift_init:
  %.24 = add i64 %arg_rcx, 2088
  %.25 = inttoptr i64 %.24 to ptr
  %mem_ld = load i32, ptr %.25, align 4
  %.30 = lshr i32 %mem_ld, 14
  %.494 = and i32 %.30, 261889
  %.49 = zext nneg i32 %.494 to i64
  ret i64 %.49
}

attributes #0 = { mustprogress nofree norecurse nosync nounwind willreturn memory(read, inaccessiblemem: none, target_mem0: none, target_mem1: none) }


; ModuleID = '<string>'
source_filename = "<string>"
target triple = "x86_64-pc-windows-msvc"

; Function Attrs: nofree nosync nounwind memory(write, inaccessiblemem: none, target_mem0: none, target_mem1: none)
define noundef i64 @PsSetLegoNotifyRoutine(i64 %arg_rcx) local_unnamed_addr #0 {
lift_init:
  %addr_PspLegoNotifyRoutine = tail call i64 @__nt_sym_addr_PspLegoNotifyRoutine()
  %.24 = inttoptr i64 %addr_PspLegoNotifyRoutine to ptr
  store i64 %arg_rcx, ptr %.24, align 4
  ret i64 720
}

; Function Attrs: nofree nosync nounwind memory(none)
declare i64 @__nt_sym_addr_PspLegoNotifyRoutine() local_unnamed_addr #1

attributes #0 = { nofree nosync nounwind memory(write, inaccessiblemem: none, target_mem0: none, target_mem1: none) }
attributes #1 = { nofree nosync nounwind memory(none) }


; ModuleID = '<string>'
source_filename = "<string>"
target triple = "x86_64-pc-windows-msvc"

define i64 @PsGetCurrentThreadStackBase() local_unnamed_addr {
lift_init:
  %gs_val = tail call i64 @__nt_read_gs_i64(i64 392)
  %.25 = add i64 %gs_val, 56
  %.26 = inttoptr i64 %.25 to ptr
  %mem_ld = load i64, ptr %.26, align 4
  ret i64 %mem_ld
}

declare i64 @__nt_read_gs_i64(i64) local_unnamed_addr


; ModuleID = '<string>'
source_filename = "<string>"
target triple = "x86_64-pc-windows-msvc"

define i64 @PsGetCurrentThreadStackLimit() local_unnamed_addr {
lift_init:
  %gs_val = tail call i64 @__nt_read_gs_i64(i64 392)
  %.25 = add i64 %gs_val, 48
  %.26 = inttoptr i64 %.25 to ptr
  %mem_ld = load i64, ptr %.26, align 4
  ret i64 %mem_ld
}

declare i64 @__nt_read_gs_i64(i64) local_unnamed_addr


; ModuleID = '<string>'
source_filename = "<string>"
target triple = "x86_64-pc-windows-msvc"

define i64 @PsGetCurrentThreadPreviousMode() local_unnamed_addr {
lift_init:
  %gs_val = tail call i64 @__nt_read_gs_i64(i64 392)
  %.25 = add i64 %gs_val, 562
  %.26 = inttoptr i64 %.25 to ptr
  %mem_ld = load i8, ptr %.26, align 1
  %.28 = and i64 %gs_val, -256
  %.29 = zext i8 %mem_ld to i64
  %.30 = or disjoint i64 %.28, %.29
  ret i64 %.30
}

declare i64 @__nt_read_gs_i64(i64) local_unnamed_addr


; ModuleID = '<string>'
source_filename = "<string>"
target triple = "x86_64-pc-windows-msvc"

; Function Attrs: mustprogress nofree norecurse nosync nounwind willreturn memory(read, inaccessiblemem: none, target_mem0: none, target_mem1: none)
define range(i64 0, 4294967296) i64 @PsGetJobSessionId(i64 %arg_rcx) local_unnamed_addr #0 {
lift_init:
  %.24 = add i64 %arg_rcx, 480
  %.25 = inttoptr i64 %.24 to ptr
  %mem_ld = load i32, ptr %.25, align 4
  %.56 = icmp ugt i32 %mem_ld, -3
  %.57 = select i1 %.56, i32 -1, i32 %mem_ld
  %.58 = zext i32 %.57 to i64
  ret i64 %.58
}

attributes #0 = { mustprogress nofree norecurse nosync nounwind willreturn memory(read, inaccessiblemem: none, target_mem0: none, target_mem1: none) }


; ModuleID = '<string>'
source_filename = "<string>"
target triple = "x86_64-pc-windows-msvc"

define i64 @PsDetachSiloFromCurrentThread(i64 %arg_rcx) local_unnamed_addr {
lift_init:
  %gs_val = tail call i64 @__nt_read_gs_i64(i64 392)
  %.26 = add i64 %gs_val, 2000
  %.27 = inttoptr i64 %.26 to ptr
  store i64 %arg_rcx, ptr %.27, align 4
  ret i64 %gs_val
}

declare i64 @__nt_read_gs_i64(i64) local_unnamed_addr


; ModuleID = '<string>'
source_filename = "<string>"
target triple = "x86_64-pc-windows-msvc"

define i64 @PsGetCurrentProcessId() local_unnamed_addr {
lift_init:
  %gs_val = tail call i64 @__nt_read_gs_i64(i64 392)
  %.25 = add i64 %gs_val, 1592
  %.26 = inttoptr i64 %.25 to ptr
  %mem_ld = load i64, ptr %.26, align 4
  ret i64 %mem_ld
}

declare i64 @__nt_read_gs_i64(i64) local_unnamed_addr


; ModuleID = '<string>'
source_filename = "<string>"
target triple = "x86_64-pc-windows-msvc"

define i64 @PsGetCurrentThreadId() local_unnamed_addr {
lift_init:
  %gs_val = tail call i64 @__nt_read_gs_i64(i64 392)
  %.25 = add i64 %gs_val, 1600
  %.26 = inttoptr i64 %.25 to ptr
  %mem_ld = load i64, ptr %.26, align 4
  ret i64 %mem_ld
}

declare i64 @__nt_read_gs_i64(i64) local_unnamed_addr


; ModuleID = '<string>'
source_filename = "<string>"
target triple = "x86_64-pc-windows-msvc"

define i64 @PsGetCurrentThreadProcess() local_unnamed_addr {
lift_init:
  %gs_val = tail call i64 @__nt_read_gs_i64(i64 392)
  %.25 = add i64 %gs_val, 544
  %.26 = inttoptr i64 %.25 to ptr
  %mem_ld = load i64, ptr %.26, align 4
  ret i64 %mem_ld
}

declare i64 @__nt_read_gs_i64(i64) local_unnamed_addr


; ModuleID = '<string>'
source_filename = "<string>"
target triple = "x86_64-pc-windows-msvc"

define i64 @PsGetCurrentThreadWin32Thread() local_unnamed_addr {
lift_init:
  %gs_val = tail call i64 @__nt_read_gs_i64(i64 392)
  %.25 = add i64 %gs_val, 456
  %.26 = inttoptr i64 %.25 to ptr
  %mem_ld = load i64, ptr %.26, align 4
  ret i64 %mem_ld
}

declare i64 @__nt_read_gs_i64(i64) local_unnamed_addr


; ModuleID = '<string>'
source_filename = "<string>"
target triple = "x86_64-pc-windows-msvc"

define range(i64 0, -254) i64 @PsIsCurrentThreadPrefetching() local_unnamed_addr {
lift_init:
  %gs_val = tail call i64 @__nt_read_gs_i64(i64 392)
  %.25 = add i64 %gs_val, 1752
  %.26 = inttoptr i64 %.25 to ptr
  %mem_ld = load i8, ptr %.26, align 1
  %.28 = and i64 %gs_val, -256
  %.33 = lshr i8 %mem_ld, 6
  %.45 = and i8 %.33, 1
  %.54 = zext nneg i8 %.45 to i64
  %.55 = or disjoint i64 %.28, %.54
  ret i64 %.55
}

declare i64 @__nt_read_gs_i64(i64) local_unnamed_addr


; ModuleID = '<string>'
source_filename = "<string>"
target triple = "x86_64-pc-windows-msvc"

; Function Attrs: mustprogress nofree norecurse nosync nounwind willreturn memory(readwrite, inaccessiblemem: none, target_mem0: none, target_mem1: none)
define noundef i64 @PsSetThreadHardErrorsAreDisabled(i64 %arg_rcx, i64 %arg_rdx) local_unnamed_addr #0 {
lift_init:
  %r_dl = trunc i64 %arg_rdx to i8
  %.26 = icmp eq i8 %r_dl, 0
  %.52 = add i64 %arg_rcx, 1744
  %.53 = inttoptr i64 %.52 to ptr
  %mem_ld.1 = load i32, ptr %.53, align 4
  %.54 = and i32 %mem_ld.1, -17
  %masksel = select i1 %.26, i32 0, i32 16
  %.54.sink = or disjoint i32 %.54, %masksel
  store i32 %.54.sink, ptr %.53, align 4
  ret i64 0
}

attributes #0 = { mustprogress nofree norecurse nosync nounwind willreturn memory(readwrite, inaccessiblemem: none, target_mem0: none, target_mem1: none) }


; ModuleID = '<string>'
source_filename = "<string>"
target triple = "x86_64-pc-windows-msvc"

define range(i64 0, 4294967296) i64 @PsGetWin32KFilterSet() local_unnamed_addr {
lift_init:
  %gs_val = tail call i64 @__nt_read_gs_i64(i64 392)
  %.25 = add i64 %gs_val, 184
  %.26 = inttoptr i64 %.25 to ptr
  %mem_ld = load i64, ptr %.26, align 4
  %.28 = add i64 %mem_ld, 1984
  %.29 = inttoptr i64 %.28 to ptr
  %mem_ld.1 = load i32, ptr %.29, align 4
  %.30 = zext i32 %mem_ld.1 to i64
  ret i64 %.30
}

declare i64 @__nt_read_gs_i64(i64) local_unnamed_addr


; ModuleID = '<string>'
source_filename = "<string>"
target triple = "x86_64-pc-windows-msvc"

define noundef i64 @PsRestoreImpersonation() local_unnamed_addr {
lift_init:
  %gs_val = tail call i64 @__nt_read_gs_i64(i64 392)
  ret i64 0
}

declare i64 @__nt_read_gs_i64(i64) local_unnamed_addr


; ModuleID = '<string>'
source_filename = "<string>"
target triple = "x86_64-pc-windows-msvc"

; Function Attrs: mustprogress nofree norecurse nosync nounwind willreturn memory(read, inaccessiblemem: none, target_mem0: none, target_mem1: none)
define range(i64 0, 65536) i64 @PsWow64GetProcessMachine(i64 %arg_rcx) local_unnamed_addr #0 {
lift_init:
  %.24 = add i64 %arg_rcx, 1064
  %.25 = inttoptr i64 %.24 to ptr
  %mem_ld = load i64, ptr %.25, align 4
  %.28.not = icmp eq i64 %mem_ld, 0
  br i1 %.28.not, label %common.ret, label %bb_1404e9de2

common.ret:                                       ; preds = %lift_init, %bb_1404e9de2
  %common.ret.op = phi i64 [ %.48, %bb_1404e9de2 ], [ 34404, %lift_init ]
  ret i64 %common.ret.op

bb_1404e9de2:                                     ; preds = %lift_init
  %.45 = add i64 %mem_ld, 8
  %.46 = inttoptr i64 %.45 to ptr
  %mem_ld.1 = load i16, ptr %.46, align 2
  %.48 = zext i16 %mem_ld.1 to i64
  br label %common.ret
}

attributes #0 = { mustprogress nofree norecurse nosync nounwind willreturn memory(read, inaccessiblemem: none, target_mem0: none, target_mem1: none) }


; ModuleID = '<string>'
source_filename = "<string>"
target triple = "x86_64-pc-windows-msvc"

define i64 @PsAttachSiloToCurrentThread(i64 %arg_rcx) local_unnamed_addr {
lift_init:
  %gs_val = tail call i64 @__nt_read_gs_i64(i64 392)
  %.26 = add i64 %gs_val, 2000
  %.27 = inttoptr i64 %.26 to ptr
  %mem_ld = load i64, ptr %.27, align 4
  store i64 %arg_rcx, ptr %.27, align 4
  ret i64 %mem_ld
}

declare i64 @__nt_read_gs_i64(i64) local_unnamed_addr


; ModuleID = '<string>'
source_filename = "<string>"
target triple = "x86_64-pc-windows-msvc"

define i64 @PsGetCurrentProcessWin32Process() local_unnamed_addr {
lift_init:
  %gs_val = tail call i64 @__nt_read_gs_i64(i64 392)
  %.25 = add i64 %gs_val, 184
  %.26 = inttoptr i64 %.25 to ptr
  %mem_ld = load i64, ptr %.26, align 4
  %.28 = add i64 %mem_ld, 936
  %.29 = inttoptr i64 %.28 to ptr
  %mem_ld.1 = load i64, ptr %.29, align 4
  ret i64 %mem_ld.1
}

declare i64 @__nt_read_gs_i64(i64) local_unnamed_addr


; ModuleID = '<string>'
source_filename = "<string>"
target triple = "x86_64-pc-windows-msvc"

define i64 @PsGetProcessExitTime() local_unnamed_addr {
lift_init:
  %gs_val = tail call i64 @__nt_read_gs_i64(i64 392)
  %.25 = add i64 %gs_val, 184
  %.26 = inttoptr i64 %.25 to ptr
  %mem_ld = load i64, ptr %.26, align 4
  %.28 = add i64 %mem_ld, 1680
  %.29 = inttoptr i64 %.28 to ptr
  %mem_ld.1 = load i64, ptr %.29, align 4
  ret i64 %mem_ld.1
}

declare i64 @__nt_read_gs_i64(i64) local_unnamed_addr


; ModuleID = '<string>'
source_filename = "<string>"
target triple = "x86_64-pc-windows-msvc"

; Function Attrs: mustprogress nofree norecurse nosync nounwind willreturn memory(read, inaccessiblemem: none, target_mem0: none, target_mem1: none)
define i64 @PsGetProcessStartKey(i64 %arg_rcx) local_unnamed_addr #0 {
lift_init:
  %mem_ld = load i32, ptr inttoptr (i64 -9345848835388 to ptr), align 4
  %.26 = zext i32 %mem_ld to i64
  %.30 = shl i64 %.26, 48
  %.38 = add i64 %arg_rcx, 1872
  %.39 = inttoptr i64 %.38 to ptr
  %mem_ld.1 = load i64, ptr %.39, align 4
  %.40 = or i64 %.30, %mem_ld.1
  ret i64 %.40
}

attributes #0 = { mustprogress nofree norecurse nosync nounwind willreturn memory(read, inaccessiblemem: none, target_mem0: none, target_mem1: none) }


; ModuleID = '<string>'
source_filename = "<string>"
target triple = "x86_64-pc-windows-msvc"

; Function Attrs: nofree nosync nounwind memory(none)
define noundef i64 @PsChargeProcessNonPagedPoolQuota(i64 %arg_rcx, i64 %arg_rdx) local_unnamed_addr #0 {
lift_init:
  %addr_PsInitialSystemProcess = tail call i64 @__nt_sym_addr_PsInitialSystemProcess()
  ret i64 0
}

; Function Attrs: nofree nosync nounwind memory(none)
declare i64 @__nt_sym_addr_PsInitialSystemProcess() local_unnamed_addr #0

attributes #0 = { nofree nosync nounwind memory(none) }


; ModuleID = '<string>'
source_filename = "<string>"
target triple = "x86_64-pc-windows-msvc"

; Function Attrs: mustprogress nofree norecurse nosync nounwind willreturn memory(read, inaccessiblemem: none, target_mem0: none, target_mem1: none)
define range(i64 0, 4294967296) i64 @PsGetThreadFreezeCount(i64 %arg_rcx, i64 %arg_rdx) local_unnamed_addr #0 {
lift_init:
  %.25 = add i64 %arg_rcx, 544
  %.26 = inttoptr i64 %.25 to ptr
  %mem_ld = load i64, ptr %.26, align 4
  %.28 = add i64 %mem_ld, 440
  %.29 = inttoptr i64 %.28 to ptr
  %mem_ld.1 = load i32, ptr %.29, align 4
  %.34 = lshr i32 %mem_ld.1, 3
  %.43 = and i32 %.34, 1
  %.52 = add i64 %mem_ld, 616
  %.53 = inttoptr i64 %.52 to ptr
  %mem_ld.2 = load i32, ptr %.53, align 4
  %.54 = add i32 %.43, %mem_ld.2
  %.67 = zext i32 %.54 to i64
  ret i64 %.67
}

attributes #0 = { mustprogress nofree norecurse nosync nounwind willreturn memory(read, inaccessiblemem: none, target_mem0: none, target_mem1: none) }


; ModuleID = '<string>'
source_filename = "<string>"
target triple = "x86_64-pc-windows-msvc"

define range(i64 0, 130818) i64 @PsIsWin32KFilterAuditEnabled() local_unnamed_addr {
lift_init:
  %gs_val = tail call i64 @__nt_read_gs_i64(i64 392)
  %.25 = add i64 %gs_val, 184
  %.26 = inttoptr i64 %.25 to ptr
  %mem_ld = load i64, ptr %.26, align 4
  %.28 = add i64 %mem_ld, 2088
  %.29 = inttoptr i64 %.28 to ptr
  %mem_ld.1 = load i32, ptr %.29, align 4
  %.34 = lshr i32 %mem_ld.1, 15
  %.535 = and i32 %.34, 130817
  %.53 = zext nneg i32 %.535 to i64
  ret i64 %.53
}

declare i64 @__nt_read_gs_i64(i64) local_unnamed_addr


; ModuleID = '<string>'
source_filename = "<string>"
target triple = "x86_64-pc-windows-msvc"

define range(i64 0, 261890) i64 @PsIsWin32KFilterEnabled() local_unnamed_addr {
lift_init:
  %gs_val = tail call i64 @__nt_read_gs_i64(i64 392)
  %.25 = add i64 %gs_val, 184
  %.26 = inttoptr i64 %.25 to ptr
  %mem_ld = load i64, ptr %.26, align 4
  %.28 = add i64 %mem_ld, 2088
  %.29 = inttoptr i64 %.28 to ptr
  %mem_ld.1 = load i32, ptr %.29, align 4
  %.34 = lshr i32 %mem_ld.1, 14
  %.535 = and i32 %.34, 261889
  %.53 = zext nneg i32 %.535 to i64
  ret i64 %.53
}

declare i64 @__nt_read_gs_i64(i64) local_unnamed_addr


; ModuleID = '<string>'
source_filename = "<string>"
target triple = "x86_64-pc-windows-msvc"

define i64 @PsEnterPriorityRegion() local_unnamed_addr {
lift_init:
  %gs_val = tail call i64 @__nt_read_gs_i64(i64 392)
  %call_res = tail call i64 @PsBoostThreadIoEx(i64 %gs_val, i64 0, i64 0, i64 0)
  ret i64 %call_res
}

declare i64 @__nt_read_gs_i64(i64) local_unnamed_addr

declare i64 @PsBoostThreadIoEx(i64, i64, i64, i64) local_unnamed_addr


; ModuleID = '<string>'
source_filename = "<string>"
target triple = "x86_64-pc-windows-msvc"

define i64 @PsGetCurrentThreadWin32ThreadAndEnterCriticalRegion(i64 %arg_rcx) local_unnamed_addr {
lift_init:
  %gs_val = tail call i64 @__nt_read_gs_i64(i64 392)
  %.26 = add i64 %gs_val, 1592
  %.27 = inttoptr i64 %.26 to ptr
  %mem_ld = load i64, ptr %.27, align 4
  %.29 = inttoptr i64 %arg_rcx to ptr
  store i64 %mem_ld, ptr %.29, align 4
  %.31 = add i64 %gs_val, 484
  %.32 = inttoptr i64 %.31 to ptr
  %mem_ld.1 = load i16, ptr %.32, align 2
  %.34 = add i16 %mem_ld.1, -1
  store i16 %.34, ptr %.32, align 2
  %.51 = add i64 %gs_val, 456
  %.52 = inttoptr i64 %.51 to ptr
  %mem_ld.2 = load i64, ptr %.52, align 4
  ret i64 %mem_ld.2
}

declare i64 @__nt_read_gs_i64(i64) local_unnamed_addr


; ModuleID = '<string>'
source_filename = "<string>"
target triple = "x86_64-pc-windows-msvc"

; Function Attrs: mustprogress nofree norecurse nosync nounwind willreturn memory(none)
define noundef i64 @MmMapUserAddressesToPage() local_unnamed_addr #0 {
lift_init:
  ret i64 3221225659
}

attributes #0 = { mustprogress nofree norecurse nosync nounwind willreturn memory(none) }


; ModuleID = '<string>'
source_filename = "<string>"
target triple = "x86_64-pc-windows-msvc"

; Function Attrs: mustprogress nofree norecurse nosync nounwind willreturn memory(none)
define noundef i64 @MmQuerySystemSize() local_unnamed_addr #0 {
lift_init:
  ret i64 2
}

attributes #0 = { mustprogress nofree norecurse nosync nounwind willreturn memory(none) }


; ModuleID = '<string>'
source_filename = "<string>"
target triple = "x86_64-pc-windows-msvc"

; Function Attrs: mustprogress nofree norecurse nosync nounwind willreturn memory(none)
define noundef i64 @MmSetBankedSection() local_unnamed_addr #0 {
lift_init:
  ret i64 3221225659
}

attributes #0 = { mustprogress nofree norecurse nosync nounwind willreturn memory(none) }


; ModuleID = '<string>'
source_filename = "<string>"
target triple = "x86_64-pc-windows-msvc"

; Function Attrs: nofree nosync nounwind memory(read, inaccessiblemem: none, target_mem0: none, target_mem1: none)
define range(i64 0, 256) i64 @MmIsThisAnNtAsSystem() local_unnamed_addr #0 {
lift_init:
  %addr___nt_data_1403f8130 = tail call i64 @__nt_sym_addr___nt_data_1403f8130()
  %.23 = inttoptr i64 %addr___nt_data_1403f8130 to ptr
  %mem_ld = load i8, ptr %.23, align 1
  %.26 = zext i8 %mem_ld to i64
  ret i64 %.26
}

; Function Attrs: nofree nosync nounwind memory(none)
declare i64 @__nt_sym_addr___nt_data_1403f8130() local_unnamed_addr #1

attributes #0 = { nofree nosync nounwind memory(read, inaccessiblemem: none, target_mem0: none, target_mem1: none) }
attributes #1 = { nofree nosync nounwind memory(none) }


; ModuleID = '<string>'
source_filename = "<string>"
target triple = "x86_64-pc-windows-msvc"

; Function Attrs: mustprogress nofree norecurse nosync nounwind willreturn memory(read, inaccessiblemem: none, target_mem0: none, target_mem1: none)
define range(i64 0, 2) i64 @MmIsDriverVerifying(i64 %arg_rcx) local_unnamed_addr #0 {
lift_init:
  %.24 = add i64 %arg_rcx, 40
  %.25 = inttoptr i64 %.24 to ptr
  %mem_ld = load i64, ptr %.25, align 4
  %.28 = icmp eq i64 %mem_ld, 0
  br i1 %.28, label %common.ret, label %bb_140113c09

common.ret:                                       ; preds = %lift_init, %bb_140113c09
  %common.ret.op = phi i64 [ %.61, %bb_140113c09 ], [ 0, %lift_init ]
  ret i64 %common.ret.op

bb_140113c09:                                     ; preds = %lift_init
  %.39 = add i64 %mem_ld, 104
  %.40 = inttoptr i64 %.39 to ptr
  %mem_ld.1 = load i32, ptr %.40, align 4
  %.45 = lshr i32 %mem_ld.1, 25
  %.54 = and i32 %.45, 1
  %.61 = zext nneg i32 %.54 to i64
  br label %common.ret
}

attributes #0 = { mustprogress nofree norecurse nosync nounwind willreturn memory(read, inaccessiblemem: none, target_mem0: none, target_mem1: none) }


; ModuleID = '<string>'
source_filename = "<string>"
target triple = "x86_64-pc-windows-msvc"

; Function Attrs: nofree nosync nounwind memory(readwrite, inaccessiblemem: none, target_mem0: none, target_mem1: none)
define range(i64 0, 3221225660) i64 @MmIsVerifierEnabled(i64 %arg_rcx) local_unnamed_addr #0 {
lift_init:
  %addr_ViVerifierDriverAddedThunkListHead = tail call i64 @__nt_sym_addr_ViVerifierDriverAddedThunkListHead()
  %.24 = inttoptr i64 %addr_ViVerifierDriverAddedThunkListHead to ptr
  %mem_ld = load i64, ptr %.24, align 4
  %.26.not = icmp eq i64 %mem_ld, 0
  br i1 %.26.not, label %common.ret, label %bb_14079b230

common.ret:                                       ; preds = %lift_init, %bb_14079b230
  %mem_ld.2.sink = phi i32 [ %mem_ld.2, %bb_14079b230 ], [ 0, %lift_init ]
  %common.ret.op = phi i64 [ 0, %bb_14079b230 ], [ 3221225659, %lift_init ]
  %.62 = inttoptr i64 %arg_rcx to ptr
  store i32 %mem_ld.2.sink, ptr %.62, align 4
  ret i64 %common.ret.op

bb_14079b230:                                     ; preds = %lift_init
  %addr_MmVerifierData = tail call i64 @__nt_sym_addr_MmVerifierData()
  %.59 = inttoptr i64 %addr_MmVerifierData to ptr
  %mem_ld.2 = load i32, ptr %.59, align 4
  br label %common.ret
}

; Function Attrs: nofree nosync nounwind memory(none)
declare i64 @__nt_sym_addr_ViVerifierDriverAddedThunkListHead() local_unnamed_addr #1

; Function Attrs: nofree nosync nounwind memory(none)
declare i64 @__nt_sym_addr_MmVerifierData() local_unnamed_addr #1

attributes #0 = { nofree nosync nounwind memory(readwrite, inaccessiblemem: none, target_mem0: none, target_mem1: none) }
attributes #1 = { nofree nosync nounwind memory(none) }


; ModuleID = '<string>'
source_filename = "<string>"
target triple = "x86_64-pc-windows-msvc"

define range(i64 0, 2) i64 @MmIsRecursiveIoFault(i64 %arg_rcx) local_unnamed_addr {
lift_init:
  %gs_val = tail call i64 @__nt_read_gs_i64(i64 392)
  %.34 = add i64 %gs_val, 1757
  %.35 = inttoptr i64 %.34 to ptr
  %mem_ld = load i8, ptr %.35, align 1
  %.37.not = icmp eq i8 %mem_ld, 0
  br i1 %.37.not, label %bb_1400a9593, label %bb_1400a959c

bb_1400a9593:                                     ; preds = %lift_init
  %.55 = add i64 %gs_val, 1756
  %.56 = inttoptr i64 %.55 to ptr
  %mem_ld.1 = load i8, ptr %.56, align 1
  %.58.not = icmp eq i8 %mem_ld.1, 1
  br i1 %.58.not, label %bb_1400a959c, label %bb_1400a959e

bb_1400a959c:                                     ; preds = %bb_1400a9593, %lift_init
  br label %bb_1400a959e

bb_1400a959e:                                     ; preds = %bb_1400a959c, %bb_1400a9593
  %reg_rax.0 = phi i64 [ 1, %bb_1400a959c ], [ 0, %bb_1400a9593 ]
  ret i64 %reg_rax.0
}

declare i64 @__nt_read_gs_i64(i64) local_unnamed_addr


; ModuleID = '<string>'
source_filename = "<string>"
target triple = "x86_64-pc-windows-msvc"

define i64 @MmProbeAndLockPages(i64 %arg_rcx, i64 %arg_rdx, i64 %arg_r8) local_unnamed_addr {
lift_init:
  %r_r8d = trunc i64 %arg_r8 to i32
  %.41 = icmp ne i32 %r_r8d, 0
  %r_dl = trunc i64 %arg_rdx to i8
  %.59 = zext i1 %.41 to i64
  %.62 = icmp ne i8 %r_dl, 0
  %.78 = zext i1 %.62 to i64
  %call_res = tail call i64 @MiProbeAndLockPages(i64 %arg_rcx, i64 %.78, i64 %.59, i64 %.59)
  ret i64 %call_res
}

declare i64 @MiProbeAndLockPages(i64, i64, i64, i64) local_unnamed_addr


; ModuleID = '<string>'
source_filename = "<string>"
target triple = "x86_64-pc-windows-msvc"

define i64 @MmMapViewInSystemSpaceEx(i64 %arg_rcx, i64 %arg_rdx, i64 %arg_r8, i64 %arg_r9) local_unnamed_addr {
lift_init:
  %addr___nt_rodata_1403802b8 = tail call i64 @__nt_sym_addr___nt_rodata_1403802b8()
  %call_res = tail call i64 @MiMapViewInSystemSpace(i64 %arg_rcx, i64 %addr___nt_rodata_1403802b8, i64 %arg_rdx, i64 %arg_r8)
  ret i64 %call_res
}

; Function Attrs: nofree nosync nounwind memory(none)
declare i64 @__nt_sym_addr___nt_rodata_1403802b8() local_unnamed_addr #0

declare i64 @MiMapViewInSystemSpace(i64, i64, i64, i64) local_unnamed_addr

attributes #0 = { nofree nosync nounwind memory(none) }


; ModuleID = '<string>'
source_filename = "<string>"
target triple = "x86_64-pc-windows-msvc"

define i64 @MmLoadSystemImage(i64 %arg_rcx, i64 %arg_rdx, i64 %arg_r8, i64 %arg_r9) local_unnamed_addr {
lift_init:
  %.4110 = and i64 %arg_r9, 2147483644
  %.42 = icmp eq i64 %.4110, 0
  br i1 %.42, label %bb_1406d9294, label %bb_1406d92ad

bb_1406d9294:                                     ; preds = %lift_init
  %call_res = tail call i64 @MmLoadSystemImageEx(i64 %arg_rcx, i64 %arg_rdx, i64 %arg_r8, i64 %arg_r9)
  br label %bb_1406d92ad

bb_1406d92ad:                                     ; preds = %lift_init, %bb_1406d9294
  %reg_rax.0 = phi i64 [ %call_res, %bb_1406d9294 ], [ 3221225714, %lift_init ]
  ret i64 %reg_rax.0
}

declare i64 @MmLoadSystemImageEx(i64, i64, i64, i64) local_unnamed_addr


; ModuleID = '<string>'
source_filename = "<string>"
target triple = "x86_64-pc-windows-msvc"

define i64 @MmMapViewInSystemSpace(i64 %arg_rcx, i64 %arg_rdx, i64 %arg_r8) local_unnamed_addr {
lift_init:
  %stack_frame = alloca [1024 x i8], align 16
  %.26 = getelementptr inbounds nuw i8, ptr %stack_frame, i64 768
  %.27 = ptrtoint ptr %.26 to i64
  %.52 = add i64 %.27, -24
  %.53 = inttoptr i64 %.52 to ptr
  store i64 0, ptr %.53, align 8
  %.65 = add i64 %.27, -32
  %.66 = inttoptr i64 %.65 to ptr
  store i64 0, ptr %.66, align 16
  %addr___nt_rodata_1403802b8 = tail call i64 @__nt_sym_addr___nt_rodata_1403802b8()
  %.78 = add i64 %.27, 32
  %.79 = inttoptr i64 %.78 to ptr
  store i64 0, ptr %.79, align 16
  %.83 = add i64 %.27, -40
  %.84 = inttoptr i64 %.83 to ptr
  store i64 %.78, ptr %.84, align 8
  %call_res = call i64 @MiMapViewInSystemSpace(i64 %arg_rcx, i64 %addr___nt_rodata_1403802b8, i64 %arg_rdx, i64 %arg_r8)
  ret i64 %call_res
}

; Function Attrs: nofree nosync nounwind memory(none)
declare i64 @__nt_sym_addr___nt_rodata_1403802b8() local_unnamed_addr #0

declare i64 @MiMapViewInSystemSpace(i64, i64, i64, i64) local_unnamed_addr

attributes #0 = { nofree nosync nounwind memory(none) }


; ModuleID = '<string>'
source_filename = "<string>"
target triple = "x86_64-pc-windows-msvc"

define noundef i64 @MmUnloadSystemImage(i64 %arg_rcx) local_unnamed_addr {
lift_init:
  %call_res = tail call i64 @MmAcquireLoadLock(i64 %arg_rcx, i64 0, i64 0, i64 0)
  %call_res.1 = tail call i64 @MiUnloadSystemImage(i64 %arg_rcx, i64 4294967295, i64 0, i64 0)
  %call_res.2 = tail call i64 @MmReleaseLoadLock(i64 %call_res, i64 4294967295, i64 0, i64 0)
  ret i64 0
}

declare i64 @MmAcquireLoadLock(i64, i64, i64, i64) local_unnamed_addr

declare i64 @MiUnloadSystemImage(i64, i64, i64, i64) local_unnamed_addr

declare i64 @MmReleaseLoadLock(i64, i64, i64, i64) local_unnamed_addr


; ModuleID = '<string>'
source_filename = "<string>"
target triple = "x86_64-pc-windows-msvc"

define i64 @MmAllocateContiguousMemory(i64 %arg_rcx, i64 %arg_rdx) local_unnamed_addr {
lift_init:
  %.41 = lshr i64 %arg_rdx, 12
  %addr___nt_data_140380458 = tail call i64 @__nt_sym_addr___nt_data_140380458()
  %.49 = inttoptr i64 %addr___nt_data_140380458 to ptr
  %mem_ld = load i64, ptr %.49, align 4
  %.75 = tail call i64 @llvm.umin.i64(i64 %.41, i64 %mem_ld)
  %call_res = tail call i64 @MiAllocateContiguousMemory(i64 %arg_rcx, i64 0, i64 %.75, i64 0)
  ret i64 %call_res
}

; Function Attrs: nofree nosync nounwind memory(none)
declare i64 @__nt_sym_addr___nt_data_140380458() local_unnamed_addr #0

declare i64 @MiAllocateContiguousMemory(i64, i64, i64, i64) local_unnamed_addr

; Function Attrs: nocallback nocreateundeforpoison nofree nosync nounwind speculatable willreturn memory(none)
declare i64 @llvm.umin.i64(i64, i64) #1

attributes #0 = { nofree nosync nounwind memory(none) }
attributes #1 = { nocallback nocreateundeforpoison nofree nosync nounwind speculatable willreturn memory(none) }


; ModuleID = '<string>'
source_filename = "<string>"
target triple = "x86_64-pc-windows-msvc"

; Function Attrs: mustprogress nofree norecurse nosync nounwind willreturn memory(read, inaccessiblemem: none, target_mem0: none, target_mem1: none)
define range(i64 -140737488355328, 140737488359423) i64 @MmGetVirtualForPhysical(i64 %arg_rcx) local_unnamed_addr #0 {
lift_init:
  %.27 = lshr i64 %arg_rcx, 12
  %.53 = mul nuw nsw i64 %.27, 48
  %.54 = add nsw i64 %.53, -6047313952760
  %.55 = inttoptr i64 %.54 to ptr
  %mem_ld = load i64, ptr %.55, align 8
  %.59 = shl i64 %mem_ld, 25
  %.85 = and i64 %arg_rcx, 4095
  %.103 = ashr exact i64 %.59, 16
  %.111 = add nsw i64 %.103, %.85
  ret i64 %.111
}

attributes #0 = { mustprogress nofree norecurse nosync nounwind willreturn memory(read, inaccessiblemem: none, target_mem0: none, target_mem1: none) }


; ModuleID = '<string>'
source_filename = "<string>"
target triple = "x86_64-pc-windows-msvc"

define i64 @MmAddPhysicalMemory(i64 %arg_rcx, i64 %arg_rdx) local_unnamed_addr {
lift_init:
  %stack_frame = alloca [1024 x i8], align 16
  %.39 = inttoptr i64 %arg_rcx to ptr
  %mem_ld = load i64, ptr %.39, align 4
  %r_eax = trunc i64 %mem_ld to i32
  %.49 = and i32 %r_eax, 4095
  %.50.not = icmp eq i32 %.49, 0
  %.57 = getelementptr inbounds nuw i8, ptr %stack_frame, i64 776
  store i64 %mem_ld, ptr %.57, align 8
  %0 = and i64 %mem_ld, 1
  %.9219 = icmp eq i64 %0, 0
  %.92 = or i1 %.9219, %.50.not
  br i1 %.92, label %bb_1406d74f2, label %bb_1406d74e3

bb_1406d74e3:                                     ; preds = %lift_init
  %.103 = and i32 %r_eax, -2
  store i32 %.103, ptr %.57, align 8
  br label %bb_1406d74f2

bb_1406d74f2:                                     ; preds = %bb_1406d74e3, %lift_init
  %reg_r9.0 = phi i64 [ 0, %lift_init ], [ 256, %bb_1406d74e3 ]
  %addr_MiSystemPartition = tail call i64 @__nt_sym_addr_MiSystemPartition()
  %.123 = ptrtoint ptr %.57 to i64
  %call_res = call i64 @MiAddPhysicalMemoryChunks(i64 %addr_MiSystemPartition, i64 %.123, i64 %arg_rdx, i64 %reg_r9.0)
  ret i64 %call_res
}

; Function Attrs: nofree nosync nounwind memory(none)
declare i64 @__nt_sym_addr_MiSystemPartition() local_unnamed_addr #0

declare i64 @MiAddPhysicalMemoryChunks(i64, i64, i64, i64) local_unnamed_addr

attributes #0 = { nofree nosync nounwind memory(none) }


; ModuleID = '<string>'
source_filename = "<string>"
target triple = "x86_64-pc-windows-msvc"

define i64 @MmAllocatePagesForMdl(i64 %arg_rcx, i64 %arg_rdx, i64 %arg_r8, i64 %arg_r9) local_unnamed_addr {
lift_init:
  %gs_val = tail call i64 @__nt_read_gs_i64(i64 392)
  %addr_KiProcessorBlock = tail call i64 @__nt_sym_addr_KiProcessorBlock()
  %addr_MiSystemPartition = tail call i64 @__nt_sym_addr_MiSystemPartition()
  %call_res = tail call i64 @MiAllocatePagesForMdl(i64 %addr_MiSystemPartition, i64 %arg_rcx, i64 %arg_rdx, i64 %arg_r8)
  ret i64 %call_res
}

declare i64 @__nt_read_gs_i64(i64) local_unnamed_addr

; Function Attrs: nofree nosync nounwind memory(none)
declare i64 @__nt_sym_addr_KiProcessorBlock() local_unnamed_addr #0

; Function Attrs: nofree nosync nounwind memory(none)
declare i64 @__nt_sym_addr_MiSystemPartition() local_unnamed_addr #0

declare i64 @MiAllocatePagesForMdl(i64, i64, i64, i64) local_unnamed_addr

attributes #0 = { nofree nosync nounwind memory(none) }


; ModuleID = '<string>'
source_filename = "<string>"
target triple = "x86_64-pc-windows-msvc"

; Function Attrs: mustprogress nofree norecurse nosync nounwind willreturn memory(readwrite, inaccessiblemem: none, target_mem0: none, target_mem1: none)
define i64 @ObReferenceSecurityDescriptor(i64 %arg_rcx, i64 %arg_rdx) local_unnamed_addr #0 {
lift_init:
  %.25 = and i64 %arg_rdx, 4294967295
  %.27 = add i64 %arg_rcx, -16
  %.28 = inttoptr i64 %.27 to ptr
  %mem_ld = load i64, ptr %.28, align 4
  %.29 = add i64 %mem_ld, %.25
  store i64 %.29, ptr %.28, align 4
  ret i64 %mem_ld
}

attributes #0 = { mustprogress nofree norecurse nosync nounwind willreturn memory(readwrite, inaccessiblemem: none, target_mem0: none, target_mem1: none) }


; ModuleID = '<string>'
source_filename = "<string>"
target triple = "x86_64-pc-windows-msvc"

define i64 @ObCreateObjectType(i64 %arg_rcx, i64 %arg_rdx, i64 %arg_r8, i64 %arg_r9) local_unnamed_addr {
lift_init:
  %call_res = tail call i64 @ObCreateObjectTypeEx(i64 %arg_rcx, i64 %arg_rdx, i64 %arg_r8, i64 0)
  ret i64 %call_res
}

declare i64 @ObCreateObjectTypeEx(i64, i64, i64, i64) local_unnamed_addr


; ModuleID = '<string>'
source_filename = "<string>"
target triple = "x86_64-pc-windows-msvc"

define i64 @ObpLookupDirectoryEntry(i64 %arg_rcx, i64 %arg_rdx, i64 %arg_r8, i64 %arg_r9) local_unnamed_addr {
lift_init:
  %call_res = tail call i64 @ObpLookupDirectoryEntryEx(i64 %arg_rcx, i64 %arg_rdx, i64 %arg_r8, i64 0)
  ret i64 %call_res
}

declare i64 @ObpLookupDirectoryEntryEx(i64, i64, i64, i64) local_unnamed_addr


; ModuleID = '<string>'
source_filename = "<string>"
target triple = "x86_64-pc-windows-msvc"

define i64 @ObpIncrPointerCount(i64 %arg_rcx, i64 %arg_rdx) local_unnamed_addr {
lift_init:
  %.41 = inttoptr i64 %arg_rcx to ptr
  %mem_ld = load i64, ptr %.41, align 4
  %.42 = add i64 %mem_ld, 1
  store i64 %.42, ptr %.41, align 4
  %.75 = icmp eq i64 %mem_ld, 0
  %0 = or i64 %.42, %mem_ld
  %.91 = icmp slt i64 %0, 0
  %.92 = or i1 %.75, %.91
  br i1 %.92, label %bb_14017858e, label %common.ret

common.ret:                                       ; preds = %lift_init, %bb_14017858e
  %common.ret.op = phi i64 [ %call_res, %bb_14017858e ], [ %.42, %lift_init ]
  ret i64 %common.ret.op

bb_14017858e:                                     ; preds = %lift_init
  %.123 = add i64 %arg_rcx, 48
  %call_res = tail call i64 @KeBugCheckEx(i64 24, i64 0, i64 %.123, i64 16)
  br label %common.ret
}

declare i64 @KeBugCheckEx(i64, i64, i64, i64) local_unnamed_addr


; ModuleID = '<string>'
source_filename = "<string>"
target triple = "x86_64-pc-windows-msvc"

define i64 @ObpIncrPointerCountEx(i64 %arg_rcx, i64 %arg_rdx) local_unnamed_addr {
lift_init:
  %0 = shl i64 %arg_rdx, 32
  %.39 = ashr exact i64 %0, 32
  %.42 = inttoptr i64 %arg_rcx to ptr
  %mem_ld = load i64, ptr %.42, align 4
  %.43 = add i64 %mem_ld, %.39
  store i64 %.43, ptr %.42, align 4
  %.71 = icmp slt i64 %mem_ld, 1
  br i1 %.71, label %bb_140194b14, label %common.ret

common.ret:                                       ; preds = %lift_init, %bb_140194b14
  %common.ret.op = phi i64 [ %call_res, %bb_140194b14 ], [ %.43, %lift_init ]
  ret i64 %common.ret.op

bb_140194b14:                                     ; preds = %lift_init
  %.126 = add i64 %arg_rcx, 48
  %call_res = tail call i64 @KeBugCheckEx(i64 24, i64 0, i64 %.126, i64 16)
  br label %common.ret
}

declare i64 @KeBugCheckEx(i64, i64, i64, i64) local_unnamed_addr


; ModuleID = '<string>'
source_filename = "<string>"
target triple = "x86_64-pc-windows-msvc"

define i64 @ObpDirectoryTeardownCallback(i64 %arg_rcx) local_unnamed_addr {
lift_init:
  %.44 = inttoptr i64 %arg_rcx to ptr
  %mem_ld = load i64, ptr %.44, align 4
  %call_res = tail call i64 @ObMakeTemporaryObject(i64 %mem_ld, i64 0, i64 0, i64 0)
  %mem_ld.1 = load i64, ptr %.44, align 4
  %call_res.1 = tail call i64 @ObfDereferenceObjectWithTag(i64 %mem_ld.1, i64 1918132815)
  ret i64 %call_res.1
}

declare i64 @ObMakeTemporaryObject(i64, i64, i64, i64) local_unnamed_addr

declare i64 @ObfDereferenceObjectWithTag(i64, i64) local_unnamed_addr


; ModuleID = '<string>'
source_filename = "<string>"
target triple = "x86_64-pc-windows-msvc"

define noundef i64 @ObDeassignSecurity(i64 %arg_rcx) local_unnamed_addr {
lift_init:
  %.39 = inttoptr i64 %arg_rcx to ptr
  %mem_ld = load i64, ptr %.39, align 4
  %r_ecx = trunc i64 %mem_ld to i32
  %.43 = and i64 %mem_ld, -16
  %.51 = and i32 %r_ecx, 15
  %.61 = add nuw nsw i32 %.51, 1
  %.75 = zext nneg i32 %.61 to i64
  store i64 0, ptr %.39, align 4
  %call_res = tail call i64 @ObDereferenceSecurityDescriptor(i64 %.43, i64 %.75, i64 0, i64 0)
  ret i64 0
}

declare i64 @ObDereferenceSecurityDescriptor(i64, i64, i64, i64) local_unnamed_addr


; ModuleID = '<string>'
source_filename = "<string>"
target triple = "x86_64-pc-windows-msvc"

define range(i64 0, 256) i64 @ObpReferenceNamedObject(i64 %arg_rcx) local_unnamed_addr {
lift_init:
  %.46 = add i64 %arg_rcx, 48
  %call_res = tail call i64 @ObfReferenceObjectWithTag(i64 %.46, i64 1953261124, i64 0, i64 0)
  %.61 = add i64 %arg_rcx, 26
  %.62 = inttoptr i64 %.61 to ptr
  %mem_ld = load i8, ptr %.62, align 1
  %addr_ObpInfoMaskToOffset = tail call i64 @__nt_sym_addr_ObpInfoMaskToOffset()
  %0 = and i8 %mem_ld, 3
  %.74 = zext nneg i8 %0 to i64
  %.76 = add i64 %addr_ObpInfoMaskToOffset, %.74
  %.77 = inttoptr i64 %.76 to ptr
  %mem_ld.1 = load i8, ptr %.77, align 1
  %.79 = zext i8 %mem_ld.1 to i64
  %.81 = add i64 %arg_rcx, 24
  %.95 = sub i64 %.81, %.79
  %.96 = inttoptr i64 %.95 to ptr
  %mem_ld.2 = load i32, ptr %.96, align 4
  %.98 = add i32 %mem_ld.2, 1
  store i32 %.98, ptr %.96, align 4
  ret i64 %.79
}

declare i64 @ObfReferenceObjectWithTag(i64, i64, i64, i64) local_unnamed_addr

; Function Attrs: nofree nosync nounwind memory(none)
declare i64 @__nt_sym_addr_ObpInfoMaskToOffset() local_unnamed_addr #0

attributes #0 = { nofree nosync nounwind memory(none) }


; ModuleID = '<string>'
source_filename = "<string>"
target triple = "x86_64-pc-windows-msvc"

; Function Attrs: mustprogress nofree norecurse nosync nounwind willreturn memory(read, inaccessiblemem: none, target_mem0: none, target_mem1: none)
define range(i64 0, 2) i64 @IoIsFileOriginRemote(i64 %arg_rcx) local_unnamed_addr #0 {
lift_init:
  %.24 = add i64 %arg_rcx, 83
  %.25 = inttoptr i64 %.24 to ptr
  %mem_ld = load i8, ptr %.25, align 1
  %.31 = and i8 %mem_ld, 1
  %.40 = zext nneg i8 %.31 to i64
  ret i64 %.40
}

attributes #0 = { mustprogress nofree norecurse nosync nounwind willreturn memory(read, inaccessiblemem: none, target_mem0: none, target_mem1: none) }


; ModuleID = '<string>'
source_filename = "<string>"
target triple = "x86_64-pc-windows-msvc"

; Function Attrs: mustprogress nofree norecurse nosync nounwind willreturn memory(none)
define noundef i64 @IoSizeofGenericIrpExtension() local_unnamed_addr #0 {
lift_init:
  ret i64 4
}

attributes #0 = { mustprogress nofree norecurse nosync nounwind willreturn memory(none) }


; ModuleID = '<string>'
source_filename = "<string>"
target triple = "x86_64-pc-windows-msvc"

; Function Attrs: mustprogress nofree norecurse nosync nounwind willreturn memory(none)
define noundef i64 @IoSizeofWorkItem() local_unnamed_addr #0 {
lift_init:
  ret i64 88
}

attributes #0 = { mustprogress nofree norecurse nosync nounwind willreturn memory(none) }


; ModuleID = '<string>'
source_filename = "<string>"
target triple = "x86_64-pc-windows-msvc"

; Function Attrs: nofree nosync nounwind memory(none)
define i64 @IoGetConfigurationInformation() local_unnamed_addr #0 {
lift_init:
  %addr___nt_rodata_1407ea710 = tail call i64 @__nt_sym_addr___nt_rodata_1407ea710()
  ret i64 %addr___nt_rodata_1407ea710
}

; Function Attrs: nofree nosync nounwind memory(none)
declare i64 @__nt_sym_addr___nt_rodata_1407ea710() local_unnamed_addr #0

attributes #0 = { nofree nosync nounwind memory(none) }


; ModuleID = '<string>'
source_filename = "<string>"
target triple = "x86_64-pc-windows-msvc"

; Function Attrs: mustprogress nofree norecurse nosync nounwind willreturn memory(read, inaccessiblemem: none, target_mem0: none, target_mem1: none)
define i64 @IoGetDeviceToVerify(i64 %arg_rcx) local_unnamed_addr #0 {
lift_init:
  %.24 = add i64 %arg_rcx, 1672
  %.25 = inttoptr i64 %.24 to ptr
  %mem_ld = load i64, ptr %.25, align 4
  ret i64 %mem_ld
}

attributes #0 = { mustprogress nofree norecurse nosync nounwind willreturn memory(read, inaccessiblemem: none, target_mem0: none, target_mem1: none) }


; ModuleID = '<string>'
source_filename = "<string>"
target triple = "x86_64-pc-windows-msvc"

; Function Attrs: nofree nosync nounwind memory(none)
define i64 @IoGetFileObjectGenericMapping() local_unnamed_addr #0 {
lift_init:
  %addr___nt_rodata_1407835d0 = tail call i64 @__nt_sym_addr___nt_rodata_1407835d0()
  ret i64 %addr___nt_rodata_1407835d0
}

; Function Attrs: nofree nosync nounwind memory(none)
declare i64 @__nt_sym_addr___nt_rodata_1407835d0() local_unnamed_addr #0

attributes #0 = { nofree nosync nounwind memory(none) }


; ModuleID = '<string>'
source_filename = "<string>"
target triple = "x86_64-pc-windows-msvc"

; Function Attrs: mustprogress nofree norecurse nosync nounwind willreturn memory(write, inaccessiblemem: none, target_mem0: none, target_mem1: none)
define noundef i64 @IoSetDeviceToVerify(i64 %arg_rcx, i64 %arg_rdx) local_unnamed_addr #0 {
lift_init:
  %.25 = add i64 %arg_rcx, 1672
  %.26 = inttoptr i64 %.25 to ptr
  store i64 %arg_rdx, ptr %.26, align 4
  ret i64 0
}

attributes #0 = { mustprogress nofree norecurse nosync nounwind willreturn memory(write, inaccessiblemem: none, target_mem0: none, target_mem1: none) }


; ModuleID = '<string>'
source_filename = "<string>"
target triple = "x86_64-pc-windows-msvc"

; Function Attrs: mustprogress nofree norecurse nosync nounwind willreturn memory(readwrite, inaccessiblemem: none, target_mem0: none, target_mem1: none)
define range(i64 0, 4294967296) i64 @IoClearIrpExtraCreateParameter(i64 %arg_rcx) local_unnamed_addr #0 {
lift_init:
  %.24 = add i64 %arg_rcx, 16
  %.25 = inttoptr i64 %.24 to ptr
  %mem_ld = load i32, ptr %.25, align 4
  %0 = and i32 %mem_ld, 128
  %.31.not = icmp eq i32 %0, 0
  br i1 %.31.not, label %bb_1401f274c, label %bb_1401f2747

bb_1401f2747:                                     ; preds = %lift_init
  %.41 = add i64 %arg_rcx, 112
  %.42 = inttoptr i64 %.41 to ptr
  store i64 0, ptr %.42, align 4
  br label %bb_1401f274c

bb_1401f274c:                                     ; preds = %bb_1401f2747, %lift_init
  %.26 = zext i32 %mem_ld to i64
  ret i64 %.26
}

attributes #0 = { mustprogress nofree norecurse nosync nounwind willreturn memory(readwrite, inaccessiblemem: none, target_mem0: none, target_mem1: none) }


; ModuleID = '<string>'
source_filename = "<string>"
target triple = "x86_64-pc-windows-msvc"

define i64 @IoGetInitialStack() local_unnamed_addr {
lift_init:
  %gs_val = tail call i64 @__nt_read_gs_i64(i64 392)
  %.25 = add i64 %gs_val, 40
  %.26 = inttoptr i64 %.25 to ptr
  %mem_ld = load i64, ptr %.26, align 4
  ret i64 %mem_ld
}

declare i64 @__nt_read_gs_i64(i64) local_unnamed_addr


; ModuleID = '<string>'
source_filename = "<string>"
target triple = "x86_64-pc-windows-msvc"

define i64 @IoClearActivityIdThread(i64 %arg_rcx) local_unnamed_addr {
lift_init:
  %gs_val = tail call i64 @__nt_read_gs_i64(i64 392)
  %.26 = add i64 %gs_val, 1880
  %.27 = inttoptr i64 %.26 to ptr
  store i64 %arg_rcx, ptr %.27, align 4
  ret i64 %gs_val
}

declare i64 @__nt_read_gs_i64(i64) local_unnamed_addr


; ModuleID = '<string>'
source_filename = "<string>"
target triple = "x86_64-pc-windows-msvc"

define i64 @IoGetActivityIdThread() local_unnamed_addr {
lift_init:
  %gs_val = tail call i64 @__nt_read_gs_i64(i64 392)
  %.25 = add i64 %gs_val, 1880
  %.26 = inttoptr i64 %.25 to ptr
  %mem_ld = load i64, ptr %.26, align 4
  ret i64 %mem_ld
}

declare i64 @__nt_read_gs_i64(i64) local_unnamed_addr


; ModuleID = '<string>'
source_filename = "<string>"
target triple = "x86_64-pc-windows-msvc"

define i64 @IoGetTopLevelIrp() local_unnamed_addr {
lift_init:
  %gs_val = tail call i64 @__nt_read_gs_i64(i64 392)
  %.25 = add i64 %gs_val, 1664
  %.26 = inttoptr i64 %.25 to ptr
  %mem_ld = load i64, ptr %.26, align 4
  ret i64 %mem_ld
}

declare i64 @__nt_read_gs_i64(i64) local_unnamed_addr


; ModuleID = '<string>'
source_filename = "<string>"
target triple = "x86_64-pc-windows-msvc"

define i64 @IoSetTopLevelIrp(i64 %arg_rcx) local_unnamed_addr {
lift_init:
  %gs_val = tail call i64 @__nt_read_gs_i64(i64 392)
  %.26 = add i64 %gs_val, 1664
  %.27 = inttoptr i64 %.26 to ptr
  store i64 %arg_rcx, ptr %.27, align 4
  ret i64 %gs_val
}

declare i64 @__nt_read_gs_i64(i64) local_unnamed_addr


; ModuleID = '<string>'
source_filename = "<string>"
target triple = "x86_64-pc-windows-msvc"

; Function Attrs: mustprogress nofree norecurse nosync nounwind willreturn memory(none)
define range(i64 0, 2) i64 @IoIsWdmVersionAvailable(i64 %arg_rcx, i64 %arg_rdx) local_unnamed_addr #0 {
lift_init:
  %r_cl = trunc i64 %arg_rcx to i8
  %.30 = icmp ult i8 %r_cl, 6
  br i1 %.30, label %bb_1406b65d0, label %bb_1406b65c5

bb_1406b65c5:                                     ; preds = %lift_init
  %.52.not = icmp eq i8 %r_cl, 6
  %r_dl = trunc i64 %arg_rdx to i8
  %.71.not = icmp eq i8 %r_dl, 0
  %or.cond = select i1 %.52.not, i1 %.71.not, i1 false
  br i1 %or.cond, label %bb_1406b65d0, label %bb_1406b65d2

bb_1406b65d0:                                     ; preds = %bb_1406b65c5, %lift_init
  br label %bb_1406b65d2

bb_1406b65d2:                                     ; preds = %bb_1406b65d0, %bb_1406b65c5
  %reg_rax.1 = phi i64 [ 1, %bb_1406b65d0 ], [ 0, %bb_1406b65c5 ]
  ret i64 %reg_rax.1
}

attributes #0 = { mustprogress nofree norecurse nosync nounwind willreturn memory(none) }


; ModuleID = '<string>'
source_filename = "<string>"
target triple = "x86_64-pc-windows-msvc"

; Function Attrs: mustprogress nofree norecurse nosync nounwind willreturn memory(write, inaccessiblemem: none, target_mem0: none, target_mem1: none)
define noundef i64 @IoInitializeMiniCompletionPacket(i64 %arg_rcx, i64 %arg_rdx, i64 %arg_r8) local_unnamed_addr #0 {
lift_init:
  %.26 = add i64 %arg_rcx, 16
  %.27 = inttoptr i64 %.26 to ptr
  store i32 4, ptr %.27, align 4
  %.29 = add i64 %arg_rcx, 56
  %.30 = inttoptr i64 %.29 to ptr
  store i64 %arg_rdx, ptr %.30, align 4
  %.32 = add i64 %arg_rcx, 64
  %.33 = inttoptr i64 %.32 to ptr
  store i64 %arg_r8, ptr %.33, align 4
  %.35 = add i64 %arg_rcx, 72
  %.36 = inttoptr i64 %.35 to ptr
  store i8 0, ptr %.36, align 1
  ret i64 0
}

attributes #0 = { mustprogress nofree norecurse nosync nounwind willreturn memory(write, inaccessiblemem: none, target_mem0: none, target_mem1: none) }


; ModuleID = '<string>'
source_filename = "<string>"
target triple = "x86_64-pc-windows-msvc"

define i64 @IoPropagateIrpExtension(i64 %arg_rcx, i64 %arg_rdx, i64 %arg_r8) local_unnamed_addr {
lift_init:
  %.40 = and i64 %arg_r8, 4294967295
  %call_res = tail call i64 @IoPropagateIrpExtensionEx(i64 %arg_rcx, i64 %arg_rdx, i64 0, i64 %.40)
  ret i64 %call_res
}

declare i64 @IoPropagateIrpExtensionEx(i64, i64, i64, i64) local_unnamed_addr


; ModuleID = '<string>'
source_filename = "<string>"
target triple = "x86_64-pc-windows-msvc"

; Function Attrs: mustprogress nofree norecurse nosync nounwind willreturn memory(readwrite, inaccessiblemem: none, target_mem0: none, target_mem1: none)
define i64 @IoSetHardErrorOrVerifyDevice(i64 %arg_rcx, i64 %arg_rdx) local_unnamed_addr #0 {
lift_init:
  %.25 = add i64 %arg_rcx, 152
  %.26 = inttoptr i64 %.25 to ptr
  %mem_ld = load i64, ptr %.26, align 4
  %.29 = icmp eq i64 %mem_ld, 0
  br i1 %.29, label %bb_1401f2f83, label %bb_1401f2f7c

bb_1401f2f7c:                                     ; preds = %lift_init
  %.40 = add i64 %mem_ld, 1672
  %.41 = inttoptr i64 %.40 to ptr
  store i64 %arg_rdx, ptr %.41, align 4
  br label %bb_1401f2f83

bb_1401f2f83:                                     ; preds = %bb_1401f2f7c, %lift_init
  ret i64 %mem_ld
}

attributes #0 = { mustprogress nofree norecurse nosync nounwind willreturn memory(readwrite, inaccessiblemem: none, target_mem0: none, target_mem1: none) }


; ModuleID = '<string>'
source_filename = "<string>"
target triple = "x86_64-pc-windows-msvc"

define i64 @IoSetActivityIdThread(i64 %arg_rcx) local_unnamed_addr {
lift_init:
  %gs_val = tail call i64 @__nt_read_gs_i64(i64 392)
  %.26 = add i64 %gs_val, 1880
  %.27 = inttoptr i64 %.26 to ptr
  %mem_ld = load i64, ptr %.27, align 4
  store i64 %arg_rcx, ptr %.27, align 4
  ret i64 %mem_ld
}

declare i64 @__nt_read_gs_i64(i64) local_unnamed_addr


; ModuleID = '<string>'
source_filename = "<string>"
target triple = "x86_64-pc-windows-msvc"

define i64 @IoQueueWorkItemEx(i64 %arg_rcx, i64 %arg_rdx, i64 %arg_r8, i64 %arg_r9) local_unnamed_addr {
lift_init:
  %.46 = and i64 %arg_r8, 4294967295
  %call_res = tail call i64 @IopQueueWorkItemProlog(i64 %arg_rcx, i64 %arg_rdx, i64 %arg_r9, i64 %arg_r9)
  %call_res.1 = tail call i64 @ExQueueWorkItemFromIo(i64 %call_res, i64 %.46, i64 %arg_r9, i64 %arg_r9)
  ret i64 %call_res.1
}

declare i64 @IopQueueWorkItemProlog(i64, i64, i64, i64) local_unnamed_addr

declare i64 @ExQueueWorkItemFromIo(i64, i64, i64, i64) local_unnamed_addr


; ModuleID = '<string>'
source_filename = "<string>"
target triple = "x86_64-pc-windows-msvc"

; Function Attrs: mustprogress nofree norecurse nosync nounwind willreturn memory(read, inaccessiblemem: none, target_mem0: none, target_mem1: none)
define range(i64 0, 3) i64 @IoGetPagingIoPriority(i64 %arg_rcx) local_unnamed_addr #0 {
lift_init:
  %.24 = add i64 %arg_rcx, 16
  %.25 = inttoptr i64 %.24 to ptr
  %mem_ld = load i32, ptr %.25, align 4
  %.287 = and i32 %mem_ld, 2
  %.29 = icmp eq i32 %.287, 0
  %0 = and i32 %mem_ld, 917504
  %cond = icmp eq i32 %0, 655360
  %spec.select = select i1 %cond, i64 2, i64 1
  %common.ret.op = select i1 %.29, i64 0, i64 %spec.select
  ret i64 %common.ret.op
}

attributes #0 = { mustprogress nofree norecurse nosync nounwind willreturn memory(read, inaccessiblemem: none, target_mem0: none, target_mem1: none) }


; ModuleID = '<string>'
source_filename = "<string>"
target triple = "x86_64-pc-windows-msvc"

; Function Attrs: mustprogress nofree norecurse nosync nounwind willreturn memory(readwrite, inaccessiblemem: none, target_mem0: none, target_mem1: none)
define range(i64 0, 3221225486) i64 @IoSetIoPriorityHint(i64 %arg_rcx, i64 %arg_rdx) local_unnamed_addr #0 {
lift_init:
  %r_edx = trunc i64 %arg_rdx to i32
  %.30 = icmp ugt i32 %r_edx, 4
  br i1 %.30, label %common.ret, label %bb_1400f7955

common.ret:                                       ; preds = %lift_init, %bb_1400f7955
  %common.ret.op = phi i64 [ 0, %bb_1400f7955 ], [ 3221225485, %lift_init ]
  ret i64 %common.ret.op

bb_1400f7955:                                     ; preds = %lift_init
  %.44 = add i64 %arg_rcx, 16
  %.45 = inttoptr i64 %.44 to ptr
  %mem_ld = load i32, ptr %.45, align 4
  %.50 = shl nuw nsw i32 %r_edx, 17
  %.59 = and i32 %mem_ld, -917505
  %.68 = add nuw nsw i32 %.50, 131072
  %.83 = or i32 %.59, %.68
  store i32 %.83, ptr %.45, align 4
  br label %common.ret
}

attributes #0 = { mustprogress nofree norecurse nosync nounwind willreturn memory(readwrite, inaccessiblemem: none, target_mem0: none, target_mem1: none) }


; ModuleID = '<string>'
source_filename = "<string>"
target triple = "x86_64-pc-windows-msvc"

define i64 @IoQueryFileInformation(i64 %arg_rcx, i64 %arg_rdx, i64 %arg_r8, i64 %arg_r9) local_unnamed_addr {
lift_init:
  %call_res = tail call i64 @IopQueryXxxInformation(i64 %arg_rcx, i64 %arg_rdx, i64 %arg_r8, i64 0)
  ret i64 %call_res
}

declare i64 @IopQueryXxxInformation(i64, i64, i64, i64) local_unnamed_addr


; ModuleID = '<string>'
source_filename = "<string>"
target triple = "x86_64-pc-windows-msvc"

define i64 @IoQueryVolumeInformation(i64 %arg_rcx, i64 %arg_rdx, i64 %arg_r8, i64 %arg_r9) local_unnamed_addr {
lift_init:
  %call_res = tail call i64 @IopQueryXxxInformation(i64 %arg_rcx, i64 %arg_rdx, i64 %arg_r8, i64 0)
  ret i64 %call_res
}

declare i64 @IopQueryXxxInformation(i64, i64, i64, i64) local_unnamed_addr


; ModuleID = '<string>'
source_filename = "<string>"
target triple = "x86_64-pc-windows-msvc"

define i64 @IoQueueWorkItem(i64 %arg_rcx, i64 %arg_rdx, i64 %arg_r8, i64 %arg_r9) local_unnamed_addr {
lift_init:
  %.46 = add i64 %arg_rcx, 64
  %.47 = inttoptr i64 %.46 to ptr
  store i32 0, ptr %.47, align 4
  %.58 = and i64 %arg_r8, 4294967295
  %call_res = tail call i64 @IopQueueWorkItemProlog(i64 %arg_rcx, i64 %arg_rdx, i64 %arg_r9, i64 %arg_r9)
  %call_res.1 = tail call i64 @ExQueueWorkItemFromIo(i64 %call_res, i64 %.58, i64 %arg_r9, i64 %arg_r9)
  ret i64 %call_res.1
}

declare i64 @IopQueueWorkItemProlog(i64, i64, i64, i64) local_unnamed_addr

declare i64 @ExQueueWorkItemFromIo(i64, i64, i64, i64) local_unnamed_addr


; ModuleID = '<string>'
source_filename = "<string>"
target triple = "x86_64-pc-windows-msvc"

; Function Attrs: mustprogress nofree norecurse nosync nounwind willreturn memory(read, inaccessiblemem: none, target_mem0: none, target_mem1: none)
define range(i64 0, 3221226022) i64 @IoGetActivityIdIrp(i64 %arg_rcx, i64 %arg_rdx) local_unnamed_addr #0 {
lift_init:
  %.25 = add i64 %arg_rcx, 200
  %.26 = inttoptr i64 %.25 to ptr
  %mem_ld = load i64, ptr %.26, align 4
  %.28 = add i64 %arg_rcx, 71
  %.29 = inttoptr i64 %.28 to ptr
  %mem_ld.1 = load i8, ptr %.29, align 1
  %.33 = icmp slt i8 %mem_ld.1, 0
  %.50 = icmp eq i64 %mem_ld, 0
  %or.cond = select i1 %.33, i1 true, i1 %.50
  br i1 %or.cond, label %common.ret, label %bb_1400e8882

bb_1400e8882:                                     ; preds = %lift_init
  %.61 = add i64 %mem_ld, 2
  %.62 = inttoptr i64 %.61 to ptr
  %mem_ld.2 = load i8, ptr %.62, align 1
  %.63 = and i8 %mem_ld.2, 1
  %.64.not = icmp eq i8 %.63, 0
  %spec.select = select i1 %.64.not, i64 3221226021, i64 0
  br label %common.ret

common.ret:                                       ; preds = %bb_1400e8882, %lift_init
  %common.ret.op = phi i64 [ 3221226021, %lift_init ], [ %spec.select, %bb_1400e8882 ]
  ret i64 %common.ret.op
}

attributes #0 = { mustprogress nofree norecurse nosync nounwind willreturn memory(read, inaccessiblemem: none, target_mem0: none, target_mem1: none) }


; ModuleID = '<string>'
source_filename = "<string>"
target triple = "x86_64-pc-windows-msvc"

; Function Attrs: mustprogress nofree norecurse nosync nounwind willreturn memory(readwrite, inaccessiblemem: none, target_mem0: none, target_mem1: none)
define range(i64 0, 3221226022) i64 @IoGetIoAttributionHandle(i64 %arg_rcx, i64 %arg_rdx) local_unnamed_addr #0 {
lift_init:
  %.25 = add i64 %arg_rcx, 200
  %.26 = inttoptr i64 %.25 to ptr
  %mem_ld = load i64, ptr %.26, align 4
  %.28 = add i64 %arg_rcx, 71
  %.29 = inttoptr i64 %.28 to ptr
  %mem_ld.1 = load i8, ptr %.29, align 1
  %.33 = icmp slt i8 %mem_ld.1, 0
  %.50 = icmp eq i64 %mem_ld, 0
  %or.cond = select i1 %.33, i1 true, i1 %.50
  br i1 %or.cond, label %common.ret, label %bb_1400f1262

bb_1400f1262:                                     ; preds = %lift_init
  %.61 = add i64 %mem_ld, 2
  %.62 = inttoptr i64 %.61 to ptr
  %mem_ld.2 = load i8, ptr %.62, align 1
  %.63 = and i8 %mem_ld.2, 64
  %.64.not = icmp eq i8 %.63, 0
  br i1 %.64.not, label %common.ret, label %bb_1400f126e

common.ret:                                       ; preds = %lift_init, %bb_1400f1262, %bb_1400f126e
  %common.ret.op = phi i64 [ 0, %bb_1400f126e ], [ 3221226021, %bb_1400f1262 ], [ 3221226021, %lift_init ]
  ret i64 %common.ret.op

bb_1400f126e:                                     ; preds = %bb_1400f1262
  %.84 = add i64 %mem_ld, 16
  %.85 = inttoptr i64 %.84 to ptr
  %mem_ld.4 = load i64, ptr %.85, align 4
  %.95 = inttoptr i64 %arg_rdx to ptr
  store i64 %mem_ld.4, ptr %.95, align 4
  br label %common.ret
}

attributes #0 = { mustprogress nofree norecurse nosync nounwind willreturn memory(readwrite, inaccessiblemem: none, target_mem0: none, target_mem1: none) }


; ModuleID = '<string>'
source_filename = "<string>"
target triple = "x86_64-pc-windows-msvc"

; Function Attrs: mustprogress nofree norecurse nosync nounwind willreturn memory(read, inaccessiblemem: none, target_mem0: none, target_mem1: none)
define range(i64 0, 4294967296) i64 @IoSizeOfIrpEx(i64 %arg_rcx, i64 %arg_rdx) local_unnamed_addr #0 {
lift_init:
  %extract.t = trunc i64 %arg_rdx to i8
  switch i64 %arg_rcx, label %bb_1401f304b [
    i64 0, label %bb_1401f3056
    i64 -1, label %bb_1401f3054
  ]

bb_1401f304b:                                     ; preds = %lift_init
  %.55 = add i64 %arg_rcx, 48
  %.56 = inttoptr i64 %.55 to ptr
  %mem_ld = load i32, ptr %.56, align 4
  %.57 = and i32 %mem_ld, 134217728
  %.58 = icmp eq i32 %.57, 0
  br i1 %.58, label %bb_1401f3056, label %bb_1401f3054

bb_1401f3054:                                     ; preds = %lift_init, %bb_1401f304b
  %.70 = add i8 %extract.t, 1
  br label %bb_1401f3056

bb_1401f3056:                                     ; preds = %lift_init, %bb_1401f3054, %bb_1401f304b
  %reg_rdx.0.off0 = phi i8 [ %extract.t, %lift_init ], [ %.70, %bb_1401f3054 ], [ %extract.t, %bb_1401f304b ]
  %.90 = sext i8 %reg_rdx.0.off0 to i32
  %r_ax = sext i8 %reg_rdx.0.off0 to i16
  %.132 = mul nsw i16 %r_ax, 72
  %.144 = add nsw i16 %.132, 208
  %0 = and i32 %.90, -65536
  %1 = zext i16 %.144 to i32
  %.16021 = or disjoint i32 %0, %1
  %.160 = zext i32 %.16021 to i64
  ret i64 %.160
}

attributes #0 = { mustprogress nofree norecurse nosync nounwind willreturn memory(read, inaccessiblemem: none, target_mem0: none, target_mem1: none) }


; ModuleID = '<string>'
source_filename = "<string>"
target triple = "x86_64-pc-windows-msvc"

; Function Attrs: mustprogress nofree norecurse nosync nounwind willreturn memory(none)
define noundef i64 @ExCleanupRundownProtectionCacheAware() local_unnamed_addr #0 {
lift_init:
  ret i64 0
}

attributes #0 = { mustprogress nofree norecurse nosync nounwind willreturn memory(none) }


; ModuleID = '<string>'
source_filename = "<string>"
target triple = "x86_64-pc-windows-msvc"

; Function Attrs: mustprogress nofree norecurse nosync nounwind willreturn memory(none)
define noundef i64 @ExSizeOfAutoExpandPushLock() local_unnamed_addr #0 {
lift_init:
  ret i64 16
}

attributes #0 = { mustprogress nofree norecurse nosync nounwind willreturn memory(none) }


; ModuleID = '<string>'
source_filename = "<string>"
target triple = "x86_64-pc-windows-msvc"

; Function Attrs: nofree nosync nounwind memory(read, inaccessiblemem: none, target_mem0: none, target_mem1: none)
define range(i64 0, 4294967296) i64 @ExGetFirmwareType() local_unnamed_addr #0 {
lift_init:
  %addr___nt_data_140356610 = tail call i64 @__nt_sym_addr___nt_data_140356610()
  %.23 = inttoptr i64 %addr___nt_data_140356610 to ptr
  %mem_ld = load i32, ptr %.23, align 4
  %.24 = zext i32 %mem_ld to i64
  ret i64 %.24
}

; Function Attrs: nofree nosync nounwind memory(none)
declare i64 @__nt_sym_addr___nt_data_140356610() local_unnamed_addr #1

attributes #0 = { nofree nosync nounwind memory(read, inaccessiblemem: none, target_mem0: none, target_mem1: none) }
attributes #1 = { nofree nosync nounwind memory(none) }


; ModuleID = '<string>'
source_filename = "<string>"
target triple = "x86_64-pc-windows-msvc"

; Function Attrs: mustprogress nofree norecurse nosync nounwind willreturn memory(readwrite, inaccessiblemem: none, target_mem0: none, target_mem1: none)
define i64 @ExRundownCompleted(i64 %arg_rcx) local_unnamed_addr #0 {
lift_init:
  %.26 = inttoptr i64 %arg_rcx to ptr
  %mem_ld = load i64, ptr %.26, align 4
  store i64 1, ptr %.26, align 4
  ret i64 %mem_ld
}

attributes #0 = { mustprogress nofree norecurse nosync nounwind willreturn memory(readwrite, inaccessiblemem: none, target_mem0: none, target_mem1: none) }


; ModuleID = '<string>'
source_filename = "<string>"
target triple = "x86_64-pc-windows-msvc"

; Function Attrs: nofree nosync nounwind memory(read, inaccessiblemem: none, target_mem0: none, target_mem1: none)
define range(i64 0, 2) i64 @ExIsManufacturingModeEnabled() local_unnamed_addr #0 {
lift_init:
  %addr_ExpManufacturingInformation = tail call i64 @__nt_sym_addr_ExpManufacturingInformation()
  %.23 = inttoptr i64 %addr_ExpManufacturingInformation to ptr
  %mem_ld = load i32, ptr %.23, align 4
  %.26 = and i32 %mem_ld, 1
  %.33 = zext nneg i32 %.26 to i64
  ret i64 %.33
}

; Function Attrs: nofree nosync nounwind memory(none)
declare i64 @__nt_sym_addr_ExpManufacturingInformation() local_unnamed_addr #1

attributes #0 = { nofree nosync nounwind memory(read, inaccessiblemem: none, target_mem0: none, target_mem1: none) }
attributes #1 = { nofree nosync nounwind memory(none) }


; ModuleID = '<string>'
source_filename = "<string>"
target triple = "x86_64-pc-windows-msvc"

; Function Attrs: nofree nosync nounwind memory(read, inaccessiblemem: none, target_mem0: none, target_mem1: none)
define range(i64 0, 2) i64 @ExRealTimeIsUniversal() local_unnamed_addr #0 {
lift_init:
  %addr_ExpRealTimeIsUniversal = tail call i64 @__nt_sym_addr_ExpRealTimeIsUniversal()
  %.23 = inttoptr i64 %addr_ExpRealTimeIsUniversal to ptr
  %mem_ld = load i32, ptr %.23, align 4
  %.25 = icmp ne i32 %mem_ld, 0
  %.45 = zext i1 %.25 to i64
  ret i64 %.45
}

; Function Attrs: nofree nosync nounwind memory(none)
declare i64 @__nt_sym_addr_ExpRealTimeIsUniversal() local_unnamed_addr #1

attributes #0 = { nofree nosync nounwind memory(read, inaccessiblemem: none, target_mem0: none, target_mem1: none) }
attributes #1 = { nofree nosync nounwind memory(none) }


; ModuleID = '<string>'
source_filename = "<string>"
target triple = "x86_64-pc-windows-msvc"

; Function Attrs: nofree nosync nounwind memory(read, inaccessiblemem: none, target_mem0: none, target_mem1: none)
define range(i64 0, 2) i64 @ExIsSoftBoot() local_unnamed_addr #0 {
lift_init:
  %addr___nt_data_140356618 = tail call i64 @__nt_sym_addr___nt_data_140356618()
  %.23 = inttoptr i64 %addr___nt_data_140356618 to ptr
  %mem_ld = load i8, ptr %.23, align 1
  %.30 = lshr i8 %mem_ld, 2
  %.42 = and i8 %.30, 1
  %.51 = zext nneg i8 %.42 to i64
  ret i64 %.51
}

; Function Attrs: nofree nosync nounwind memory(none)
declare i64 @__nt_sym_addr___nt_data_140356618() local_unnamed_addr #1

attributes #0 = { nofree nosync nounwind memory(read, inaccessiblemem: none, target_mem0: none, target_mem1: none) }
attributes #1 = { nofree nosync nounwind memory(none) }


; ModuleID = '<string>'
source_filename = "<string>"
target triple = "x86_64-pc-windows-msvc"

; Function Attrs: mustprogress nofree norecurse nosync nounwind willreturn memory(readwrite, inaccessiblemem: none, target_mem0: none, target_mem1: none)
define i64 @ExConvertPushLockExclusiveToShared(i64 %arg_rcx) local_unnamed_addr #0 {
lift_init:
  %.30 = inttoptr i64 %arg_rcx to ptr
  %mem_ld = load i64, ptr %.30, align 4
  %.44 = icmp eq i64 %mem_ld, 1
  %.45 = select i1 %.44, i64 17, i64 %mem_ld
  store i64 %.45, ptr %.30, align 4
  ret i64 %mem_ld
}

attributes #0 = { mustprogress nofree norecurse nosync nounwind willreturn memory(readwrite, inaccessiblemem: none, target_mem0: none, target_mem1: none) }


; ModuleID = '<string>'
source_filename = "<string>"
target triple = "x86_64-pc-windows-msvc"

; Function Attrs: nofree nosync nounwind memory(readwrite, inaccessiblemem: none, target_mem0: none, target_mem1: none)
define i64 @ExSystemTimeToLocalTime(i64 %arg_rcx, i64 %arg_rdx) local_unnamed_addr #0 {
lift_init:
  %.25 = inttoptr i64 %arg_rcx to ptr
  %mem_ld = load i64, ptr %.25, align 4
  %addr_ExpTimeZoneBias = tail call i64 @__nt_sym_addr_ExpTimeZoneBias()
  %.27 = inttoptr i64 %addr_ExpTimeZoneBias to ptr
  %mem_ld.1 = load i64, ptr %.27, align 4
  %.28 = sub i64 %mem_ld, %mem_ld.1
  %.42 = inttoptr i64 %arg_rdx to ptr
  store i64 %.28, ptr %.42, align 4
  ret i64 %.28
}

; Function Attrs: nofree nosync nounwind memory(none)
declare i64 @__nt_sym_addr_ExpTimeZoneBias() local_unnamed_addr #1

attributes #0 = { nofree nosync nounwind memory(readwrite, inaccessiblemem: none, target_mem0: none, target_mem1: none) }
attributes #1 = { nofree nosync nounwind memory(none) }


; ModuleID = '<string>'
source_filename = "<string>"
target triple = "x86_64-pc-windows-msvc"

define range(i64 0, 256) i64 @ExGetPreviousMode() local_unnamed_addr {
lift_init:
  %gs_val = tail call i64 @__nt_read_gs_i64(i64 392)
  %.25 = add i64 %gs_val, 562
  %.26 = inttoptr i64 %.25 to ptr
  %mem_ld = load i8, ptr %.26, align 1
  %.28 = zext i8 %mem_ld to i64
  ret i64 %.28
}

declare i64 @__nt_read_gs_i64(i64) local_unnamed_addr


; ModuleID = '<string>'
source_filename = "<string>"
target triple = "x86_64-pc-windows-msvc"

; Function Attrs: nofree nosync nounwind memory(readwrite, inaccessiblemem: none, target_mem0: none, target_mem1: none)
define noundef i64 @ExLocalTimeToSystemTime(i64 %arg_rcx, i64 %arg_rdx) local_unnamed_addr #0 {
lift_init:
  %.25 = inttoptr i64 %arg_rcx to ptr
  %mem_ld = load i64, ptr %.25, align 4
  %addr_ExpTimeZoneBias = tail call i64 @__nt_sym_addr_ExpTimeZoneBias()
  %.27 = inttoptr i64 %addr_ExpTimeZoneBias to ptr
  %mem_ld.1 = load i64, ptr %.27, align 4
  %.29 = add i64 %mem_ld.1, %mem_ld
  %.43 = inttoptr i64 %arg_rdx to ptr
  store i64 %.29, ptr %.43, align 4
  ret i64 0
}

; Function Attrs: nofree nosync nounwind memory(none)
declare i64 @__nt_sym_addr_ExpTimeZoneBias() local_unnamed_addr #1

attributes #0 = { nofree nosync nounwind memory(readwrite, inaccessiblemem: none, target_mem0: none, target_mem1: none) }
attributes #1 = { nofree nosync nounwind memory(none) }


; ModuleID = '<string>'
source_filename = "<string>"
target triple = "x86_64-pc-windows-msvc"

define range(i64 0, 2) i64 @ExSystemExceptionFilter() local_unnamed_addr {
lift_init:
  %gs_val = tail call i64 @__nt_read_gs_i64(i64 392)
  %.33 = add i64 %gs_val, 562
  %.34 = inttoptr i64 %.33 to ptr
  %mem_ld = load i8, ptr %.34, align 1
  %.36 = icmp ne i8 %mem_ld, 0
  %.56 = zext i1 %.36 to i64
  ret i64 %.56
}

declare i64 @__nt_read_gs_i64(i64) local_unnamed_addr


; ModuleID = '<string>'
source_filename = "<string>"
target triple = "x86_64-pc-windows-msvc"

; Function Attrs: mustprogress nofree norecurse nosync nounwind willreturn memory(readwrite, inaccessiblemem: none, target_mem0: none, target_mem1: none)
define noundef i64 @ExInitializeAutoExpandPushLock(i64 %arg_rcx, i64 %arg_rdx) local_unnamed_addr #0 {
lift_init:
  %.33 = inttoptr i64 %arg_rcx to ptr
  %.35 = add i64 %arg_rcx, 8
  %.36 = inttoptr i64 %.35 to ptr
  store i64 0, ptr %.36, align 4
  store i64 0, ptr %.33, align 4
  %.4010 = and i64 %arg_rdx, 1
  %.41 = icmp eq i64 %.4010, 0
  br i1 %.41, label %bb_1400fb862, label %common.ret

common.ret:                                       ; preds = %lift_init, %bb_1400fb862
  ret i64 0

bb_1400fb862:                                     ; preds = %lift_init
  %mem_ld = load i32, ptr %.36, align 4
  %.57 = or i32 %mem_ld, 4
  store i32 %.57, ptr %.36, align 4
  br label %common.ret
}

attributes #0 = { mustprogress nofree norecurse nosync nounwind willreturn memory(readwrite, inaccessiblemem: none, target_mem0: none, target_mem1: none) }


; ModuleID = '<string>'
source_filename = "<string>"
target triple = "x86_64-pc-windows-msvc"

; Function Attrs: nofree nosync nounwind memory(readwrite, inaccessiblemem: none, target_mem0: none, target_mem1: none)
define range(i64 0, 4294967296) i64 @ExQueryTimerResolution(i64 %arg_rcx, i64 %arg_rdx, i64 %arg_r8) local_unnamed_addr #0 {
lift_init:
  %addr_KeMaximumIncrement = tail call i64 @__nt_sym_addr_KeMaximumIncrement()
  %.26 = inttoptr i64 %addr_KeMaximumIncrement to ptr
  %mem_ld = load i32, ptr %.26, align 4
  %.29 = inttoptr i64 %arg_rcx to ptr
  store i32 %mem_ld, ptr %.29, align 4
  %addr_KeMinimumIncrement = tail call i64 @__nt_sym_addr_KeMinimumIncrement()
  %.31 = inttoptr i64 %addr_KeMinimumIncrement to ptr
  %mem_ld.1 = load i32, ptr %.31, align 4
  %.34 = inttoptr i64 %arg_rdx to ptr
  store i32 %mem_ld.1, ptr %.34, align 4
  %addr_KeTimeIncrement = tail call i64 @__nt_sym_addr_KeTimeIncrement()
  %.36 = inttoptr i64 %addr_KeTimeIncrement to ptr
  %mem_ld.2 = load i32, ptr %.36, align 4
  %.37 = zext i32 %mem_ld.2 to i64
  %.39 = inttoptr i64 %arg_r8 to ptr
  store i32 %mem_ld.2, ptr %.39, align 4
  ret i64 %.37
}

; Function Attrs: nofree nosync nounwind memory(none)
declare i64 @__nt_sym_addr_KeMaximumIncrement() local_unnamed_addr #1

; Function Attrs: nofree nosync nounwind memory(none)
declare i64 @__nt_sym_addr_KeMinimumIncrement() local_unnamed_addr #1

; Function Attrs: nofree nosync nounwind memory(none)
declare i64 @__nt_sym_addr_KeTimeIncrement() local_unnamed_addr #1

attributes #0 = { nofree nosync nounwind memory(readwrite, inaccessiblemem: none, target_mem0: none, target_mem1: none) }
attributes #1 = { nofree nosync nounwind memory(none) }


; ModuleID = '<string>'
source_filename = "<string>"
target triple = "x86_64-pc-windows-msvc"

define i64 @ExAllocateTimer(i64 %arg_rcx, i64 %arg_rdx, i64 %arg_r8) local_unnamed_addr {
lift_init:
  %.4014 = and i64 %arg_r8, 2
  %.41.not = icmp eq i64 %.4014, 0
  br i1 %.41.not, label %bb_14011ad6e, label %bb_140182e34

common.ret:                                       ; preds = %bb_140182e34, %bb_14011ad6e
  %common.ret.op = phi i64 [ %call_res, %bb_14011ad6e ], [ %call_res.1, %bb_140182e34 ]
  ret i64 %common.ret.op

bb_14011ad6e:                                     ; preds = %lift_init
  %.53 = and i64 %arg_r8, 4294967293
  %call_res = tail call i64 @ExAllocateTimerInternal2(i64 %arg_rcx, i64 %arg_rdx, i64 0, i64 %.53)
  br label %common.ret

bb_140182e34:                                     ; preds = %lift_init
  %.97 = and i64 %arg_r8, 4294967295
  %call_res.1 = tail call i64 @KeBugCheckEx(i64 199, i64 9, i64 0, i64 %.97)
  br label %common.ret
}

declare i64 @ExAllocateTimerInternal2(i64, i64, i64, i64) local_unnamed_addr

declare i64 @KeBugCheckEx(i64, i64, i64, i64) local_unnamed_addr


; ModuleID = '<string>'
source_filename = "<string>"
target triple = "x86_64-pc-windows-msvc"

define i64 @ExTryConvertPushLockSharedToExclusiveEx(i64 %arg_rcx, i64 %arg_rdx) local_unnamed_addr {
lift_init:
  %0 = and i64 %arg_rdx, 4294967292
  %.40.not = icmp eq i64 %0, 0
  br i1 %.40.not, label %bb_14012eae0, label %bb_1401d20ba

common.ret:                                       ; preds = %bb_1401d20ba, %bb_14012eae0
  %common.ret.op = phi i64 [ %.86, %bb_14012eae0 ], [ %call_res, %bb_1401d20ba ]
  ret i64 %common.ret.op

bb_14012eae0:                                     ; preds = %lift_init
  %.58 = inttoptr i64 %arg_rcx to ptr
  %mem_ld = load i64, ptr %.58, align 4
  %.60 = icmp eq i64 %mem_ld, 17
  %.73 = select i1 %.60, i64 1, i64 %mem_ld
  store i64 %.73, ptr %.58, align 4
  %.84 = and i64 %mem_ld, -256
  %.85 = zext i1 %.60 to i64
  %.86 = or disjoint i64 %.84, %.85
  br label %common.ret

bb_1401d20ba:                                     ; preds = %lift_init
  %.122 = and i64 %arg_rdx, 4294967295
  %call_res = tail call i64 @KeBugCheckEx(i64 338, i64 %.122, i64 %arg_rcx, i64 0)
  br label %common.ret
}

declare i64 @KeBugCheckEx(i64, i64, i64, i64) local_unnamed_addr


; ModuleID = '<string>'
source_filename = "<string>"
target triple = "x86_64-pc-windows-msvc"

define i64 @ExTryQueueWorkItem(i64 %arg_rcx, i64 %arg_rdx) local_unnamed_addr {
lift_init:
  %addr_PspSystemPartition = tail call i64 @__nt_sym_addr_PspSystemPartition()
  %.39 = inttoptr i64 %addr_PspSystemPartition to ptr
  %mem_ld = load i64, ptr %.39, align 4
  %.42 = and i64 %arg_rdx, 4294967295
  %.53 = add i64 %mem_ld, 16
  %.54 = inttoptr i64 %.53 to ptr
  %mem_ld.1 = load i64, ptr %.54, align 4
  %call_res = tail call i64 @ExpTryQueueWorkItem(i64 %mem_ld.1, i64 %arg_rcx, i64 %.42, i64 0)
  ret i64 %call_res
}

; Function Attrs: nofree nosync nounwind memory(none)
declare i64 @__nt_sym_addr_PspSystemPartition() local_unnamed_addr #0

declare i64 @ExpTryQueueWorkItem(i64, i64, i64, i64) local_unnamed_addr

attributes #0 = { nofree nosync nounwind memory(none) }


; ModuleID = '<string>'
source_filename = "<string>"
target triple = "x86_64-pc-windows-msvc"

define range(i64 0, 4294967296) i64 @ExGetExclusiveWaiterCount(i64 %arg_rcx) local_unnamed_addr {
lift_init:
  %.38 = add i64 %arg_rcx, 26
  %.39 = inttoptr i64 %.38 to ptr
  %mem_ld = load i8, ptr %.39, align 1
  %.40 = and i8 %mem_ld, 1
  %.41 = icmp eq i8 %.40, 0
  br i1 %.41, label %bb_14027d425, label %bb_14027d40a

bb_14027d40a:                                     ; preds = %lift_init
  %call_res = tail call i64 @KeBugCheckEx(i64 454, i64 14, i64 %arg_rcx, i64 0)
  br label %bb_14027d425

bb_14027d425:                                     ; preds = %bb_14027d40a, %lift_init
  %reg_rcx.0 = phi i64 [ %arg_rcx, %lift_init ], [ 454, %bb_14027d40a ]
  %.83 = add i64 %reg_rcx.0, 76
  %.84 = inttoptr i64 %.83 to ptr
  %mem_ld.2 = load i32, ptr %.84, align 4
  %.85 = zext i32 %mem_ld.2 to i64
  ret i64 %.85
}

declare i64 @KeBugCheckEx(i64, i64, i64, i64) local_unnamed_addr


; ModuleID = '<string>'
source_filename = "<string>"
target triple = "x86_64-pc-windows-msvc"

define range(i64 0, 4294967296) i64 @ExGetSharedWaiterCount(i64 %arg_rcx) local_unnamed_addr {
lift_init:
  %.38 = add i64 %arg_rcx, 26
  %.39 = inttoptr i64 %.38 to ptr
  %mem_ld = load i8, ptr %.39, align 1
  %.40 = and i8 %mem_ld, 1
  %.41 = icmp eq i8 %.40, 0
  br i1 %.41, label %bb_14027d465, label %bb_14027d44a

bb_14027d44a:                                     ; preds = %lift_init
  %call_res = tail call i64 @KeBugCheckEx(i64 454, i64 14, i64 %arg_rcx, i64 0)
  br label %bb_14027d465

bb_14027d465:                                     ; preds = %bb_14027d44a, %lift_init
  %reg_rcx.0 = phi i64 [ %arg_rcx, %lift_init ], [ 454, %bb_14027d44a ]
  %.83 = add i64 %reg_rcx.0, 72
  %.84 = inttoptr i64 %.83 to ptr
  %mem_ld.2 = load i32, ptr %.84, align 4
  %.85 = zext i32 %mem_ld.2 to i64
  ret i64 %.85
}

declare i64 @KeBugCheckEx(i64, i64, i64, i64) local_unnamed_addr


; ModuleID = '<string>'
source_filename = "<string>"
target triple = "x86_64-pc-windows-msvc"

define range(i64 0, 4294967296) i64 @ExGetCurrentProcessorCounts(i64 %arg_rcx, i64 %arg_rdx, i64 %arg_r8) local_unnamed_addr {
lift_init:
  %gs_val = tail call i64 @__nt_read_gs_i64(i64 32)
  %.28 = add i64 %gs_val, 24
  %.29 = inttoptr i64 %.28 to ptr
  %mem_ld = load i64, ptr %.29, align 4
  %.31 = add i64 %mem_ld, 652
  %.32 = inttoptr i64 %.31 to ptr
  %mem_ld.1 = load i32, ptr %.32, align 4
  %.35 = inttoptr i64 %arg_rcx to ptr
  store i32 %mem_ld.1, ptr %.35, align 4
  %.37 = add i64 %gs_val, 23432
  %.38 = inttoptr i64 %.37 to ptr
  %mem_ld.2 = load i32, ptr %.38, align 4
  %.41 = add i64 %gs_val, 23428
  %.42 = inttoptr i64 %.41 to ptr
  %mem_ld.3 = load i32, ptr %.42, align 4
  %.43 = add i32 %mem_ld.3, %mem_ld.2
  %.58 = inttoptr i64 %arg_rdx to ptr
  store i32 %.43, ptr %.58, align 4
  %.60 = add i64 %gs_val, 36
  %.61 = inttoptr i64 %.60 to ptr
  %mem_ld.4 = load i32, ptr %.61, align 4
  %.62 = zext i32 %mem_ld.4 to i64
  %.64 = inttoptr i64 %arg_r8 to ptr
  store i32 %mem_ld.4, ptr %.64, align 4
  ret i64 %.62
}

declare i64 @__nt_read_gs_i64(i64) local_unnamed_addr


; ModuleID = '<string>'
source_filename = "<string>"
target triple = "x86_64-pc-windows-msvc"

define i64 @ExEnterCriticalRegionAndAcquireSharedWaitForExclusive() local_unnamed_addr {
lift_init:
  %gs_val = tail call i64 @__nt_read_gs_i64(i64 392)
  %.39 = add i64 %gs_val, 484
  %.40 = inttoptr i64 %.39 to ptr
  %mem_ld = load i16, ptr %.40, align 2
  %.42 = add i16 %mem_ld, -1
  store i16 %.42, ptr %.40, align 2
  %call_res = tail call i64 @ExAcquireSharedWaitForExclusive(i64 0, i64 1, i64 0, i64 0)
  %gs_val.1 = tail call i64 @__nt_read_gs_i64(i64 392)
  %.67 = add i64 %gs_val.1, 456
  %.68 = inttoptr i64 %.67 to ptr
  %mem_ld.1 = load i64, ptr %.68, align 4
  ret i64 %mem_ld.1
}

declare i64 @__nt_read_gs_i64(i64) local_unnamed_addr

declare i64 @ExAcquireSharedWaitForExclusive(i64, i64, i64, i64) local_unnamed_addr


; ModuleID = '<string>'
source_filename = "<string>"
target triple = "x86_64-pc-windows-msvc"

; Function Attrs: mustprogress nofree norecurse nosync nounwind willreturn memory(read, inaccessiblemem: none, target_mem0: none, target_mem1: none)
define range(i64 0, 4294967296) i64 @RtlNumberGenericTableElements(i64 %arg_rcx) local_unnamed_addr #0 {
lift_init:
  %.24 = add i64 %arg_rcx, 36
  %.25 = inttoptr i64 %.24 to ptr
  %mem_ld = load i32, ptr %.25, align 4
  %.26 = zext i32 %mem_ld to i64
  ret i64 %.26
}

attributes #0 = { mustprogress nofree norecurse nosync nounwind willreturn memory(read, inaccessiblemem: none, target_mem0: none, target_mem1: none) }


; ModuleID = '<string>'
source_filename = "<string>"
target triple = "x86_64-pc-windows-msvc"

; Function Attrs: mustprogress nofree norecurse nosync nounwind willreturn memory(read, inaccessiblemem: none, target_mem0: none, target_mem1: none)
define range(i64 0, 4294967296) i64 @RtlNumberGenericTableElementsAvl(i64 %arg_rcx) local_unnamed_addr #0 {
lift_init:
  %.24 = add i64 %arg_rcx, 44
  %.25 = inttoptr i64 %.24 to ptr
  %mem_ld = load i32, ptr %.25, align 4
  %.26 = zext i32 %mem_ld to i64
  ret i64 %.26
}

attributes #0 = { mustprogress nofree norecurse nosync nounwind willreturn memory(read, inaccessiblemem: none, target_mem0: none, target_mem1: none) }


; ModuleID = '<string>'
source_filename = "<string>"
target triple = "x86_64-pc-windows-msvc"

; Function Attrs: mustprogress nofree norecurse nosync nounwind willreturn memory(write, inaccessiblemem: none, target_mem0: none, target_mem1: none)
define noundef i64 @RtlRunOnceInitialize(i64 %arg_rcx) local_unnamed_addr #0 {
lift_init:
  %.24 = inttoptr i64 %arg_rcx to ptr
  store i64 0, ptr %.24, align 4
  ret i64 0
}

attributes #0 = { mustprogress nofree norecurse nosync nounwind willreturn memory(write, inaccessiblemem: none, target_mem0: none, target_mem1: none) }


; ModuleID = '<string>'
source_filename = "<string>"
target triple = "x86_64-pc-windows-msvc"

; Function Attrs: mustprogress nofree norecurse nosync nounwind willreturn memory(none)
define i64 @RtlSubAuthorityCountSid(i64 %arg_rcx) local_unnamed_addr #0 {
lift_init:
  %.24 = add i64 %arg_rcx, 1
  ret i64 %.24
}

attributes #0 = { mustprogress nofree norecurse nosync nounwind willreturn memory(none) }


; ModuleID = '<string>'
source_filename = "<string>"
target triple = "x86_64-pc-windows-msvc"

; Function Attrs: mustprogress nofree norecurse nosync nounwind willreturn memory(none)
define range(i64 0, 2) i64 @RtlAreAnyAccessesGranted(i64 %arg_rcx, i64 %arg_rdx) local_unnamed_addr #0 {
lift_init:
  %.252 = and i64 %arg_rdx, %arg_rcx
  %.25 = trunc i64 %.252 to i32
  %.26 = icmp ne i32 %.25, 0
  %.40 = zext i1 %.26 to i64
  ret i64 %.40
}

attributes #0 = { mustprogress nofree norecurse nosync nounwind willreturn memory(none) }


; ModuleID = '<string>'
source_filename = "<string>"
target triple = "x86_64-pc-windows-msvc"

; Function Attrs: mustprogress nofree norecurse nosync nounwind willreturn memory(none)
define range(i64 0, 2) i64 @RtlEqualWnfChangeStamps(i64 %arg_rcx, i64 %arg_rdx) local_unnamed_addr #0 {
lift_init:
  %r_ecx = trunc i64 %arg_rcx to i32
  %r_edx = trunc i64 %arg_rdx to i32
  %.26 = icmp eq i32 %r_ecx, %r_edx
  %.45 = zext i1 %.26 to i64
  ret i64 %.45
}

attributes #0 = { mustprogress nofree norecurse nosync nounwind willreturn memory(none) }


; ModuleID = '<string>'
source_filename = "<string>"
target triple = "x86_64-pc-windows-msvc"

; Function Attrs: mustprogress nofree norecurse nosync nounwind willreturn memory(none)
define noundef i64 @XIPDispatch() local_unnamed_addr #0 {
lift_init:
  ret i64 3221225486
}

attributes #0 = { mustprogress nofree norecurse nosync nounwind willreturn memory(none) }


; ModuleID = '<string>'
source_filename = "<string>"
target triple = "x86_64-pc-windows-msvc"

; Function Attrs: mustprogress nofree norecurse nosync nounwind willreturn memory(read, inaccessiblemem: none, target_mem0: none, target_mem1: none)
define range(i64 0, -15) i64 @FirstEntrySList(i64 %arg_rcx) local_unnamed_addr #0 {
lift_init:
  %.24 = add i64 %arg_rcx, 8
  %.25 = inttoptr i64 %.24 to ptr
  %mem_ld = load i64, ptr %.25, align 4
  %.37 = and i64 %mem_ld, -16
  ret i64 %.37
}

attributes #0 = { mustprogress nofree norecurse nosync nounwind willreturn memory(read, inaccessiblemem: none, target_mem0: none, target_mem1: none) }


; ModuleID = '<string>'
source_filename = "<string>"
target triple = "x86_64-pc-windows-msvc"

; Function Attrs: mustprogress nofree norecurse nosync nounwind willreturn memory(readwrite, inaccessiblemem: none, target_mem0: none, target_mem1: none)
define i64 @RtlCopyLuid(i64 %arg_rcx, i64 %arg_rdx) local_unnamed_addr #0 {
lift_init:
  %.25 = inttoptr i64 %arg_rdx to ptr
  %mem_ld = load i64, ptr %.25, align 4
  %.27 = inttoptr i64 %arg_rcx to ptr
  store i64 %mem_ld, ptr %.27, align 4
  ret i64 %mem_ld
}

attributes #0 = { mustprogress nofree norecurse nosync nounwind willreturn memory(readwrite, inaccessiblemem: none, target_mem0: none, target_mem1: none) }


; ModuleID = '<string>'
source_filename = "<string>"
target triple = "x86_64-pc-windows-msvc"

; Function Attrs: nofree nosync nounwind memory(read, inaccessiblemem: none, target_mem0: none, target_mem1: none)
define range(i64 0, 4294967296) i64 @RtlGetNtGlobalFlags() local_unnamed_addr #0 {
lift_init:
  %addr_NtGlobalFlag = tail call i64 @__nt_sym_addr_NtGlobalFlag()
  %.23 = inttoptr i64 %addr_NtGlobalFlag to ptr
  %mem_ld = load i32, ptr %.23, align 4
  %.24 = zext i32 %mem_ld to i64
  ret i64 %.24
}

; Function Attrs: nofree nosync nounwind memory(none)
declare i64 @__nt_sym_addr_NtGlobalFlag() local_unnamed_addr #1

attributes #0 = { nofree nosync nounwind memory(read, inaccessiblemem: none, target_mem0: none, target_mem1: none) }
attributes #1 = { nofree nosync nounwind memory(none) }


; ModuleID = '<string>'
source_filename = "<string>"
target triple = "x86_64-pc-windows-msvc"

; Function Attrs: nofree nosync nounwind memory(write, inaccessiblemem: none, target_mem0: none, target_mem1: none)
define noundef i64 @srand(i64 %arg_rcx) local_unnamed_addr #0 {
lift_init:
  %r_ecx = trunc i64 %arg_rcx to i32
  %addr___nt_data_14034e510 = tail call i64 @__nt_sym_addr___nt_data_14034e510()
  %.24 = inttoptr i64 %addr___nt_data_14034e510 to ptr
  store i32 %r_ecx, ptr %.24, align 4
  ret i64 0
}

; Function Attrs: nofree nosync nounwind memory(none)
declare i64 @__nt_sym_addr___nt_data_14034e510() local_unnamed_addr #1

attributes #0 = { nofree nosync nounwind memory(write, inaccessiblemem: none, target_mem0: none, target_mem1: none) }
attributes #1 = { nofree nosync nounwind memory(none) }


; ModuleID = '<string>'
source_filename = "<string>"
target triple = "x86_64-pc-windows-msvc"

; Function Attrs: mustprogress nofree norecurse nosync nounwind willreturn memory(none)
define range(i64 0, 2) i64 @RtlAreAllAccessesGranted(i64 %arg_rcx, i64 %arg_rdx) local_unnamed_addr #0 {
lift_init:
  %r_ecx = trunc i64 %arg_rcx to i32
  %.25 = xor i32 %r_ecx, -1
  %r_edx = trunc i64 %arg_rdx to i32
  %.28 = and i32 %r_edx, %.25
  %.29 = icmp eq i32 %.28, 0
  %.42 = zext i1 %.29 to i64
  ret i64 %.42
}

attributes #0 = { mustprogress nofree norecurse nosync nounwind willreturn memory(none) }


; ModuleID = '<string>'
source_filename = "<string>"
target triple = "x86_64-pc-windows-msvc"

; Function Attrs: mustprogress nofree norecurse nosync nounwind willreturn memory(write, inaccessiblemem: none, target_mem0: none, target_mem1: none)
define noundef i64 @RtlInitializeBitMapEx(i64 %arg_rcx, i64 %arg_rdx, i64 %arg_r8) local_unnamed_addr #0 {
lift_init:
  %.26 = inttoptr i64 %arg_rcx to ptr
  store i64 %arg_r8, ptr %.26, align 4
  %.28 = add i64 %arg_rcx, 8
  %.29 = inttoptr i64 %.28 to ptr
  store i64 %arg_rdx, ptr %.29, align 4
  ret i64 0
}

attributes #0 = { mustprogress nofree norecurse nosync nounwind willreturn memory(write, inaccessiblemem: none, target_mem0: none, target_mem1: none) }


; ModuleID = '<string>'
source_filename = "<string>"
target triple = "x86_64-pc-windows-msvc"

; Function Attrs: mustprogress nofree norecurse nosync nounwind willreturn memory(read, inaccessiblemem: none, target_mem0: none, target_mem1: none)
define range(i64 0, 2) i64 @RtlIsGenericTableEmpty(i64 %arg_rcx) local_unnamed_addr #0 {
lift_init:
  %.24 = inttoptr i64 %arg_rcx to ptr
  %mem_ld = load i64, ptr %.24, align 4
  %.26 = icmp eq i64 %mem_ld, 0
  %.45 = zext i1 %.26 to i64
  ret i64 %.45
}

attributes #0 = { mustprogress nofree norecurse nosync nounwind willreturn memory(read, inaccessiblemem: none, target_mem0: none, target_mem1: none) }


; ModuleID = '<string>'
source_filename = "<string>"
target triple = "x86_64-pc-windows-msvc"

; Function Attrs: mustprogress nofree norecurse nosync nounwind willreturn memory(read, inaccessiblemem: none, target_mem0: none, target_mem1: none)
define range(i64 0, 2) i64 @RtlIsGenericTableEmptyAvl(i64 %arg_rcx) local_unnamed_addr #0 {
lift_init:
  %.24 = add i64 %arg_rcx, 44
  %.25 = inttoptr i64 %.24 to ptr
  %mem_ld = load i32, ptr %.25, align 4
  %.27 = icmp eq i32 %mem_ld, 0
  %.46 = zext i1 %.27 to i64
  ret i64 %.46
}

attributes #0 = { mustprogress nofree norecurse nosync nounwind willreturn memory(read, inaccessiblemem: none, target_mem0: none, target_mem1: none) }


; ModuleID = '<string>'
source_filename = "<string>"
target triple = "x86_64-pc-windows-msvc"

; Function Attrs: mustprogress nofree norecurse nosync nounwind willreturn memory(readwrite, inaccessiblemem: none, target_mem0: none, target_mem1: none)
define i64 @RtlClearBitEx(i64 %arg_rcx, i64 %arg_rdx) local_unnamed_addr #0 {
lift_init:
  %.25 = add i64 %arg_rcx, 8
  %.26 = inttoptr i64 %.25 to ptr
  %mem_ld = load i64, ptr %.26, align 4
  %0 = ashr i64 %arg_rdx, 3
  %.29 = and i64 %0, -8
  %.30 = add i64 %mem_ld, %.29
  %.31 = inttoptr i64 %.30 to ptr
  %bt_word = load i64, ptr %.31, align 4
  %.32 = and i64 %arg_rdx, 63
  %.36 = shl nuw i64 1, %.32
  %.37 = xor i64 %.36, -1
  %.38 = and i64 %bt_word, %.37
  store i64 %.38, ptr %.31, align 4
  ret i64 %mem_ld
}

attributes #0 = { mustprogress nofree norecurse nosync nounwind willreturn memory(readwrite, inaccessiblemem: none, target_mem0: none, target_mem1: none) }


; ModuleID = '<string>'
source_filename = "<string>"
target triple = "x86_64-pc-windows-msvc"

; Function Attrs: mustprogress nofree norecurse nosync nounwind willreturn memory(readwrite, inaccessiblemem: none, target_mem0: none, target_mem1: none)
define i64 @RtlSetBitEx(i64 %arg_rcx, i64 %arg_rdx) local_unnamed_addr #0 {
lift_init:
  %.25 = add i64 %arg_rcx, 8
  %.26 = inttoptr i64 %.25 to ptr
  %mem_ld = load i64, ptr %.26, align 4
  %0 = ashr i64 %arg_rdx, 3
  %.29 = and i64 %0, -8
  %.30 = add i64 %mem_ld, %.29
  %.31 = inttoptr i64 %.30 to ptr
  %bt_word = load i64, ptr %.31, align 4
  %.32 = and i64 %arg_rdx, 63
  %.36 = shl nuw i64 1, %.32
  %.37 = or i64 %bt_word, %.36
  store i64 %.37, ptr %.31, align 4
  ret i64 %mem_ld
}

attributes #0 = { mustprogress nofree norecurse nosync nounwind willreturn memory(readwrite, inaccessiblemem: none, target_mem0: none, target_mem1: none) }


; ModuleID = '<string>'
source_filename = "<string>"
target triple = "x86_64-pc-windows-msvc"

; Function Attrs: mustprogress nofree norecurse nosync nounwind willreturn memory(none)
define noundef i64 @RtlClearBits(i64 %arg_rcx, i64 %arg_rdx, i64 %arg_r8) local_unnamed_addr #0 {
lift_init:
  ret i64 0
}

attributes #0 = { mustprogress nofree norecurse nosync nounwind willreturn memory(none) }


; ModuleID = '<string>'
source_filename = "<string>"
target triple = "x86_64-pc-windows-msvc"

; Function Attrs: mustprogress nofree norecurse nosync nounwind willreturn memory(none)
define noundef i64 @RtlClearBitsEx(i64 %arg_rcx, i64 %arg_rdx, i64 %arg_r8) local_unnamed_addr #0 {
lift_init:
  ret i64 0
}

attributes #0 = { mustprogress nofree norecurse nosync nounwind willreturn memory(none) }


; ModuleID = '<string>'
source_filename = "<string>"
target triple = "x86_64-pc-windows-msvc"

; Function Attrs: mustprogress nofree norecurse nosync nounwind willreturn memory(write, inaccessiblemem: none, target_mem0: none, target_mem1: none)
define noundef i64 @RtlGetNonVolatileToken(i64 %arg_rcx, i64 %arg_rdx, i64 %arg_r8) local_unnamed_addr #0 {
lift_init:
  %.26 = inttoptr i64 %arg_r8 to ptr
  store i64 3, ptr %.26, align 4
  ret i64 0
}

attributes #0 = { mustprogress nofree norecurse nosync nounwind willreturn memory(write, inaccessiblemem: none, target_mem0: none, target_mem1: none) }


; ModuleID = '<string>'
source_filename = "<string>"
target triple = "x86_64-pc-windows-msvc"

; Function Attrs: mustprogress nofree norecurse nosync nounwind willreturn memory(none)
define range(i64 0, 2) i64 @RtlIsPartialPlaceholder(i64 %arg_rcx) local_unnamed_addr #0 {
lift_init:
  %.242 = and i64 %arg_rcx, 4456448
  %.25 = icmp ne i64 %.242, 0
  %.39 = zext i1 %.25 to i64
  ret i64 %.39
}

attributes #0 = { mustprogress nofree norecurse nosync nounwind willreturn memory(none) }


; ModuleID = '<string>'
source_filename = "<string>"
target triple = "x86_64-pc-windows-msvc"

; Function Attrs: mustprogress nofree norecurse nosync nounwind willreturn memory(none)
define noundef i64 @RtlSetBits(i64 %arg_rcx, i64 %arg_rdx, i64 %arg_r8) local_unnamed_addr #0 {
lift_init:
  ret i64 0
}

attributes #0 = { mustprogress nofree norecurse nosync nounwind willreturn memory(none) }


; ModuleID = '<string>'
source_filename = "<string>"
target triple = "x86_64-pc-windows-msvc"

; Function Attrs: mustprogress nofree norecurse nosync nounwind willreturn memory(write, inaccessiblemem: none, target_mem0: none, target_mem1: none)
define noundef i64 @PfxInitialize(i64 %arg_rcx) local_unnamed_addr #0 {
lift_init:
  %.24 = inttoptr i64 %arg_rcx to ptr
  store i32 512, ptr %.24, align 4
  %.26 = add i64 %arg_rcx, 8
  %.27 = inttoptr i64 %.26 to ptr
  store i64 %arg_rcx, ptr %.27, align 4
  ret i64 0
}

attributes #0 = { mustprogress nofree norecurse nosync nounwind willreturn memory(write, inaccessiblemem: none, target_mem0: none, target_mem1: none) }


; ModuleID = '<string>'
source_filename = "<string>"
target triple = "x86_64-pc-windows-msvc"

; Function Attrs: mustprogress nofree norecurse nosync nounwind willreturn memory(none)
define i64 @RtlSubAuthoritySid(i64 %arg_rcx, i64 %arg_rdx) local_unnamed_addr #0 {
lift_init:
  %.25 = shl i64 %arg_rdx, 2
  %.27 = and i64 %.25, 17179869180
  %.41 = add i64 %arg_rcx, 8
  %.42 = add i64 %.41, %.27
  ret i64 %.42
}

attributes #0 = { mustprogress nofree norecurse nosync nounwind willreturn memory(none) }


; ModuleID = '<string>'
source_filename = "<string>"
target triple = "x86_64-pc-windows-msvc"

; Function Attrs: mustprogress nofree norecurse nosync nounwind willreturn memory(read, inaccessiblemem: none, target_mem0: none, target_mem1: none)
define range(i64 8, 1029) i64 @RtlLengthSid(i64 %arg_rcx) local_unnamed_addr #0 {
lift_init:
  %.24 = add i64 %arg_rcx, 1
  %.25 = inttoptr i64 %.24 to ptr
  %mem_ld = load i8, ptr %.25, align 1
  %.27 = zext i8 %mem_ld to i32
  %.29 = shl nuw nsw i32 %.27, 2
  %.31 = add nuw nsw i32 %.29, 8
  %.33 = zext nneg i32 %.31 to i64
  ret i64 %.33
}

attributes #0 = { mustprogress nofree norecurse nosync nounwind willreturn memory(read, inaccessiblemem: none, target_mem0: none, target_mem1: none) }


; ModuleID = '<string>'
source_filename = "<string>"
target triple = "x86_64-pc-windows-msvc"

; Function Attrs: mustprogress nofree norecurse nosync nounwind willreturn memory(read, inaccessiblemem: none, target_mem0: none, target_mem1: none)
define range(i64 0, -254) i64 @RtlTestBitEx(i64 %arg_rcx, i64 %arg_rdx) local_unnamed_addr #0 {
lift_init:
  %.25 = add i64 %arg_rcx, 8
  %.26 = inttoptr i64 %.25 to ptr
  %mem_ld = load i64, ptr %.26, align 4
  %0 = ashr i64 %arg_rdx, 3
  %.29 = and i64 %0, -8
  %.30 = add i64 %mem_ld, %.29
  %.31 = inttoptr i64 %.30 to ptr
  %bt_word = load i64, ptr %.31, align 4
  %.32 = and i64 %arg_rdx, 63
  %.33 = lshr i64 %bt_word, %.32
  %.42 = and i64 %mem_ld, -256
  %.43 = and i64 %.33, 1
  %.44 = or disjoint i64 %.43, %.42
  ret i64 %.44
}

attributes #0 = { mustprogress nofree norecurse nosync nounwind willreturn memory(read, inaccessiblemem: none, target_mem0: none, target_mem1: none) }


; ModuleID = '<string>'
source_filename = "<string>"
target triple = "x86_64-pc-windows-msvc"

; Function Attrs: mustprogress nofree norecurse nosync nounwind willreturn memory(read, inaccessiblemem: none, target_mem0: none, target_mem1: none)
define i64 @RtlGetEnabledExtendedFeatures(i64 %arg_rcx) local_unnamed_addr #0 {
lift_init:
  %mem_ld = load i64, ptr inttoptr (i64 -9345848835112 to ptr), align 8
  %.27 = and i64 %mem_ld, %arg_rcx
  ret i64 %.27
}

attributes #0 = { mustprogress nofree norecurse nosync nounwind willreturn memory(read, inaccessiblemem: none, target_mem0: none, target_mem1: none) }


; ModuleID = '<string>'
source_filename = "<string>"
target triple = "x86_64-pc-windows-msvc"

; Function Attrs: mustprogress nofree norecurse nosync nounwind willreturn memory(none)
define range(i64 0, 3221225486) i64 @RtlFreeNonVolatileToken(i64 %arg_rcx) local_unnamed_addr #0 {
lift_init:
  %.559 = and i64 %arg_rcx, 1
  %0 = icmp eq i64 %.559, 0
  %.82 = select i1 %0, i64 3221225485, i64 0
  ret i64 %.82
}

attributes #0 = { mustprogress nofree norecurse nosync nounwind willreturn memory(none) }


; ModuleID = '<string>'
source_filename = "<string>"
target triple = "x86_64-pc-windows-msvc"

; Function Attrs: mustprogress nofree norecurse nosync nounwind willreturn memory(read, inaccessiblemem: none, target_mem0: none, target_mem1: none)
define range(i64 0, 8388354) i64 @RtlIsMultiUsersInSessionSku() local_unnamed_addr #0 {
lift_init:
  %mem_ld = load i32, ptr inttoptr (i64 -9345848835344 to ptr), align 16
  %.29 = lshr i32 %mem_ld, 9
  %.484 = and i32 %.29, 8388353
  %.48 = zext nneg i32 %.484 to i64
  ret i64 %.48
}

attributes #0 = { mustprogress nofree norecurse nosync nounwind willreturn memory(read, inaccessiblemem: none, target_mem0: none, target_mem1: none) }


; ModuleID = '<string>'
source_filename = "<string>"
target triple = "x86_64-pc-windows-msvc"

; Function Attrs: mustprogress nofree norecurse nosync nounwind willreturn memory(read, inaccessiblemem: none, target_mem0: none, target_mem1: none)
define range(i64 0, 4194050) i64 @RtlIsStateSeparationEnabled() local_unnamed_addr #0 {
lift_init:
  %mem_ld = load i32, ptr inttoptr (i64 -9345848835344 to ptr), align 16
  %.29 = lshr i32 %mem_ld, 10
  %.484 = and i32 %.29, 4194049
  %.48 = zext nneg i32 %.484 to i64
  ret i64 %.48
}

attributes #0 = { mustprogress nofree norecurse nosync nounwind willreturn memory(read, inaccessiblemem: none, target_mem0: none, target_mem1: none) }


; ModuleID = '<string>'
source_filename = "<string>"
target triple = "x86_64-pc-windows-msvc"

define i64 @RtlFindNextForwardRunClearEx(i64 %arg_rcx, i64 %arg_rdx, i64 %arg_r8) local_unnamed_addr {
lift_init:
  %call_res = tail call i64 @RtlFindNextForwardRunClearCappedEx(i64 %arg_rcx, i64 %arg_rdx, i64 %arg_r8, i64 %arg_r8)
  ret i64 %call_res
}

declare i64 @RtlFindNextForwardRunClearCappedEx(i64, i64, i64, i64) local_unnamed_addr


; ModuleID = '<string>'
source_filename = "<string>"
target triple = "x86_64-pc-windows-msvc"

; Function Attrs: mustprogress nofree norecurse nosync nounwind willreturn memory(write, inaccessiblemem: none, target_mem0: none, target_mem1: none)
define noundef i64 @RtlInitializeRangeList(i64 %arg_rcx) local_unnamed_addr #0 {
lift_init:
  %.32 = add i64 %arg_rcx, 8
  %.33 = inttoptr i64 %.32 to ptr
  store i64 %arg_rcx, ptr %.33, align 4
  %.35 = inttoptr i64 %arg_rcx to ptr
  store i64 %arg_rcx, ptr %.35, align 4
  %.37 = add i64 %arg_rcx, 16
  %.38 = inttoptr i64 %.37 to ptr
  store i64 0, ptr %.38, align 4
  %.40 = add i64 %arg_rcx, 24
  %.41 = inttoptr i64 %.40 to ptr
  store i32 0, ptr %.41, align 4
  ret i64 0
}

attributes #0 = { mustprogress nofree norecurse nosync nounwind willreturn memory(write, inaccessiblemem: none, target_mem0: none, target_mem1: none) }


; ModuleID = '<string>'
source_filename = "<string>"
target triple = "x86_64-pc-windows-msvc"

; Function Attrs: mustprogress nofree norecurse nosync nounwind willreturn memory(write, inaccessiblemem: none, target_mem0: none, target_mem1: none)
define noundef i64 @RtlInitializeUnicodePrefix(i64 %arg_rcx) local_unnamed_addr #0 {
lift_init:
  %.32 = inttoptr i64 %arg_rcx to ptr
  store i32 2048, ptr %.32, align 4
  %.34 = add i64 %arg_rcx, 16
  %.35 = inttoptr i64 %.34 to ptr
  store i64 0, ptr %.35, align 4
  %.37 = add i64 %arg_rcx, 8
  %.38 = inttoptr i64 %.37 to ptr
  store i64 %arg_rcx, ptr %.38, align 4
  ret i64 0
}

attributes #0 = { mustprogress nofree norecurse nosync nounwind willreturn memory(write, inaccessiblemem: none, target_mem0: none, target_mem1: none) }


; ModuleID = '<string>'
source_filename = "<string>"
target triple = "x86_64-pc-windows-msvc"

define i64 @RtlOpenImageFileOptionsKey(i64 %arg_rcx, i64 %arg_rdx, i64 %arg_r8) local_unnamed_addr {
lift_init:
  %call_res = tail call i64 @RtlpOpenImageFileOptionsKeyEx(i64 %arg_rcx, i64 %arg_rdx, i64 %arg_r8, i64 %arg_r8)
  ret i64 %call_res
}

declare i64 @RtlpOpenImageFileOptionsKeyEx(i64, i64, i64, i64) local_unnamed_addr


; ModuleID = '<string>'
source_filename = "<string>"
target triple = "x86_64-pc-windows-msvc"

define i64 @RtlCheckTokenMembership(i64 %arg_rcx, i64 %arg_rdx, i64 %arg_r8) local_unnamed_addr {
lift_init:
  %call_res = tail call i64 @RtlCheckTokenMembershipEx(i64 %arg_rcx, i64 %arg_rdx, i64 0, i64 %arg_r8)
  ret i64 %call_res
}

declare i64 @RtlCheckTokenMembershipEx(i64, i64, i64, i64) local_unnamed_addr


; ModuleID = '<string>'
source_filename = "<string>"
target triple = "x86_64-pc-windows-msvc"

; Function Attrs: mustprogress nofree norecurse nosync nounwind willreturn memory(read, inaccessiblemem: none, target_mem0: none, target_mem1: none)
define range(i64 0, 4294967042) i64 @RtlEqualLuid(i64 %arg_rcx, i64 %arg_rdx) local_unnamed_addr #0 {
lift_init:
  %.25 = add i64 %arg_rdx, 4
  %.26 = inttoptr i64 %.25 to ptr
  %mem_ld = load i32, ptr %.26, align 4
  %.29 = add i64 %arg_rcx, 4
  %.30 = inttoptr i64 %.29 to ptr
  %mem_ld.1 = load i32, ptr %.30, align 4
  %.32.not = icmp eq i32 %mem_ld.1, %mem_ld
  br i1 %.32.not, label %bb_1407182c8, label %common.ret

bb_1407182c8:                                     ; preds = %lift_init
  %.50 = inttoptr i64 %arg_rdx to ptr
  %mem_ld.2 = load i32, ptr %.50, align 4
  %.53 = inttoptr i64 %arg_rcx to ptr
  %mem_ld.3 = load i32, ptr %.53, align 4
  %.55.not = icmp eq i32 %mem_ld.3, %mem_ld.2
  br i1 %.55.not, label %bb_1407182ce, label %common.ret

common.ret:                                       ; preds = %lift_init, %bb_1407182c8, %bb_1407182ce
  %common.ret.op = phi i64 [ %.76, %bb_1407182ce ], [ 0, %bb_1407182c8 ], [ 0, %lift_init ]
  ret i64 %common.ret.op

bb_1407182ce:                                     ; preds = %bb_1407182c8
  %0 = and i32 %mem_ld.2, -256
  %1 = or disjoint i32 %0, 1
  %.76 = zext i32 %1 to i64
  br label %common.ret
}

attributes #0 = { mustprogress nofree norecurse nosync nounwind willreturn memory(read, inaccessiblemem: none, target_mem0: none, target_mem1: none) }


; ModuleID = '<string>'
source_filename = "<string>"
target triple = "x86_64-pc-windows-msvc"

; Function Attrs: mustprogress nofree norecurse nosync nounwind willreturn memory(none)
define range(i64 0, 2) i64 @RtlIsNonEmptyDirectoryReparsePointAllowed(i64 %arg_rcx) local_unnamed_addr #0 {
lift_init:
  %r_ecx = trunc i64 %arg_rcx to i32
  %0 = and i32 %r_ecx, 268435456
  %.27.not = icmp ne i32 %0, 0
  %.35 = icmp eq i32 %r_ecx, -2147483624
  %or.cond = or i1 %.35, %.27.not
  %common.ret.op = zext i1 %or.cond to i64
  ret i64 %common.ret.op
}

attributes #0 = { mustprogress nofree norecurse nosync nounwind willreturn memory(none) }


; ModuleID = '<string>'
source_filename = "<string>"
target triple = "x86_64-pc-windows-msvc"

; Function Attrs: mustprogress nofree norecurse nosync nounwind willreturn memory(none)
define range(i64 0, 4294967296) i64 @RtlLengthRequiredSid(i64 %arg_rcx) local_unnamed_addr #0 {
lift_init:
  %r_ecx = trunc i64 %arg_rcx to i32
  %.43 = icmp ugt i32 %r_ecx, 1073741815
  %0 = shl nuw i32 %r_ecx, 2
  %.48 = add i32 %0, 8
  %narrow = select i1 %.43, i32 -1, i32 %.48
  %common.ret.op = zext i32 %narrow to i64
  ret i64 %common.ret.op
}

attributes #0 = { mustprogress nofree norecurse nosync nounwind willreturn memory(none) }


; ModuleID = '<string>'
source_filename = "<string>"
target triple = "x86_64-pc-windows-msvc"

define i64 @vsprintf(i64 %arg_rcx, i64 %arg_rdx, i64 %arg_r8) local_unnamed_addr {
lift_init:
  %call_res = tail call i64 @_vsprintf_l(i64 %arg_rcx, i64 %arg_rdx, i64 0, i64 %arg_r8)
  ret i64 %call_res
}

declare i64 @_vsprintf_l(i64, i64, i64, i64) local_unnamed_addr


; ModuleID = '<string>'
source_filename = "<string>"
target triple = "x86_64-pc-windows-msvc"

; Function Attrs: mustprogress nofree norecurse nosync nounwind willreturn memory(readwrite, inaccessiblemem: none, target_mem0: none, target_mem1: none)
define range(i64 0, 65536) i64 @RtlCopyUnicodeString(i64 %arg_rcx, i64 %arg_rdx) local_unnamed_addr #0 {
lift_init:
  %.46 = icmp eq i64 %arg_rdx, 0
  br i1 %.46, label %bb_140055ccd, label %bb_140055c6e

common.ret:                                       ; preds = %bb_140055ccd, %bb_140055c6e
  %common.ret.op = phi i64 [ %.59, %bb_140055c6e ], [ 0, %bb_140055ccd ]
  ret i64 %common.ret.op

bb_140055c6e:                                     ; preds = %lift_init
  %.57 = inttoptr i64 %arg_rdx to ptr
  %mem_ld = load i16, ptr %.57, align 2
  %.59 = zext i16 %mem_ld to i64
  br label %common.ret

bb_140055ccd:                                     ; preds = %lift_init
  %.73 = inttoptr i64 %arg_rcx to ptr
  store i16 0, ptr %.73, align 2
  br label %common.ret
}

attributes #0 = { mustprogress nofree norecurse nosync nounwind willreturn memory(readwrite, inaccessiblemem: none, target_mem0: none, target_mem1: none) }


; ModuleID = '<string>'
source_filename = "<string>"
target triple = "x86_64-pc-windows-msvc"

define i64 @RtlFindNextForwardRunClear(i64 %arg_rcx, i64 %arg_rdx, i64 %arg_r8) local_unnamed_addr {
lift_init:
  %call_res = tail call i64 @RtlFindNextForwardRunClearCapped(i64 %arg_rcx, i64 %arg_rdx, i64 4294967295, i64 %arg_r8)
  ret i64 %call_res
}

declare i64 @RtlFindNextForwardRunClearCapped(i64, i64, i64, i64) local_unnamed_addr


; ModuleID = '<string>'
source_filename = "<string>"
target triple = "x86_64-pc-windows-msvc"

; Function Attrs: nofree nosync nounwind memory(readwrite, inaccessiblemem: none, target_mem0: none, target_mem1: none)
define range(i64 0, 65536) i64 @RtlGetDefaultCodePage(i64 %arg_rcx, i64 %arg_rdx) local_unnamed_addr #0 {
lift_init:
  %addr_NlsAnsiCodePage = tail call i64 @__nt_sym_addr_NlsAnsiCodePage()
  %.25 = inttoptr i64 %addr_NlsAnsiCodePage to ptr
  %mem_ld = load i16, ptr %.25, align 2
  %.29 = inttoptr i64 %arg_rcx to ptr
  store i16 %mem_ld, ptr %.29, align 2
  %addr_NlsOemCodePage = tail call i64 @__nt_sym_addr_NlsOemCodePage()
  %.31 = inttoptr i64 %addr_NlsOemCodePage to ptr
  %mem_ld.1 = load i16, ptr %.31, align 2
  %.33 = zext i16 %mem_ld.1 to i64
  %.35 = inttoptr i64 %arg_rdx to ptr
  store i16 %mem_ld.1, ptr %.35, align 2
  ret i64 %.33
}

; Function Attrs: nofree nosync nounwind memory(none)
declare i64 @__nt_sym_addr_NlsAnsiCodePage() local_unnamed_addr #1

; Function Attrs: nofree nosync nounwind memory(none)
declare i64 @__nt_sym_addr_NlsOemCodePage() local_unnamed_addr #1

attributes #0 = { nofree nosync nounwind memory(readwrite, inaccessiblemem: none, target_mem0: none, target_mem1: none) }
attributes #1 = { nofree nosync nounwind memory(none) }


; ModuleID = '<string>'
source_filename = "<string>"
target triple = "x86_64-pc-windows-msvc"

define i64 @LdrFindResource_U(i64 %arg_rcx, i64 %arg_rdx, i64 %arg_r8, i64 %arg_r9) local_unnamed_addr {
lift_init:
  %call_res = tail call i64 @LdrpSearchResourceSection_U(i64 %arg_rcx, i64 %arg_rdx, i64 %arg_r8, i64 0)
  ret i64 %call_res
}

declare i64 @LdrpSearchResourceSection_U(i64, i64, i64, i64) local_unnamed_addr


; ModuleID = '<string>'
source_filename = "<string>"
target triple = "x86_64-pc-windows-msvc"

define i64 @RtlFindFirstRunClear(i64 %arg_rcx, i64 %arg_rdx, i64 %arg_r8) local_unnamed_addr {
lift_init:
  %call_res = tail call i64 @RtlFindNextForwardRunClearCapped(i64 %arg_rcx, i64 0, i64 4294967295, i64 %arg_rdx)
  ret i64 %call_res
}

declare i64 @RtlFindNextForwardRunClearCapped(i64, i64, i64, i64) local_unnamed_addr


; ModuleID = '<string>'
source_filename = "<string>"
target triple = "x86_64-pc-windows-msvc"

define i64 @RtlCrc32(i64 %arg_rcx, i64 %arg_rdx, i64 %arg_r8) local_unnamed_addr {
lift_init:
  %.40 = and i64 %arg_r8, 4294967295
  %addr_Crc32Ctrl = tail call i64 @__nt_sym_addr_Crc32Ctrl()
  %call_res = tail call i64 @RtlpComputeCrcInternal(i64 %arg_rcx, i64 %arg_rdx, i64 %.40, i64 %addr_Crc32Ctrl)
  ret i64 %call_res
}

; Function Attrs: nofree nosync nounwind memory(none)
declare i64 @__nt_sym_addr_Crc32Ctrl() local_unnamed_addr #0

declare i64 @RtlpComputeCrcInternal(i64, i64, i64, i64) local_unnamed_addr

attributes #0 = { nofree nosync nounwind memory(none) }


; ModuleID = '<string>'
source_filename = "<string>"
target triple = "x86_64-pc-windows-msvc"

; Function Attrs: mustprogress nofree norecurse nosync nounwind willreturn memory(none)
define range(i64 0, 4294967042) i64 @RtlIsNtDdiVersionAvailable(i64 %arg_rcx) local_unnamed_addr #0 {
lift_init:
  %r_ecx = trunc i64 %arg_rcx to i32
  %.28 = lshr i32 %r_ecx, 8
  %.37 = or i32 %.28, %r_ecx
  %.44 = zext i32 %.37 to i64
  %r_al = trunc i32 %.37 to i8
  %.47.not = icmp eq i8 %r_al, 0
  %.76 = icmp ult i32 %r_ecx, 167772165
  %.80 = zext i1 %.76 to i64
  %.81 = or disjoint i64 %.44, %.80
  %.87 = and i64 %.44, 4294967040
  %common.ret.op = select i1 %.47.not, i64 %.81, i64 %.87
  ret i64 %common.ret.op
}

attributes #0 = { mustprogress nofree norecurse nosync nounwind willreturn memory(none) }


; ModuleID = '<string>'
source_filename = "<string>"
target triple = "x86_64-pc-windows-msvc"

define i64 @LdrFindResourceDirectory_U(i64 %arg_rcx, i64 %arg_rdx, i64 %arg_r8, i64 %arg_r9) local_unnamed_addr {
lift_init:
  %call_res = tail call i64 @LdrpSearchResourceSection_U(i64 %arg_rcx, i64 %arg_rdx, i64 %arg_r8, i64 2)
  ret i64 %call_res
}

declare i64 @LdrpSearchResourceSection_U(i64, i64, i64, i64) local_unnamed_addr


; ModuleID = '<string>'
source_filename = "<string>"
target triple = "x86_64-pc-windows-msvc"

define i64 @RtlCreateHashTable(i64 %arg_rcx, i64 %arg_rdx, i64 %arg_r8) local_unnamed_addr {
lift_init:
  %.40 = and i64 %arg_r8, 4294967295
  %.42 = and i64 %arg_rdx, 4294967295
  %call_res = tail call i64 @RtlpCreateHashTable(i64 %arg_rcx, i64 128, i64 %.42, i64 %.40)
  ret i64 %call_res
}

declare i64 @RtlpCreateHashTable(i64, i64, i64, i64) local_unnamed_addr


; ModuleID = '<string>'
source_filename = "<string>"
target triple = "x86_64-pc-windows-msvc"

; Function Attrs: mustprogress nofree norecurse nosync nounwind willreturn memory(none)
define range(i64 0, 2) i64 @RtlIsCloudFilesPlaceholder(i64 %arg_rcx, i64 %arg_rdx) local_unnamed_addr #0 {
lift_init:
  %0 = and i64 %arg_rcx, 1024
  %.28.not = icmp ne i64 %0, 0
  %.365 = and i64 %arg_rdx, 4294905855
  %.46.not = icmp eq i64 %.365, 2415919130
  %or.cond = select i1 %.28.not, i1 %.46.not, i1 false
  %common.ret.op = zext i1 %or.cond to i64
  ret i64 %common.ret.op
}

attributes #0 = { mustprogress nofree norecurse nosync nounwind willreturn memory(none) }


; ModuleID = '<string>'
source_filename = "<string>"
target triple = "x86_64-pc-windows-msvc"

; Function Attrs: mustprogress nofree norecurse nosync nounwind willreturn memory(readwrite, inaccessiblemem: none, target_mem0: none, target_mem1: none)
define range(i64 0, 3221225561) i64 @RtlGetControlSecurityDescriptor(i64 %arg_rcx, i64 %arg_rdx, i64 %arg_r8) local_unnamed_addr #0 {
lift_init:
  %.26 = inttoptr i64 %arg_rcx to ptr
  %mem_ld = load i8, ptr %.26, align 1
  %.27 = zext i8 %mem_ld to i32
  %.30 = inttoptr i64 %arg_r8 to ptr
  store i32 %.27, ptr %.30, align 4
  %mem_ld.1 = load i8, ptr %.26, align 1
  %.34.not = icmp eq i8 %mem_ld.1, 1
  br i1 %.34.not, label %bb_1404bf25b, label %common.ret

common.ret:                                       ; preds = %lift_init, %bb_1404bf25b
  %common.ret.op = phi i64 [ 0, %bb_1404bf25b ], [ 3221225560, %lift_init ]
  ret i64 %common.ret.op

bb_1404bf25b:                                     ; preds = %lift_init
  %.52 = add i64 %arg_rcx, 2
  %.53 = inttoptr i64 %.52 to ptr
  %mem_ld.2 = load i16, ptr %.53, align 2
  %.57 = inttoptr i64 %arg_rdx to ptr
  store i16 %mem_ld.2, ptr %.57, align 2
  br label %common.ret
}

attributes #0 = { mustprogress nofree norecurse nosync nounwind willreturn memory(readwrite, inaccessiblemem: none, target_mem0: none, target_mem1: none) }


; ModuleID = '<string>'
source_filename = "<string>"
target triple = "x86_64-pc-windows-msvc"

; Function Attrs: mustprogress nofree norecurse nosync nounwind willreturn memory(none)
define range(i64 0, 4294901762) i64 @RtlIsServicePackVersionInstalled(i64 %arg_rcx) local_unnamed_addr #0 {
lift_init:
  %.33 = and i64 %arg_rcx, 4294901760
  %0 = and i64 %arg_rcx, 4294967040
  %narrow = icmp eq i64 %0, 167772160
  %.83 = zext i1 %narrow to i64
  %common.ret.op = or disjoint i64 %.33, %.83
  ret i64 %common.ret.op
}

attributes #0 = { mustprogress nofree norecurse nosync nounwind willreturn memory(none) }


; ModuleID = '<string>'
source_filename = "<string>"
target triple = "x86_64-pc-windows-msvc"

define i64 @RtlGetSystemBootStatusEx(i64 %arg_rcx, i64 %arg_rdx, i64 %arg_r8) local_unnamed_addr {
lift_init:
  %.41 = and i64 %arg_rdx, 4294967295
  %call_res = tail call i64 @RtlpSystemBootStatusRequest(i64 31, i64 %arg_rcx, i64 %.41, i64 %arg_r8)
  ret i64 %call_res
}

declare i64 @RtlpSystemBootStatusRequest(i64, i64, i64, i64) local_unnamed_addr


; ModuleID = '<string>'
source_filename = "<string>"
target triple = "x86_64-pc-windows-msvc"

define i64 @RtlGetTokenNamedObjectPath(i64 %arg_rcx, i64 %arg_rdx, i64 %arg_r8) local_unnamed_addr {
lift_init:
  %r_dl = trunc i64 %arg_rdx to i8
  %.50 = icmp ne i8 %r_dl, 0
  %.64 = zext i1 %.50 to i64
  %call_res = tail call i64 @RtlpGetTokenNamedObjectPath(i64 %arg_rcx, i64 0, i64 %.64, i64 %arg_r8)
  ret i64 %call_res
}

declare i64 @RtlpGetTokenNamedObjectPath(i64, i64, i64, i64) local_unnamed_addr


; ModuleID = '<string>'
source_filename = "<string>"
target triple = "x86_64-pc-windows-msvc"

define i64 @RtlSetSystemBootStatusEx(i64 %arg_rcx, i64 %arg_rdx, i64 %arg_r8) local_unnamed_addr {
lift_init:
  %.41 = and i64 %arg_rdx, 4294967295
  %call_res = tail call i64 @RtlpSystemBootStatusRequest(i64 32, i64 %arg_rcx, i64 %.41, i64 %arg_r8)
  ret i64 %call_res
}

declare i64 @RtlpSystemBootStatusRequest(i64, i64, i64, i64) local_unnamed_addr


; ModuleID = '<string>'
source_filename = "<string>"
target triple = "x86_64-pc-windows-msvc"

define i64 @SkAcquirePushLockExclusive() local_unnamed_addr {
lift_init:
  %call_res = tail call i64 @KeBugCheckEx(i64 291, i64 0, i64 0, i64 0)
  ret i64 %call_res
}

declare i64 @KeBugCheckEx(i64, i64, i64, i64) local_unnamed_addr


; ModuleID = '<string>'
source_filename = "<string>"
target triple = "x86_64-pc-windows-msvc"

; Function Attrs: nofree nosync nounwind memory(read, inaccessiblemem: none, target_mem0: none, target_mem1: none)
define range(i64 0, 5) i64 @isdigit(i64 %arg_rcx, i64 %arg_rdx) local_unnamed_addr #0 {
lift_init:
  %0 = trunc i64 %arg_rcx to i32
  %.25 = and i32 %0, 255
  %.38 = icmp eq i32 %0, -1
  %.54 = select i1 %.38, i32 -1, i32 %.25
  %.57 = sext i32 %.54 to i64
  %addr___nt_data_14034b150 = tail call i64 @__nt_sym_addr___nt_data_14034b150()
  %.59 = inttoptr i64 %addr___nt_data_14034b150 to ptr
  %mem_ld = load i64, ptr %.59, align 4
  %.61 = shl nsw i64 %.57, 1
  %.62 = add i64 %mem_ld, %.61
  %.63 = inttoptr i64 %.62 to ptr
  %mem_ld.1 = load i16, ptr %.63, align 2
  %1 = and i16 %mem_ld.1, 4
  %.74 = zext nneg i16 %1 to i64
  ret i64 %.74
}

; Function Attrs: nofree nosync nounwind memory(none)
declare i64 @__nt_sym_addr___nt_data_14034b150() local_unnamed_addr #1

attributes #0 = { nofree nosync nounwind memory(read, inaccessiblemem: none, target_mem0: none, target_mem1: none) }
attributes #1 = { nofree nosync nounwind memory(none) }


; ModuleID = '<string>'
source_filename = "<string>"
target triple = "x86_64-pc-windows-msvc"

; Function Attrs: nofree nosync nounwind memory(read, inaccessiblemem: none, target_mem0: none, target_mem1: none)
define range(i64 0, 3) i64 @islower(i64 %arg_rcx, i64 %arg_rdx) local_unnamed_addr #0 {
lift_init:
  %0 = trunc i64 %arg_rcx to i32
  %.25 = and i32 %0, 255
  %.38 = icmp eq i32 %0, -1
  %.54 = select i1 %.38, i32 -1, i32 %.25
  %.57 = sext i32 %.54 to i64
  %addr___nt_data_14034b150 = tail call i64 @__nt_sym_addr___nt_data_14034b150()
  %.59 = inttoptr i64 %addr___nt_data_14034b150 to ptr
  %mem_ld = load i64, ptr %.59, align 4
  %.61 = shl nsw i64 %.57, 1
  %.62 = add i64 %mem_ld, %.61
  %.63 = inttoptr i64 %.62 to ptr
  %mem_ld.1 = load i16, ptr %.63, align 2
  %1 = and i16 %mem_ld.1, 2
  %.74 = zext nneg i16 %1 to i64
  ret i64 %.74
}

; Function Attrs: nofree nosync nounwind memory(none)
declare i64 @__nt_sym_addr___nt_data_14034b150() local_unnamed_addr #1

attributes #0 = { nofree nosync nounwind memory(read, inaccessiblemem: none, target_mem0: none, target_mem1: none) }
attributes #1 = { nofree nosync nounwind memory(none) }


; ModuleID = '<string>'
source_filename = "<string>"
target triple = "x86_64-pc-windows-msvc"

; Function Attrs: nofree nosync nounwind memory(read, inaccessiblemem: none, target_mem0: none, target_mem1: none)
define range(i64 0, 9) i64 @isspace(i64 %arg_rcx, i64 %arg_rdx) local_unnamed_addr #0 {
lift_init:
  %0 = trunc i64 %arg_rcx to i32
  %.25 = and i32 %0, 255
  %.38 = icmp eq i32 %0, -1
  %.54 = select i1 %.38, i32 -1, i32 %.25
  %.57 = sext i32 %.54 to i64
  %addr___nt_data_14034b150 = tail call i64 @__nt_sym_addr___nt_data_14034b150()
  %.59 = inttoptr i64 %addr___nt_data_14034b150 to ptr
  %mem_ld = load i64, ptr %.59, align 4
  %.61 = shl nsw i64 %.57, 1
  %.62 = add i64 %mem_ld, %.61
  %.63 = inttoptr i64 %.62 to ptr
  %mem_ld.1 = load i16, ptr %.63, align 2
  %1 = and i16 %mem_ld.1, 8
  %.74 = zext nneg i16 %1 to i64
  ret i64 %.74
}

; Function Attrs: nofree nosync nounwind memory(none)
declare i64 @__nt_sym_addr___nt_data_14034b150() local_unnamed_addr #1

attributes #0 = { nofree nosync nounwind memory(read, inaccessiblemem: none, target_mem0: none, target_mem1: none) }
attributes #1 = { nofree nosync nounwind memory(none) }


; ModuleID = '<string>'
source_filename = "<string>"
target triple = "x86_64-pc-windows-msvc"

; Function Attrs: nofree nosync nounwind memory(read, inaccessiblemem: none, target_mem0: none, target_mem1: none)
define range(i64 0, 2) i64 @isupper(i64 %arg_rcx, i64 %arg_rdx) local_unnamed_addr #0 {
lift_init:
  %0 = trunc i64 %arg_rcx to i32
  %.25 = and i32 %0, 255
  %.38 = icmp eq i32 %0, -1
  %.54 = select i1 %.38, i32 -1, i32 %.25
  %.57 = sext i32 %.54 to i64
  %addr___nt_data_14034b150 = tail call i64 @__nt_sym_addr___nt_data_14034b150()
  %.59 = inttoptr i64 %addr___nt_data_14034b150 to ptr
  %mem_ld = load i64, ptr %.59, align 4
  %.61 = shl nsw i64 %.57, 1
  %.62 = add i64 %mem_ld, %.61
  %.63 = inttoptr i64 %.62 to ptr
  %mem_ld.1 = load i16, ptr %.63, align 2
  %1 = and i16 %mem_ld.1, 1
  %.74 = zext nneg i16 %1 to i64
  ret i64 %.74
}

; Function Attrs: nofree nosync nounwind memory(none)
declare i64 @__nt_sym_addr___nt_data_14034b150() local_unnamed_addr #1

attributes #0 = { nofree nosync nounwind memory(read, inaccessiblemem: none, target_mem0: none, target_mem1: none) }
attributes #1 = { nofree nosync nounwind memory(none) }


; ModuleID = '<string>'
source_filename = "<string>"
target triple = "x86_64-pc-windows-msvc"

define i64 @sprintf_s(i64 %arg_rcx, i64 %arg_rdx, i64 %arg_r8, i64 %arg_r9) local_unnamed_addr {
lift_init:
  %stack_frame = alloca [1024 x i8], align 16
  %.28 = getelementptr inbounds nuw i8, ptr %stack_frame, i64 792
  store i64 %arg_r8, ptr %.28, align 8
  %.32 = getelementptr inbounds nuw i8, ptr %stack_frame, i64 800
  store i64 %arg_r9, ptr %.32, align 16
  %.51 = ptrtoint ptr %.32 to i64
  %call_res = call i64 @vsprintf_s(i64 %arg_rcx, i64 %arg_rdx, i64 %arg_r8, i64 %.51)
  ret i64 %call_res
}

declare i64 @vsprintf_s(i64, i64, i64, i64) local_unnamed_addr


; ModuleID = '<string>'
source_filename = "<string>"
target triple = "x86_64-pc-windows-msvc"

define i64 @swprintf_s(i64 %arg_rcx, i64 %arg_rdx, i64 %arg_r8, i64 %arg_r9) local_unnamed_addr {
lift_init:
  %stack_frame = alloca [1024 x i8], align 16
  %.28 = getelementptr inbounds nuw i8, ptr %stack_frame, i64 792
  store i64 %arg_r8, ptr %.28, align 8
  %.32 = getelementptr inbounds nuw i8, ptr %stack_frame, i64 800
  store i64 %arg_r9, ptr %.32, align 16
  %.51 = ptrtoint ptr %.32 to i64
  %call_res = call i64 @vswprintf_s(i64 %arg_rcx, i64 %arg_rdx, i64 %arg_r8, i64 %.51)
  ret i64 %call_res
}

declare i64 @vswprintf_s(i64, i64, i64, i64) local_unnamed_addr


; ModuleID = '<string>'
source_filename = "<string>"
target triple = "x86_64-pc-windows-msvc"

define i64 @RtlAddAccessAllowedAce(i64 %arg_rcx, i64 %arg_rdx, i64 %arg_r8, i64 %arg_r9) local_unnamed_addr {
lift_init:
  %.48 = and i64 %arg_r8, 4294967295
  %call_res = tail call i64 @RtlpAddKnownAce(i64 %arg_rcx, i64 %arg_rdx, i64 0, i64 %.48)
  ret i64 %call_res
}

declare i64 @RtlpAddKnownAce(i64, i64, i64, i64) local_unnamed_addr


; ModuleID = '<string>'
source_filename = "<string>"
target triple = "x86_64-pc-windows-msvc"

; Function Attrs: nofree nosync nounwind memory(readwrite, inaccessiblemem: none, target_mem0: none, target_mem1: none)
define range(i64 0, 32768) i64 @rand() local_unnamed_addr #0 {
lift_init:
  %addr___nt_data_14034e510 = tail call i64 @__nt_sym_addr___nt_data_14034e510()
  %.23 = inttoptr i64 %addr___nt_data_14034e510 to ptr
  %mem_ld = load i32, ptr %.23, align 4
  %.24 = mul i32 %mem_ld, 214013
  %.27 = add i32 %.24, 2531011
  store i32 %.27, ptr %.23, align 4
  %.46 = lshr i32 %.27, 16
  %.55 = and i32 %.46, 32767
  %.62 = zext nneg i32 %.55 to i64
  ret i64 %.62
}

; Function Attrs: nofree nosync nounwind memory(none)
declare i64 @__nt_sym_addr___nt_data_14034e510() local_unnamed_addr #1

attributes #0 = { nofree nosync nounwind memory(readwrite, inaccessiblemem: none, target_mem0: none, target_mem1: none) }
attributes #1 = { nofree nosync nounwind memory(none) }


; ModuleID = '<string>'
source_filename = "<string>"
target triple = "x86_64-pc-windows-msvc"

; Function Attrs: nofree nosync nounwind memory(none)
define range(i64 0, 3221225486) i64 @RtlDrainNonVolatileFlush(i64 %arg_rcx) local_unnamed_addr #0 {
lift_init:
  %.244 = and i64 %arg_rcx, 1
  %.25.not = icmp eq i64 %.244, 0
  br i1 %.25.not, label %common.ret, label %bb_1402484db

common.ret:                                       ; preds = %lift_init, %bb_1402484db
  %common.ret.op = phi i64 [ 0, %bb_1402484db ], [ 3221225485, %lift_init ]
  ret i64 %common.ret.op

bb_1402484db:                                     ; preds = %lift_init
  %addr_RtlpIsDrainRequired = tail call i64 @__nt_sym_addr_RtlpIsDrainRequired()
  br label %common.ret
}

; Function Attrs: nofree nosync nounwind memory(none)
declare i64 @__nt_sym_addr_RtlpIsDrainRequired() local_unnamed_addr #0

attributes #0 = { nofree nosync nounwind memory(none) }


; ModuleID = '<string>'
source_filename = "<string>"
target triple = "x86_64-pc-windows-msvc"

; Function Attrs: mustprogress nofree norecurse nosync nounwind willreturn memory(read, inaccessiblemem: none, target_mem0: none, target_mem1: none)
define range(i64 0, 65536) i64 @RtlEqualUnicodeString(i64 %arg_rcx, i64 %arg_rdx) local_unnamed_addr #0 {
lift_init:
  %.39 = inttoptr i64 %arg_rcx to ptr
  %mem_ld = load i16, ptr %.39, align 2
  %.41 = zext i16 %mem_ld to i64
  %.43 = inttoptr i64 %arg_rdx to ptr
  %mem_ld.1 = load i16, ptr %.43, align 2
  %.48 = icmp eq i16 %mem_ld, %mem_ld.1
  %.66 = and i64 %.41, 65280
  %common.ret.op = select i1 %.48, i64 %.41, i64 %.66
  ret i64 %common.ret.op
}

attributes #0 = { mustprogress nofree norecurse nosync nounwind willreturn memory(read, inaccessiblemem: none, target_mem0: none, target_mem1: none) }


; ModuleID = '<string>'
source_filename = "<string>"
target triple = "x86_64-pc-windows-msvc"

; Function Attrs: mustprogress nofree norecurse nosync nounwind willreturn memory(none)
define noundef i64 @SeShouldCheckForAccessRightsFromParent() local_unnamed_addr #0 {
lift_init:
  ret i64 0
}

attributes #0 = { mustprogress nofree norecurse nosync nounwind willreturn memory(none) }


; ModuleID = '<string>'
source_filename = "<string>"
target triple = "x86_64-pc-windows-msvc"

; Function Attrs: mustprogress nofree norecurse nosync nounwind willreturn memory(read, inaccessiblemem: none, target_mem0: none, target_mem1: none)
define range(i64 0, 4294967296) i64 @SeTokenImpersonationLevel(i64 %arg_rcx) local_unnamed_addr #0 {
lift_init:
  %.24 = add i64 %arg_rcx, 196
  %.25 = inttoptr i64 %.24 to ptr
  %mem_ld = load i32, ptr %.25, align 4
  %.26 = zext i32 %mem_ld to i64
  ret i64 %.26
}

attributes #0 = { mustprogress nofree norecurse nosync nounwind willreturn memory(read, inaccessiblemem: none, target_mem0: none, target_mem1: none) }


; ModuleID = '<string>'
source_filename = "<string>"
target triple = "x86_64-pc-windows-msvc"

; Function Attrs: mustprogress nofree norecurse nosync nounwind willreturn memory(read, inaccessiblemem: none, target_mem0: none, target_mem1: none)
define range(i64 0, 4294967296) i64 @SeTokenType(i64 %arg_rcx) local_unnamed_addr #0 {
lift_init:
  %.24 = add i64 %arg_rcx, 192
  %.25 = inttoptr i64 %.24 to ptr
  %mem_ld = load i32, ptr %.25, align 4
  %.26 = zext i32 %mem_ld to i64
  ret i64 %.26
}

attributes #0 = { mustprogress nofree norecurse nosync nounwind willreturn memory(read, inaccessiblemem: none, target_mem0: none, target_mem1: none) }


; ModuleID = '<string>'
source_filename = "<string>"
target triple = "x86_64-pc-windows-msvc"

; Function Attrs: mustprogress nofree norecurse nosync nounwind willreturn memory(read, inaccessiblemem: none, target_mem0: none, target_mem1: none)
define range(i64 0, 536870658) i64 @SeTokenIsWriteRestricted(i64 %arg_rcx) local_unnamed_addr #0 {
lift_init:
  %.24 = add i64 %arg_rcx, 200
  %.25 = inttoptr i64 %.24 to ptr
  %mem_ld = load i32, ptr %.25, align 4
  %.30 = lshr i32 %mem_ld, 3
  %.494 = and i32 %.30, 536870657
  %.49 = zext nneg i32 %.494 to i64
  ret i64 %.49
}

attributes #0 = { mustprogress nofree norecurse nosync nounwind willreturn memory(read, inaccessiblemem: none, target_mem0: none, target_mem1: none) }


; ModuleID = '<string>'
source_filename = "<string>"
target triple = "x86_64-pc-windows-msvc"

; Function Attrs: mustprogress nofree norecurse nosync nounwind willreturn memory(read, inaccessiblemem: none, target_mem0: none, target_mem1: none)
define i64 @SeSetAccessStateGenericMapping(i64 %arg_rcx, i64 %arg_rdx) local_unnamed_addr #0 {
lift_init:
  %.25 = add i64 %arg_rcx, 72
  %.26 = inttoptr i64 %.25 to ptr
  %mem_ld = load i64, ptr %.26, align 4
  ret i64 %mem_ld
}

attributes #0 = { mustprogress nofree norecurse nosync nounwind willreturn memory(read, inaccessiblemem: none, target_mem0: none, target_mem1: none) }


; ModuleID = '<string>'
source_filename = "<string>"
target triple = "x86_64-pc-windows-msvc"

; Function Attrs: mustprogress nofree norecurse nosync nounwind willreturn memory(read, inaccessiblemem: none, target_mem0: none, target_mem1: none)
define i64 @SeCaptureSubjectContextEx(i64 %arg_rcx, i64 %arg_rdx, i64 %arg_r8) local_unnamed_addr #0 {
lift_init:
  %.50 = add i64 %arg_rdx, 736
  %.51 = inttoptr i64 %.50 to ptr
  %mem_ld = load i64, ptr %.51, align 4
  ret i64 %mem_ld
}

attributes #0 = { mustprogress nofree norecurse nosync nounwind willreturn memory(read, inaccessiblemem: none, target_mem0: none, target_mem1: none) }


; ModuleID = '<string>'
source_filename = "<string>"
target triple = "x86_64-pc-windows-msvc"

define i64 @SeAdjustAccessStateForTrustLabel(i64 %arg_rcx, i64 %arg_rdx, i64 %arg_r8) local_unnamed_addr {
lift_init:
  %call_res = tail call i64 @SepAdjustAccessStateForConstraints(i64 %arg_rcx, i64 %arg_rdx, i64 0, i64 %arg_r8)
  ret i64 %call_res
}

declare i64 @SepAdjustAccessStateForConstraints(i64, i64, i64, i64) local_unnamed_addr


; ModuleID = '<string>'
source_filename = "<string>"
target triple = "x86_64-pc-windows-msvc"

define i64 @SeSetSecurityAttributesToken(i64 %arg_rcx, i64 %arg_rdx, i64 %arg_r8, i64 %arg_r9) local_unnamed_addr {
lift_init:
  %call_res = tail call i64 @SepInternalSetSecurityAttributesToken(i64 %arg_rcx, i64 %arg_rdx, i64 0, i64 %arg_r8)
  ret i64 %call_res
}

declare i64 @SepInternalSetSecurityAttributesToken(i64, i64, i64, i64) local_unnamed_addr


; ModuleID = '<string>'
source_filename = "<string>"
target triple = "x86_64-pc-windows-msvc"

define i64 @SeCaptureSubjectContext(i64 %arg_rcx) local_unnamed_addr {
lift_init:
  %gs_val = tail call i64 @__nt_read_gs_i64(i64 392)
  %gs_val.1 = tail call i64 @__nt_read_gs_i64(i64 392)
  ret i64 %gs_val
}

declare i64 @__nt_read_gs_i64(i64) local_unnamed_addr


; ModuleID = '<string>'
source_filename = "<string>"
target triple = "x86_64-pc-windows-msvc"

define range(i64 0, 4294967296) i64 @SeComputeAutoInheritByObjectType() local_unnamed_addr {
lift_init:
  %stack_frame = alloca [1024 x i8], align 16
  %.38 = getelementptr inbounds nuw i8, ptr %stack_frame, i64 800
  store i32 0, ptr %.38, align 16
  %.53 = ptrtoint ptr %.38 to i64
  %.56 = getelementptr inbounds nuw i8, ptr %stack_frame, i64 744
  store i64 0, ptr %.56, align 8
  %call_res = call i64 @SeComputeAutoInheritByObjectTypeEx(i64 0, i64 0, i64 0, i64 %.53)
  %mem_ld.2 = load i32, ptr %.38, align 16
  %.73 = zext i32 %mem_ld.2 to i64
  ret i64 %.73
}

declare i64 @SeComputeAutoInheritByObjectTypeEx(i64, i64, i64, i64) local_unnamed_addr


; ModuleID = '<string>'
source_filename = "<string>"
target triple = "x86_64-pc-windows-msvc"

define i64 @SeReportSecurityEvent(i64 %arg_rcx, i64 %arg_rdx, i64 %arg_r8, i64 %arg_r9) local_unnamed_addr {
lift_init:
  %.42 = icmp eq i64 %arg_r9, 0
  br i1 %.42, label %bb_140256600, label %bb_1402565f9

bb_1402565f9:                                     ; preds = %lift_init
  %.53 = inttoptr i64 %arg_r9 to ptr
  store i32 3, ptr %.53, align 4
  br label %bb_140256600

bb_140256600:                                     ; preds = %bb_1402565f9, %lift_init
  %call_res = tail call i64 @SeReportSecurityEventWithSubCategory(i64 %arg_rcx, i64 %arg_rdx, i64 %arg_r8, i64 %arg_r9)
  ret i64 %call_res
}

declare i64 @SeReportSecurityEventWithSubCategory(i64, i64, i64, i64) local_unnamed_addr


; ModuleID = '<string>'
source_filename = "<string>"
target triple = "x86_64-pc-windows-msvc"

define noundef i64 @SeDeassignSecurity(i64 %arg_rcx) local_unnamed_addr {
lift_init:
  %.44 = inttoptr i64 %arg_rcx to ptr
  %mem_ld = load i64, ptr %.44, align 4
  %.47 = icmp eq i64 %mem_ld, 0
  br i1 %.47, label %bb_140561a78, label %bb_140561a71

bb_140561a71:                                     ; preds = %lift_init
  %call_res = tail call i64 @ExFreePoolWithTag(i64 %mem_ld, i64 0, i64 0, i64 0)
  br label %bb_140561a78

bb_140561a78:                                     ; preds = %bb_140561a71, %lift_init
  store i64 0, ptr %.44, align 4
  ret i64 0
}

declare i64 @ExFreePoolWithTag(i64, i64, i64, i64) local_unnamed_addr


; ModuleID = '<string>'
source_filename = "<string>"
target triple = "x86_64-pc-windows-msvc"

define i64 @SeSetSecurityDescriptorInfoEx(i64 %arg_rcx, i64 %arg_rdx, i64 %arg_r8, i64 %arg_r9) local_unnamed_addr {
lift_init:
  %.41 = inttoptr i64 %arg_r9 to ptr
  %mem_ld = load i64, ptr %.41, align 4
  %.43.not = icmp eq i64 %mem_ld, 0
  br i1 %.43.not, label %bb_140726ab5, label %bb_140726a91

bb_140726a91:                                     ; preds = %lift_init
  %.68 = inttoptr i64 %arg_rdx to ptr
  %mem_ld.2 = load i32, ptr %.68, align 4
  %.69 = zext i32 %mem_ld.2 to i64
  %call_res = tail call i64 @RtlpSetSecurityObject(i64 %arg_rcx, i64 %.69, i64 %arg_r8, i64 %arg_r9)
  br label %bb_140726ab5

bb_140726ab5:                                     ; preds = %lift_init, %bb_140726a91
  %reg_rax.0 = phi i64 [ %call_res, %bb_140726a91 ], [ 3221225687, %lift_init ]
  ret i64 %reg_rax.0
}

declare i64 @RtlpSetSecurityObject(i64, i64, i64, i64) local_unnamed_addr


; ModuleID = '<string>'
source_filename = "<string>"
target triple = "x86_64-pc-windows-msvc"

define i64 @SeCreateAccessStateEx(i64 %arg_rcx, i64 %arg_rdx, i64 %arg_r8, i64 %arg_r9) local_unnamed_addr {
lift_init:
  %stack_frame = alloca [1024 x i8], align 16
  %.28 = getelementptr inbounds nuw i8, ptr %stack_frame, i64 776
  store i64 0, ptr %.28, align 8
  %.33 = getelementptr inbounds nuw i8, ptr %stack_frame, i64 760
  store i64 0, ptr %.33, align 8
  %.53 = getelementptr inbounds nuw i8, ptr %stack_frame, i64 728
  %.54 = ptrtoint ptr %.53 to i64
  %call_res = call i64 @SeCaptureSubjectContextEx(i64 %arg_rcx, i64 %arg_rdx, i64 %.54)
  %.58 = getelementptr inbounds nuw i8, ptr %stack_frame, i64 816
  %mem_ld = load i64, ptr %.58, align 16
  %.66 = getelementptr inbounds nuw i8, ptr %stack_frame, i64 808
  %mem_ld.1 = load i32, ptr %.66, align 8
  %.68 = zext i32 %mem_ld.1 to i64
  %.73 = getelementptr inbounds nuw i8, ptr %stack_frame, i64 712
  store i64 %mem_ld, ptr %.73, align 8
  %call_res.1 = call i64 @SepCreateAccessStateFromSubjectContext(i64 %.54, i64 %arg_r8, i64 %arg_r9, i64 %.68)
  ret i64 %call_res.1
}

declare i64 @SeCaptureSubjectContextEx(i64, i64, i64) local_unnamed_addr

declare i64 @SepCreateAccessStateFromSubjectContext(i64, i64, i64, i64) local_unnamed_addr


; ModuleID = '<string>'
source_filename = "<string>"
target triple = "x86_64-pc-windows-msvc"

define noundef i64 @SeQuerySessionIdToken(i64 %arg_rcx, i64 %arg_rdx) local_unnamed_addr {
lift_init:
  %gs_val = tail call i64 @__nt_read_gs_i64(i64 392)
  %.52 = add i64 %gs_val, 484
  %.53 = inttoptr i64 %.52 to ptr
  %mem_ld = load i16, ptr %.53, align 2
  %.55 = add i16 %mem_ld, -1
  store i16 %.55, ptr %.53, align 2
  %.72 = add i64 %arg_rcx, 48
  %.73 = inttoptr i64 %.72 to ptr
  %mem_ld.1 = load i64, ptr %.73, align 4
  %.76 = and i64 %arg_rdx, -256
  %.78 = or disjoint i64 %.76, 1
  %call_res = tail call i64 @ExAcquireResourceSharedLite(i64 %mem_ld.1, i64 %.78, i64 0, i64 0)
  %.81 = add i64 %arg_rcx, 120
  %.82 = inttoptr i64 %.81 to ptr
  %mem_ld.2 = load i32, ptr %.82, align 4
  %.85 = inttoptr i64 %arg_rdx to ptr
  store i32 %mem_ld.2, ptr %.85, align 4
  %mem_ld.3 = load i64, ptr %.73, align 4
  %call_res.1 = tail call i64 @ExReleaseResourceLite(i64 %mem_ld.3, i64 %.78)
  %call_res.2 = tail call i64 @KeLeaveCriticalRegion(i64 %mem_ld.3, i64 %.78, i64 0, i64 0)
  ret i64 0
}

declare i64 @__nt_read_gs_i64(i64) local_unnamed_addr

declare i64 @ExAcquireResourceSharedLite(i64, i64, i64, i64) local_unnamed_addr

declare i64 @ExReleaseResourceLite(i64, i64) local_unnamed_addr

declare i64 @KeLeaveCriticalRegion(i64, i64, i64, i64) local_unnamed_addr


; ModuleID = '<string>'
source_filename = "<string>"
target triple = "x86_64-pc-windows-msvc"

; Function Attrs: mustprogress nofree norecurse nosync nounwind willreturn memory(read, inaccessiblemem: none, target_mem0: none, target_mem1: none)
define range(i64 0, 2) i64 @CmpTryAcquireIXLockExclusive(i64 %arg_rcx) local_unnamed_addr #0 {
lift_init:
  %.24 = inttoptr i64 %arg_rcx to ptr
  %mem_ld = load i32, ptr %.24, align 4
  %.26 = icmp eq i32 %mem_ld, 0
  %.45 = zext i1 %.26 to i64
  ret i64 %.45
}

attributes #0 = { mustprogress nofree norecurse nosync nounwind willreturn memory(read, inaccessiblemem: none, target_mem0: none, target_mem1: none) }


; ModuleID = '<string>'
source_filename = "<string>"
target triple = "x86_64-pc-windows-msvc"

; Function Attrs: mustprogress nofree norecurse nosync nounwind willreturn memory(read, inaccessiblemem: none, target_mem0: none, target_mem1: none)
define range(i64 0, 2) i64 @CmpCommitDiscardReplacePre(i64 %arg_rcx) local_unnamed_addr #0 {
lift_init:
  %.32 = add i64 %arg_rcx, 32
  %.33 = inttoptr i64 %.32 to ptr
  %mem_ld = load i32, ptr %.33, align 4
  %.35 = icmp eq i32 %mem_ld, -1
  %.54 = zext i1 %.35 to i64
  ret i64 %.54
}

attributes #0 = { mustprogress nofree norecurse nosync nounwind willreturn memory(read, inaccessiblemem: none, target_mem0: none, target_mem1: none) }


; ModuleID = '<string>'
source_filename = "<string>"
target triple = "x86_64-pc-windows-msvc"

; Function Attrs: mustprogress nofree norecurse nosync nounwind willreturn memory(read, inaccessiblemem: none, target_mem0: none, target_mem1: none)
define range(i64 0, 256) i64 @CmpGetEffectiveKcbSemantics(i64 %arg_rcx) local_unnamed_addr #0 {
lift_init:
  %.24 = add i64 %arg_rcx, 58
  %.25 = inttoptr i64 %.24 to ptr
  %mem_ld = load i16, ptr %.25, align 2
  %.27.not = icmp eq i16 %mem_ld, 0
  br i1 %.27.not, label %common.ret, label %bb_14068f10a

common.ret:                                       ; preds = %lift_init, %bb_14068f10a
  %common.ret.op = phi i64 [ %.59, %bb_14068f10a ], [ 0, %lift_init ]
  ret i64 %common.ret.op

bb_14068f10a:                                     ; preds = %lift_init
  %.56 = add i64 %arg_rcx, 57
  %.57 = inttoptr i64 %.56 to ptr
  %mem_ld.1 = load i8, ptr %.57, align 1
  %.59 = zext i8 %mem_ld.1 to i64
  br label %common.ret
}

attributes #0 = { mustprogress nofree norecurse nosync nounwind willreturn memory(read, inaccessiblemem: none, target_mem0: none, target_mem1: none) }


; ModuleID = '<string>'
source_filename = "<string>"
target triple = "x86_64-pc-windows-msvc"

; Function Attrs: nofree nosync nounwind memory(readwrite, inaccessiblemem: none, target_mem0: none, target_mem1: none)
define i64 @CmpSetGlobalQuotaAllowed() local_unnamed_addr #0 {
lift_init:
  %addr_CmpGlobalQuota = tail call i64 @__nt_sym_addr_CmpGlobalQuota()
  %.23 = inttoptr i64 %addr_CmpGlobalQuota to ptr
  %mem_ld = load i64, ptr %.23, align 4
  %addr_CmpGlobalQuotaAllowed = tail call i64 @__nt_sym_addr_CmpGlobalQuotaAllowed()
  %.25 = inttoptr i64 %addr_CmpGlobalQuotaAllowed to ptr
  store i64 %mem_ld, ptr %.25, align 4
  ret i64 %mem_ld
}

; Function Attrs: nofree nosync nounwind memory(none)
declare i64 @__nt_sym_addr_CmpGlobalQuota() local_unnamed_addr #1

; Function Attrs: nofree nosync nounwind memory(none)
declare i64 @__nt_sym_addr_CmpGlobalQuotaAllowed() local_unnamed_addr #1

attributes #0 = { nofree nosync nounwind memory(readwrite, inaccessiblemem: none, target_mem0: none, target_mem1: none) }
attributes #1 = { nofree nosync nounwind memory(none) }


; ModuleID = '<string>'
source_filename = "<string>"
target triple = "x86_64-pc-windows-msvc"

; Function Attrs: nofree nosync nounwind memory(read, inaccessiblemem: none, target_mem0: none, target_mem1: none)
define range(i64 0, 2) i64 @CmpTransSilentIgnore() local_unnamed_addr #0 {
lift_init:
  %addr_CmpMiniNTBoot = tail call i64 @__nt_sym_addr_CmpMiniNTBoot()
  %.23 = inttoptr i64 %addr_CmpMiniNTBoot to ptr
  %mem_ld = load i8, ptr %.23, align 1
  %.25.not = icmp ne i8 %mem_ld, 0
  %. = zext i1 %.25.not to i64
  ret i64 %.
}

; Function Attrs: nofree nosync nounwind memory(none)
declare i64 @__nt_sym_addr_CmpMiniNTBoot() local_unnamed_addr #1

attributes #0 = { nofree nosync nounwind memory(read, inaccessiblemem: none, target_mem0: none, target_mem1: none) }
attributes #1 = { nofree nosync nounwind memory(none) }


; ModuleID = '<string>'
source_filename = "<string>"
target triple = "x86_64-pc-windows-msvc"

; Function Attrs: mustprogress nofree norecurse nosync nounwind willreturn memory(write, inaccessiblemem: none, target_mem0: none, target_mem1: none)
define noundef i64 @CmGetCallbackVersion(i64 %arg_rcx, i64 %arg_rdx) local_unnamed_addr #0 {
lift_init:
  %.28 = icmp eq i64 %arg_rcx, 0
  br i1 %.28, label %bb_14068051c, label %bb_14068051a

bb_14068051a:                                     ; preds = %lift_init
  %.39 = inttoptr i64 %arg_rcx to ptr
  store i32 1, ptr %.39, align 4
  br label %bb_14068051c

bb_14068051c:                                     ; preds = %bb_14068051a, %lift_init
  %.43 = icmp eq i64 %arg_rdx, 0
  br i1 %.43, label %bb_140680523, label %bb_140680521

bb_140680521:                                     ; preds = %bb_14068051c
  %.54 = inttoptr i64 %arg_rdx to ptr
  store i32 1, ptr %.54, align 4
  br label %bb_140680523

bb_140680523:                                     ; preds = %bb_140680521, %bb_14068051c
  ret i64 1
}

attributes #0 = { mustprogress nofree norecurse nosync nounwind willreturn memory(write, inaccessiblemem: none, target_mem0: none, target_mem1: none) }


; ModuleID = '<string>'
source_filename = "<string>"
target triple = "x86_64-pc-windows-msvc"

; Function Attrs: mustprogress nofree norecurse nosync nounwind willreturn memory(read, inaccessiblemem: none, target_mem0: none, target_mem1: none)
define range(i64 0, -1) i64 @CmGetBoundTransaction(i64 %arg_rcx, i64 %arg_rdx) local_unnamed_addr #0 {
lift_init:
  %.26 = icmp eq i64 %arg_rcx, 0
  br i1 %.26, label %bb_14057e9c2, label %bb_14057e9b5

bb_14057e9b5:                                     ; preds = %lift_init
  %.37 = add i64 %arg_rdx, 56
  %.38 = inttoptr i64 %.37 to ptr
  %mem_ld = load i64, ptr %.38, align 4
  %.406 = and i64 %mem_ld, 1
  %.41.not = icmp eq i64 %.406, 0
  br i1 %.41.not, label %common.ret, label %bb_14057e9c2

common.ret:                                       ; preds = %bb_14057e9b5, %bb_14057e9c2
  %common.ret.op = phi i64 [ 0, %bb_14057e9c2 ], [ %mem_ld, %bb_14057e9b5 ]
  ret i64 %common.ret.op

bb_14057e9c2:                                     ; preds = %bb_14057e9b5, %lift_init
  br label %common.ret
}

attributes #0 = { mustprogress nofree norecurse nosync nounwind willreturn memory(read, inaccessiblemem: none, target_mem0: none, target_mem1: none) }


; ModuleID = '<string>'
source_filename = "<string>"
target triple = "x86_64-pc-windows-msvc"

; Function Attrs: mustprogress nofree norecurse nosync nounwind willreturn memory(read, inaccessiblemem: none, target_mem0: none, target_mem1: none)
define range(i64 0, 2) i64 @CmpFlushNotifiesPreCallback(i64 %arg_rcx, i64 %arg_rdx) local_unnamed_addr #0 {
lift_init:
  %.25 = add i64 %arg_rcx, 32
  %.26 = inttoptr i64 %.25 to ptr
  %mem_ld = load i32, ptr %.26, align 4
  %.28 = icmp eq i32 %mem_ld, -1
  br i1 %.28, label %bb_14068f09f, label %bb_14068f096

bb_14068f096:                                     ; preds = %lift_init
  %.45 = add i64 %arg_rdx, 4
  %.46 = inttoptr i64 %.45 to ptr
  %mem_ld.1 = load i8, ptr %.46, align 1
  %.48 = icmp eq i8 %mem_ld.1, 0
  br i1 %.48, label %bb_14068f09f, label %common.ret

common.ret:                                       ; preds = %bb_14068f096, %bb_14068f09f
  %common.ret.op = phi i64 [ 1, %bb_14068f09f ], [ 0, %bb_14068f096 ]
  ret i64 %common.ret.op

bb_14068f09f:                                     ; preds = %bb_14068f096, %lift_init
  br label %common.ret
}

attributes #0 = { mustprogress nofree norecurse nosync nounwind willreturn memory(read, inaccessiblemem: none, target_mem0: none, target_mem1: none) }


; ModuleID = '<string>'
source_filename = "<string>"
target triple = "x86_64-pc-windows-msvc"

; Function Attrs: nofree nosync nounwind memory(readwrite, inaccessiblemem: none, target_mem0: none, target_mem1: none)
define noundef i64 @CmpValidateGlobalFlushControlFlags() local_unnamed_addr #0 {
lift_init:
  %addr_CmpGlobalFlushControlFlags = tail call i64 @__nt_sym_addr_CmpGlobalFlushControlFlags()
  %.23 = inttoptr i64 %addr_CmpGlobalFlushControlFlags to ptr
  %mem_ld = load i32, ptr %.23, align 4
  %.25 = icmp ugt i32 %mem_ld, 1
  br i1 %.25, label %bb_1405de78d, label %common.ret

common.ret:                                       ; preds = %lift_init, %bb_1405de78d
  ret i64 0

bb_1405de78d:                                     ; preds = %lift_init
  store i32 0, ptr %.23, align 4
  br label %common.ret
}

; Function Attrs: nofree nosync nounwind memory(none)
declare i64 @__nt_sym_addr_CmpGlobalFlushControlFlags() local_unnamed_addr #1

attributes #0 = { nofree nosync nounwind memory(readwrite, inaccessiblemem: none, target_mem0: none, target_mem1: none) }
attributes #1 = { nofree nosync nounwind memory(none) }


; ModuleID = '<string>'
source_filename = "<string>"
target triple = "x86_64-pc-windows-msvc"

define i64 @CmpValueEnumStackGetCurrentValueHive(i64 %arg_rcx) local_unnamed_addr {
lift_init:
  %.38 = add i64 %arg_rcx, 6
  %.39 = inttoptr i64 %.38 to ptr
  %mem_ld = load i16, ptr %.39, align 2
  %.41 = zext i16 %mem_ld to i64
  %call_res = tail call i64 @CmpSortedValueEnumStackGetEntryAtLayerHeight(i64 %arg_rcx, i64 %.41)
  %.44 = inttoptr i64 %call_res to ptr
  %mem_ld.1 = load i64, ptr %.44, align 4
  ret i64 %mem_ld.1
}

declare i64 @CmpSortedValueEnumStackGetEntryAtLayerHeight(i64, i64) local_unnamed_addr


; ModuleID = '<string>'
source_filename = "<string>"
target triple = "x86_64-pc-windows-msvc"

; Function Attrs: mustprogress nofree norecurse nosync nounwind willreturn memory(read, inaccessiblemem: none, target_mem0: none, target_mem1: none)
define range(i64 0, 4294967296) i64 @CmpComputeKcbConvKey(i64 %arg_rcx) local_unnamed_addr #0 {
lift_init:
  %.24 = add i64 %arg_rcx, 64
  %.25 = inttoptr i64 %.24 to ptr
  %mem_ld = load i64, ptr %.25, align 4
  %.28 = icmp eq i64 %mem_ld, 0
  br i1 %.28, label %bb_1406868e5, label %bb_1406868d9

common.ret:                                       ; preds = %bb_1406868e5, %bb_1406868d9
  %common.ret.op.in = phi i32 [ %.49, %bb_1406868d9 ], [ %mem_ld.5, %bb_1406868e5 ]
  %common.ret.op = zext i32 %common.ret.op.in to i64
  ret i64 %common.ret.op

bb_1406868d9:                                     ; preds = %lift_init
  %.39 = add i64 %mem_ld, 8
  %.40 = inttoptr i64 %.39 to ptr
  %mem_ld.1 = load i32, ptr %.40, align 4
  %.41 = mul i32 %mem_ld.1, 37
  %.44 = add i64 %arg_rcx, 72
  %.45 = inttoptr i64 %.44 to ptr
  %mem_ld.2 = load i64, ptr %.45, align 4
  %.47 = add i64 %mem_ld.2, 8
  %.48 = inttoptr i64 %.47 to ptr
  %mem_ld.3 = load i32, ptr %.48, align 4
  %.49 = add i32 %mem_ld.3, %.41
  br label %common.ret

bb_1406868e5:                                     ; preds = %lift_init
  %.67 = add i64 %arg_rcx, 72
  %.68 = inttoptr i64 %.67 to ptr
  %mem_ld.4 = load i64, ptr %.68, align 4
  %.70 = add i64 %mem_ld.4, 8
  %.71 = inttoptr i64 %.70 to ptr
  %mem_ld.5 = load i32, ptr %.71, align 4
  br label %common.ret
}

attributes #0 = { mustprogress nofree norecurse nosync nounwind willreturn memory(read, inaccessiblemem: none, target_mem0: none, target_mem1: none) }


; ModuleID = '<string>'
source_filename = "<string>"
target triple = "x86_64-pc-windows-msvc"

define noundef i64 @CmpInitializeKcbStack(i64 %arg_rcx) local_unnamed_addr {
lift_init:
  %call_res = tail call i64 @memset(i64 %arg_rcx, i64 0, i64 32, i64 0)
  %.66 = add i64 %arg_rcx, 2
  %.67 = inttoptr i64 %.66 to ptr
  store i16 -1, ptr %.67, align 2
  ret i64 4294967295
}

declare i64 @memset(i64, i64, i64, i64) local_unnamed_addr


; ModuleID = '<string>'
source_filename = "<string>"
target triple = "x86_64-pc-windows-msvc"

; Function Attrs: mustprogress nofree norecurse nosync nounwind willreturn memory(read, inaccessiblemem: none, target_mem0: none, target_mem1: none)
define i64 @CmpGetComponentNameAtIndex(i64 %arg_rcx, i64 %arg_rdx) local_unnamed_addr #0 {
lift_init:
  %0 = and i64 %arg_rdx, 4294967288
  %.30.not = icmp eq i64 %0, 0
  br i1 %.30.not, label %bb_140568a61, label %bb_140568a6f

common.ret:                                       ; preds = %bb_140568a6f, %bb_140568a61
  %common.ret.op = phi i64 [ %.70, %bb_140568a61 ], [ %.117, %bb_140568a6f ]
  ret i64 %common.ret.op

bb_140568a61:                                     ; preds = %lift_init
  %.44 = shl i64 %arg_rdx, 4
  %.46 = and i64 %.44, 112
  %.62 = add i64 %arg_rcx, 32
  %.70 = add i64 %.62, %.46
  br label %common.ret

bb_140568a6f:                                     ; preds = %lift_init
  %1 = trunc i64 %arg_rdx to i32
  %.88 = add i32 %1, -8
  %.89 = zext i32 %.88 to i64
  %.91 = shl nuw nsw i64 %.89, 4
  %.107 = add nuw nsw i64 %.91, 96
  %.115 = add i64 %arg_rcx, 160
  %.116 = inttoptr i64 %.115 to ptr
  %mem_ld = load i64, ptr %.116, align 4
  %.117 = add i64 %.107, %mem_ld
  br label %common.ret
}

attributes #0 = { mustprogress nofree norecurse nosync nounwind willreturn memory(read, inaccessiblemem: none, target_mem0: none, target_mem1: none) }


; ModuleID = '<string>'
source_filename = "<string>"
target triple = "x86_64-pc-windows-msvc"

; Function Attrs: mustprogress nofree norecurse nosync nounwind willreturn memory(read, inaccessiblemem: none, target_mem0: none, target_mem1: none)
define i64 @CmpKeyNodeStackGetEntryAtLayerHeight(i64 %arg_rcx, i64 %arg_rdx) local_unnamed_addr #0 {
lift_init:
  %r_dx = trunc i64 %arg_rdx to i16
  %.25 = add i16 %r_dx, -2
  %.28 = icmp slt i16 %.25, 0
  %.32 = icmp slt i16 %r_dx, 0
  %.33 = xor i16 %.25, %r_dx
  %.34 = icmp slt i16 %.33, 0
  %.35 = and i1 %.32, %.34
  %.42 = xor i1 %.28, %.35
  br i1 %.42, label %bb_14068f1da, label %bb_14068f1ca

common.ret:                                       ; preds = %bb_14068f1da, %bb_14068f1ca
  %common.ret.op = phi i64 [ %.62, %bb_14068f1ca ], [ %.105, %bb_14068f1da ]
  ret i64 %common.ret.op

bb_14068f1ca:                                     ; preds = %lift_init
  %.48 = sext i16 %.25 to i64
  %.52 = shl nsw i64 %.48, 5
  %.60 = add i64 %arg_rcx, 72
  %.61 = inttoptr i64 %.60 to ptr
  %mem_ld = load i64, ptr %.61, align 4
  %.62 = add i64 %mem_ld, %.52
  br label %common.ret

bb_14068f1da:                                     ; preds = %lift_init
  %.79 = sext i16 %r_dx to i64
  %.83 = shl nsw i64 %.79, 5
  %.91 = add i64 %arg_rcx, 8
  %.105 = add i64 %.91, %.83
  br label %common.ret
}

attributes #0 = { mustprogress nofree norecurse nosync nounwind willreturn memory(read, inaccessiblemem: none, target_mem0: none, target_mem1: none) }


; ModuleID = '<string>'
source_filename = "<string>"
target triple = "x86_64-pc-windows-msvc"

; Function Attrs: mustprogress nofree norecurse nosync nounwind willreturn memory(read, inaccessiblemem: none, target_mem0: none, target_mem1: none)
define i64 @CmpSortedValueEnumStackGetEntryAtLayerHeight(i64 %arg_rcx, i64 %arg_rdx) local_unnamed_addr #0 {
lift_init:
  %r_dx = trunc i64 %arg_rdx to i16
  %.25 = add i16 %r_dx, -2
  %.28 = icmp slt i16 %.25, 0
  %.32 = icmp slt i16 %r_dx, 0
  %.33 = xor i16 %.25, %r_dx
  %.34 = icmp slt i16 %.33, 0
  %.35 = and i1 %.32, %.34
  %.42 = xor i1 %.28, %.35
  br i1 %.42, label %bb_140693726, label %bb_140693716

common.ret:                                       ; preds = %bb_140693726, %bb_140693716
  %common.ret.op = phi i64 [ %.62, %bb_140693716 ], [ %.105, %bb_140693726 ]
  ret i64 %common.ret.op

bb_140693716:                                     ; preds = %lift_init
  %.48 = sext i16 %.25 to i64
  %.52 = shl nsw i64 %.48, 5
  %.60 = add i64 %arg_rcx, 80
  %.61 = inttoptr i64 %.60 to ptr
  %mem_ld = load i64, ptr %.61, align 4
  %.62 = add i64 %mem_ld, %.52
  br label %common.ret

bb_140693726:                                     ; preds = %lift_init
  %.79 = sext i16 %r_dx to i64
  %.83 = shl nsw i64 %.79, 5
  %.91 = add i64 %arg_rcx, 16
  %.105 = add i64 %.91, %.83
  br label %common.ret
}

attributes #0 = { mustprogress nofree norecurse nosync nounwind willreturn memory(read, inaccessiblemem: none, target_mem0: none, target_mem1: none) }


; ModuleID = '<string>'
source_filename = "<string>"
target triple = "x86_64-pc-windows-msvc"

; Function Attrs: mustprogress nofree norecurse nosync nounwind willreturn memory(readwrite, inaccessiblemem: none, target_mem0: none, target_mem1: none)
define range(i64 0, 33554178) i64 @CmpSetRespectIoPriorityThread(i64 %arg_rcx, i64 %arg_rdx) local_unnamed_addr #0 {
lift_init:
  %.25 = add i64 %arg_rcx, 1748
  %.26 = inttoptr i64 %.25 to ptr
  %mem_ld = load i32, ptr %.26, align 4
  %0 = trunc i64 %arg_rdx to i32
  %.31 = shl i32 %0, 7
  %.56 = lshr i32 %mem_ld, 7
  %.8412 = and i32 %.56, 33554177
  %.84 = zext nneg i32 %.8412 to i64
  %1 = and i32 %.31, 128
  %2 = and i32 %mem_ld, -129
  %.86 = or disjoint i32 %2, %1
  store i32 %.86, ptr %.26, align 4
  ret i64 %.84
}

attributes #0 = { mustprogress nofree norecurse nosync nounwind willreturn memory(readwrite, inaccessiblemem: none, target_mem0: none, target_mem1: none) }


; ModuleID = '<string>'
source_filename = "<string>"
target triple = "x86_64-pc-windows-msvc"

define range(i64 0, 4294967296) i64 @CmpFindValueByName(i64 %arg_rcx, i64 %arg_rdx) local_unnamed_addr {
lift_init:
  %stack_frame = alloca [1024 x i8], align 16
  %.40 = getelementptr inbounds nuw i8, ptr %stack_frame, i64 784
  %.41 = ptrtoint ptr %.40 to i64
  %.43 = add i64 %arg_rdx, 36
  %.58 = getelementptr inbounds nuw i8, ptr %stack_frame, i64 752
  store i64 %.41, ptr %.58, align 16
  %.70 = getelementptr inbounds nuw i8, ptr %stack_frame, i64 744
  store i64 0, ptr %.70, align 8
  %call_res = call i64 @CmpFindNameInList(i64 %arg_rcx, i64 %.43, i64 0, i64 0)
  %mem_ld.1 = load i32, ptr %.40, align 16
  %.87 = zext i32 %mem_ld.1 to i64
  ret i64 %.87
}

declare i64 @CmpFindNameInList(i64, i64, i64, i64) local_unnamed_addr


; ModuleID = '<string>'
source_filename = "<string>"
target triple = "x86_64-pc-windows-msvc"

define i64 @CmpInitializeDiscardReplaceContext(i64 %arg_rcx) local_unnamed_addr {
lift_init:
  %call_res = tail call i64 @memset(i64 %arg_rcx, i64 0, i64 32, i64 0)
  %.57 = add i64 %arg_rcx, 16
  %.59 = add i64 %arg_rcx, 8
  %.60 = inttoptr i64 %.59 to ptr
  store i32 -1073741823, ptr %.60, align 4
  %.62 = add i64 %arg_rcx, 24
  %.63 = inttoptr i64 %.62 to ptr
  store i64 %.57, ptr %.63, align 4
  %.65 = inttoptr i64 %.57 to ptr
  store i64 %.57, ptr %.65, align 4
  ret i64 %.57
}

declare i64 @memset(i64, i64, i64, i64) local_unnamed_addr


; ModuleID = '<string>'
source_filename = "<string>"
target triple = "x86_64-pc-windows-msvc"

; Function Attrs: nofree nosync nounwind memory(readwrite, inaccessiblemem: none, target_mem0: none, target_mem1: none)
define i64 @CmpInitializeFreezeThaw() local_unnamed_addr #0 {
lift_init:
  %addr_CmpFreezeListLock = tail call i64 @__nt_sym_addr_CmpFreezeListLock()
  %.23 = inttoptr i64 %addr_CmpFreezeListLock to ptr
  store i64 0, ptr %.23, align 4
  %addr_CmpFreezeThawWaitListHead = tail call i64 @__nt_sym_addr_CmpFreezeThawWaitListHead()
  %addr_CmFreezeThawTimeoutInSeconds = tail call i64 @__nt_sym_addr_CmFreezeThawTimeoutInSeconds()
  %.34 = inttoptr i64 %addr_CmFreezeThawTimeoutInSeconds to ptr
  %mem_ld.1 = load i32, ptr %.34, align 4
  %addr___nt_data_14037ee48 = tail call i64 @__nt_sym_addr___nt_data_14037ee48()
  %.48 = inttoptr i64 %addr___nt_data_14037ee48 to ptr
  store i64 %addr_CmpFreezeThawWaitListHead, ptr %.48, align 4
  %.50 = inttoptr i64 %addr_CmpFreezeThawWaitListHead to ptr
  store i64 %addr_CmpFreezeThawWaitListHead, ptr %.50, align 4
  %.58 = icmp ugt i32 %mem_ld.1, 900
  br i1 %.58, label %bb_14067df74, label %common.ret

common.ret:                                       ; preds = %lift_init, %bb_14067df74
  ret i64 %addr_CmpFreezeThawWaitListHead

bb_14067df74:                                     ; preds = %lift_init
  store i32 60, ptr %.34, align 4
  br label %common.ret
}

; Function Attrs: nofree nosync nounwind memory(none)
declare i64 @__nt_sym_addr_CmpFreezeListLock() local_unnamed_addr #1

; Function Attrs: nofree nosync nounwind memory(none)
declare i64 @__nt_sym_addr_CmpFreezeThawWaitListHead() local_unnamed_addr #1

; Function Attrs: nofree nosync nounwind memory(none)
declare i64 @__nt_sym_addr_CmFreezeThawTimeoutInSeconds() local_unnamed_addr #1

; Function Attrs: nofree nosync nounwind memory(none)
declare i64 @__nt_sym_addr___nt_data_14037ee48() local_unnamed_addr #1

attributes #0 = { nofree nosync nounwind memory(readwrite, inaccessiblemem: none, target_mem0: none, target_mem1: none) }
attributes #1 = { nofree nosync nounwind memory(none) }


; ModuleID = '<string>'
source_filename = "<string>"
target triple = "x86_64-pc-windows-msvc"

define i64 @CmpSystemQuotaWarningWorker() local_unnamed_addr {
lift_init:
  %stack_frame = alloca [1024 x i8], align 16
  %call_res = tail call i64 @ExFreePoolWithTag(i64 0, i64 0, i64 0, i64 0)
  %.47 = getelementptr inbounds nuw i8, ptr %stack_frame, i64 784
  %.48 = ptrtoint ptr %.47 to i64
  %.59 = getelementptr inbounds nuw i8, ptr %stack_frame, i64 752
  store i64 %.48, ptr %.59, align 16
  %.79 = getelementptr inbounds nuw i8, ptr %stack_frame, i64 744
  store i32 1, ptr %.79, align 8
  %call_res.1 = call i64 @ExRaiseHardError(i64 3221226070, i64 0, i64 0, i64 0)
  ret i64 %call_res.1
}

declare i64 @ExFreePoolWithTag(i64, i64, i64, i64) local_unnamed_addr

declare i64 @ExRaiseHardError(i64, i64, i64, i64) local_unnamed_addr


; ModuleID = '<string>'
source_filename = "<string>"
target triple = "x86_64-pc-windows-msvc"

define i64 @CmpArmDelayDerefKCBWorker() local_unnamed_addr {
lift_init:
  %stack_frame = alloca [1024 x i8], align 16
  %.23 = getelementptr inbounds nuw i8, ptr %stack_frame, i64 768
  %.24 = ptrtoint ptr %.23 to i64
  %.48 = add i64 %.24, -24
  %.50 = add i64 %.24, -16
  %.51 = inttoptr i64 %.50 to ptr
  %addr_CmpDelayDerefKCBTimer = tail call i64 @__nt_sym_addr_CmpDelayDerefKCBTimer()
  store i64 -1, ptr %.51, align 16
  %.76 = inttoptr i64 %.48 to ptr
  store i64 0, ptr %.76, align 8
  %call_res = call i64 @KeSetTimer2(i64 %addr_CmpDelayDerefKCBTimer, i64 -10000000, i64 0, i64 %.48)
  ret i64 %call_res
}

; Function Attrs: nofree nosync nounwind memory(none)
declare i64 @__nt_sym_addr_CmpDelayDerefKCBTimer() local_unnamed_addr #0

declare i64 @KeSetTimer2(i64, i64, i64, i64) local_unnamed_addr

attributes #0 = { nofree nosync nounwind memory(none) }


; ModuleID = '<string>'
source_filename = "<string>"
target triple = "x86_64-pc-windows-msvc"

; Function Attrs: mustprogress nofree norecurse nosync nounwind willreturn memory(none)
define noundef i64 @CcTestControl() local_unnamed_addr #0 {
lift_init:
  ret i64 3221225474
}

attributes #0 = { mustprogress nofree norecurse nosync nounwind willreturn memory(none) }


; ModuleID = '<string>'
source_filename = "<string>"
target triple = "x86_64-pc-windows-msvc"

; Function Attrs: mustprogress nofree norecurse nosync nounwind willreturn memory(readwrite, inaccessiblemem: none, target_mem0: none, target_mem1: none)
define i64 @CcSetReadAheadGranularity(i64 %arg_rcx, i64 %arg_rdx) local_unnamed_addr #0 {
lift_init:
  %.25 = add i64 %arg_rcx, 48
  %.26 = inttoptr i64 %.25 to ptr
  %mem_ld = load i64, ptr %.26, align 4
  %r_edx = trunc i64 %arg_rdx to i32
  %.29 = add i32 %r_edx, -1
  %.45 = add i64 %mem_ld, 4
  %.46 = inttoptr i64 %.45 to ptr
  store i32 %.29, ptr %.46, align 4
  ret i64 %mem_ld
}

attributes #0 = { mustprogress nofree norecurse nosync nounwind willreturn memory(readwrite, inaccessiblemem: none, target_mem0: none, target_mem1: none) }


; ModuleID = '<string>'
source_filename = "<string>"
target triple = "x86_64-pc-windows-msvc"

; Function Attrs: mustprogress nofree norecurse nosync nounwind willreturn memory(write, inaccessiblemem: none, target_mem0: none, target_mem1: none)
define i64 @CcMapAndRead(i64 %arg_rcx, i64 %arg_rdx, i64 %arg_r8) local_unnamed_addr #0 {
lift_init:
  %stack_frame = alloca [1024 x i8], align 16
  %.26 = getelementptr inbounds nuw i8, ptr %stack_frame, i64 768
  %.27 = ptrtoint ptr %.26 to i64
  %r_r8b = trunc i64 %arg_r8 to i8
  %.29 = add i64 %.27, 24
  %.30 = inttoptr i64 %.29 to ptr
  store i8 %r_r8b, ptr %.30, align 8
  ret i64 %.27
}

attributes #0 = { mustprogress nofree norecurse nosync nounwind willreturn memory(write, inaccessiblemem: none, target_mem0: none, target_mem1: none) }


; ModuleID = '<string>'
source_filename = "<string>"
target triple = "x86_64-pc-windows-msvc"

; Function Attrs: mustprogress nofree norecurse nosync nounwind willreturn memory(read, inaccessiblemem: none, target_mem0: none, target_mem1: none)
define range(i64 0, -15) i64 @CcGetFileObjectFromBcb(i64 %arg_rcx) local_unnamed_addr #0 {
lift_init:
  %.24 = add i64 %arg_rcx, 176
  %.25 = inttoptr i64 %.24 to ptr
  %mem_ld = load i64, ptr %.25, align 4
  %.27 = add i64 %mem_ld, 96
  %.28 = inttoptr i64 %.27 to ptr
  %mem_ld.1 = load i64, ptr %.28, align 4
  %.30 = and i64 %mem_ld.1, -16
  ret i64 %.30
}

attributes #0 = { mustprogress nofree norecurse nosync nounwind willreturn memory(read, inaccessiblemem: none, target_mem0: none, target_mem1: none) }


; ModuleID = '<string>'
source_filename = "<string>"
target triple = "x86_64-pc-windows-msvc"

define range(i64 1, -254) i64 @CcNotifyWriteBehindHelper() local_unnamed_addr {
lift_init:
  %call_res = tail call i64 @CcNotifyWriteBehindInternal(i64 0, i64 0, i64 0, i64 0)
  %.39 = and i64 %call_res, -256
  %.41 = or disjoint i64 %.39, 1
  ret i64 %.41
}

declare i64 @CcNotifyWriteBehindInternal(i64, i64, i64, i64) local_unnamed_addr


; ModuleID = '<string>'
source_filename = "<string>"
target triple = "x86_64-pc-windows-msvc"

; Function Attrs: mustprogress nofree norecurse nosync nounwind willreturn memory(read, inaccessiblemem: none, target_mem0: none, target_mem1: none)
define range(i64 0, 4294967296) i64 @CcGetCachedDirtyPageCountForFile(i64 %arg_rcx) local_unnamed_addr #0 {
lift_init:
  %.34 = icmp eq i64 %arg_rcx, 0
  br i1 %.34, label %bb_1401da5b4, label %bb_1401da5a8

bb_1401da5a8:                                     ; preds = %lift_init
  %.45 = add i64 %arg_rcx, 8
  %.46 = inttoptr i64 %.45 to ptr
  %mem_ld = load i64, ptr %.46, align 4
  %.49 = icmp eq i64 %mem_ld, 0
  br i1 %.49, label %bb_1401da5b4, label %bb_1401da5b1

bb_1401da5b1:                                     ; preds = %bb_1401da5a8
  %.60 = add i64 %mem_ld, 112
  %.61 = inttoptr i64 %.60 to ptr
  %mem_ld.1 = load i32, ptr %.61, align 4
  %.62 = zext i32 %mem_ld.1 to i64
  br label %bb_1401da5b4

bb_1401da5b4:                                     ; preds = %bb_1401da5b1, %bb_1401da5a8, %lift_init
  %reg_rax.0 = phi i64 [ 4294967295, %lift_init ], [ 4294967295, %bb_1401da5a8 ], [ %.62, %bb_1401da5b1 ]
  ret i64 %reg_rax.0
}

attributes #0 = { mustprogress nofree norecurse nosync nounwind willreturn memory(read, inaccessiblemem: none, target_mem0: none, target_mem1: none) }


; ModuleID = '<string>'
source_filename = "<string>"
target triple = "x86_64-pc-windows-msvc"

define i64 @CcFlushCache(i64 %arg_rcx, i64 %arg_rdx, i64 %arg_r8, i64 %arg_r9) local_unnamed_addr {
lift_init:
  %call_res = tail call i64 @CcFlushCachePriv(i64 %arg_rcx, i64 %arg_rdx, i64 %arg_r8, i64 0)
  ret i64 %call_res
}

declare i64 @CcFlushCachePriv(i64, i64, i64, i64) local_unnamed_addr


; ModuleID = '<string>'
source_filename = "<string>"
target triple = "x86_64-pc-windows-msvc"

define i64 @CcFlushCacheToLsn(i64 %arg_rcx, i64 %arg_rdx, i64 %arg_r8) local_unnamed_addr {
lift_init:
  %call_res = tail call i64 @CcFlushCachePriv(i64 %arg_rcx, i64 0, i64 0, i64 %arg_rdx)
  ret i64 %call_res
}

declare i64 @CcFlushCachePriv(i64, i64, i64, i64) local_unnamed_addr


; ModuleID = '<string>'
source_filename = "<string>"
target triple = "x86_64-pc-windows-msvc"

define i64 @CcIncrementVacbActiveCount(i64 %arg_rcx) local_unnamed_addr {
lift_init:
  %.40 = add i64 %arg_rcx, 16
  %.41 = inttoptr i64 %.40 to ptr
  %mem_ld = load i32, ptr %.41, align 4
  %.42 = add i32 %mem_ld, 1
  store i32 %.42, ptr %.41, align 4
  %r_ax = trunc i32 %.42 to i16
  %.86 = icmp eq i16 %r_ax, 0
  br i1 %.86, label %bb_1401aee40, label %bb_1400b6ae3

common.ret:                                       ; preds = %bb_1401aee40, %bb_1400b6ae3
  %common.ret.op = phi i64 [ %.75, %bb_1400b6ae3 ], [ %call_res, %bb_1401aee40 ]
  ret i64 %common.ret.op

bb_1400b6ae3:                                     ; preds = %lift_init
  %.75 = zext i32 %.42 to i64
  br label %common.ret

bb_1401aee40:                                     ; preds = %lift_init
  %call_res = tail call i64 @KeBugCheckEx(i64 52, i64 2479, i64 -1073740768, i64 0)
  br label %common.ret
}

declare i64 @KeBugCheckEx(i64, i64, i64, i64) local_unnamed_addr


; ModuleID = '<string>'
source_filename = "<string>"
target triple = "x86_64-pc-windows-msvc"

; Function Attrs: mustprogress nofree norecurse nosync nounwind willreturn memory(readwrite, inaccessiblemem: none, target_mem0: none, target_mem1: none)
define range(i64 4, 0) i64 @CcSetDirtyPageThreshold(i64 %arg_rcx, i64 %arg_rdx) local_unnamed_addr #0 {
lift_init:
  %.25 = add i64 %arg_rcx, 40
  %.26 = inttoptr i64 %.25 to ptr
  %mem_ld = load i64, ptr %.26, align 4
  %.28 = add i64 %mem_ld, 8
  %.29 = inttoptr i64 %.28 to ptr
  %mem_ld.1 = load i64, ptr %.29, align 4
  %.32 = icmp eq i64 %mem_ld.1, 0
  br i1 %.32, label %bb_1401d9994, label %bb_1401d998d

bb_1401d998d:                                     ; preds = %lift_init
  %r_edx = trunc i64 %arg_rdx to i32
  %.43 = add i64 %mem_ld.1, 264
  %.44 = inttoptr i64 %.43 to ptr
  store i32 %r_edx, ptr %.44, align 4
  br label %bb_1401d9994

bb_1401d9994:                                     ; preds = %bb_1401d998d, %lift_init
  %.47 = add i64 %arg_rcx, 24
  %.48 = inttoptr i64 %.47 to ptr
  %mem_ld.2 = load i64, ptr %.48, align 4
  %.50 = add i64 %mem_ld.2, 4
  %.51 = inttoptr i64 %.50 to ptr
  %mem_ld.3 = load i8, ptr %.51, align 1
  %.57 = and i8 %mem_ld.3, 4
  %.58.not = icmp eq i8 %.57, 0
  br i1 %.58.not, label %bb_1401d999f, label %bb_1401d99a4

bb_1401d999f:                                     ; preds = %bb_1401d9994
  %.70 = or disjoint i8 %mem_ld.3, 4
  store i8 %.70, ptr %.51, align 1
  br label %bb_1401d99a4

bb_1401d99a4:                                     ; preds = %bb_1401d999f, %bb_1401d9994
  %.54.pn.in = phi i8 [ %mem_ld.3, %bb_1401d9994 ], [ %.70, %bb_1401d999f ]
  %.53 = and i64 %mem_ld, -256
  %.54.pn = zext i8 %.54.pn.in to i64
  %reg_rax.0 = or disjoint i64 %.53, %.54.pn
  ret i64 %reg_rax.0
}

attributes #0 = { mustprogress nofree norecurse nosync nounwind willreturn memory(readwrite, inaccessiblemem: none, target_mem0: none, target_mem1: none) }


; ModuleID = '<string>'
source_filename = "<string>"
target triple = "x86_64-pc-windows-msvc"

; Function Attrs: mustprogress nofree norecurse nosync nounwind willreturn memory(readwrite, inaccessiblemem: none, target_mem0: none, target_mem1: none)
define noundef i64 @CcCopyReadExceptionFilter(i64 %arg_rcx, i64 %arg_rdx) local_unnamed_addr #0 {
lift_init:
  %.25 = inttoptr i64 %arg_rcx to ptr
  %mem_ld = load i64, ptr %.25, align 4
  %.27 = inttoptr i64 %mem_ld to ptr
  %mem_ld.1 = load i32, ptr %.27, align 4
  %.30 = inttoptr i64 %arg_rdx to ptr
  store i32 %mem_ld.1, ptr %.30, align 4
  %.33.not = icmp eq i32 %mem_ld.1, -1073741818
  br i1 %.33.not, label %bb_1401d9ed6, label %bb_1401d9ee4

bb_1401d9ed6:                                     ; preds = %lift_init
  %mem_ld.2 = load i64, ptr %.25, align 4
  %.53 = add i64 %mem_ld.2, 24
  %.54 = inttoptr i64 %.53 to ptr
  %mem_ld.3 = load i32, ptr %.54, align 4
  %.60 = icmp ult i32 %mem_ld.3, 3
  br i1 %.60, label %bb_1401d9ee4, label %bb_1401d9edf

bb_1401d9edf:                                     ; preds = %bb_1401d9ed6
  %.73 = add i64 %mem_ld.2, 48
  %.74 = inttoptr i64 %.73 to ptr
  %mem_ld.4 = load i32, ptr %.74, align 4
  store i32 %mem_ld.4, ptr %.30, align 4
  br label %bb_1401d9ee4

bb_1401d9ee4:                                     ; preds = %bb_1401d9edf, %bb_1401d9ed6, %lift_init
  ret i64 1
}

attributes #0 = { mustprogress nofree norecurse nosync nounwind willreturn memory(readwrite, inaccessiblemem: none, target_mem0: none, target_mem1: none) }


; ModuleID = '<string>'
source_filename = "<string>"
target triple = "x86_64-pc-windows-msvc"

define range(i64 0, 4294967296) i64 @CcWaitForCurrentLazyWriterActivity() local_unnamed_addr {
lift_init:
  %stack_frame = alloca [1024 x i8], align 16
  %.38 = getelementptr inbounds nuw i8, ptr %stack_frame, i64 776
  store i32 0, ptr %.38, align 8
  %.53 = ptrtoint ptr %.38 to i64
  %addr___nt_rodata_14013f350 = tail call i64 @__nt_sym_addr___nt_rodata_14013f350()
  %call_res = call i64 @CcForEachPartition(i64 %addr___nt_rodata_14013f350, i64 %.53, i64 0, i64 0)
  %mem_ld.1 = load i32, ptr %.38, align 8
  %.68 = zext i32 %mem_ld.1 to i64
  ret i64 %.68
}

; Function Attrs: nofree nosync nounwind memory(none)
declare i64 @__nt_sym_addr___nt_rodata_14013f350() local_unnamed_addr #0

declare i64 @CcForEachPartition(i64, i64, i64, i64) local_unnamed_addr

attributes #0 = { nofree nosync nounwind memory(none) }


; ModuleID = '<string>'
source_filename = "<string>"
target triple = "x86_64-pc-windows-msvc"

define i64 @CcFastCopyWrite(i64 %arg_rcx, i64 %arg_rdx, i64 %arg_r8, i64 %arg_r9) local_unnamed_addr {
lift_init:
  %stack_frame = alloca [1024 x i8], align 16
  %.27 = getelementptr inbounds nuw i8, ptr %stack_frame, i64 768
  %.28 = ptrtoint ptr %.27 to i64
  %.44 = add i64 %.28, -32
  %.45 = inttoptr i64 %.44 to ptr
  store i64 0, ptr %.45, align 16
  %.56 = and i64 %arg_rdx, 4294967295
  %.58 = add i64 %.28, -24
  %.60 = add i64 %.28, -40
  %.61 = inttoptr i64 %.60 to ptr
  store i64 %arg_r9, ptr %.61, align 8
  %.64 = and i64 %arg_r9, -256
  %.66 = or disjoint i64 %.64, 1
  %.69 = inttoptr i64 %.58 to ptr
  store i64 %.56, ptr %.69, align 8
  %call_res = call i64 @CcCopyWriteEx(i64 %arg_rcx, i64 %.58, i64 %arg_r8, i64 %.66)
  ret i64 %call_res
}

declare i64 @CcCopyWriteEx(i64, i64, i64, i64) local_unnamed_addr


; ModuleID = '<string>'
source_filename = "<string>"
target triple = "x86_64-pc-windows-msvc"

; Function Attrs: mustprogress nofree norecurse nosync nounwind willreturn memory(read, inaccessiblemem: none, target_mem0: none, target_mem1: none)
define i64 @CcGetPartition(i64 %arg_rcx) local_unnamed_addr #0 {
lift_init:
  %.47 = add i64 %arg_rcx, 168
  %.48 = inttoptr i64 %.47 to ptr
  %mem_ld = load i64, ptr %.48, align 4
  %.50 = icmp eq i64 %mem_ld, 0
  br i1 %.50, label %bb_14005f13a, label %bb_14005f08f

common.ret:                                       ; preds = %bb_14005f13a, %bb_14005f08f
  %common.ret.op = phi i64 [ %.73, %bb_14005f08f ], [ %mem_ld.1, %bb_14005f13a ]
  ret i64 %common.ret.op

bb_14005f08f:                                     ; preds = %lift_init
  %.70 = add i64 %arg_rcx, 96
  %.71 = inttoptr i64 %.70 to ptr
  %mem_ld.2 = load i64, ptr %.71, align 4
  %.73 = and i64 %mem_ld.2, -16
  br label %common.ret

bb_14005f13a:                                     ; preds = %lift_init
  %.62 = add i64 %arg_rcx, 528
  %.63 = inttoptr i64 %.62 to ptr
  %mem_ld.1 = load i64, ptr %.63, align 4
  br label %common.ret
}

attributes #0 = { mustprogress nofree norecurse nosync nounwind willreturn memory(read, inaccessiblemem: none, target_mem0: none, target_mem1: none) }


; ModuleID = '<string>'
source_filename = "<string>"
target triple = "x86_64-pc-windows-msvc"

define i64 @CcIsThereDirtyData(i64 %arg_rcx) local_unnamed_addr {
lift_init:
  %stack_frame = alloca [1024 x i8], align 16
  %.39 = getelementptr inbounds nuw i8, ptr %stack_frame, i64 744
  store i64 %arg_rcx, ptr %.39, align 8
  %.44 = ptrtoint ptr %.39 to i64
  %addr___nt_rodata_1401da7a0 = tail call i64 @__nt_sym_addr___nt_rodata_1401da7a0()
  %.48 = getelementptr inbounds nuw i8, ptr %stack_frame, i64 752
  store i8 0, ptr %.48, align 16
  %call_res = call i64 @CcForEachPartition(i64 %addr___nt_rodata_1401da7a0, i64 %.44, i64 1, i64 0)
  %mem_ld = load i8, ptr %.48, align 16
  %.59 = and i64 %call_res, -256
  %.60 = zext i8 %mem_ld to i64
  %.61 = or disjoint i64 %.59, %.60
  ret i64 %.61
}

; Function Attrs: nofree nosync nounwind memory(none)
declare i64 @__nt_sym_addr___nt_rodata_1401da7a0() local_unnamed_addr #0

declare i64 @CcForEachPartition(i64, i64, i64, i64) local_unnamed_addr

attributes #0 = { nofree nosync nounwind memory(none) }


; ModuleID = '<string>'
source_filename = "<string>"
target triple = "x86_64-pc-windows-msvc"

; Function Attrs: mustprogress nofree norecurse nosync nounwind willreturn memory(none)
define noundef i64 @FsRtlInitializeOplock() local_unnamed_addr #0 {
lift_init:
  ret i64 0
}

attributes #0 = { mustprogress nofree norecurse nosync nounwind willreturn memory(none) }


; ModuleID = '<string>'
source_filename = "<string>"
target triple = "x86_64-pc-windows-msvc"

; Function Attrs: mustprogress nofree norecurse nosync nounwind willreturn memory(none)
define noundef i64 @FsRtlSyncVolumes() local_unnamed_addr #0 {
lift_init:
  ret i64 0
}

attributes #0 = { mustprogress nofree norecurse nosync nounwind willreturn memory(none) }


; ModuleID = '<string>'
source_filename = "<string>"
target triple = "x86_64-pc-windows-msvc"

; Function Attrs: mustprogress nofree norecurse nosync nounwind willreturn memory(readwrite, inaccessiblemem: none, target_mem0: none, target_mem1: none)
define noundef i64 @FsRtlAcknowledgeEcp(i64 %arg_rcx) local_unnamed_addr #0 {
lift_init:
  %.24 = add i64 %arg_rcx, -24
  %.25 = inttoptr i64 %.24 to ptr
  %mem_ld = load i32, ptr %.25, align 4
  %.26 = or i32 %mem_ld, 8
  store i32 %.26, ptr %.25, align 4
  ret i64 0
}

attributes #0 = { mustprogress nofree norecurse nosync nounwind willreturn memory(readwrite, inaccessiblemem: none, target_mem0: none, target_mem1: none) }


; ModuleID = '<string>'
source_filename = "<string>"
target triple = "x86_64-pc-windows-msvc"

; Function Attrs: mustprogress nofree norecurse nosync nounwind willreturn memory(readwrite, inaccessiblemem: none, target_mem0: none, target_mem1: none)
define noundef i64 @FsRtlPrepareToReuseEcp(i64 %arg_rcx) local_unnamed_addr #0 {
lift_init:
  %.24 = add i64 %arg_rcx, -24
  %.25 = inttoptr i64 %.24 to ptr
  %mem_ld = load i32, ptr %.25, align 4
  %.26 = and i32 %mem_ld, -9
  store i32 %.26, ptr %.25, align 4
  ret i64 0
}

attributes #0 = { mustprogress nofree norecurse nosync nounwind willreturn memory(readwrite, inaccessiblemem: none, target_mem0: none, target_mem1: none) }


; ModuleID = '<string>'
source_filename = "<string>"
target triple = "x86_64-pc-windows-msvc"

; Function Attrs: mustprogress nofree norecurse nosync nounwind willreturn memory(write, inaccessiblemem: none, target_mem0: none, target_mem1: none)
define noundef i64 @FsRtlResetBaseMcb(i64 %arg_rcx) local_unnamed_addr #0 {
lift_init:
  %.24 = add i64 %arg_rcx, 4
  %.25 = inttoptr i64 %.24 to ptr
  store i32 0, ptr %.25, align 4
  ret i64 0
}

attributes #0 = { mustprogress nofree norecurse nosync nounwind willreturn memory(write, inaccessiblemem: none, target_mem0: none, target_mem1: none) }


; ModuleID = '<string>'
source_filename = "<string>"
target triple = "x86_64-pc-windows-msvc"

; Function Attrs: nofree nosync nounwind memory(read, inaccessiblemem: none, target_mem0: none, target_mem1: none)
define range(i64 0, 256) i64 @FsRtlAreVolumeStartupApplicationsComplete() local_unnamed_addr #0 {
lift_init:
  %addr_FsRtlpVolumeStartupApplicationsComplete = tail call i64 @__nt_sym_addr_FsRtlpVolumeStartupApplicationsComplete()
  %.23 = inttoptr i64 %addr_FsRtlpVolumeStartupApplicationsComplete to ptr
  %mem_ld = load i8, ptr %.23, align 1
  %.26 = zext i8 %mem_ld to i64
  ret i64 %.26
}

; Function Attrs: nofree nosync nounwind memory(none)
declare i64 @__nt_sym_addr_FsRtlpVolumeStartupApplicationsComplete() local_unnamed_addr #1

attributes #0 = { nofree nosync nounwind memory(read, inaccessiblemem: none, target_mem0: none, target_mem1: none) }
attributes #1 = { nofree nosync nounwind memory(none) }


; ModuleID = '<string>'
source_filename = "<string>"
target triple = "x86_64-pc-windows-msvc"

; Function Attrs: nofree nosync nounwind memory(write, inaccessiblemem: none, target_mem0: none, target_mem1: none)
define noundef i64 @FsRtlRegisterFltMgrCalls(i64 %arg_rcx) local_unnamed_addr #0 {
lift_init:
  %addr_FltMgrCallbacks = tail call i64 @__nt_sym_addr_FltMgrCallbacks()
  %.24 = inttoptr i64 %addr_FltMgrCallbacks to ptr
  store i64 %arg_rcx, ptr %.24, align 4
  ret i64 0
}

; Function Attrs: nofree nosync nounwind memory(none)
declare i64 @__nt_sym_addr_FltMgrCallbacks() local_unnamed_addr #1

attributes #0 = { nofree nosync nounwind memory(write, inaccessiblemem: none, target_mem0: none, target_mem1: none) }
attributes #1 = { nofree nosync nounwind memory(none) }


; ModuleID = '<string>'
source_filename = "<string>"
target triple = "x86_64-pc-windows-msvc"

; Function Attrs: nofree nosync nounwind memory(write, inaccessiblemem: none, target_mem0: none, target_mem1: none)
define noundef i64 @FsRtlRegisterMupCalls(i64 %arg_rcx) local_unnamed_addr #0 {
lift_init:
  %addr_pFsRtlpMupCalls = tail call i64 @__nt_sym_addr_pFsRtlpMupCalls()
  %.24 = inttoptr i64 %addr_pFsRtlpMupCalls to ptr
  store i64 %arg_rcx, ptr %.24, align 4
  ret i64 0
}

; Function Attrs: nofree nosync nounwind memory(none)
declare i64 @__nt_sym_addr_pFsRtlpMupCalls() local_unnamed_addr #1

attributes #0 = { nofree nosync nounwind memory(write, inaccessiblemem: none, target_mem0: none, target_mem1: none) }
attributes #1 = { nofree nosync nounwind memory(none) }


; ModuleID = '<string>'
source_filename = "<string>"
target triple = "x86_64-pc-windows-msvc"

; Function Attrs: mustprogress nofree norecurse nosync nounwind willreturn memory(read, inaccessiblemem: none, target_mem0: none, target_mem1: none)
define range(i64 0, 536870658) i64 @FsRtlIsEcpAcknowledged(i64 %arg_rcx) local_unnamed_addr #0 {
lift_init:
  %.24 = add i64 %arg_rcx, -24
  %.25 = inttoptr i64 %.24 to ptr
  %mem_ld = load i32, ptr %.25, align 4
  %.30 = lshr i32 %mem_ld, 3
  %.494 = and i32 %.30, 536870657
  %.49 = zext nneg i32 %.494 to i64
  ret i64 %.49
}

attributes #0 = { mustprogress nofree norecurse nosync nounwind willreturn memory(read, inaccessiblemem: none, target_mem0: none, target_mem1: none) }


; ModuleID = '<string>'
source_filename = "<string>"
target triple = "x86_64-pc-windows-msvc"

; Function Attrs: mustprogress nofree norecurse nosync nounwind willreturn memory(read, inaccessiblemem: none, target_mem0: none, target_mem1: none)
define range(i64 0, 268435202) i64 @FsRtlIsEcpFromUserMode(i64 %arg_rcx) local_unnamed_addr #0 {
lift_init:
  %.24 = add i64 %arg_rcx, -24
  %.25 = inttoptr i64 %.24 to ptr
  %mem_ld = load i32, ptr %.25, align 4
  %.30 = lshr i32 %mem_ld, 4
  %.494 = and i32 %.30, 268435201
  %.49 = zext nneg i32 %.494 to i64
  ret i64 %.49
}

attributes #0 = { mustprogress nofree norecurse nosync nounwind willreturn memory(read, inaccessiblemem: none, target_mem0: none, target_mem1: none) }


; ModuleID = '<string>'
source_filename = "<string>"
target triple = "x86_64-pc-windows-msvc"

; Function Attrs: mustprogress nofree norecurse nosync nounwind willreturn memory(read, inaccessiblemem: none, target_mem0: none, target_mem1: none)
define i64 @FsRtlLookupReservedPerFileContext(i64 %arg_rcx) local_unnamed_addr #0 {
lift_init:
  %.24 = inttoptr i64 %arg_rcx to ptr
  %mem_ld = load i64, ptr %.24, align 4
  %.27.not = icmp eq i64 %mem_ld, 0
  br i1 %.27.not, label %common.ret, label %bb_140007499

common.ret:                                       ; preds = %lift_init, %bb_140007499
  %common.ret.op = phi i64 [ %mem_ld.1, %bb_140007499 ], [ 0, %lift_init ]
  ret i64 %common.ret.op

bb_140007499:                                     ; preds = %lift_init
  %.42 = add i64 %mem_ld, 24
  %.43 = inttoptr i64 %.42 to ptr
  %mem_ld.1 = load i64, ptr %.43, align 4
  br label %common.ret
}

attributes #0 = { mustprogress nofree norecurse nosync nounwind willreturn memory(read, inaccessiblemem: none, target_mem0: none, target_mem1: none) }


; ModuleID = '<string>'
source_filename = "<string>"
target triple = "x86_64-pc-windows-msvc"

define range(i64 1, -254) i64 @FsRtlMdlReadCompleteDev() local_unnamed_addr {
lift_init:
  %call_res = tail call i64 @CcMdlReadComplete2(i64 0, i64 0, i64 0, i64 0)
  %.39 = and i64 %call_res, -256
  %.41 = or disjoint i64 %.39, 1
  ret i64 %.41
}

declare i64 @CcMdlReadComplete2(i64, i64, i64, i64) local_unnamed_addr


; ModuleID = '<string>'
source_filename = "<string>"
target triple = "x86_64-pc-windows-msvc"

; Function Attrs: mustprogress nofree norecurse nosync nounwind willreturn memory(write, inaccessiblemem: none, target_mem0: none, target_mem1: none)
define noundef i64 @FsRtlInitializeFileLock(i64 %arg_rcx, i64 %arg_rdx, i64 %arg_r8) local_unnamed_addr #0 {
lift_init:
  %.34 = inttoptr i64 %arg_rcx to ptr
  store i64 %arg_rdx, ptr %.34, align 4
  %.36 = add i64 %arg_rcx, 24
  %.37 = inttoptr i64 %.36 to ptr
  store i64 0, ptr %.37, align 4
  %.39 = add i64 %arg_rcx, 16
  %.40 = inttoptr i64 %.39 to ptr
  store i8 0, ptr %.40, align 1
  %.42 = add i64 %arg_rcx, 8
  %.43 = inttoptr i64 %.42 to ptr
  store i64 %arg_r8, ptr %.43, align 4
  %.45 = add i64 %arg_rcx, 88
  %.46 = inttoptr i64 %.45 to ptr
  store i32 0, ptr %.46, align 4
  ret i64 0
}

attributes #0 = { mustprogress nofree norecurse nosync nounwind willreturn memory(write, inaccessiblemem: none, target_mem0: none, target_mem1: none) }


; ModuleID = '<string>'
source_filename = "<string>"
target triple = "x86_64-pc-windows-msvc"

define i64 @FsRtlAddMcbEntry(i64 %arg_rcx, i64 %arg_rdx, i64 %arg_r8, i64 %arg_r9) local_unnamed_addr {
lift_init:
  %.41 = and i64 %arg_r9, 4294967295
  %.43 = and i64 %arg_r8, 4294967295
  %.45 = and i64 %arg_rdx, 4294967295
  %call_res = tail call i64 @FsRtlAddLargeMcbEntry(i64 %arg_rcx, i64 %.45, i64 %.43, i64 %.41)
  ret i64 %call_res
}

declare i64 @FsRtlAddLargeMcbEntry(i64, i64, i64, i64) local_unnamed_addr


; ModuleID = '<string>'
source_filename = "<string>"
target triple = "x86_64-pc-windows-msvc"

; Function Attrs: mustprogress nofree norecurse nosync nounwind willreturn memory(readwrite, inaccessiblemem: none, target_mem0: none, target_mem1: none)
define range(i64 0, 3221225486) i64 @FsRtlGetEcpListFromIrp(i64 %arg_rcx, i64 %arg_rdx) local_unnamed_addr #0 {
lift_init:
  %.25 = add i64 %arg_rcx, 16
  %.26 = inttoptr i64 %.25 to ptr
  %mem_ld = load i32, ptr %.26, align 4
  %0 = and i32 %mem_ld, 128
  %.32.not = icmp eq i32 %0, 0
  br i1 %.32.not, label %common.ret, label %bb_1404f2cf7

common.ret:                                       ; preds = %lift_init, %bb_1404f2cf7
  %common.ret.op = phi i64 [ 0, %bb_1404f2cf7 ], [ 3221225485, %lift_init ]
  ret i64 %common.ret.op

bb_1404f2cf7:                                     ; preds = %lift_init
  %.42 = add i64 %arg_rcx, 112
  %.43 = inttoptr i64 %.42 to ptr
  %mem_ld.1 = load i64, ptr %.43, align 4
  %.45 = inttoptr i64 %arg_rdx to ptr
  store i64 %mem_ld.1, ptr %.45, align 4
  br label %common.ret
}

attributes #0 = { mustprogress nofree norecurse nosync nounwind willreturn memory(readwrite, inaccessiblemem: none, target_mem0: none, target_mem1: none) }


; ModuleID = '<string>'
source_filename = "<string>"
target triple = "x86_64-pc-windows-msvc"

; Function Attrs: mustprogress nofree norecurse nosync nounwind willreturn memory(read, inaccessiblemem: none, target_mem0: none, target_mem1: none)
define range(i64 0, 2) i64 @FsRtlAreThereWaitingFileLocks(i64 %arg_rcx) local_unnamed_addr #0 {
lift_init:
  %.33 = icmp eq i64 %arg_rcx, 0
  br i1 %.33, label %bb_1401e0c57, label %bb_1401e0c47

bb_1401e0c47:                                     ; preds = %lift_init
  %.44 = add i64 %arg_rcx, 24
  %.45 = inttoptr i64 %.44 to ptr
  %mem_ld = load i64, ptr %.45, align 4
  %.48 = icmp eq i64 %mem_ld, 0
  br i1 %.48, label %bb_1401e0c57, label %bb_1401e0c50

bb_1401e0c50:                                     ; preds = %bb_1401e0c47
  %.59 = add i64 %mem_ld, 48
  %.60 = inttoptr i64 %.59 to ptr
  %mem_ld.1 = load i64, ptr %.60, align 4
  %.62 = icmp ne i64 %mem_ld.1, 0
  %.82 = zext i1 %.62 to i64
  br label %bb_1401e0c57

bb_1401e0c57:                                     ; preds = %bb_1401e0c50, %bb_1401e0c47, %lift_init
  %reg_rax.0 = phi i64 [ 0, %lift_init ], [ 0, %bb_1401e0c47 ], [ %.82, %bb_1401e0c50 ]
  ret i64 %reg_rax.0
}

attributes #0 = { mustprogress nofree norecurse nosync nounwind willreturn memory(read, inaccessiblemem: none, target_mem0: none, target_mem1: none) }


; ModuleID = '<string>'
source_filename = "<string>"
target triple = "x86_64-pc-windows-msvc"

; Function Attrs: mustprogress nofree norecurse nosync nounwind willreturn memory(readwrite, inaccessiblemem: none, target_mem0: none, target_mem1: none)
define i64 @FsRtlRemoveReservedPerFileContext(i64 %arg_rcx) local_unnamed_addr #0 {
lift_init:
  %.24 = inttoptr i64 %arg_rcx to ptr
  %mem_ld = load i64, ptr %.24, align 4
  %.27.not = icmp eq i64 %mem_ld, 0
  br i1 %.27.not, label %common.ret, label %bb_1401e241b

common.ret:                                       ; preds = %bb_1401e241b, %bb_1401e2424, %lift_init
  %common.ret.op = phi i64 [ 0, %lift_init ], [ %mem_ld.1, %bb_1401e2424 ], [ 0, %bb_1401e241b ]
  ret i64 %common.ret.op

bb_1401e241b:                                     ; preds = %lift_init
  %.50 = add i64 %mem_ld, 24
  %.51 = inttoptr i64 %.50 to ptr
  %mem_ld.1 = load i64, ptr %.51, align 4
  %.54 = icmp eq i64 %mem_ld.1, 0
  br i1 %.54, label %common.ret, label %bb_1401e2424

bb_1401e2424:                                     ; preds = %bb_1401e241b
  store i64 0, ptr %.51, align 4
  br label %common.ret
}

attributes #0 = { mustprogress nofree norecurse nosync nounwind willreturn memory(readwrite, inaccessiblemem: none, target_mem0: none, target_mem1: none) }


; ModuleID = '<string>'
source_filename = "<string>"
target triple = "x86_64-pc-windows-msvc"

define i64 @FsRtlFastUnlockAll(i64 %arg_rcx, i64 %arg_rdx, i64 %arg_r8, i64 %arg_r9) local_unnamed_addr {
lift_init:
  %call_res = tail call i64 @FsRtlPrivateFastUnlockAll(i64 %arg_rcx, i64 %arg_rdx, i64 %arg_r8, i64 0)
  ret i64 %call_res
}

declare i64 @FsRtlPrivateFastUnlockAll(i64, i64, i64, i64) local_unnamed_addr


; ModuleID = '<string>'
source_filename = "<string>"
target triple = "x86_64-pc-windows-msvc"

; Function Attrs: nofree nosync nounwind memory(readwrite, inaccessiblemem: none, target_mem0: none, target_mem1: none)
define i64 @FsRtlAllocateResource() local_unnamed_addr #0 {
lift_init:
  %addr_FsRtlPagingIoResourceSelector = tail call i64 @__nt_sym_addr_FsRtlPagingIoResourceSelector()
  %.23 = inttoptr i64 %addr_FsRtlPagingIoResourceSelector to ptr
  %mem_ld = load i32, ptr %.23, align 4
  %.28 = and i32 %mem_ld, 15
  %.38 = add i32 %mem_ld, 1
  %narrow = mul nuw nsw i32 %.28, 104
  %.54 = zext nneg i32 %narrow to i64
  store i32 %.38, ptr %.23, align 4
  %addr_FsRtlPagingIoResources = tail call i64 @__nt_sym_addr_FsRtlPagingIoResources()
  %.58 = inttoptr i64 %addr_FsRtlPagingIoResources to ptr
  %mem_ld.1 = load i64, ptr %.58, align 4
  %.59 = add i64 %mem_ld.1, %.54
  ret i64 %.59
}

; Function Attrs: nofree nosync nounwind memory(none)
declare i64 @__nt_sym_addr_FsRtlPagingIoResourceSelector() local_unnamed_addr #1

; Function Attrs: nofree nosync nounwind memory(none)
declare i64 @__nt_sym_addr_FsRtlPagingIoResources() local_unnamed_addr #1

attributes #0 = { nofree nosync nounwind memory(readwrite, inaccessiblemem: none, target_mem0: none, target_mem1: none) }
attributes #1 = { nofree nosync nounwind memory(none) }


; ModuleID = '<string>'
source_filename = "<string>"
target triple = "x86_64-pc-windows-msvc"

; Function Attrs: mustprogress nofree norecurse nosync nounwind willreturn memory(read, inaccessiblemem: none, target_mem0: none, target_mem1: none)
define range(i64 0, 4294967042) i64 @FsRtlIsDaxVolume(i64 %arg_rcx) local_unnamed_addr #0 {
lift_init:
  %.24 = add i64 %arg_rcx, 16
  %.25 = inttoptr i64 %.24 to ptr
  %mem_ld = load i64, ptr %.25, align 4
  %.28 = icmp eq i64 %mem_ld, 0
  br i1 %.28, label %bb_1401e04be, label %bb_1401e04a9

bb_1401e04a9:                                     ; preds = %lift_init
  %.39 = add i64 %mem_ld, 8
  %.40 = inttoptr i64 %.39 to ptr
  %mem_ld.1 = load i64, ptr %.40, align 4
  %.43 = icmp eq i64 %mem_ld.1, 0
  br i1 %.43, label %bb_1401e04be, label %bb_1401e04b2

bb_1401e04b2:                                     ; preds = %bb_1401e04a9
  %.54 = add i64 %mem_ld.1, 48
  %.55 = inttoptr i64 %.54 to ptr
  %mem_ld.2 = load i32, ptr %.55, align 4
  %.56 = zext i32 %mem_ld.2 to i64
  %0 = and i32 %mem_ld.2, 268435456
  %.61.not = icmp eq i32 %0, 0
  br i1 %.61.not, label %bb_1401e04be, label %bb_1401e04bb

common.ret:                                       ; preds = %bb_1401e04be, %bb_1401e04bb
  %common.ret.op = phi i64 [ %.72, %bb_1401e04bb ], [ %.78, %bb_1401e04be ]
  ret i64 %common.ret.op

bb_1401e04bb:                                     ; preds = %bb_1401e04b2
  %.70 = and i64 %.56, 4294967040
  %.72 = or disjoint i64 %.70, 1
  br label %common.ret

bb_1401e04be:                                     ; preds = %bb_1401e04b2, %bb_1401e04a9, %lift_init
  %reg_rax.0 = phi i64 [ 0, %lift_init ], [ 0, %bb_1401e04a9 ], [ %.56, %bb_1401e04b2 ]
  %.78 = and i64 %reg_rax.0, 4294967040
  br label %common.ret
}

attributes #0 = { mustprogress nofree norecurse nosync nounwind willreturn memory(read, inaccessiblemem: none, target_mem0: none, target_mem1: none) }


; ModuleID = '<string>'
source_filename = "<string>"
target triple = "x86_64-pc-windows-msvc"

; Function Attrs: mustprogress nofree norecurse nosync nounwind willreturn memory(readwrite, inaccessiblemem: none, target_mem0: none, target_mem1: none)
define range(i64 0, 3221225713) i64 @FsRtlSetEcpListIntoIrp(i64 %arg_rcx, i64 %arg_rdx) local_unnamed_addr #0 {
lift_init:
  %.25 = add i64 %arg_rcx, 16
  %.26 = inttoptr i64 %.25 to ptr
  %mem_ld = load i32, ptr %.26, align 4
  %0 = and i32 %mem_ld, 128
  %.32.not = icmp eq i32 %0, 0
  br i1 %.32.not, label %common.ret, label %bb_1405527d7

bb_1405527d7:                                     ; preds = %lift_init
  %.42 = add i64 %arg_rcx, 112
  %.43 = inttoptr i64 %.42 to ptr
  %mem_ld.1 = load i64, ptr %.43, align 4
  %.45.not = icmp eq i64 %mem_ld.1, 0
  br i1 %.45.not, label %bb_1405527de, label %common.ret

common.ret:                                       ; preds = %bb_1405527d7, %lift_init, %bb_1405527de
  %common.ret.op = phi i64 [ 0, %bb_1405527de ], [ 3221225711, %lift_init ], [ 3221225712, %bb_1405527d7 ]
  ret i64 %common.ret.op

bb_1405527de:                                     ; preds = %bb_1405527d7
  store i64 %arg_rdx, ptr %.43, align 4
  br label %common.ret
}

attributes #0 = { mustprogress nofree norecurse nosync nounwind willreturn memory(readwrite, inaccessiblemem: none, target_mem0: none, target_mem1: none) }


; ModuleID = '<string>'
source_filename = "<string>"
target triple = "x86_64-pc-windows-msvc"

; Function Attrs: mustprogress nofree norecurse nosync nounwind willreturn memory(none)
define range(i64 0, 2) i64 @FsRtlIsTotalDeviceFailure(i64 %arg_rcx) local_unnamed_addr #0 {
lift_init:
  %r_ecx = trunc i64 %arg_rcx to i32
  %.27 = icmp slt i32 %r_ecx, 0
  %.38 = icmp ne i32 %r_ecx, -1073741761
  %or.cond.not12 = and i1 %.27, %.38
  %.56 = icmp ne i32 %r_ecx, -1073741668
  %or.cond9.not11 = and i1 %.56, %or.cond.not12
  %.74 = icmp ne i32 %r_ecx, -1073740688
  %or.cond10.not = and i1 %.74, %or.cond9.not11
  %common.ret.op = zext i1 %or.cond10.not to i64
  ret i64 %common.ret.op
}

attributes #0 = { mustprogress nofree norecurse nosync nounwind willreturn memory(none) }


; ModuleID = '<string>'
source_filename = "<string>"
target triple = "x86_64-pc-windows-msvc"

; Function Attrs: mustprogress nofree norecurse nosync nounwind willreturn memory(read, inaccessiblemem: none, target_mem0: none, target_mem1: none)
define range(i64 0, -254) i64 @FsRtlAreThereCurrentOrInProgressFileLocks(i64 %arg_rcx) local_unnamed_addr #0 {
lift_init:
  %.24 = add i64 %arg_rcx, 24
  %.25 = inttoptr i64 %.24 to ptr
  %mem_ld = load i64, ptr %.25, align 4
  %.36 = icmp eq i64 %mem_ld, 0
  br i1 %.36, label %bb_1401e0c27, label %bb_1401e0c1b

bb_1401e0c1b:                                     ; preds = %lift_init
  %.47 = add i64 %mem_ld, 32
  %.48 = inttoptr i64 %.47 to ptr
  %mem_ld.1 = load i64, ptr %.48, align 4
  %.50.not = icmp eq i64 %mem_ld.1, 0
  br i1 %.50.not, label %bb_1401e0c21, label %bb_1401e0c30

bb_1401e0c21:                                     ; preds = %bb_1401e0c1b
  %.68 = add i64 %mem_ld, 40
  %.69 = inttoptr i64 %.68 to ptr
  %mem_ld.2 = load i64, ptr %.69, align 4
  %.71.not = icmp eq i64 %mem_ld.2, 0
  br i1 %.71.not, label %bb_1401e0c27, label %bb_1401e0c30

bb_1401e0c27:                                     ; preds = %bb_1401e0c21, %lift_init
  %.89 = add i64 %arg_rcx, 88
  %.90 = inttoptr i64 %.89 to ptr
  %mem_ld.3 = load i32, ptr %.90, align 4
  %.91 = zext i32 %mem_ld.3 to i64
  %.94 = icmp ne i32 %mem_ld.3, 0
  %spec.select = zext i1 %.94 to i8
  br label %bb_1401e0c30

bb_1401e0c30:                                     ; preds = %bb_1401e0c27, %bb_1401e0c1b, %bb_1401e0c21
  %reg_rdx.0.off0 = phi i8 [ %spec.select, %bb_1401e0c27 ], [ 1, %bb_1401e0c21 ], [ 1, %bb_1401e0c1b ]
  %reg_rax.1 = phi i64 [ %.91, %bb_1401e0c27 ], [ %mem_ld, %bb_1401e0c21 ], [ %mem_ld, %bb_1401e0c1b ]
  %.112 = and i64 %reg_rax.1, -256
  %.113 = zext nneg i8 %reg_rdx.0.off0 to i64
  %.114 = or disjoint i64 %.112, %.113
  ret i64 %.114
}

attributes #0 = { mustprogress nofree norecurse nosync nounwind willreturn memory(read, inaccessiblemem: none, target_mem0: none, target_mem1: none) }


; ModuleID = '<string>'
source_filename = "<string>"
target triple = "x86_64-pc-windows-msvc"

; Function Attrs: mustprogress nofree norecurse nosync nounwind willreturn memory(write, inaccessiblemem: none, target_mem0: none, target_mem1: none)
define range(i64 0, 3221225486) i64 @FsRtlInitializeExtraCreateParameterList(i64 %arg_rcx) local_unnamed_addr #0 {
lift_init:
  %.25 = icmp eq i64 %arg_rcx, 0
  br i1 %.25, label %common.ret, label %bb_1405d9195

common.ret:                                       ; preds = %lift_init, %bb_1405d9195
  %common.ret.op = phi i64 [ 0, %bb_1405d9195 ], [ 3221225485, %lift_init ]
  ret i64 %common.ret.op

bb_1405d9195:                                     ; preds = %lift_init
  %.36 = add i64 %arg_rcx, 8
  %.38 = add i64 %arg_rcx, 16
  %.39 = inttoptr i64 %.38 to ptr
  store i64 %.36, ptr %.39, align 4
  %.41 = inttoptr i64 %.36 to ptr
  store i64 %.36, ptr %.41, align 4
  %.43 = add i64 %arg_rcx, 4
  %.44 = inttoptr i64 %.43 to ptr
  store i32 0, ptr %.44, align 4
  %.63 = inttoptr i64 %arg_rcx to ptr
  store i32 1282433861, ptr %.63, align 4
  br label %common.ret
}

attributes #0 = { mustprogress nofree norecurse nosync nounwind willreturn memory(write, inaccessiblemem: none, target_mem0: none, target_mem1: none) }


; ModuleID = '<string>'
source_filename = "<string>"
target triple = "x86_64-pc-windows-msvc"

; Function Attrs: mustprogress nofree norecurse nosync nounwind willreturn memory(readwrite, inaccessiblemem: none, target_mem0: none, target_mem1: none)
define noundef i64 @PoEndDeviceBusy(i64 %arg_rcx) local_unnamed_addr #0 {
lift_init:
  %.24 = add i64 %arg_rcx, 8
  %.25 = inttoptr i64 %.24 to ptr
  %mem_ld = load i32, ptr %.25, align 4
  %.27 = add i32 %mem_ld, -1
  store i32 %.27, ptr %.25, align 4
  ret i64 0
}

attributes #0 = { mustprogress nofree norecurse nosync nounwind willreturn memory(readwrite, inaccessiblemem: none, target_mem0: none, target_mem1: none) }


; ModuleID = '<string>'
source_filename = "<string>"
target triple = "x86_64-pc-windows-msvc"

; Function Attrs: mustprogress nofree norecurse nosync nounwind willreturn memory(readwrite, inaccessiblemem: none, target_mem0: none, target_mem1: none)
define noundef i64 @PoSetDeviceBusyEx(i64 %arg_rcx) local_unnamed_addr #0 {
lift_init:
  %.24 = add i64 %arg_rcx, 4
  %.25 = inttoptr i64 %.24 to ptr
  %mem_ld = load i32, ptr %.25, align 4
  %.27 = add i32 %mem_ld, 1
  store i32 %.27, ptr %.25, align 4
  ret i64 0
}

attributes #0 = { mustprogress nofree norecurse nosync nounwind willreturn memory(readwrite, inaccessiblemem: none, target_mem0: none, target_mem1: none) }


; ModuleID = '<string>'
source_filename = "<string>"
target triple = "x86_64-pc-windows-msvc"

; Function Attrs: nofree nosync nounwind memory(read, inaccessiblemem: none, target_mem0: none, target_mem1: none)
define range(i64 0, 256) i64 @PoEnergyEstimationEnabled() local_unnamed_addr #0 {
lift_init:
  %addr_PopEnergyEstimationEnabled = tail call i64 @__nt_sym_addr_PopEnergyEstimationEnabled()
  %.23 = inttoptr i64 %addr_PopEnergyEstimationEnabled to ptr
  %mem_ld = load i8, ptr %.23, align 1
  %.26 = zext i8 %mem_ld to i64
  ret i64 %.26
}

; Function Attrs: nofree nosync nounwind memory(none)
declare i64 @__nt_sym_addr_PopEnergyEstimationEnabled() local_unnamed_addr #1

attributes #0 = { nofree nosync nounwind memory(read, inaccessiblemem: none, target_mem0: none, target_mem1: none) }
attributes #1 = { nofree nosync nounwind memory(none) }


; ModuleID = '<string>'
source_filename = "<string>"
target triple = "x86_64-pc-windows-msvc"

; Function Attrs: nofree nosync nounwind memory(readwrite, inaccessiblemem: none, target_mem0: none, target_mem1: none)
define noundef i64 @PoSetFixedWakeSource(i64 %arg_rcx) local_unnamed_addr #0 {
lift_init:
  %addr_PopFixedWakeSourceMask = tail call i64 @__nt_sym_addr_PopFixedWakeSourceMask()
  %.24 = inttoptr i64 %addr_PopFixedWakeSourceMask to ptr
  %mem_ld = load i32, ptr %.24, align 4
  %r_ecx = trunc i64 %arg_rcx to i32
  %.25 = or i32 %mem_ld, %r_ecx
  store i32 %.25, ptr %.24, align 4
  ret i64 0
}

; Function Attrs: nofree nosync nounwind memory(none)
declare i64 @__nt_sym_addr_PopFixedWakeSourceMask() local_unnamed_addr #1

attributes #0 = { nofree nosync nounwind memory(readwrite, inaccessiblemem: none, target_mem0: none, target_mem1: none) }
attributes #1 = { nofree nosync nounwind memory(none) }


; ModuleID = '<string>'
source_filename = "<string>"
target triple = "x86_64-pc-windows-msvc"

; Function Attrs: mustprogress nofree norecurse nosync nounwind willreturn memory(read, inaccessiblemem: none, target_mem0: none, target_mem1: none)
define range(i64 0, 2) i64 @PoIsRunningStopWatch(i64 %arg_rcx) local_unnamed_addr #0 {
lift_init:
  %.24 = add i64 %arg_rcx, 24
  %.25 = inttoptr i64 %.24 to ptr
  %mem_ld = load i64, ptr %.25, align 4
  %.27 = icmp ne i64 %mem_ld, 0
  %.47 = zext i1 %.27 to i64
  ret i64 %.47
}

attributes #0 = { mustprogress nofree norecurse nosync nounwind willreturn memory(read, inaccessiblemem: none, target_mem0: none, target_mem1: none) }


; ModuleID = '<string>'
source_filename = "<string>"
target triple = "x86_64-pc-windows-msvc"

; Function Attrs: mustprogress nofree norecurse nosync nounwind willreturn memory(readwrite, inaccessiblemem: none, target_mem0: none, target_mem1: none)
define noundef i64 @PoStartDeviceBusy(i64 %arg_rcx) local_unnamed_addr #0 {
lift_init:
  %.24 = add i64 %arg_rcx, 4
  %.25 = inttoptr i64 %.24 to ptr
  %mem_ld = load i32, ptr %.25, align 4
  %.27 = add i32 %mem_ld, 1
  store i32 %.27, ptr %.25, align 4
  %.44 = add i64 %arg_rcx, 8
  %.45 = inttoptr i64 %.44 to ptr
  %mem_ld.1 = load i32, ptr %.45, align 4
  %.47 = add i32 %mem_ld.1, 1
  store i32 %.47, ptr %.45, align 4
  ret i64 0
}

attributes #0 = { mustprogress nofree norecurse nosync nounwind willreturn memory(readwrite, inaccessiblemem: none, target_mem0: none, target_mem1: none) }


; ModuleID = '<string>'
source_filename = "<string>"
target triple = "x86_64-pc-windows-msvc"

; Function Attrs: nofree nosync nounwind memory(write, inaccessiblemem: none, target_mem0: none, target_mem1: none)
define noundef i64 @PopSetShutdownMarker() local_unnamed_addr #0 {
lift_init:
  %addr_PopBsdShutdownInProgress = tail call i64 @__nt_sym_addr_PopBsdShutdownInProgress()
  %.23 = inttoptr i64 %addr_PopBsdShutdownInProgress to ptr
  store i32 1, ptr %.23, align 4
  ret i64 0
}

; Function Attrs: nofree nosync nounwind memory(none)
declare i64 @__nt_sym_addr_PopBsdShutdownInProgress() local_unnamed_addr #1

attributes #0 = { nofree nosync nounwind memory(write, inaccessiblemem: none, target_mem0: none, target_mem1: none) }
attributes #1 = { nofree nosync nounwind memory(none) }


; ModuleID = '<string>'
source_filename = "<string>"
target triple = "x86_64-pc-windows-msvc"

; Function Attrs: mustprogress nofree norecurse nosync nounwind willreturn memory(none)
define range(i64 0, 1073793026) i64 @PopIsUserInitiatedMonitorRequestReason(i64 %arg_rcx) local_unnamed_addr #0 {
lift_init:
  %r_ecx = trunc i64 %arg_rcx to i32
  %.43 = icmp ult i32 %r_ecx, 31
  %.48 = lshr i32 1073793026, %r_ecx
  %.49 = trunc i32 %.48 to i1
  %or.cond = select i1 %.43, i1 %.49, i1 false
  %common.ret.op = select i1 %or.cond, i64 1073793025, i64 0
  ret i64 %common.ret.op
}

attributes #0 = { mustprogress nofree norecurse nosync nounwind willreturn memory(none) }


; ModuleID = '<string>'
source_filename = "<string>"
target triple = "x86_64-pc-windows-msvc"

define noundef i64 @PopExecutionRequiredCallback(i64 %arg_rcx, i64 %arg_rdx) local_unnamed_addr {
lift_init:
  %call_res = tail call i64 @PopHandleConvergedPowerRequestUpdate(i64 %arg_rdx, i64 3, i64 0, i64 0)
  ret i64 0
}

declare i64 @PopHandleConvergedPowerRequestUpdate(i64, i64, i64, i64) local_unnamed_addr


; ModuleID = '<string>'
source_filename = "<string>"
target triple = "x86_64-pc-windows-msvc"

; Function Attrs: nofree nosync nounwind memory(readwrite, inaccessiblemem: none, target_mem0: none, target_mem1: none)
define range(i64 0, 3221225486) i64 @PopIntSteerSetTimeUnparkTrigger(i64 %arg_rcx, i64 %arg_rdx, i64 %arg_r8) local_unnamed_addr #0 {
lift_init:
  %r_r8d = trunc i64 %arg_r8 to i32
  %.27.not = icmp eq i32 %r_r8d, 4
  br i1 %.27.not, label %bb_140141f26, label %common.ret

common.ret:                                       ; preds = %lift_init, %bb_140141f26
  %common.ret.op = phi i64 [ 0, %bb_140141f26 ], [ 3221225485, %lift_init ]
  ret i64 %common.ret.op

bb_140141f26:                                     ; preds = %lift_init
  %.45 = inttoptr i64 %arg_rdx to ptr
  %mem_ld = load i32, ptr %.45, align 4
  %.46 = zext i32 %mem_ld to i64
  %addr_PpmIntSteerTriggerMax = tail call i64 @__nt_sym_addr_PpmIntSteerTriggerMax()
  %.48 = inttoptr i64 %addr_PpmIntSteerTriggerMax to ptr
  store i64 %.46, ptr %.48, align 4
  br label %common.ret
}

; Function Attrs: nofree nosync nounwind memory(none)
declare i64 @__nt_sym_addr_PpmIntSteerTriggerMax() local_unnamed_addr #1

attributes #0 = { nofree nosync nounwind memory(readwrite, inaccessiblemem: none, target_mem0: none, target_mem1: none) }
attributes #1 = { nofree nosync nounwind memory(none) }


; ModuleID = '<string>'
source_filename = "<string>"
target triple = "x86_64-pc-windows-msvc"

define i64 @PoFxIssueComponentPerfStateChange(i64 %arg_rcx, i64 %arg_rdx, i64 %arg_r8, i64 %arg_r9) local_unnamed_addr {
lift_init:
  %call_res = tail call i64 @PoFxIssueComponentPerfStateChangeMultiple(i64 %arg_rcx, i64 %arg_rdx, i64 %arg_r8, i64 1)
  ret i64 %call_res
}

declare i64 @PoFxIssueComponentPerfStateChangeMultiple(i64, i64, i64, i64) local_unnamed_addr


; ModuleID = '<string>'
source_filename = "<string>"
target triple = "x86_64-pc-windows-msvc"

; Function Attrs: mustprogress nofree norecurse nosync nounwind willreturn memory(read, inaccessiblemem: none, target_mem0: none, target_mem1: none)
define range(i64 0, 2) i64 @PoGetThermalRequestSupport(i64 %arg_rcx, i64 %arg_rdx, i64 %arg_r8) local_unnamed_addr #0 {
lift_init:
  %.26 = add i64 %arg_rcx, 32
  %.27 = inttoptr i64 %.26 to ptr
  %mem_ld = load i64, ptr %.27, align 4
  %r_edx = trunc i64 %arg_rdx to i32
  switch i32 %r_edx, label %bb_1406ed572 [
    i32 0, label %bb_1406ed568
    i32 1, label %bb_1406ed56f
  ]

bb_1406ed568:                                     ; preds = %lift_init
  br label %bb_1406ed56f

bb_1406ed56f:                                     ; preds = %lift_init, %bb_1406ed568
  %.sink = phi i64 [ 136, %bb_1406ed568 ], [ 128, %lift_init ]
  %.84 = add i64 %mem_ld, %.sink
  %flg_zf.0.in.in = inttoptr i64 %.84 to ptr
  %flg_zf.0.in = load i64, ptr %flg_zf.0.in.in, align 4
  %flg_zf.0 = icmp ne i64 %flg_zf.0.in, 0
  %.108 = zext i1 %flg_zf.0 to i64
  br label %bb_1406ed572

bb_1406ed572:                                     ; preds = %lift_init, %bb_1406ed56f
  %reg_rax.0 = phi i64 [ %.108, %bb_1406ed56f ], [ 0, %lift_init ]
  ret i64 %reg_rax.0
}

attributes #0 = { mustprogress nofree norecurse nosync nounwind willreturn memory(read, inaccessiblemem: none, target_mem0: none, target_mem1: none) }


; ModuleID = '<string>'
source_filename = "<string>"
target triple = "x86_64-pc-windows-msvc"

define i64 @PopAcquireTransitionLock() local_unnamed_addr {
lift_init:
  %addr_PopTransitionLock = tail call i64 @__nt_sym_addr_PopTransitionLock()
  %call_res = tail call i64 @KeWaitForSingleObject(i64 %addr_PopTransitionLock, i64 7, i64 0, i64 0)
  ret i64 %call_res
}

; Function Attrs: nofree nosync nounwind memory(none)
declare i64 @__nt_sym_addr_PopTransitionLock() local_unnamed_addr #0

declare i64 @KeWaitForSingleObject(i64, i64, i64, i64) local_unnamed_addr

attributes #0 = { nofree nosync nounwind memory(none) }


; ModuleID = '<string>'
source_filename = "<string>"
target triple = "x86_64-pc-windows-msvc"

; Function Attrs: mustprogress nofree norecurse nosync nounwind willreturn memory(read, inaccessiblemem: none, target_mem0: none, target_mem1: none)
define i64 @PoGetSystemWake(i64 %arg_rcx, i64 %arg_rdx, i64 %arg_r8) local_unnamed_addr #0 {
lift_init:
  %.26 = add i64 %arg_rcx, 66
  %.27 = inttoptr i64 %.26 to ptr
  %mem_ld = load i8, ptr %.27, align 1
  %.28 = sext i8 %mem_ld to i32
  %.32 = add nsw i32 %.28, -1
  %.48 = sext i32 %.32 to i64
  %.54 = and i64 %.48, -256
  %.64 = mul nsw i64 %.48, 72
  %.65 = add i64 %arg_rcx, 272
  %.66 = add i64 %.65, %.64
  %.67 = inttoptr i64 %.66 to ptr
  %mem_ld.1 = load i64, ptr %.67, align 4
  %.69 = add i64 %mem_ld.1, 184
  %.70 = inttoptr i64 %.69 to ptr
  %mem_ld.2 = load i8, ptr %.70, align 1
  %.72.not = icmp eq i8 %mem_ld.2, 0
  br i1 %.72.not, label %bb_14014116f, label %bb_140141176

bb_14014116f:                                     ; preds = %lift_init
  %.90 = add i64 %mem_ld.1, 240
  %.91 = inttoptr i64 %.90 to ptr
  %mem_ld.3 = load i8, ptr %.91, align 1
  %.94 = zext i8 %mem_ld.3 to i64
  %.95 = or disjoint i64 %.54, %.94
  br label %bb_140141176

bb_140141176:                                     ; preds = %bb_14014116f, %lift_init
  %reg_rax.0 = phi i64 [ %.54, %lift_init ], [ %.95, %bb_14014116f ]
  ret i64 %reg_rax.0
}

attributes #0 = { mustprogress nofree norecurse nosync nounwind willreturn memory(read, inaccessiblemem: none, target_mem0: none, target_mem1: none) }


; ModuleID = '<string>'
source_filename = "<string>"
target triple = "x86_64-pc-windows-msvc"

define i64 @PoGetProcessorIdleAccounting(i64 %arg_rcx) local_unnamed_addr {
lift_init:
  %gs_val = tail call i64 @__nt_read_gs_i64(i64 32)
  %.26 = add i64 %gs_val, 23816
  %.27 = inttoptr i64 %.26 to ptr
  %mem_ld = load i64, ptr %.27, align 4
  %.30.not = icmp eq i64 %mem_ld, 0
  %.43 = icmp eq i64 %arg_rcx, 0
  br i1 %.30.not, label %bb_140231f25, label %bb_140231f2e

bb_140231f25:                                     ; preds = %lift_init
  br i1 %.43, label %common.ret, label %common.ret.sink.split

common.ret.sink.split:                            ; preds = %bb_140231f25, %bb_140231f33
  %.81.sink = phi i64 [ %.81, %bb_140231f33 ], [ 0, %bb_140231f25 ]
  %common.ret.op.ph = phi i64 [ %.81, %bb_140231f33 ], [ %gs_val, %bb_140231f25 ]
  %.83 = inttoptr i64 %arg_rcx to ptr
  store i64 %.81.sink, ptr %.83, align 4
  br label %common.ret

common.ret:                                       ; preds = %common.ret.sink.split, %bb_140231f25, %bb_140231f2e
  %common.ret.op = phi i64 [ %gs_val, %bb_140231f25 ], [ %gs_val, %bb_140231f2e ], [ %common.ret.op.ph, %common.ret.sink.split ]
  ret i64 %common.ret.op

bb_140231f2e:                                     ; preds = %lift_init
  br i1 %.43, label %common.ret, label %bb_140231f33

bb_140231f33:                                     ; preds = %bb_140231f2e
  %.79 = add i64 %mem_ld, 4
  %.80 = inttoptr i64 %.79 to ptr
  %mem_ld.2 = load i32, ptr %.80, align 4
  %.81 = zext i32 %mem_ld.2 to i64
  br label %common.ret.sink.split
}

declare i64 @__nt_read_gs_i64(i64) local_unnamed_addr


; ModuleID = '<string>'
source_filename = "<string>"
target triple = "x86_64-pc-windows-msvc"

define range(i64 0, 4294967296) i64 @PopGetTransitionsToOnCount() local_unnamed_addr {
lift_init:
  %addr_PopTelemetryOsState = tail call i64 @__nt_sym_addr_PopTelemetryOsState()
  %call_res = tail call i64 @PopAcquireRwLockExclusive(i64 %addr_PopTelemetryOsState, i64 0, i64 0, i64 0)
  %addr___nt_data_14035cd88 = tail call i64 @__nt_sym_addr___nt_data_14035cd88()
  %.44 = inttoptr i64 %addr___nt_data_14035cd88 to ptr
  %mem_ld = load i32, ptr %.44, align 4
  %.45 = zext i32 %mem_ld to i64
  %call_res.1 = tail call i64 @PopReleaseRwLock(i64 %addr_PopTelemetryOsState, i64 0, i64 0, i64 0)
  ret i64 %.45
}

; Function Attrs: nofree nosync nounwind memory(none)
declare i64 @__nt_sym_addr_PopTelemetryOsState() local_unnamed_addr #0

declare i64 @PopAcquireRwLockExclusive(i64, i64, i64, i64) local_unnamed_addr

; Function Attrs: nofree nosync nounwind memory(none)
declare i64 @__nt_sym_addr___nt_data_14035cd88() local_unnamed_addr #0

declare i64 @PopReleaseRwLock(i64, i64, i64, i64) local_unnamed_addr

attributes #0 = { nofree nosync nounwind memory(none) }


; ModuleID = '<string>'
source_filename = "<string>"
target triple = "x86_64-pc-windows-msvc"

define i64 @PopDispatchCallback(i64 %arg_rcx) local_unnamed_addr {
lift_init:
  %.38 = and i64 %arg_rcx, 4294967295
  %addr_ExCbPowerState = tail call i64 @__nt_sym_addr_ExCbPowerState()
  %.48 = inttoptr i64 %addr_ExCbPowerState to ptr
  %mem_ld = load i64, ptr %.48, align 4
  %call_res = tail call i64 @ExNotifyCallback(i64 %mem_ld, i64 %.38, i64 0, i64 0)
  %gs_val = tail call i64 @__nt_read_gs_i64(i64 392)
  ret i64 %gs_val
}

; Function Attrs: nofree nosync nounwind memory(none)
declare i64 @__nt_sym_addr_ExCbPowerState() local_unnamed_addr #0

declare i64 @ExNotifyCallback(i64, i64, i64, i64) local_unnamed_addr

declare i64 @__nt_read_gs_i64(i64) local_unnamed_addr

attributes #0 = { nofree nosync nounwind memory(none) }


; ModuleID = '<string>'
source_filename = "<string>"
target triple = "x86_64-pc-windows-msvc"

define i64 @AlpcpLockForCachedReferenceBlob(i64 %arg_rcx) local_unnamed_addr {
lift_init:
  %.52 = add i64 %arg_rcx, -16
  %call_res = tail call i64 @ExAcquirePushLockExclusiveEx(i64 %.52, i64 0, i64 0, i64 0)
  %.67 = add i64 %arg_rcx, -32
  %.68 = inttoptr i64 %.67 to ptr
  %mem_ld = load i8, ptr %.68, align 1
  %.69 = or i8 %mem_ld, 1
  store i8 %.69, ptr %.68, align 1
  %.83 = add i64 %arg_rcx, -24
  %.84 = inttoptr i64 %.83 to ptr
  %mem_ld.1 = load i64, ptr %.84, align 4
  %.85 = add i64 %mem_ld.1, 65536
  store i64 %.85, ptr %.84, align 4
  %.128 = icmp slt i64 %.85, 1
  br i1 %.128, label %bb_14063439c, label %common.ret

common.ret:                                       ; preds = %lift_init, %bb_14063439c
  %common.ret.op = phi i64 [ %call_res.1, %bb_14063439c ], [ %.85, %lift_init ]
  ret i64 %common.ret.op

bb_14063439c:                                     ; preds = %lift_init
  %call_res.1 = tail call i64 @KeBugCheckEx(i64 24, i64 0, i64 %arg_rcx, i64 38)
  br label %common.ret
}

declare i64 @ExAcquirePushLockExclusiveEx(i64, i64, i64, i64) local_unnamed_addr

declare i64 @KeBugCheckEx(i64, i64, i64, i64) local_unnamed_addr


; ModuleID = '<string>'
source_filename = "<string>"
target triple = "x86_64-pc-windows-msvc"

define range(i64 0, 4097) i64 @AlpcpSetOwnerPortMessage(i64 %arg_rcx, i64 %arg_rdx, i64 %arg_r8) local_unnamed_addr {
lift_init:
  %r_r8b = trunc i64 %arg_r8 to i8
  %.61 = icmp eq i8 %r_r8b, 0
  br i1 %.61, label %bb_14048edbd, label %bb_14048edb5

bb_14048edb5:                                     ; preds = %lift_init
  %call_res = tail call i64 @ObfReferenceObject(i64 %arg_rdx, i64 %arg_rdx, i64 %arg_r8, i64 0)
  br label %bb_14048edbd

bb_14048edbd:                                     ; preds = %bb_14048edb5, %lift_init
  %0 = trunc i64 %arg_r8 to i32
  %.75 = shl i32 %0, 12
  %.93 = add i64 %arg_rcx, 40
  %.94 = inttoptr i64 %.93 to ptr
  %mem_ld.1 = load i32, ptr %.94, align 4
  %.95 = xor i32 %mem_ld.1, %.75
  %.104 = and i32 %.95, 4096
  %.111 = zext nneg i32 %.104 to i64
  %.113 = add i64 %arg_rcx, 24
  %.114 = inttoptr i64 %.113 to ptr
  store i64 %arg_rdx, ptr %.114, align 4
  %mem_ld.2 = load i32, ptr %.94, align 4
  %.118 = xor i32 %mem_ld.2, %.104
  store i32 %.118, ptr %.94, align 4
  ret i64 %.111
}

declare i64 @ObfReferenceObject(i64, i64, i64, i64) local_unnamed_addr


; ModuleID = '<string>'
source_filename = "<string>"
target triple = "x86_64-pc-windows-msvc"

; Function Attrs: mustprogress nofree norecurse nosync nounwind willreturn memory(none)
define noundef i64 @DbgBreakPoint() local_unnamed_addr #0 {
lift_init:
  ret i64 0
}

attributes #0 = { mustprogress nofree norecurse nosync nounwind willreturn memory(none) }


; ModuleID = '<string>'
source_filename = "<string>"
target triple = "x86_64-pc-windows-msvc"

; Function Attrs: mustprogress nofree norecurse nosync nounwind willreturn memory(none)
define noundef i64 @DbgBreakPointWithStatus() local_unnamed_addr #0 {
lift_init:
  ret i64 0
}

attributes #0 = { mustprogress nofree norecurse nosync nounwind willreturn memory(none) }


; ModuleID = '<string>'
source_filename = "<string>"
target triple = "x86_64-pc-windows-msvc"

; Function Attrs: nofree nosync nounwind memory(write, inaccessiblemem: none, target_mem0: none, target_mem1: none)
define noundef i64 @KdSetEventLoggingPresent(i64 %arg_rcx) local_unnamed_addr #0 {
lift_init:
  %r_cl = trunc i64 %arg_rcx to i8
  %addr_KdEventLoggingPresent = tail call i64 @__nt_sym_addr_KdEventLoggingPresent()
  %.24 = inttoptr i64 %addr_KdEventLoggingPresent to ptr
  store i8 %r_cl, ptr %.24, align 1
  ret i64 0
}

; Function Attrs: nofree nosync nounwind memory(none)
declare i64 @__nt_sym_addr_KdEventLoggingPresent() local_unnamed_addr #1

attributes #0 = { nofree nosync nounwind memory(write, inaccessiblemem: none, target_mem0: none, target_mem1: none) }
attributes #1 = { nofree nosync nounwind memory(none) }


; ModuleID = '<string>'
source_filename = "<string>"
target triple = "x86_64-pc-windows-msvc"

; Function Attrs: nofree nosync nounwind memory(none)
define i64 @KdGetDebugDevice() local_unnamed_addr #0 {
lift_init:
  %addr_KdDebugDevice = tail call i64 @__nt_sym_addr_KdDebugDevice()
  ret i64 %addr_KdDebugDevice
}

; Function Attrs: nofree nosync nounwind memory(none)
declare i64 @__nt_sym_addr_KdDebugDevice() local_unnamed_addr #0

attributes #0 = { nofree nosync nounwind memory(none) }


; ModuleID = '<string>'
source_filename = "<string>"
target triple = "x86_64-pc-windows-msvc"

; Function Attrs: nofree nosync nounwind memory(readwrite, inaccessiblemem: none, target_mem0: none, target_mem1: none)
define range(i64 0, 3221225474) i64 @HvlQueryConnection(i64 %arg_rcx) local_unnamed_addr #0 {
lift_init:
  %addr_HvlpHypercallCodeVa = tail call i64 @__nt_sym_addr_HvlpHypercallCodeVa()
  %.24 = inttoptr i64 %addr_HvlpHypercallCodeVa to ptr
  %mem_ld = load i64, ptr %.24, align 4
  %.27.not = icmp eq i64 %mem_ld, 0
  br i1 %.27.not, label %common.ret, label %bb_140182e28

common.ret:                                       ; preds = %bb_140182e28, %bb_140182e2d, %lift_init
  %common.ret.op = phi i64 [ 3221225473, %lift_init ], [ 0, %bb_140182e2d ], [ 0, %bb_140182e28 ]
  ret i64 %common.ret.op

bb_140182e28:                                     ; preds = %lift_init
  %.45 = icmp eq i64 %arg_rcx, 0
  br i1 %.45, label %common.ret, label %bb_140182e2d

bb_140182e2d:                                     ; preds = %bb_140182e28
  %.56 = inttoptr i64 %arg_rcx to ptr
  store i64 %mem_ld, ptr %.56, align 4
  br label %common.ret
}

; Function Attrs: nofree nosync nounwind memory(none)
declare i64 @__nt_sym_addr_HvlpHypercallCodeVa() local_unnamed_addr #1

attributes #0 = { nofree nosync nounwind memory(readwrite, inaccessiblemem: none, target_mem0: none, target_mem1: none) }
attributes #1 = { nofree nosync nounwind memory(none) }


; ModuleID = '<string>'
source_filename = "<string>"
target triple = "x86_64-pc-windows-msvc"

; Function Attrs: nofree nosync nounwind memory(readwrite, inaccessiblemem: none, target_mem0: none, target_mem1: none)
define range(i64 0, 3221225474) i64 @HvlQueryActiveHypervisorProcessorCount(i64 %arg_rcx) local_unnamed_addr #0 {
lift_init:
  %addr_HvlpFlags = tail call i64 @__nt_sym_addr_HvlpFlags()
  %.24 = inttoptr i64 %addr_HvlpFlags to ptr
  %mem_ld = load i32, ptr %.24, align 4
  %.275 = and i32 %mem_ld, 2
  %.28.not = icmp eq i32 %.275, 0
  br i1 %.28.not, label %common.ret, label %bb_1401e3470

common.ret:                                       ; preds = %lift_init, %bb_1401e3470
  %common.ret.op = phi i64 [ 0, %bb_1401e3470 ], [ 3221225473, %lift_init ]
  ret i64 %common.ret.op

bb_1401e3470:                                     ; preds = %lift_init
  %addr_HvlpLogicalProcessorCount = tail call i64 @__nt_sym_addr_HvlpLogicalProcessorCount()
  %.45 = inttoptr i64 %addr_HvlpLogicalProcessorCount to ptr
  %mem_ld.1 = load i32, ptr %.45, align 4
  %.48 = inttoptr i64 %arg_rcx to ptr
  store i32 %mem_ld.1, ptr %.48, align 4
  br label %common.ret
}

; Function Attrs: nofree nosync nounwind memory(none)
declare i64 @__nt_sym_addr_HvlpFlags() local_unnamed_addr #1

; Function Attrs: nofree nosync nounwind memory(none)
declare i64 @__nt_sym_addr_HvlpLogicalProcessorCount() local_unnamed_addr #1

attributes #0 = { nofree nosync nounwind memory(readwrite, inaccessiblemem: none, target_mem0: none, target_mem1: none) }
attributes #1 = { nofree nosync nounwind memory(none) }


; ModuleID = '<string>'
source_filename = "<string>"
target triple = "x86_64-pc-windows-msvc"

define i64 @WmiTraceMessage(i64 %arg_rcx, i64 %arg_rdx, i64 %arg_r8, i64 %arg_r9) local_unnamed_addr {
lift_init:
  %stack_frame = alloca [1024 x i8], align 16
  %r_r9w = trunc i64 %arg_r9 to i16
  %.28 = getelementptr inbounds nuw i8, ptr %stack_frame, i64 800
  store i16 %r_r9w, ptr %.28, align 16
  %.46 = getelementptr inbounds nuw i8, ptr %stack_frame, i64 808
  %.47 = ptrtoint ptr %.46 to i64
  %.50 = getelementptr inbounds nuw i8, ptr %stack_frame, i64 752
  store i8 0, ptr %.50, align 16
  %.53 = getelementptr inbounds nuw i8, ptr %stack_frame, i64 744
  store i64 %.47, ptr %.53, align 8
  %call_res = call i64 @EtwpTraceMessageVa(i64 %arg_rcx, i64 %arg_rdx, i64 %arg_r8, i64 %arg_r9)
  ret i64 %call_res
}

declare i64 @EtwpTraceMessageVa(i64, i64, i64, i64) local_unnamed_addr


; ModuleID = '<string>'
source_filename = "<string>"
target triple = "x86_64-pc-windows-msvc"

define i64 @WheaDeferredRecoveryService() local_unnamed_addr {
lift_init:
  %addr_WheaPendingDrsInterrupt = tail call i64 @__nt_sym_addr_WheaPendingDrsInterrupt()
  %.45 = inttoptr i64 %addr_WheaPendingDrsInterrupt to ptr
  %mem_ld = load i32, ptr %.45, align 4
  store i32 0, ptr %.45, align 4
  %.48 = zext i32 %mem_ld to i64
  %.51.not = icmp eq i32 %mem_ld, 1
  br i1 %.51.not, label %bb_140281a31, label %bb_140281a4a

bb_140281a31:                                     ; preds = %lift_init
  %addr_WheaDrsDpc = tail call i64 @__nt_sym_addr_WheaDrsDpc()
  %call_res = tail call i64 @KiInsertQueueDpc(i64 %addr_WheaDrsDpc, i64 0, i64 0, i64 0)
  br label %bb_140281a4a

bb_140281a4a:                                     ; preds = %bb_140281a31, %lift_init
  %reg_rax.0 = phi i64 [ %.48, %lift_init ], [ %call_res, %bb_140281a31 ]
  ret i64 %reg_rax.0
}

; Function Attrs: nofree nosync nounwind memory(none)
declare i64 @__nt_sym_addr_WheaPendingDrsInterrupt() local_unnamed_addr #0

; Function Attrs: nofree nosync nounwind memory(none)
declare i64 @__nt_sym_addr_WheaDrsDpc() local_unnamed_addr #0

declare i64 @KiInsertQueueDpc(i64, i64, i64, i64) local_unnamed_addr

attributes #0 = { nofree nosync nounwind memory(none) }


; ModuleID = '<string>'
source_filename = "<string>"
target triple = "x86_64-pc-windows-msvc"

define i64 @EtwWriteTransfer(i64 %arg_rcx, i64 %arg_rdx, i64 %arg_r8, i64 %arg_r9) local_unnamed_addr {
lift_init:
  %call_res = tail call i64 @EtwWriteEx(i64 %arg_rcx, i64 %arg_rdx, i64 0, i64 0)
  ret i64 %call_res
}

declare i64 @EtwWriteEx(i64, i64, i64, i64) local_unnamed_addr


; ModuleID = '<string>'
source_filename = "<string>"
target triple = "x86_64-pc-windows-msvc"

define i64 @EtwWrite(i64 %arg_rcx, i64 %arg_rdx, i64 %arg_r8, i64 %arg_r9) local_unnamed_addr {
lift_init:
  %call_res = tail call i64 @EtwWriteEx(i64 %arg_rcx, i64 %arg_rdx, i64 0, i64 0)
  ret i64 %call_res
}

declare i64 @EtwWriteEx(i64, i64, i64, i64) local_unnamed_addr


; ModuleID = '<string>'
source_filename = "<string>"
target triple = "x86_64-pc-windows-msvc"

define i64 @DbgPrintReturnControlC(i64 %arg_rcx, i64 %arg_rdx, i64 %arg_r8, i64 %arg_r9) local_unnamed_addr {
lift_init:
  %stack_frame = alloca [1024 x i8], align 16
  %.27 = getelementptr inbounds nuw i8, ptr %stack_frame, i64 768
  %.28 = ptrtoint ptr %.27 to i64
  %.30 = or disjoint i64 %.28, 8
  %.31 = inttoptr i64 %.30 to ptr
  store i64 %arg_rcx, ptr %.31, align 8
  %.33 = add i64 %.28, 16
  %.34 = inttoptr i64 %.33 to ptr
  store i64 %arg_rdx, ptr %.34, align 16
  %.36 = add i64 %.28, 24
  %.37 = inttoptr i64 %.36 to ptr
  store i64 %arg_r8, ptr %.37, align 8
  %.39 = add i64 %.28, 32
  %.40 = inttoptr i64 %.39 to ptr
  store i64 %arg_r9, ptr %.40, align 16
  %.65 = getelementptr inbounds nuw i8, ptr %stack_frame, i64 752
  store i8 0, ptr %.65, align 16
  %addr___nt_rodata_1401702f0 = tail call i64 @__nt_sym_addr___nt_rodata_1401702f0()
  %.71 = add i64 %.28, -24
  %.72 = inttoptr i64 %.71 to ptr
  store i64 %.33, ptr %.72, align 8
  %call_res = call i64 @vDbgPrintExWithPrefixInternal(i64 %addr___nt_rodata_1401702f0, i64 101, i64 0, i64 %arg_rcx)
  ret i64 %call_res
}

; Function Attrs: nofree nosync nounwind memory(none)
declare i64 @__nt_sym_addr___nt_rodata_1401702f0() local_unnamed_addr #0

declare i64 @vDbgPrintExWithPrefixInternal(i64, i64, i64, i64) local_unnamed_addr

attributes #0 = { nofree nosync nounwind memory(none) }


; ModuleID = '<string>'
source_filename = "<string>"
target triple = "x86_64-pc-windows-msvc"

; Function Attrs: mustprogress nofree norecurse nosync nounwind willreturn memory(none)
define noundef i64 @NtVdmControl() local_unnamed_addr #0 {
lift_init:
  ret i64 3221225474
}

attributes #0 = { mustprogress nofree norecurse nosync nounwind willreturn memory(none) }


; ModuleID = '<string>'
source_filename = "<string>"
target triple = "x86_64-pc-windows-msvc"

define i64 @NtOpenProcessToken(i64 %arg_rcx, i64 %arg_rdx, i64 %arg_r8) local_unnamed_addr {
lift_init:
  %call_res = tail call i64 @NtOpenProcessTokenEx(i64 %arg_rcx, i64 %arg_rdx, i64 0, i64 %arg_r8)
  ret i64 %call_res
}

declare i64 @NtOpenProcessTokenEx(i64, i64, i64, i64) local_unnamed_addr


; ModuleID = '<string>'
source_filename = "<string>"
target triple = "x86_64-pc-windows-msvc"

define i64 @NtOpenThreadToken(i64 %arg_rcx, i64 %arg_rdx, i64 %arg_r8, i64 %arg_r9) local_unnamed_addr {
lift_init:
  %call_res = tail call i64 @NtOpenThreadTokenEx(i64 %arg_rcx, i64 %arg_rdx, i64 %arg_r8, i64 0)
  ret i64 %call_res
}

declare i64 @NtOpenThreadTokenEx(i64, i64, i64, i64) local_unnamed_addr


; ModuleID = '<string>'
source_filename = "<string>"
target triple = "x86_64-pc-windows-msvc"

; Function Attrs: mustprogress nofree norecurse nosync nounwind willreturn memory(none)
define range(i64 8, 161) i64 @AlpcGetHeaderSize(i64 %arg_rcx) local_unnamed_addr #0 {
lift_init:
  %r_ecx = trunc i64 %arg_rcx to i32
  %isneg = icmp slt i32 %r_ecx, 0
  %.48 = select i1 %isneg, i32 32, i32 8
  %0 = lshr i32 %r_ecx, 25
  %1 = and i32 %0, 32
  %.77 = add nuw nsw i32 %.48, %1
  %2 = lshr i32 %r_ecx, 24
  %3 = and i32 %2, 32
  %.94 = add nuw nsw i32 %.77, %3
  %4 = and i32 %r_ecx, 268435456
  %.100.not = icmp eq i32 %4, 0
  %narrow = add nuw nsw i32 %.94, 24
  %.111 = select i1 %.100.not, i32 %.94, i32 %narrow
  %5 = and i32 %r_ecx, 134217728
  %.117.not = icmp eq i32 %5, 0
  %narrow28 = add nuw nsw i32 %.111, 24
  %.128 = select i1 %.117.not, i32 %.111, i32 %narrow28
  %6 = lshr i32 %r_ecx, 23
  %7 = and i32 %6, 8
  %8 = lshr i32 %r_ecx, 22
  %9 = and i32 %8, 8
  %.145 = add nuw nsw i32 %9, %7
  %.162 = add nuw nsw i32 %.145, %.128
  %.163 = zext nneg i32 %.162 to i64
  ret i64 %.163
}

attributes #0 = { mustprogress nofree norecurse nosync nounwind willreturn memory(none) }


; ModuleID = '<string>'
source_filename = "<string>"
target triple = "x86_64-pc-windows-msvc"

define noundef i64 @AlpcRegionDestroyProcedure(i64 %arg_rcx, i64 %arg_rdx) local_unnamed_addr {
lift_init:
  %.48 = add i64 %arg_rcx, 16
  %.49 = inttoptr i64 %.48 to ptr
  %mem_ld = load i64, ptr %.49, align 4
  %call_res = tail call i64 @AlpcpLockForCachedReferenceBlob(i64 %mem_ld)
  %.54 = add i64 %arg_rcx, 8
  %.55 = inttoptr i64 %.54 to ptr
  %mem_ld.1 = load i64, ptr %.55, align 4
  %.58 = inttoptr i64 %arg_rcx to ptr
  %mem_ld.2 = load i64, ptr %.58, align 4
  %.60 = inttoptr i64 %mem_ld.1 to ptr
  store i64 %mem_ld.2, ptr %.60, align 4
  %mem_ld.3 = load i64, ptr %.58, align 4
  %mem_ld.4 = load i64, ptr %.55, align 4
  %.67 = add i64 %mem_ld.3, 8
  %.68 = inttoptr i64 %.67 to ptr
  store i64 %mem_ld.4, ptr %.68, align 4
  %.70 = add i64 %mem_ld, 52
  %.71 = inttoptr i64 %.70 to ptr
  %mem_ld.5 = load i32, ptr %.71, align 4
  %.73 = add i32 %mem_ld.5, -1
  store i32 %.73, ptr %.71, align 4
  %call_res.1 = tail call i64 @AlpcpUnlockBlob(i64 %mem_ld, i64 %mem_ld.3, i64 0, i64 0)
  %call_res.2 = tail call i64 @AlpcpDereferenceBlobEx(i64 %mem_ld, i64 1, i64 0, i64 0)
  ret i64 0
}

declare i64 @AlpcpLockForCachedReferenceBlob(i64) local_unnamed_addr

declare i64 @AlpcpUnlockBlob(i64, i64, i64, i64) local_unnamed_addr

declare i64 @AlpcpDereferenceBlobEx(i64, i64, i64, i64) local_unnamed_addr


; ModuleID = '<string>'
source_filename = "<string>"
target triple = "x86_64-pc-windows-msvc"

; Function Attrs: mustprogress nofree norecurse nosync nounwind willreturn memory(none)
define range(i64 8, 153) i64 @AlpcpGetMessageAttributeOffset(i64 %arg_rcx, i64 %arg_rdx) local_unnamed_addr #0 {
lift_init:
  %arg_rdx.tr = trunc i64 %arg_rdx to i32
  %.neg = mul i32 %arg_rdx.tr, -2
  %r_ecx = trunc i64 %arg_rcx to i32
  %.36 = and i32 %.neg, %r_ecx
  %isneg = icmp slt i32 %.36, 0
  %narrow = select i1 %isneg, i32 32, i32 8
  %0 = lshr i32 %.36, 25
  %1 = and i32 %0, 32
  %narrow28 = add nuw nsw i32 %narrow, %1
  %2 = lshr i32 %.36, 24
  %3 = and i32 %2, 32
  %narrow29 = add nuw nsw i32 %narrow28, %3
  %.106 = zext nneg i32 %narrow29 to i64
  %4 = and i32 %.36, 268435456
  %.111.not = icmp eq i32 %4, 0
  %.113 = add nuw nsw i64 %.106, 24
  %.120 = select i1 %.111.not, i64 %.106, i64 %.113
  %5 = and i32 %.36, 134217728
  %.125.not = icmp eq i32 %5, 0
  %.127 = add nuw nsw i64 %.120, 24
  %.134 = select i1 %.125.not, i64 %.120, i64 %.127
  %6 = lshr i32 %.36, 22
  %7 = and i32 %6, 8
  %8 = zext nneg i32 %7 to i64
  %.148 = add nuw nsw i64 %.134, %8
  ret i64 %.148
}

attributes #0 = { mustprogress nofree norecurse nosync nounwind willreturn memory(none) }


; ModuleID = '<string>'
source_filename = "<string>"
target triple = "x86_64-pc-windows-msvc"

; Function Attrs: mustprogress nofree norecurse nosync nounwind willreturn memory(none)
define range(i64 8, 105) i64 @AlpcpGetMessageAttributeOffset32(i64 %arg_rcx, i64 %arg_rdx) local_unnamed_addr #0 {
lift_init:
  %arg_rdx.tr = trunc i64 %arg_rdx to i32
  %.neg = mul i32 %arg_rdx.tr, -2
  %r_ecx = trunc i64 %arg_rcx to i32
  %.36 = and i32 %.neg, %r_ecx
  %isneg = icmp slt i32 %.36, 0
  %narrow = select i1 %isneg, i32 20, i32 8
  %0 = lshr i32 %.36, 26
  %1 = and i32 %0, 16
  %narrow28 = add nuw nsw i32 %narrow, %1
  %.92 = zext nneg i32 %narrow28 to i64
  %2 = and i32 %.36, 536870912
  %.97.not = icmp eq i32 %2, 0
  %.99 = add nuw nsw i64 %.92, 20
  %.106 = select i1 %.97.not, i64 %.92, i64 %.99
  %3 = lshr i32 %.36, 24
  %4 = and i32 %3, 16
  %5 = zext nneg i32 %4 to i64
  %.120 = add nuw nsw i64 %.106, %5
  %6 = and i32 %.36, 134217728
  %.125.not = icmp eq i32 %6, 0
  %.127 = add nuw nsw i64 %.120, 24
  %.134 = select i1 %.125.not, i64 %.120, i64 %.127
  %7 = lshr i32 %.36, 22
  %8 = and i32 %7, 8
  %9 = zext nneg i32 %8 to i64
  %.148 = add nuw nsw i64 %.134, %9
  ret i64 %.148
}

attributes #0 = { mustprogress nofree norecurse nosync nounwind willreturn memory(none) }


; ModuleID = '<string>'
source_filename = "<string>"
target triple = "x86_64-pc-windows-msvc"

define range(i64 0, 4294967296) i64 @LpcRequestPort(i64 %arg_rcx, i64 %arg_rdx) local_unnamed_addr {
lift_init:
  %stack_frame = alloca [1024 x i8], align 16
  %.26 = getelementptr inbounds nuw i8, ptr %stack_frame, i64 776
  store i64 0, ptr %.26, align 8
  %.31 = getelementptr inbounds nuw i8, ptr %stack_frame, i64 760
  store i64 0, ptr %.31, align 8
  %.59 = getelementptr inbounds nuw i8, ptr %stack_frame, i64 704
  %.60 = ptrtoint ptr %.59 to i64
  %call_res = call i64 @memset(i64 %.60, i64 0, i64 56, i64 0)
  %gs_val = call i64 @__nt_read_gs_i64(i64 392)
  %.69 = add i64 %gs_val, 484
  %.70 = inttoptr i64 %.69 to ptr
  %mem_ld = load i16, ptr %.70, align 2
  %.72 = add i16 %mem_ld, -1
  store i16 %.72, ptr %.70, align 2
  %.98 = getelementptr inbounds nuw i8, ptr %stack_frame, i64 696
  store i64 %arg_rcx, ptr %.98, align 8
  %.110 = getelementptr inbounds nuw i8, ptr %stack_frame, i64 744
  store i32 65538, ptr %.110, align 8
  %.116 = ptrtoint ptr %.98 to i64
  %call_res.1 = call i64 @AlpcpSendMessage(i64 %.116, i64 %arg_rdx, i64 0, i64 0)
  %.119 = and i64 %call_res.1, 4294967295
  %call_res.2 = call i64 @KeLeaveCriticalRegion(i64 %.116, i64 %arg_rdx, i64 0, i64 0)
  ret i64 %.119
}

declare i64 @memset(i64, i64, i64, i64) local_unnamed_addr

declare i64 @__nt_read_gs_i64(i64) local_unnamed_addr

declare i64 @AlpcpSendMessage(i64, i64, i64, i64) local_unnamed_addr

declare i64 @KeLeaveCriticalRegion(i64, i64, i64, i64) local_unnamed_addr
