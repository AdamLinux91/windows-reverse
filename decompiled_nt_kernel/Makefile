# ==============================================================================
# Decompiled Windows NT 10.0.16299.15 Hybrid Kernel — Hardware Build System
#
# Targets:
#   make all          - Build UEFI BOOTX64.EFI, Multiboot2 ELF64 & GPT/FAT32 USB image
#   make uefi         - Build relocatable UEFI PE32+ image (build/BOOTX64.EFI)
#   make multiboot2   - Build bare-metal Multiboot2 ELF64 image (build/nt_hybrid_kernel.elf)
#   make usb-image    - Build bootable GPT+FAT32 USB flash image (build/nt_hardware_usb.img)
#   make test-silicon - Execute all 8 decompiled NT subsystems on live x86-64 CPU silicon
#   make clean        - Clean build artifacts
# ==============================================================================

CC       ?= gcc
LD       ?= ld
PYTHON   ?= python3
BUILD    := build

NT_SUBSYS_SRCS := \
	src/ke/ke_kernel.c \
	src/ps/ps_process.c \
	src/mm/mm_memory.c \
	src/ob/ob_object.c \
	src/io/io_manager.c \
	src/ex/ex_executive.c \
	src/rtl/rtl_library.c \
	src/se/se_kd_security.c \
	src/runtime/nt_reconstructed_globals.c \
	src/runtime/nt_kernel_runtime.c

COMMON_WARN := -Wall -Werror -Wno-unused-function -Wno-unused-variable -Wno-unused-but-set-variable
FREESTANDING_CFLAGS := \
	$(COMMON_WARN) -O2 -std=gnu11 -ffreestanding -fno-builtin \
	-fno-stack-protector -fno-ident -fno-asynchronous-unwind-tables \
	-mno-red-zone -mcx16 -mabi=ms -Iinclude

UEFI_CFLAGS := $(FREESTANDING_CFLAGS) -fPIE
MB2_CFLAGS  := $(FREESTANDING_CFLAGS) -fno-pie -fno-pic
HOST_CFLAGS := $(COMMON_WARN) -O2 -std=gnu11 -fno-builtin -mcx16 -DNT_HOST_SILICON_TEST -Iinclude

UEFI_OBJS := $(patsubst %.c,$(BUILD)/uefi/%.o,$(NT_SUBSYS_SRCS) arch/x86_64/uefi_entry.c)
MB2_OBJS  := $(BUILD)/mb2/arch/x86_64/multiboot2_entry.o \
             $(patsubst %.c,$(BUILD)/mb2/%.o,$(NT_SUBSYS_SRCS) arch/x86_64/baremetal_main.c)
HOST_NT_OBJS := $(patsubst %.c,$(BUILD)/host/%.o,$(NT_SUBSYS_SRCS))

EFI_BIN  := $(BUILD)/BOOTX64.EFI
MB2_ELF  := $(BUILD)/nt_hybrid_kernel.elf
USB_IMG  := $(BUILD)/nt_hardware_usb.img
HOST_BIN := $(BUILD)/silicon_harness

.PHONY: all uefi multiboot2 usb-image test-silicon clean

all: uefi multiboot2 usb-image

uefi: $(EFI_BIN)

multiboot2: $(MB2_ELF)

usb-image: $(USB_IMG)

$(BUILD)/uefi/%.o: %.c
	@mkdir -p $(dir $@)
	$(CC) $(UEFI_CFLAGS) -c $< -o $@

$(EFI_BIN): $(UEFI_OBJS)
	@mkdir -p $(dir $@)
	$(LD) -m i386pep --oformat pei-x86-64 --subsystem 10 --dynamicbase -pie \
		-e efi_main -o $@ $(UEFI_OBJS)

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

$(USB_IMG): $(EFI_BIN) $(MB2_ELF) scripts/make_bootable_usb.py
	@mkdir -p $(dir $@)
	$(PYTHON) scripts/make_bootable_usb.py $(EFI_BIN) $(MB2_ELF) $@

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
