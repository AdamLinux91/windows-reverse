/*
 * Reconstructed Retail ntoskrnl.exe Data Symbols, Read-Only Lookup Tables & Weak Callees
 * Extracted via RIP-relative symbol/section recovery (weak linkage — overridden in Ring-0
 * standalone builds by `nt_kernel_runtime.c` and bound directly to real kernel RVAs in
 * `ntoskrnl_dropin.exe`).
 */
#include "../../include/nt_reconstructed_types.h"

__attribute__((weak)) uint8_t KiInitialProcess[4096] = {0};
__attribute__((weak)) uint8_t KeForceGroupAwareness = (uint8_t)(0);
__attribute__((weak)) uint16_t KiActiveGroups = (uint16_t)(0);
__attribute__((weak)) uint8_t ObHeaderCookie = (uint8_t)(0);
__attribute__((weak)) struct _OBJECT_TYPE* ObTypeIndexTable[256] = {0};
__attribute__((weak)) uint8_t ObpInfoMaskToOffset[4096] = {0};
__attribute__((weak)) uint64_t SeAliasAdminsSid = (uint64_t)(0);
__attribute__((weak)) uint8_t KdPitchDebugger = (uint8_t)(0);
__attribute__((weak)) const uint8_t __nt_rodata_1401715d0[64] = { 0x4b, 0x44, 0x54, 0x41, 0x52, 0x47, 0x45, 0x54, 0x3a, 0x20, 0x52, 0x65, 0x66, 0x72, 0x65, 0x73, 0x68, 0x69, 0x6e, 0x67, 0x20, 0x4b, 0x44, 0x20, 0x63, 0x6f, 0x6e, 0x6e, 0x65, 0x63, 0x74, 0x69, 0x6f, 0x6e, 0x0a, 0x00, 0xcc, 0xcc, 0xcc, 0xcc, 0xcc, 0xcc, 0xcc, 0xcc, 0xcc, 0xcc, 0xcc, 0xcc, 0x0d, 0x0a, 0x00, 0xcc, 0xcc, 0xcc, 0xcc, 0xcc, 0xcc, 0xcc, 0xcc, 0xcc, 0xcc, 0xcc, 0xcc, 0xcc };
__attribute__((weak)) uint8_t KdDebuggerNotPresent = (uint8_t)(0);
__attribute__((weak)) uint32_t KiClockStats = (uint32_t)(0);
__attribute__((weak)) uint8_t KiNxForceEnable = (uint8_t)(0);
__attribute__((weak)) uint32_t KiClockTimerOwner = (uint32_t)(0);
__attribute__((weak)) uint32_t KeLargestCacheLine = (uint32_t)(0);
__attribute__((weak)) uint32_t KeMaximumIncrement = (uint32_t)(0);
__attribute__((weak)) uint8_t KiLastBranchRecordInUse = (uint8_t)(0);
__attribute__((weak)) uint8_t KiProfileIrql = (uint8_t)(0);
__attribute__((weak)) uint32_t KiSystemFullyCoherent = (uint32_t)(0);
__attribute__((weak)) uint64_t __nt_data_14037f730 = (uint64_t)(0);
__attribute__((weak)) uint64_t __nt_data_1403f93e8 = (uint64_t)(0);
__attribute__((weak)) uint16_t KiMaximumGroups = (uint16_t)(0);
__attribute__((weak)) uint16_t KeNumberNodes = (uint16_t)(0);
__attribute__((weak)) uint8_t __nt_data_14037f728 = (uint8_t)(0);
__attribute__((weak)) uint8_t KiProcessorBlock[4096] = {0};
__attribute__((weak)) uint32_t KeTimeIncrement = (uint32_t)(0);
__attribute__((weak)) uint32_t KiLastRequestedTimeIncrement = (uint32_t)(0);
__attribute__((weak)) uint8_t KiProcessorIndexToNumberMappingTable[4096] = {0};
__attribute__((weak)) uint8_t KeActiveProcessors[4096] = {0};
__attribute__((weak)) uint32_t KeNumberProcessors = (uint32_t)(0);
__attribute__((weak)) uint8_t __nt_rodata_1403f93e8[4096] = {0};
__attribute__((weak)) uint32_t KiDefaultHeteroCpuPolicy = (uint32_t)(0);
__attribute__((weak)) uint32_t PsDisableDiskCounters = (uint32_t)(0);
__attribute__((weak)) uint64_t PspLegoNotifyRoutine = (uint64_t)(0);
__attribute__((weak)) uint64_t PsInitialSystemProcess = (uint64_t)(0);
__attribute__((weak)) uint8_t __nt_data_1403f8130 = (uint8_t)(0);
__attribute__((weak)) uint64_t ViVerifierDriverAddedThunkListHead = (uint64_t)(0);
__attribute__((weak)) uint32_t MmVerifierData = (uint32_t)(0);
__attribute__((weak)) uint8_t __nt_rodata_1403802b8[4096] = {0};
__attribute__((weak)) uint64_t __nt_data_140380458 = (uint64_t)(0);
__attribute__((weak)) uint8_t MiSystemPartition[4096] = {0};
__attribute__((weak)) uint8_t __nt_rodata_1407ea710[64] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x28, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x55, 0x00, 0x6e, 0x00, 0x69, 0x00, 0x64, 0x00, 0x65, 0x00, 0x6e, 0x00, 0x74, 0x00, 0x69, 0x00, 0x66, 0x00, 0x69, 0x00, 0x65, 0x00, 0x64, 0x00 };
__attribute__((weak)) const uint8_t __nt_rodata_1407835d0[64] = { 0x89, 0x00, 0x12, 0x00, 0x16, 0x01, 0x12, 0x00, 0xa0, 0x00, 0x12, 0x00, 0xff, 0x01, 0x1f, 0x00, 0x0e, 0x00, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x50, 0x95, 0x78, 0x40, 0x01, 0x00, 0x00, 0x00, 0x1a, 0x00, 0x1c, 0x00, 0x00, 0x00, 0x00, 0x00, 0x88, 0x95, 0x78, 0x40, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00 };
__attribute__((weak)) uint32_t __nt_data_140356610 = (uint32_t)(0);
__attribute__((weak)) uint32_t ExpManufacturingInformation = (uint32_t)(0);
__attribute__((weak)) uint32_t ExpRealTimeIsUniversal = (uint32_t)(0);
__attribute__((weak)) uint8_t __nt_data_140356618 = (uint8_t)(0);
__attribute__((weak)) uint64_t ExpTimeZoneBias = (uint64_t)(0);
__attribute__((weak)) uint32_t KeMinimumIncrement = (uint32_t)(0);
__attribute__((weak)) uint64_t PspSystemPartition = (uint64_t)(0);
__attribute__((weak)) uint32_t NtGlobalFlag = (uint32_t)(0);
__attribute__((weak)) uint32_t __nt_data_14034e510 = (uint32_t)(0);
__attribute__((weak)) uint16_t NlsAnsiCodePage = (uint16_t)(0);
__attribute__((weak)) uint16_t NlsOemCodePage = (uint16_t)(0);
__attribute__((weak)) const uint8_t Crc32Ctrl[64] = { 0x00, 0xc1, 0x2c, 0x40, 0x01, 0x00, 0x00, 0x00, 0x00, 0xc3, 0x2c, 0x40, 0x01, 0x00, 0x00, 0x00, 0x00, 0xcb, 0x2c, 0x40, 0x01, 0x00, 0x00, 0x00, 0x78, 0x3b, 0xf6, 0x82, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0xe0, 0xc0, 0x2c, 0x40, 0x01, 0x00, 0x00, 0x00, 0x30, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
__attribute__((weak)) uint64_t __nt_data_14034b150 = (uint64_t)(0);
__attribute__((weak)) uint8_t RtlpIsDrainRequired = (uint8_t)(0);
__attribute__((weak)) uint64_t CmpGlobalQuota = (uint64_t)(0);
__attribute__((weak)) uint64_t CmpGlobalQuotaAllowed = (uint64_t)(0);
__attribute__((weak)) uint8_t CmpMiniNTBoot = (uint8_t)(0);
__attribute__((weak)) uint32_t CmpGlobalFlushControlFlags = (uint32_t)(0);
__attribute__((weak)) uint64_t CmpFreezeListLock = (uint64_t)(0);
__attribute__((weak)) uint8_t CmpFreezeThawWaitListHead[4096] = {0};
__attribute__((weak)) uint32_t CmFreezeThawTimeoutInSeconds = (uint32_t)(0);
__attribute__((weak)) uint64_t __nt_data_14037ee48 = (uint64_t)(0);
__attribute__((weak)) uint8_t CmpDelayDerefKCBTimer[4096] = {0};
__attribute__((weak)) const uint8_t __nt_rodata_14013f350[64] = { 0x40, 0x53, 0x48, 0x83, 0xec, 0x20, 0x48, 0x8b, 0xda, 0xe8, 0x1e, 0x00, 0x00, 0x00, 0x83, 0x3b, 0x00, 0x7c, 0x04, 0x85, 0xc0, 0x78, 0x08, 0xb0, 0x01, 0x48, 0x83, 0xc4, 0x20, 0x5b, 0xc3, 0x89, 0x03, 0xeb, 0xf4, 0xcc, 0xcc, 0xcc, 0xcc, 0xcc, 0xcc, 0xcc, 0xcc, 0xcc, 0x48, 0x89, 0x5c, 0x24, 0x08, 0x48, 0x89, 0x7c, 0x24, 0x18, 0x55, 0x48, 0x8b, 0xec, 0x48, 0x83, 0xec, 0x60, 0x48, 0x8d };
__attribute__((weak)) const uint8_t __nt_rodata_1401da7a0[64] = { 0x48, 0x8b, 0xc4, 0x48, 0x89, 0x58, 0x08, 0x48, 0x89, 0x68, 0x10, 0x48, 0x89, 0x70, 0x18, 0x57, 0x41, 0x56, 0x41, 0x57, 0x48, 0x83, 0xec, 0x40, 0x48, 0x8d, 0xa9, 0x80, 0x00, 0x00, 0x00, 0x4c, 0x8b, 0xf2, 0x4c, 0x8d, 0x79, 0x30, 0x33, 0xff, 0x48, 0x8b, 0xcd, 0x48, 0x8d, 0x50, 0xc8, 0xe8, 0xfc, 0x57, 0xe8, 0xff, 0x49, 0x8b, 0x1f, 0xeb, 0x78, 0x48, 0x8b, 0x4b, 0x60, 0x8b, 0x93, 0x98 };
__attribute__((weak)) uint8_t FsRtlpVolumeStartupApplicationsComplete = (uint8_t)(0);
__attribute__((weak)) uint64_t FltMgrCallbacks = (uint64_t)(0);
__attribute__((weak)) uint64_t pFsRtlpMupCalls = (uint64_t)(0);
__attribute__((weak)) uint32_t FsRtlPagingIoResourceSelector = (uint32_t)(0);
__attribute__((weak)) uint64_t FsRtlPagingIoResources = (uint64_t)(0);
__attribute__((weak)) uint8_t PopEnergyEstimationEnabled = (uint8_t)(0);
__attribute__((weak)) uint32_t PopFixedWakeSourceMask = (uint32_t)(0);
__attribute__((weak)) uint32_t PopBsdShutdownInProgress = (uint32_t)(0);
__attribute__((weak)) uint64_t PpmIntSteerTriggerMax = (uint64_t)(0);
__attribute__((weak)) uint8_t PopTransitionLock[4096] = {0};
__attribute__((weak)) uint8_t PopTelemetryOsState[4096] = {0};
__attribute__((weak)) uint32_t __nt_data_14035cd88 = (uint32_t)(0);
__attribute__((weak)) uint64_t ExCbPowerState = (uint64_t)(0);
__attribute__((weak)) uint8_t KdEventLoggingPresent = (uint8_t)(0);
__attribute__((weak)) uint8_t KdDebugDevice[4096] = {0};
__attribute__((weak)) uint64_t HvlpHypercallCodeVa = (uint64_t)(0);
__attribute__((weak)) uint32_t HvlpFlags = (uint32_t)(0);
__attribute__((weak)) uint32_t HvlpLogicalProcessorCount = (uint32_t)(0);
__attribute__((weak)) uint32_t WheaPendingDrsInterrupt = (uint32_t)(0);
__attribute__((weak)) uint8_t WheaDrsDpc[4096] = {0};
__attribute__((weak)) const uint8_t __nt_rodata_1401702f0[64] = { 0x00, 0xcc, 0xcc, 0xcc, 0xcc, 0xcc, 0xcc, 0xcc, 0xcc, 0xcc, 0xcc, 0xcc, 0xcc, 0xcc, 0xcc, 0xcc, 0x2e, 0x00, 0xcc, 0xcc, 0xcc, 0xcc, 0xcc, 0xcc, 0xcc, 0xcc, 0xcc, 0xcc, 0xcc, 0xcc, 0xcc, 0xcc, 0x43, 0x43, 0x3a, 0x20, 0x52, 0x65, 0x75, 0x73, 0x69, 0x6e, 0x67, 0x20, 0x73, 0x68, 0x61, 0x72, 0x65, 0x64, 0x20, 0x63, 0x61, 0x63, 0x68, 0x65, 0x20, 0x6d, 0x61, 0x70, 0x20, 0x74, 0x68, 0x61 };

/* Weak Linkage Stubs for Preserved Assembly External Targets */
__attribute__((weak, naked)) void __nt_asm_ext_140013438(void) { __asm__ __volatile__ ("ret"); }
__attribute__((weak, naked)) void __nt_asm_ext_14001db90(void) { __asm__ __volatile__ ("ret"); }
__attribute__((weak, naked)) void __nt_asm_ext_140032134(void) { __asm__ __volatile__ ("ret"); }
__attribute__((weak, naked)) void __nt_asm_ext_1400322dc(void) { __asm__ __volatile__ ("ret"); }
__attribute__((weak, naked)) void __nt_asm_ext_140032380(void) { __asm__ __volatile__ ("ret"); }
__attribute__((weak, naked)) void __nt_asm_ext_140032590(void) { __asm__ __volatile__ ("ret"); }
__attribute__((weak, naked)) void __nt_asm_ext_140032820(void) { __asm__ __volatile__ ("ret"); }
__attribute__((weak, naked)) void __nt_asm_ext_140036930(void) { __asm__ __volatile__ ("ret"); }
__attribute__((weak, naked)) void __nt_asm_ext_140058ba0(void) { __asm__ __volatile__ ("ret"); }
__attribute__((weak, naked)) void __nt_asm_ext_14005a040(void) { __asm__ __volatile__ ("ret"); }
__attribute__((weak, naked)) void __nt_asm_ext_14005a1c0(void) { __asm__ __volatile__ ("ret"); }
__attribute__((weak, naked)) void __nt_asm_ext_14005f170(void) { __asm__ __volatile__ ("ret"); }
__attribute__((weak, naked)) void __nt_asm_ext_14005ffd0(void) { __asm__ __volatile__ ("ret"); }
__attribute__((weak, naked)) void __nt_asm_ext_140060750(void) { __asm__ __volatile__ ("ret"); }
__attribute__((weak, naked)) void __nt_asm_ext_1400622a0(void) { __asm__ __volatile__ ("ret"); }
__attribute__((weak, naked)) void __nt_asm_ext_1400665c4(void) { __asm__ __volatile__ ("ret"); }
__attribute__((weak, naked)) void __nt_asm_ext_140068fa0(void) { __asm__ __volatile__ ("ret"); }
__attribute__((weak, naked)) void __nt_asm_ext_140084d90(void) { __asm__ __volatile__ ("ret"); }
__attribute__((weak, naked)) void __nt_asm_ext_1400879b0(void) { __asm__ __volatile__ ("ret"); }
__attribute__((weak, naked)) void __nt_asm_ext_1400a52d0(void) { __asm__ __volatile__ ("ret"); }
__attribute__((weak, naked)) void __nt_asm_ext_1400b4e70(void) { __asm__ __volatile__ ("ret"); }
__attribute__((weak, naked)) void __nt_asm_ext_1400b9e20(void) { __asm__ __volatile__ ("ret"); }
__attribute__((weak, naked)) void __nt_asm_ext_1400bd9c0(void) { __asm__ __volatile__ ("ret"); }
__attribute__((weak, naked)) void __nt_asm_ext_1400bff30(void) { __asm__ __volatile__ ("ret"); }
__attribute__((weak, naked)) void __nt_asm_ext_1400c0420(void) { __asm__ __volatile__ ("ret"); }
__attribute__((weak, naked)) void __nt_asm_ext_1400c04f0(void) { __asm__ __volatile__ ("ret"); }
__attribute__((weak, naked)) void __nt_asm_ext_1400c0930(void) { __asm__ __volatile__ ("ret"); }
__attribute__((weak, naked)) void __nt_asm_ext_1400c0e60(void) { __asm__ __volatile__ ("ret"); }
__attribute__((weak, naked)) void __nt_asm_ext_1400c4010(void) { __asm__ __volatile__ ("ret"); }
__attribute__((weak, naked)) void __nt_asm_ext_1400c45a0(void) { __asm__ __volatile__ ("ret"); }
__attribute__((weak, naked)) void __nt_asm_ext_1400ca260(void) { __asm__ __volatile__ ("ret"); }
__attribute__((weak, naked)) void __nt_asm_ext_1400fe28c(void) { __asm__ __volatile__ ("ret"); }
__attribute__((weak, naked)) void __nt_asm_ext_1401266b0(void) { __asm__ __volatile__ ("ret"); }
__attribute__((weak, naked)) void __nt_asm_ext_140126eb8(void) { __asm__ __volatile__ ("ret"); }
__attribute__((weak, naked)) void __nt_asm_ext_140126f10(void) { __asm__ __volatile__ ("ret"); }
__attribute__((weak, naked)) void __nt_asm_ext_140127404(void) { __asm__ __volatile__ ("ret"); }
__attribute__((weak, naked)) void __nt_asm_ext_140127ba8(void) { __asm__ __volatile__ ("ret"); }
__attribute__((weak, naked)) void __nt_asm_ext_1401637d0(void) { __asm__ __volatile__ ("ret"); }
__attribute__((weak, naked)) void __nt_asm_ext_140163ca0(void) { __asm__ __volatile__ ("ret"); }
__attribute__((weak, naked)) void __nt_asm_ext_140163d20(void) { __asm__ __volatile__ ("ret"); }
__attribute__((weak, naked)) void __nt_asm_ext_140164530(void) { __asm__ __volatile__ ("ret"); }
__attribute__((weak, naked)) void __nt_asm_ext_1401668d0(void) { __asm__ __volatile__ ("ret"); }
__attribute__((weak, naked)) void __nt_asm_ext_140166a40(void) { __asm__ __volatile__ ("ret"); }
__attribute__((weak, naked)) void __nt_asm_ext_140166eb0(void) { __asm__ __volatile__ ("ret"); }
__attribute__((weak, naked)) void __nt_asm_ext_14016e900(void) { __asm__ __volatile__ ("ret"); }
__attribute__((weak, naked)) void __nt_asm_ext_14016f080(void) { __asm__ __volatile__ ("ret"); }
__attribute__((weak, naked)) void __nt_asm_ext_14016fa40(void) { __asm__ __volatile__ ("ret"); }
__attribute__((weak, naked)) void __nt_asm_ext_14016fcc0(void) { __asm__ __volatile__ ("ret"); }
__attribute__((weak, naked)) void __nt_asm_ext_140170180(void) { __asm__ __volatile__ ("ret"); }
__attribute__((weak, naked)) void __nt_asm_ext_1401fbe54(void) { __asm__ __volatile__ ("ret"); }
__attribute__((weak, naked)) void __nt_asm_ext_1401fe10c(void) { __asm__ __volatile__ ("ret"); }
__attribute__((weak, naked)) void __nt_asm_ext_1401fe1d8(void) { __asm__ __volatile__ ("ret"); }
__attribute__((weak, naked)) void __nt_asm_ext_140201a60(void) { __asm__ __volatile__ ("ret"); }
__attribute__((weak, naked)) void __nt_asm_ext_1402728fc(void) { __asm__ __volatile__ ("ret"); }
__attribute__((weak, naked)) void __nt_asm_ext_140272d0c(void) { __asm__ __volatile__ ("ret"); }
__attribute__((weak, naked)) void __nt_asm_ext_140275690(void) { __asm__ __volatile__ ("ret"); }
__attribute__((weak, naked)) void __nt_asm_ext_140275710(void) { __asm__ __volatile__ ("ret"); }
__attribute__((weak, naked)) void __nt_asm_ext_14027d9fc(void) { __asm__ __volatile__ ("ret"); }
__attribute__((weak, naked)) void __nt_asm_ext_14027db74(void) { __asm__ __volatile__ ("ret"); }
__attribute__((weak, naked)) void __nt_asm_ext_14027dba0(void) { __asm__ __volatile__ ("ret"); }
__attribute__((weak, naked)) void __nt_asm_ext_1402a6460(void) { __asm__ __volatile__ ("ret"); }
__attribute__((weak, naked)) void __nt_asm_ext_14041b3d0(void) { __asm__ __volatile__ ("ret"); }
__attribute__((weak, naked)) void __nt_asm_ext_14041cad0(void) { __asm__ __volatile__ ("ret"); }
__attribute__((weak, naked)) void __nt_asm_ext_14041e460(void) { __asm__ __volatile__ ("ret"); }
__attribute__((weak, naked)) void __nt_asm_ext_140513cd0(void) { __asm__ __volatile__ ("ret"); }
__attribute__((weak, naked)) void __nt_asm_ext_140710720(void) { __asm__ __volatile__ ("ret"); }
__attribute__((weak, naked)) void __nt_asm_ext_14078d140(void) { __asm__ __volatile__ ("ret"); }
__attribute__((weak)) unsigned char __nt_asm_sym_140166a60[64] = {0};
__attribute__((weak)) unsigned char __nt_asm_sym_140169d10[64] = {0};
__attribute__((weak)) unsigned char __nt_asm_sym_140169d60[64] = {0};
__attribute__((weak)) unsigned char __nt_asm_sym_1402c1210[64] = {0};
__attribute__((weak)) unsigned char __nt_asm_sym_140350ba0[64] = {0};
__attribute__((weak)) unsigned char __nt_asm_sym_140350ba8[64] = {0};
__attribute__((weak)) unsigned char __nt_asm_sym_140353370[64] = {0};
__attribute__((weak)) unsigned char __nt_asm_sym_14037fe80[64] = {0};
__attribute__((weak)) unsigned char __nt_asm_sym_14037ff80[64] = {0};
__attribute__((weak)) unsigned char __nt_asm_sym_1403808b0[64] = {0};
__attribute__((weak)) unsigned char __nt_asm_sym_140381e40[64] = {0};
__attribute__((weak)) unsigned char __nt_asm_sym_1403f5440[64] = {0};
__attribute__((weak)) unsigned char __nt_asm_sym_1403f5448[64] = {0};
__attribute__((weak)) unsigned char __nt_asm_sym_1403f5450[64] = {0};
__attribute__((weak)) unsigned char __nt_asm_sym_1403f5458[64] = {0};
__attribute__((weak)) unsigned char __nt_asm_sym_1403f5460[64] = {0};
__attribute__((weak)) unsigned char __nt_asm_sym_1403f5468[64] = {0};
__attribute__((weak)) unsigned char __nt_asm_sym_1403f5470[64] = {0};
__attribute__((weak)) unsigned char __nt_asm_sym_1403f5478[64] = {0};
__attribute__((weak)) unsigned char __nt_asm_sym_1403f5480[64] = {0};
__attribute__((weak)) unsigned char __nt_asm_sym_1403f5488[64] = {0};
__attribute__((weak)) unsigned char __nt_asm_sym_1403f5490[64] = {0};
__attribute__((weak)) unsigned char __nt_asm_sym_1403f5498[64] = {0};
__attribute__((weak)) unsigned char __nt_asm_sym_1403f54a0[64] = {0};
__attribute__((weak)) unsigned char __nt_asm_sym_1403f54a8[64] = {0};
__attribute__((weak)) unsigned char __nt_asm_sym_1403f54b0[64] = {0};
__attribute__((weak)) unsigned char __nt_asm_sym_1403f54b8[64] = {0};
__attribute__((weak)) unsigned char __nt_asm_sym_1403f54c0[64] = {0};
__attribute__((weak)) unsigned char __nt_asm_sym_1403f54c8[64] = {0};
__attribute__((weak)) unsigned char __nt_asm_sym_1403f54d0[64] = {0};
__attribute__((weak)) unsigned char __nt_asm_sym_1403f54d8[64] = {0};
__attribute__((weak)) unsigned char __nt_asm_sym_1403f54e0[64] = {0};
__attribute__((weak)) unsigned char __nt_asm_sym_1403f54e8[64] = {0};
__attribute__((weak)) unsigned char __nt_asm_sym_1403f54f0[64] = {0};
__attribute__((weak)) unsigned char __nt_asm_sym_1403f54f8[64] = {0};
__attribute__((weak)) unsigned char __nt_asm_sym_1403f5500[64] = {0};
__attribute__((weak)) unsigned char __nt_asm_sym_1403f5508[64] = {0};
__attribute__((weak)) unsigned char __nt_asm_sym_1403f5510[64] = {0};
__attribute__((weak)) unsigned char __nt_asm_sym_1403f5518[64] = {0};
__attribute__((weak)) unsigned char __nt_asm_sym_1403f5520[64] = {0};
__attribute__((weak)) unsigned char __nt_asm_sym_1403f5528[64] = {0};
__attribute__((weak)) unsigned char __nt_asm_sym_1403f5530[64] = {0};
__attribute__((weak)) unsigned char __nt_asm_sym_1403f5538[64] = {0};
__attribute__((weak)) unsigned char __nt_asm_sym_1403f5540[64] = {0};
__attribute__((weak)) unsigned char __nt_asm_sym_1403f5548[64] = {0};
__attribute__((weak)) unsigned char __nt_asm_sym_1403f5550[64] = {0};
__attribute__((weak)) unsigned char __nt_asm_sym_1403f5558[64] = {0};
__attribute__((weak)) unsigned char __nt_asm_sym_1403f5560[64] = {0};
__attribute__((weak)) unsigned char __nt_asm_sym_1403f5568[64] = {0};
__attribute__((weak)) unsigned char __nt_asm_sym_1403f5570[64] = {0};
__attribute__((weak)) unsigned char __nt_asm_sym_1403f5578[64] = {0};
__attribute__((weak)) unsigned char __nt_asm_sym_1403f5580[64] = {0};
__attribute__((weak)) unsigned char __nt_asm_sym_1403f5588[64] = {0};
__attribute__((weak)) unsigned char __nt_asm_sym_1403f5590[64] = {0};
__attribute__((weak)) unsigned char __nt_asm_sym_1403f5598[64] = {0};
__attribute__((weak)) unsigned char __nt_asm_sym_1403f55a0[64] = {0};
__attribute__((weak)) unsigned char __nt_asm_sym_1403f55a8[64] = {0};
__attribute__((weak)) unsigned char __nt_asm_sym_1403f55b0[64] = {0};
__attribute__((weak)) unsigned char __nt_asm_sym_1403f55b8[64] = {0};
__attribute__((weak)) unsigned char __nt_asm_sym_1403f55d8[64] = {0};
__attribute__((weak)) unsigned char __nt_asm_sym_1403f55e0[64] = {0};
__attribute__((weak)) unsigned char __nt_asm_sym_1403f55e8[64] = {0};
__attribute__((weak)) unsigned char __nt_asm_sym_1403f55f0[64] = {0};
__attribute__((weak)) unsigned char __nt_asm_sym_1403f55f8[64] = {0};
__attribute__((weak)) unsigned char __nt_asm_sym_1403f5600[64] = {0};
__attribute__((weak)) unsigned char __nt_asm_sym_1403f5618[64] = {0};
__attribute__((weak)) unsigned char __nt_asm_sym_1403f5620[64] = {0};
__attribute__((weak)) unsigned char __nt_asm_sym_1403f5628[64] = {0};
__attribute__((weak)) unsigned char __nt_asm_sym_1403f5630[64] = {0};
__attribute__((weak)) unsigned char __nt_asm_sym_1403f5640[64] = {0};
__attribute__((weak)) unsigned char __nt_asm_sym_1403f5648[64] = {0};
__attribute__((weak)) unsigned char __nt_asm_sym_1403f5650[64] = {0};
__attribute__((weak)) unsigned char __nt_asm_sym_1403f5658[64] = {0};
__attribute__((weak)) unsigned char __nt_asm_sym_1403f5660[64] = {0};
__attribute__((weak)) unsigned char __nt_asm_sym_1403f5668[64] = {0};
__attribute__((weak)) unsigned char __nt_asm_sym_1403f5670[64] = {0};
__attribute__((weak)) unsigned char __nt_asm_sym_1403f5678[64] = {0};
__attribute__((weak)) unsigned char __nt_asm_sym_1403f5680[64] = {0};
__attribute__((weak)) unsigned char __nt_asm_sym_1403f5688[64] = {0};
__attribute__((weak)) unsigned char __nt_asm_sym_1403f5690[64] = {0};
__attribute__((weak)) unsigned char __nt_asm_sym_1403f5698[64] = {0};
__attribute__((weak)) unsigned char __nt_asm_sym_1403f5778[64] = {0};
__attribute__((weak)) unsigned char __nt_asm_sym_1403f5780[64] = {0};
__attribute__((weak)) unsigned char __nt_asm_sym_1403f8068[64] = {0};
__attribute__((weak)) unsigned char __nt_asm_sym_1403f90c8[64] = {0};
__attribute__((weak)) unsigned char __nt_asm_sym_1403f9130[64] = {0};
__attribute__((weak)) unsigned char __nt_asm_sym_1403f9144[64] = {0};
__attribute__((weak)) unsigned char __nt_asm_sym_1403f9146[64] = {0};
__attribute__((weak)) unsigned char __nt_asm_sym_1403f9148[64] = {0};
__attribute__((weak)) unsigned char __nt_asm_sym_1403f9288[64] = {0};
__attribute__((weak)) unsigned char __nt_asm_sym_1403f96c0[64] = {0};
__attribute__((weak)) unsigned char __nt_asm_sym_1403f96e0[64] = {0};
__attribute__((weak)) unsigned char __nt_asm_sym_1403f9700[64] = {0};
__attribute__((weak)) unsigned char __nt_asm_sym_140410880[64] = {0};
__attribute__((weak)) unsigned char __nt_asm_sym_140411280[64] = {0};

/* Weak Fallback Stubs for External Internal Callees (Overridden by Strong Runtime) */
__attribute__((weak)) uint64_t __attribute__((ms_abi)) AlpcpDereferenceBlobEx(uint64_t a0, uint64_t a1, uint64_t a2, uint64_t a3) { return 0ULL; }
__attribute__((weak)) uint64_t __attribute__((ms_abi)) AlpcpSendMessage(uint64_t a0, uint64_t a1, uint64_t a2, uint64_t a3) { return 0ULL; }
__attribute__((weak)) uint64_t __attribute__((ms_abi)) AlpcpUnlockBlob(uint64_t a0, uint64_t a1, uint64_t a2, uint64_t a3) { return 0ULL; }
__attribute__((weak)) uint64_t __attribute__((ms_abi)) CcCopyWriteEx(uint64_t a0, uint64_t a1, uint64_t a2, uint64_t a3) { return 0ULL; }
__attribute__((weak)) uint64_t __attribute__((ms_abi)) CcFlushCachePriv(uint64_t a0, uint64_t a1, uint64_t a2, uint64_t a3) { return 0ULL; }
__attribute__((weak)) uint64_t __attribute__((ms_abi)) CcForEachPartition(uint64_t a0, uint64_t a1, uint64_t a2, uint64_t a3) { return 0ULL; }
__attribute__((weak)) uint64_t __attribute__((ms_abi)) CcMdlReadComplete2(uint64_t a0, uint64_t a1, uint64_t a2, uint64_t a3) { return 0ULL; }
__attribute__((weak)) uint64_t __attribute__((ms_abi)) CcNotifyWriteBehindInternal(uint64_t a0, uint64_t a1, uint64_t a2, uint64_t a3) { return 0ULL; }
__attribute__((weak)) uint64_t __attribute__((ms_abi)) CmpFindNameInList(uint64_t a0, uint64_t a1, uint64_t a2, uint64_t a3) { return 0ULL; }
__attribute__((weak)) uint64_t __attribute__((ms_abi)) EtwWriteEx(uint64_t a0, uint64_t a1, uint64_t a2, uint64_t a3) { return 0ULL; }
__attribute__((weak)) uint64_t __attribute__((ms_abi)) EtwpTraceMessageVa(uint64_t a0, uint64_t a1, uint64_t a2, uint64_t a3) { return 0ULL; }
__attribute__((weak)) uint64_t __attribute__((ms_abi)) ExAcquirePushLockExclusiveEx(uint64_t a0, uint64_t a1, uint64_t a2, uint64_t a3) { return 0ULL; }
__attribute__((weak)) uint64_t __attribute__((ms_abi)) ExAcquireResourceSharedLite(uint64_t a0, uint64_t a1, uint64_t a2, uint64_t a3) { return 0ULL; }
__attribute__((weak)) uint64_t __attribute__((ms_abi)) ExAcquireSharedWaitForExclusive(uint64_t a0, uint64_t a1, uint64_t a2, uint64_t a3) { return 0ULL; }
__attribute__((weak)) uint64_t __attribute__((ms_abi)) ExAllocateTimerInternal2(uint64_t a0, uint64_t a1, uint64_t a2, uint64_t a3) { return 0ULL; }
__attribute__((weak)) uint64_t __attribute__((ms_abi)) ExFreePoolWithTag(uint64_t a0, uint64_t a1, uint64_t a2, uint64_t a3) { return 0ULL; }
__attribute__((weak)) uint64_t __attribute__((ms_abi)) ExNotifyCallback(uint64_t a0, uint64_t a1, uint64_t a2, uint64_t a3) { return 0ULL; }
__attribute__((weak)) uint64_t __attribute__((ms_abi)) ExQueueWorkItemFromIo(uint64_t a0, uint64_t a1, uint64_t a2, uint64_t a3) { return 0ULL; }
__attribute__((weak)) uint64_t __attribute__((ms_abi)) ExRaiseHardError(uint64_t a0, uint64_t a1, uint64_t a2, uint64_t a3) { return 0ULL; }
__attribute__((weak)) uint64_t __attribute__((ms_abi)) ExpTryQueueWorkItem(uint64_t a0, uint64_t a1, uint64_t a2, uint64_t a3) { return 0ULL; }
__attribute__((weak)) uint64_t __attribute__((ms_abi)) FsRtlAddLargeMcbEntry(uint64_t a0, uint64_t a1, uint64_t a2, uint64_t a3) { return 0ULL; }
__attribute__((weak)) uint64_t __attribute__((ms_abi)) FsRtlPrivateFastUnlockAll(uint64_t a0, uint64_t a1, uint64_t a2, uint64_t a3) { return 0ULL; }
__attribute__((weak)) uint64_t __attribute__((ms_abi)) IoPropagateIrpExtensionEx(uint64_t a0, uint64_t a1, uint64_t a2, uint64_t a3) { return 0ULL; }
__attribute__((weak)) uint64_t __attribute__((ms_abi)) IopQueryXxxInformation(uint64_t a0, uint64_t a1, uint64_t a2, uint64_t a3) { return 0ULL; }
__attribute__((weak)) uint64_t __attribute__((ms_abi)) IopQueueWorkItemProlog(uint64_t a0, uint64_t a1, uint64_t a2, uint64_t a3) { return 0ULL; }
__attribute__((weak)) BOOLEAN __attribute__((ms_abi)) KdEnterDebugger(void* TrapFrame) { return (BOOLEAN)0; }
__attribute__((weak)) void __attribute__((ms_abi)) KdExitDebugger(BOOLEAN Enable) { return (void)0; }
__attribute__((weak)) BOOLEAN __attribute__((ms_abi)) KdpPrintString(struct _STRING* Output) { return (BOOLEAN)0; }
__attribute__((weak)) uint64_t __attribute__((ms_abi)) KeBugCheckEx(uint64_t a0, uint64_t a1, uint64_t a2, uint64_t a3) { return 0ULL; }
__attribute__((weak)) uint64_t __attribute__((ms_abi)) KeCopyAffinityEx(uint64_t a0, uint64_t a1, uint64_t a2, uint64_t a3) { return 0ULL; }
__attribute__((weak)) uint64_t __attribute__((ms_abi)) KeExpandKernelStackAndCalloutInternal(uint64_t a0, uint64_t a1, uint64_t a2, uint64_t a3) { return 0ULL; }
__attribute__((weak)) uint64_t __attribute__((ms_abi)) KeLeaveCriticalRegion(uint64_t a0, uint64_t a1, uint64_t a2, uint64_t a3) { return 0ULL; }
__attribute__((weak)) uint64_t __attribute__((ms_abi)) KeQueryActiveProcessorCountEx(uint64_t a0, uint64_t a1, uint64_t a2, uint64_t a3) { return 0ULL; }
__attribute__((weak)) uint64_t __attribute__((ms_abi)) KeSetTargetProcessorDpcEx(uint64_t a0, uint64_t a1, uint64_t a2, uint64_t a3) { return 0ULL; }
__attribute__((weak)) uint64_t __attribute__((ms_abi)) KeSetTimer2(uint64_t a0, uint64_t a1, uint64_t a2, uint64_t a3) { return 0ULL; }
__attribute__((weak)) uint64_t __attribute__((ms_abi)) KeWaitForSingleObject(uint64_t a0, uint64_t a1, uint64_t a2, uint64_t a3) { return 0ULL; }
__attribute__((weak)) uint64_t __attribute__((ms_abi)) KiInsertQueueDpc(uint64_t a0, uint64_t a1, uint64_t a2, uint64_t a3) { return 0ULL; }
__attribute__((weak)) uint64_t __attribute__((ms_abi)) KiSetHeteroPolicyThread(uint64_t a0, uint64_t a1, uint64_t a2, uint64_t a3) { return 0ULL; }
__attribute__((weak)) BOOLEAN __attribute__((ms_abi)) KiSetTimerEx(void* Timer, int64_t DueTime, ULONG Period, uint64_t Hand) { return (BOOLEAN)0; }
__attribute__((weak)) uint64_t __attribute__((ms_abi)) LdrpSearchResourceSection_U(uint64_t a0, uint64_t a1, uint64_t a2, uint64_t a3) { return 0ULL; }
__attribute__((weak)) uint64_t __attribute__((ms_abi)) MiAddPhysicalMemoryChunks(uint64_t a0, uint64_t a1, uint64_t a2, uint64_t a3) { return 0ULL; }
__attribute__((weak)) uint64_t __attribute__((ms_abi)) MiAllocateContiguousMemory(uint64_t a0, uint64_t a1, uint64_t a2, uint64_t a3) { return 0ULL; }
__attribute__((weak)) uint64_t __attribute__((ms_abi)) MiAllocatePagesForMdl(uint64_t a0, uint64_t a1, uint64_t a2, uint64_t a3) { return 0ULL; }
__attribute__((weak)) uint64_t __attribute__((ms_abi)) MiFreePagesFromMdl(uint64_t a0, uint64_t a1, uint64_t a2, uint64_t a3) { return 0ULL; }
__attribute__((weak)) BOOLEAN __attribute__((ms_abi)) MiIsAddressValid(void* VirtualAddress) { return (BOOLEAN)0; }
__attribute__((weak)) uint64_t __attribute__((ms_abi)) MiMapViewInSystemSpace(uint64_t a0, uint64_t a1, uint64_t a2, uint64_t a3) { return 0ULL; }
__attribute__((weak)) uint64_t __attribute__((ms_abi)) MiProbeAndLockPages(uint64_t a0, uint64_t a1, uint64_t a2, uint64_t a3) { return 0ULL; }
__attribute__((weak)) uint64_t __attribute__((ms_abi)) MiUnloadSystemImage(uint64_t a0, uint64_t a1, uint64_t a2, uint64_t a3) { return 0ULL; }
__attribute__((weak)) uint64_t __attribute__((ms_abi)) MmAcquireLoadLock(uint64_t a0, uint64_t a1, uint64_t a2, uint64_t a3) { return 0ULL; }
__attribute__((weak)) uint64_t __attribute__((ms_abi)) MmLoadSystemImageEx(uint64_t a0, uint64_t a1, uint64_t a2, uint64_t a3) { return 0ULL; }
__attribute__((weak)) uint64_t __attribute__((ms_abi)) MmReleaseLoadLock(uint64_t a0, uint64_t a1, uint64_t a2, uint64_t a3) { return 0ULL; }
__attribute__((weak)) uint64_t __attribute__((ms_abi)) NtOpenProcessTokenEx(uint64_t a0, uint64_t a1, uint64_t a2, uint64_t a3) { return 0ULL; }
__attribute__((weak)) uint64_t __attribute__((ms_abi)) NtOpenThreadTokenEx(uint64_t a0, uint64_t a1, uint64_t a2, uint64_t a3) { return 0ULL; }
__attribute__((weak)) uint64_t __attribute__((ms_abi)) ObCreateObjectTypeEx(uint64_t a0, uint64_t a1, uint64_t a2, uint64_t a3) { return 0ULL; }
__attribute__((weak)) uint64_t __attribute__((ms_abi)) ObDereferenceSecurityDescriptor(uint64_t a0, uint64_t a1, uint64_t a2, uint64_t a3) { return 0ULL; }
__attribute__((weak)) uint64_t __attribute__((ms_abi)) ObMakeTemporaryObject(uint64_t a0, uint64_t a1, uint64_t a2, uint64_t a3) { return 0ULL; }
__attribute__((weak)) int64_t __attribute__((ms_abi)) ObfDereferenceObjectWithTag(void* Object, ULONG Tag) { return (int64_t)0; }
__attribute__((weak)) uint64_t __attribute__((ms_abi)) ObfReferenceObject(uint64_t a0, uint64_t a1, uint64_t a2, uint64_t a3) { return 0ULL; }
__attribute__((weak)) uint64_t __attribute__((ms_abi)) ObfReferenceObjectWithTag(uint64_t a0, uint64_t a1, uint64_t a2, uint64_t a3) { return 0ULL; }
__attribute__((weak)) BOOLEAN __attribute__((ms_abi)) ObpIsKernelHandle(void* Handle, BOOLEAN PreviousMode) { return (BOOLEAN)0; }
__attribute__((weak)) uint64_t __attribute__((ms_abi)) ObpLookupDirectoryEntryEx(uint64_t a0, uint64_t a1, uint64_t a2, uint64_t a3) { return 0ULL; }
__attribute__((weak)) uint64_t __attribute__((ms_abi)) PoFxIssueComponentPerfStateChangeMultiple(uint64_t a0, uint64_t a1, uint64_t a2, uint64_t a3) { return 0ULL; }
__attribute__((weak)) uint64_t __attribute__((ms_abi)) PopAcquireRwLockExclusive(uint64_t a0, uint64_t a1, uint64_t a2, uint64_t a3) { return 0ULL; }
__attribute__((weak)) uint64_t __attribute__((ms_abi)) PopHandleConvergedPowerRequestUpdate(uint64_t a0, uint64_t a1, uint64_t a2, uint64_t a3) { return 0ULL; }
__attribute__((weak)) uint64_t __attribute__((ms_abi)) PopReleaseRwLock(uint64_t a0, uint64_t a1, uint64_t a2, uint64_t a3) { return 0ULL; }
__attribute__((weak)) uint64_t __attribute__((ms_abi)) PsBoostThreadIoEx(uint64_t a0, uint64_t a1, uint64_t a2, uint64_t a3) { return 0ULL; }
__attribute__((weak)) uint64_t __attribute__((ms_abi)) RtlCheckTokenMembershipEx(uint64_t a0, uint64_t a1, uint64_t a2, uint64_t a3) { return 0ULL; }
__attribute__((weak)) uint64_t __attribute__((ms_abi)) RtlFindNextForwardRunClearCapped(uint64_t a0, uint64_t a1, uint64_t a2, uint64_t a3) { return 0ULL; }
__attribute__((weak)) uint64_t __attribute__((ms_abi)) RtlFindNextForwardRunClearCappedEx(uint64_t a0, uint64_t a1, uint64_t a2, uint64_t a3) { return 0ULL; }
__attribute__((weak)) NTSTATUS __attribute__((ms_abi)) RtlHashUnicodeString(const struct _UNICODE_STRING* String, BOOLEAN CaseInSensitive, ULONG HashAlgorithm, ULONG* HashValue) { return (NTSTATUS)0; }
__attribute__((weak)) uint64_t __attribute__((ms_abi)) RtlpAddKnownAce(uint64_t a0, uint64_t a1, uint64_t a2, uint64_t a3) { return 0ULL; }
__attribute__((weak)) uint64_t __attribute__((ms_abi)) RtlpComputeCrcInternal(uint64_t a0, uint64_t a1, uint64_t a2, uint64_t a3) { return 0ULL; }
__attribute__((weak)) uint64_t __attribute__((ms_abi)) RtlpCreateHashTable(uint64_t a0, uint64_t a1, uint64_t a2, uint64_t a3) { return 0ULL; }
__attribute__((weak)) uint64_t __attribute__((ms_abi)) RtlpGetTokenNamedObjectPath(uint64_t a0, uint64_t a1, uint64_t a2, uint64_t a3) { return 0ULL; }
__attribute__((weak)) uint64_t __attribute__((ms_abi)) RtlpOpenImageFileOptionsKeyEx(uint64_t a0, uint64_t a1, uint64_t a2, uint64_t a3) { return 0ULL; }
__attribute__((weak)) uint64_t __attribute__((ms_abi)) RtlpSetSecurityObject(uint64_t a0, uint64_t a1, uint64_t a2, uint64_t a3) { return 0ULL; }
__attribute__((weak)) uint64_t __attribute__((ms_abi)) RtlpSystemBootStatusRequest(uint64_t a0, uint64_t a1, uint64_t a2, uint64_t a3) { return 0ULL; }
__attribute__((weak)) uint64_t __attribute__((ms_abi)) SeComputeAutoInheritByObjectTypeEx(uint64_t a0, uint64_t a1, uint64_t a2, uint64_t a3) { return 0ULL; }
__attribute__((weak)) uint64_t __attribute__((ms_abi)) SeReportSecurityEventWithSubCategory(uint64_t a0, uint64_t a1, uint64_t a2, uint64_t a3) { return 0ULL; }
__attribute__((weak)) uint64_t __attribute__((ms_abi)) SepAdjustAccessStateForConstraints(uint64_t a0, uint64_t a1, uint64_t a2, uint64_t a3) { return 0ULL; }
__attribute__((weak)) uint64_t __attribute__((ms_abi)) SepCreateAccessStateFromSubjectContext(uint64_t a0, uint64_t a1, uint64_t a2, uint64_t a3) { return 0ULL; }
__attribute__((weak)) uint64_t __attribute__((ms_abi)) SepInternalSetSecurityAttributesToken(uint64_t a0, uint64_t a1, uint64_t a2, uint64_t a3) { return 0ULL; }
__attribute__((weak)) BOOLEAN __attribute__((ms_abi)) SepSidInToken(void* Token, void* PrincipalSelfSid, void* Sid, BOOLEAN DenyOnly, BOOLEAN Restricted, BOOLEAN SingleSid) { return (BOOLEAN)0; }
__attribute__((weak)) uint64_t __attribute__((ms_abi)) _vsprintf_l(uint64_t a0, uint64_t a1, uint64_t a2, uint64_t a3) { return 0ULL; }
__attribute__((weak)) uint64_t __attribute__((ms_abi)) memset(uint64_t a0, uint64_t a1, uint64_t a2, uint64_t a3) { return 0ULL; }
__attribute__((weak)) uint64_t __attribute__((ms_abi)) vDbgPrintExWithPrefixInternal(uint64_t a0, uint64_t a1, uint64_t a2, uint64_t a3) { return 0ULL; }
__attribute__((weak)) uint64_t __attribute__((ms_abi)) vsprintf_s(uint64_t a0, uint64_t a1, uint64_t a2, uint64_t a3) { return 0ULL; }
__attribute__((weak)) uint64_t __attribute__((ms_abi)) vswprintf_s(uint64_t a0, uint64_t a1, uint64_t a2, uint64_t a3) { return 0ULL; }
