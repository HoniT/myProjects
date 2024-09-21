; ============================================
; boot.asm
; The main file for the bootloader.
;
; Author: Ioane Baidoshvili
; Date: 01-Sep-24
; ============================================

; Starting in real mode
[BITS 16]

[ORG 0x7C00] ; BIOS will load the bootloader to this address


; Addresses and offsets
CODE_SEG_OFFSET equ 0x08
DATA_SEG_OFFSET equ 0x10
VGA_SEG_OFFSET equ 0x18

KERNEL_LOAD_SEG equ 0x1000
KERNEL_START_ADDRESS equ 0x100000 ; 0x7C00 + 1 MiB


start:
    cli ; Dissabling interferences

    ; Setting up segment registers and stack pointer
    mov ax, 0x00
    mov ds, ax
    mov es, ax
    mov ss, ax
    mov sp, 0x7c00

    sti ; Enabling interferences


    ; Set VGA text mode (80x25) to ensure control over the screen
    mov ah, 0x00
    mov al, 0x03    ; 80x25 text mode
    int 0x10

    ; Loading the Global Descriptor Table
    lgdt [gdt_descriptor]

    ; Loading the kernel using CHS
    mov bx, KERNEL_LOAD_SEG
    mov dh, 0x00
    mov dl, 0x80
    mov cl, 0x02
    mov ch, 0x00
    mov ah, 0x02
    mov al, 8
    int 0x13

    jc disk_read_error ; Managing disk read errors


    ; Checking the first few bytes of the kernel in memory
    ; mov bx, KERNEL_LOAD_SEG        ; Load segment where kernel is loaded
    ; mov ds, bx                     ; Set data segment to point to kernel

    ; mov eax, [0x1000]               ; Load the first word from kernel (segment 0x1000)
    ; cmp eax, 0xDEADBEE1             ; Magic number
    ; jne kernel_load_error          ; If not matching, jump to the failure handler


load_protected_mode:
    cli ; Dissabling interferences

    ; Switching to protected mode
    mov eax, cr0
    or eax, 1
    mov cr0, eax

    ; Far jump
    jmp CODE_SEG_OFFSET:protected_mode_main



; Error labels
disk_read_error:
    mov eax, 0xB8000         ; VGA buffer address
    mov byte [eax], 'D'      ; Display 'D' for disk
    mov byte [eax + 1], 0x4F ; Red background with white text

    jmp $


kernel_load_error:
    mov eax, 0xB8000               ; VGA buffer address
    mov byte [eax], 'K'            ; Display 'K' for kernel
    mov byte [eax + 1], 0x4F       ; Red background with white text

    jmp $ 


; Global Descriptor Table
gdt_table:
    ; Null descriptor
    dd 0x0
    dd 0x0

    ; Code segment descriptor
    dw 0xFFFF ; Limit
    dw 0x0000 ; Base address low
    db 0x00 ; Base address middle
    db 10011010b ; Defining access rights of the memory segment
    db 11001111b ; Defining flags
    db 0x00 ; Base high

    ; Data segment descriptor
    dw 0xFFFF ; Limit
    dw 0x0000 ; Base address low
    db 0x00 ; Base address middle
    db 10010010b ; Defining access rights of the memory segment
    db 11001111b ; Defining flags
    db 0x00 ; Base high

    ; VGA text mode segment descriptor
    dw 0x0FFF ; Limit
    dw 0x8000 ; Base address low
    db 0x0B ; Base address middle
    db 10010010b ; Defining access rights of the memory segment
    db 11001111b ; Defining flags
    db 0x00 ; Base high

gdt_end:
    ; For calculating GDT size

gdt_descriptor:
    dw gdt_end - gdt_table - 1 
    dd gdt_table




[BITS 32] ; 32 bit protected mode

protected_mode_main:
    ; Setting up segment registers
    mov ax, DATA_SEG_OFFSET
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov ss, ax
    mov gs, ax

    mov ebp, 0x9C00
    mov esp, ebp

    ; Set VGA segment descriptor for ES to access VGA memory at 0xB8000
    mov ax, VGA_SEG_OFFSET
    mov es, ax   

    mov eax, 0xB8000
    mov byte [eax], '+'
    mov byte [eax + 1], 0x4F
    

    ; Far jump to kernel_jump.asm
    jmp CODE_SEG_OFFSET:KERNEL_START_ADDRESS

    
; Filling remaining space with 0
times 510 - ($ - $$) db 0
dw 0xAA55