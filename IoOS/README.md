# IoOS is a test project!

I'm gonna create my own bootloader and kernel.

The Bootloader will be made from the assembly language, the kernel, UI and other things by c++ (for now).
I will be using QEMU - to emulate the OS, and Makefiles.


# MAKE PREPARATION:
export PREFIX="$HOME/opt/cross"
export TARGET=i686-elf
export PATH="$PREFIX/bin:$PATH"

# RUNNING QEMU WITH GDB:
add-symbol-file ./build/linkedKernel.o 0x100000
break kernel_main
target remote | qemu-system-i386 -drive file=bin/os.bin,format=raw -gdb stdio


# Updates

First successful version date: 06-Sep-24
    Running Bootloader without error, printing, switching to protected mode.

Second successful version date: 15-Sep-24
    Linked ASM bootloader to kernel_main.


# Author: Ioane Baidoshvili