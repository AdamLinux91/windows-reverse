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
  %stack_frame = alloca [4096 x i8], align 16
  %.26 = getelementptr inbounds nuw i8, ptr %stack_frame, i64 3080
  store i64 0, ptr %.26, align 8
  %.30 = getelementptr inbounds nuw i8, ptr %stack_frame, i64 3096
  store i64 0, ptr %.30, align 8
  %.35 = getelementptr inbounds nuw i8, ptr %stack_frame, i64 3064
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
  %.138 = getelementptr inbounds nuw i8, ptr %stack_frame, i64 3088
  %.139 = ptrtoint ptr %.138 to i64
  store i16 %reg_rbx.0.off0, ptr %.138, align 16
  %.147 = getelementptr inbounds nuw i8, ptr %stack_frame, i64 3090
  store i8 %r_dl, ptr %.147, align 2
  %.150 = getelementptr inbounds nuw i8, ptr %stack_frame, i64 3091
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
  %stack_frame = alloca [4096 x i8], align 16
  %.39 = getelementptr inbounds nuw i8, ptr %stack_frame, i64 3088
  store i32 0, ptr %.39, align 16
  %.54 = getelementptr inbounds nuw i8, ptr %stack_frame, i64 3048
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
  %stack_frame.sroa.0.3088.insert.ext = and i64 %arg_rcx, 4294967295
  %0 = mul nuw nsw i64 %stack_frame.sroa.0.3088.insert.ext, 10000000
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
  %stack_frame.sroa.0.3088.insert.ext = and i64 %arg_rcx, 4294967295
  %0 = mul nuw nsw i64 %stack_frame.sroa.0.3088.insert.ext, 10000000
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
  %stack_frame = alloca [4096 x i8], align 16
  %.24 = getelementptr inbounds nuw i8, ptr %stack_frame, i64 3080
  store i64 0, ptr %.24, align 8
  %.29 = getelementptr inbounds nuw i8, ptr %stack_frame, i64 3064
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
  %.107 = getelementptr inbounds nuw i8, ptr %stack_frame, i64 3056
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
  %.157 = getelementptr inbounds nuw i8, ptr %stack_frame, i64 3048
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
