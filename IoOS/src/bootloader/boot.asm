; ============================================
; boot.asm
; The main file for the bootloader.
;
; Author: Ioane Baidoshvili
; Date: 01-Sep-24
; ============================================

[BITS 16]

; Addresses and Offsets
CODE_SEG_OFFSET equ 0x08
DATA_SEG_OFFSET equ 0x10
VGA_SEG_OFFSET  equ 0x18

KERNEL_LOAD_SEG equ 0x1000     ; Where the kernel will be loaded initially
KERNEL_START_ADDRESS equ 0x100000 ; Kernel's final memory address (1 MiB)

start:
    cli                         ; Disable interrupts

    ; Set up segment registers and stack
    xor ax, ax
    mov ds, ax
    mov es, ax
    mov ss, ax
    mov sp, 0x7c00              ; Temporary stack in real mode

    sti                         ; Enable interrupts

    ; Set VGA text mode (80x25) to ensure control over the screen
    mov ah, 0x00
    mov al, 0x03                ; 80x25 text mode
    int 0x10

    ; Enable A20 line (required for accessing memory above 1MB)
    call enable_a20

    ; Load Global Descriptor Table (GDT)
    lgdt [gdt_descriptor]

    ; Load the kernel (using BIOS interrupt 13h for disk reading)
    mov bx, KERNEL_LOAD_SEG     ; Segment where kernel is loaded
    xor dh, dh                  ; Head 0
    mov dl, 0x80                ; First hard disk (0x80)
    mov cl, 0x02                ; Start reading from sector 2
    xor ch, ch                  ; Cylinder 0
    mov ah, 0x02                ; Function to read sectors
    mov al, 8                   ; Number of sectors to read (adjust if necessary)
    int 0x13                    ; BIOS interrupt to read sectors

    jc disk_read_error          ; If carry flag is set, handle disk read error

    ; Prepare for protected mode transition
    cli                         ; Disable interrupts again
    mov eax, cr0
    or eax, 1                   ; Set protected mode bit (PE) in CR0
    mov cr0, eax                ; Enable protected mode

    ; Perform a far jump to protected mode entry point
    jmp CODE_SEG_OFFSET:protected_mode_main

; --------------------------------------------
; Enable A20 line (for accessing memory >1MB)
; --------------------------------------------
enable_a20:
    ; Wait for keyboard controller to be ready
    mov dx, 0x64
    in al, dx
    test al, 0x02               ; Check if bit 1 is set (input buffer full)
    jnz enable_a20

    ; Send command to enable A20 line
    mov dx, 0x64
    mov al, 0xD1                ; Command to write next byte to output port
    out dx, al
    call wait_kbd_ready

    ; Write to output port to enable A20
    mov al, 0xDF                ; Enable A20 by clearing bit 1
    mov dx, 0x60
    out dx, al
    call wait_kbd_ready

    ret

wait_kbd_ready:
    ; Wait for the keyboard controller to be ready
    mov dx, 0x64
.wait:
    in al, dx
    test al, 0x02               ; Check if bit 1 is set (input buffer full)
    jnz .wait
    ret

; --------------------------------------------
; Protected Mode Entry Point
; --------------------------------------------
[BITS 32]
protected_mode_main:
    ; Set up segment registers in protected mode
    mov ax, DATA_SEG_OFFSET
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax

    ; Set up stack in protected mode
    mov ebp, 0x9C00
    mov esp, ebp

    ; Relocate the kernel to its final address (1 MiB)
    extern __kernel_start
    extern __kernel_end

    mov esi, KERNEL_LOAD_SEG << 4  ; Source address (loaded segment, shifted by 4)
    mov edi, KERNEL_START_ADDRESS  ; Destination address (1 MiB)

    ; Calculate the size of the kernel
    mov eax, __kernel_end
    sub eax, __kernel_start        ; Calculate size (__kernel_end - __kernel_start)
    mov ecx, eax                   ; Set size in ECX for copying

    rep movsb                      ; Copy kernel to final address (EDI)

    ; Set VGA segment descriptor to access VGA memory at 0xB8000
    mov ax, VGA_SEG_OFFSET
    mov es, ax

    ; Display success message by writing to VGA memory
    mov eax, 0xB8000               ; VGA memory address
    mov byte [eax], '+'            ; Display '+' character
    mov byte [eax + 1], 0x4F       ; Red background, white foreground

    ; Far jump to the kernel's entry point
    jmp CODE_SEG_OFFSET:KERNEL_START_ADDRESS

; --------------------------------------------
; Error Handling
; --------------------------------------------
disk_read_error:
    mov eax, 0xB8000               ; VGA buffer address
    mov byte [eax], 'D'            ; Display 'D' for disk error
    mov byte [eax + 1], 0x4F       ; Red background with white text
    jmp $

; Global Descriptor Table (GDT)
gdt_table:
    ; Null descriptor
    dd 0x0
    dd 0x0

    ; Code segment descriptor
    dw 0xFFFF                      ; Limit
    dw 0x0000                      ; Base address low
    db 0x00                        ; Base address middle
    db 10011010b                   ; Access rights: executable, readable, non-conforming, etc.
    db 11001111b                   ; Flags: 32-bit, 4K granularity
    db 0x00                        ; Base high

    ; Data segment descriptor
    dw 0xFFFF                      ; Limit
    dw 0x0000                      ; Base address low
    db 0x00                        ; Base address middle
    db 10010010b                   ; Access rights: writable, non-executable
    db 11001111b                   ; Flags: 32-bit, 4K granularity
    db 0x00                        ; Base high

    ; VGA text mode segment descriptor
    dw 0x0FFF                      ; Limit (4KB)
    dw 0x8000                      ; Base address low (0xB8000 for VGA memory)
    db 0x0B                        ; Base address middle
    db 10010010b                   ; Access rights: writable, non-executable
    db 11001111b                   ; Flags: 32-bit, 4K granularity
    db 0x00                        ; Base high

gdt_end:

gdt_descriptor:
    dw gdt_end - gdt_table - 1      ; GDT size
    dd gdt_table                    ; GDT base address

times 510 - ($ - $$) db 0           ; Fill boot sector with zeros
dw 0xAA55                           ; Boot signature
