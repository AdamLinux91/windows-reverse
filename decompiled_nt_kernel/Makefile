# ==============================================================================
# Decompiled Windows NT 10.0.16299.15 Hybrid Kernel — Build & Drop-In System
#
# Targets:
#   make all            - Build standalone Windows ntoskrnl.exe, UEFI BOOTX64.EFI,
#                         Multiboot2 ELF64 & GPT/FAT32 USB image
#   make windows-kernel - Build pure from-source Windows PE32+ kernel (build/ntoskrnl.exe)
#   make windows-dropin - Legacy graft of all compiled subsystem entry points onto a host
#                         ntoskrnl.exe (not a behavior proof; use windows-hybrid first).
#   make windows-hybrid - Conservative host-backed graft: only functions in
#                         specs/hybrid_patch_allowlist.json are redirected; every other
#                         kernel path remains the stock host implementation.
#   make uefi           - Build relocatable UEFI PE32+ image (build/BOOTX64.EFI)
#   make multiboot2     - Build bare-metal Multiboot2 ELF64 image (build/nt_hybrid_kernel.elf)
#   make usb-image      - Build bootable GPT+FAT32 USB flash image (build/nt_hardware_usb.img)
#   make test-silicon   - Execute all 16 decompiled NT subsystems on live x86-64 CPU silicon
#   make clean          - Clean build artifacts
# ==============================================================================

CC       ?= gcc
LD       ?= ld
PYTHON   ?= python3
BUILD    := build
NTOSKRNL ?= /mnt/c/Windows/System32/ntoskrnl.exe
HYBRID_PATCH_LIST ?= specs/hybrid_patch_allowlist.json

NT_PURE_SUBSYS_SRCS := \
	src/ke/ke_kernel.c \
	src/ps/ps_process.c \
	src/mm/mm_memory.c \
	src/ob/ob_object.c \
	src/io/io_manager.c \
	src/ex/ex_executive.c \
	src/rtl/rtl_library.c \
	src/se/se_kd_security.c \
	src/cm/cm_registry.c \
	src/cc/cc_cache.c \
	src/fsrtl/fsrtl_filesystem.c \
	src/po/po_power.c \
	src/alpc/alpc_ipc.c \
	src/tm/tm_transactions.c \
	src/etw_vf/etw_vf_diag.c \
	src/zw_nt/zw_nt_syscalls.c

NT_SUBSYS_SRCS := \
	$(NT_PURE_SUBSYS_SRCS) \
	src/runtime/nt_reconstructed_globals.c \
	src/runtime/nt_kernel_runtime.c

COMMON_WARN := -Wall -Werror -Wno-address-of-packed-member -Wno-unused-function -Wno-unused-variable -Wno-unused-but-set-variable
FREESTANDING_CFLAGS := \
	$(COMMON_WARN) -O2 -std=gnu11 -ffreestanding -fno-builtin \
	-fno-stack-protector -fno-ident -fno-asynchronous-unwind-tables \
	-mno-red-zone -mcx16 -mabi=ms -Iinclude

UEFI_CFLAGS   := $(FREESTANDING_CFLAGS) -fPIE
MB2_CFLAGS    := $(FREESTANDING_CFLAGS) -fno-pie -fno-pic
WINPE_CFLAGS  := $(FREESTANDING_CFLAGS) -fno-pie -fno-pic
HOST_CFLAGS   := $(COMMON_WARN) -O2 -std=gnu11 -fno-builtin -mcx16 -DNT_HOST_SILICON_TEST -Iinclude

WINPE_PURE_OBJS := $(patsubst %.c,$(BUILD)/winpe/%.o,$(NT_PURE_SUBSYS_SRCS))
# Dynamic cross-build grafts also need weak reconstructed-global definitions;
# exact retail-PDB mode still binds those symbols to the host kernel.
WINPE_DROPIN_OBJS := $(WINPE_PURE_OBJS) $(BUILD)/winpe/src/runtime/nt_reconstructed_globals.o
WINPE_ALL_OBJS  := $(BUILD)/winpe/arch/x86_64/ki_system_startup.o \
                   $(BUILD)/winpe/src/runtime/nt_windows_boot.o \
                   $(patsubst %.c,$(BUILD)/winpe/%.o,$(NT_SUBSYS_SRCS))

UEFI_OBJS    := $(patsubst %.c,$(BUILD)/uefi/%.o,$(NT_SUBSYS_SRCS) arch/x86_64/uefi_entry.c)
MB2_OBJS     := $(BUILD)/mb2/arch/x86_64/multiboot2_entry.o \
                $(patsubst %.c,$(BUILD)/mb2/%.o,$(NT_SUBSYS_SRCS) arch/x86_64/baremetal_main.c)
HOST_NT_OBJS := $(patsubst %.c,$(BUILD)/host/%.o,$(NT_SUBSYS_SRCS))

WIN_PE    := $(BUILD)/ntoskrnl.exe
DROPIN_PE := $(BUILD)/ntoskrnl_dropin.exe
HYBRID_PE := $(BUILD)/ntoskrnl_hybrid.exe
EFI_BIN   := $(BUILD)/BOOTX64.EFI
MB2_ELF   := $(BUILD)/nt_hybrid_kernel.elf
USB_IMG   := $(BUILD)/nt_hardware_usb.img
HOST_BIN  := $(BUILD)/silicon_harness

.PHONY: all windows-kernel windows-dropin windows-hybrid uefi multiboot2 usb-image test-silicon clean

all: windows-kernel uefi multiboot2 usb-image

windows-kernel: $(WIN_PE)

windows-dropin: $(DROPIN_PE)

windows-hybrid: $(HYBRID_PE)

uefi: $(EFI_BIN)

multiboot2: $(MB2_ELF)

usb-image: $(USB_IMG)

# --- Windows IMAGE_SUBSYSTEM_NATIVE (1) PE32+ Kernel Targets ---
$(BUILD)/winpe/arch/x86_64/ki_system_startup.o: arch/x86_64/ki_system_startup.S
	@mkdir -p $(dir $@)
	$(CC) -O2 -fno-pie -fno-pic -c $< -o $@

$(BUILD)/winpe/%.o: %.c
	@mkdir -p $(dir $@)
	$(CC) $(WINPE_CFLAGS) -c $< -o $@

$(WIN_PE): $(WINPE_ALL_OBJS) scripts/link_ntoskrnl_pe.py scripts/pe_elf_linker.py
	@mkdir -p $(dir $@)
	$(PYTHON) scripts/link_ntoskrnl_pe.py \
		--specs-dir specs \
		--proofs proofs/z3_equivalence_proofs.json \
		--out $@ \
		$(WINPE_ALL_OBJS)

$(DROPIN_PE): $(WINPE_DROPIN_OBJS) scripts/build_windows_dropin_ntoskrnl.py scripts/pe_elf_linker.py
	@mkdir -p $(dir $@)
	$(PYTHON) scripts/build_windows_dropin_ntoskrnl.py \
		--host-ntoskrnl "$(NTOSKRNL)" \
		--specs-dir specs \
		--proofs proofs/z3_equivalence_proofs.json \
		--out $@ \
		$(WINPE_DROPIN_OBJS)

# A functional staged target: the stock host kernel handles all unlisted entry points,
# so Windows application compatibility is preserved while the allowlisted candidates are
# exercised in a real guest. This is intentionally separate from the legacy all-grafted
# target, which is useful for research but is not an application-compatibility claim.
$(HYBRID_PE): $(WINPE_DROPIN_OBJS) scripts/build_windows_dropin_ntoskrnl.py scripts/pe_elf_linker.py specs/hybrid_patch_allowlist.json
	@mkdir -p $(dir $@)
	$(PYTHON) scripts/build_windows_dropin_ntoskrnl.py \
		--host-ntoskrnl "$(NTOSKRNL)" \
		--specs-dir specs \
		--proofs proofs/z3_equivalence_proofs.json \
		--patch-list "$(HYBRID_PATCH_LIST)" \
		--out $@ \
		$(WINPE_DROPIN_OBJS)

# --- UEFI Bare-Metal Hardware Target ---
$(BUILD)/uefi/%.o: %.c
	@mkdir -p $(dir $@)
	$(CC) $(UEFI_CFLAGS) -c $< -o $@

$(EFI_BIN): $(UEFI_OBJS)
	@mkdir -p $(dir $@)
	$(LD) -m i386pep --oformat pei-x86-64 --subsystem 10 --dynamicbase -pie \
		-e efi_main -o $@ $(UEFI_OBJS)

# --- Multiboot2 / GRUB2 Bare-Metal Ring-0 Target ---
$(BUILD)/mb2/arch/x86_64/multiboot2_entry.o: arch/x86_64/multiboot2_entry.S
	@mkdir -p $(dir $@)
	$(CC) -O2 -fno-pie -fno-pic -c $< -o $@

$(BUILD)/mb2/%.o: %.c
	@mkdir -p $(dir $@)
	$(CC) $(MB2_CFLAGS) -c $< -o $@

$(MB2_ELF): $(MB2_OBJS) arch/x86_64/linker_multiboot2.ld
	@mkdir -p $(dir $@)
	$(LD) -m elf_x86_64 -n -nostdlib --no-warn-rwx-segments \
		-T arch/x86_64/linker_multiboot2.ld -o $@ $(MB2_OBJS)

# --- GPT + FAT32 Physical USB Boot Image ---
$(USB_IMG): $(EFI_BIN) $(MB2_ELF) scripts/make_bootable_usb.py
	@mkdir -p $(dir $@)
	$(PYTHON) scripts/make_bootable_usb.py $(EFI_BIN) $(MB2_ELF) $@

# --- Live x86-64 CPU Silicon Execution Harness ---
$(BUILD)/host/%.o: %.c
	@mkdir -p $(dir $@)
	$(CC) $(HOST_CFLAGS) -ffreestanding -fno-stack-protector -fno-tree-loop-distribute-patterns -mabi=ms -c $< -o $@

$(BUILD)/host/silicon_harness.o: tests/silicon_harness.c
	@mkdir -p $(dir $@)
	$(CC) $(HOST_CFLAGS) -c $< -o $@

$(HOST_BIN): $(HOST_NT_OBJS) $(BUILD)/host/silicon_harness.o
	$(CC) -O2 -mcx16 -o $@ $^

test-silicon: $(HOST_BIN)
	./$(HOST_BIN)

clean:
	rm -rf $(BUILD)
