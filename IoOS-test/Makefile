# ==========================================
# Makefile
# For building the project
#
# Author: Ioane Baidoshvili
# Date: 02-Sep-24
# ==========================================

# Directories
BOOTLOADER_DIR = src/bootloader
KERNEL_DIR = src/kernel

BIN = $(CURDIR)/bin
BUILD = $(CURDIR)/build

# HPP include directories
INCLUDE = -I src/include

# Tools
ASM = nasm

CXX = i686-elf-g++
CXX_FLAGS = $(INCLUDE) -g -Wall -O2 -ffreestanding -fno-exceptions -fno-rtti

ELF_LD = i686-elf-ld
ELF_LD_FLAGS = -g -relocatable

GCC = i686-elf-gcc 
GCC_FLAGS = -g -ffreestanding -nostdlib -nostartfiles -nodefaultlibs -Wall -O0 -Iinc

# Output files
OS_BIN = $(BIN)/os.bin # Linked binary file

BOOT_OBJ = $(BUILD)/boot.o
KERNEL_JUMP_OBJ = $(BUILD)/kernel_jump.o
KERNEL_OBJ = $(BUILD)/kernel_main.o 
LINKED_OBJ = $(BUILD)/linkedKernel.o
PRINT_OBJ = $(BUILD)/print.o

all:

# Compiling ASM files
	$(ASM) -f elf -g $(BOOTLOADER_DIR)/boot.asm -o $(BOOT_OBJ)
	$(ASM) -f elf -g $(BOOTLOADER_DIR)/kernel_jump.asm -o $(KERNEL_JUMP_OBJ)

# Compiling CPP files
	$(CXX) -I./src $(CXX_FLAGS) -std=c++23 -c $(KERNEL_DIR)/kernel_main.cpp -o $(KERNEL_OBJ)
	$(CXX) -I./src $(CXX_FLAGS) -std=c++23 -c $(KERNEL_DIR)/drivers_src/print_src/print.cpp -o $(PRINT_OBJ)

# Merging kernel_jump.o with kernel_main.o
	$(ELF_LD) $(ELF_LD_FLAGS) $(KERNEL_JUMP_OBJ) $(KERNEL_OBJ) $(PRINT_OBJ) -o $(LINKED_OBJ)

# Link all object files (bootloader + kernel) into a single binary
	$(GCC) $(GCC_FLAGS) -T tools/linker.ld -o $(OS_BIN) -ffreestanding -O0 -nostdlib $(BOOT_OBJ) $(LINKED_OBJ)


# Cleaning the build
clean:
	rm -f $(OS_BIN)

	rm -f $(KERNEL_JUMP_OBJ) 
	rm -f $(BOOT_OBJ) 
	rm -f $(KERNEL_OBJ) 
	rm -f $(LINKED_OBJ) 
	rm -f $(PRINT_OBJ) 
