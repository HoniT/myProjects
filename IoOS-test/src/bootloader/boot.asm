; ============================================
; boot.asm (Version 2)
; The main file for the bootloader.
;
; Author: Ioane Baidoshvili
; Date: 23-Sep-24
; ============================================

[BITS 16] ; We start in Real Mode

; =============================================
; Variables- Addresses and Offsets
; =============================================

; GDT segments
CODE_SEG_OFFSET equ gdt_code_seg - gdt_start
DATA_SEG_OFFSET equ gdt_data_seg - gdt_start
VGA_SEG_OFFSET equ gdt_vga_seg - gdt_start 

; Kernel locations
KERNEL_LOAD_SEG equ 0x1000           ; Where the kernel will be loaded initially
KERNEL_START_ADDRESS equ 0x100000    ; Kernel's final memory address (1 MiB)


; =============================================
; Start- Real Mode
; =============================================

start:
    cli ; Disabling interrupts

    ; Setting up segment registers and stack pointer
    xor ax, ax ; Zero AX

    mov ds, ax
    mov es, ax
    mov ss, ax

    mov sp, 0x7000 ; Temporary stack pointer in real mode

    sti ; Enabling interrupts

    ; Clears VGA text memory
    mov ah, 0x00
    mov al, 0x03 ; 80x25 text mode
    int 0x10


    ; Enable A20 line
    call enable_a20

    ; Load GDT
    lgdt [gdt_descriptor] 


    ; Loading kernel using CHS
    mov bx, KERNEL_LOAD_SEG     ; Segment where kernel is loaded

    xor dh, dh                  ; Head 0 (same as mov dh, 0)
    mov dl, 0x80                ; First hard disk (0x80)
    mov cl, 0x02                ; Start reading from sector 2
    xor ch, ch                  ; Cylinder 0
    mov ah, 0x02                ; Function to read sectors
    mov al, 8                   ; Number of sectors to read (adjust if necessary)

    int 0x13                    ; BIOS interrupt to read sectors

    jc disk_read_error          ; If carry flag is set, handle disk read error



    ; Switching to protected mode
    cli             ; Dissabling interrupts

    mov eax, cr0
    or eax, 1       ; Set protected mode bit (PE) in CR0
    mov cr0, eax    ; Enable protected mode


    ; Perform a far jump to protected mode entry point
    jmp CODE_SEG_OFFSET:protected_mode_init


; =============================================
; A20 line
; =============================================

enable_a20:

    ; Wait for keyboard controller to be ready
    mov dx, 0x64
    in al, dx
    test al, 0x02            ; Check if bit 1 is set (input buffer full)
    jnz enable_a20

    ; Send command to enable A20 line
    mov dx, 0x64
    mov al, 0xD1             ; Command to write next byte to output port
    out dx, al
    call wait_kbd_ready

    ; Write to output port to enable A20
    mov al, 0xDF                ; Enable A20 by clearing bit 1
    mov dx, 0x60
    out dx, al
    call wait_kbd_ready

    ret

wait_kbd_ready:
    ; Wait until the keyboard controller is ready
    mov dx, 0x64  ; Set the port address to 0x64 (the status register of the keyboard controller)
    
wait_kbd_ready_loop:
    in al, dx     ; Read the status register from port 0x64 into AL
    test al, 0x02 ; Test if the 2nd bit of the status register is set (input buffer full)
    jnz wait_kbd_ready_loop ; If the bit is set (meaning the input buffer is full), loop until it is cleared

    ret           ; Once the input buffer is clear, return


; =============================================
; GDT (Global Descriptor Table)
; =============================================


gdt_start:
    ; Null descriptor
    dd 0x0
    dd 0x0

    ; Code segment descriptor
    gdt_code_seg:
        dw 0xFFFF      ; Limit
        dw 0x0000      ; Base address low
        db 0x00        ; Base address middle
        db 10011010b   ; Access rights: executable, readable, non-conforming, etc.
        db 11001111b   ; Flags: 32-bit, 4K granularity
        db 0x00        ; Base high

    ; Data segment descriptor
    gdt_data_seg:
        dw 0xFFFF      ; Limit
        dw 0x0000      ; Base address low
        db 0x00        ; Base address middle
        db 10010010b   ; Access rights: writable, non-executable
        db 11001111b   ; Flags: 32-bit, 4K granularity
        db 0x00        ; Base high

    ; VGA segment descriptor
    gdt_vga_seg:
        dw 0x0FFF      ; Limit
        dw 0x8000      ; Base address low
        db 0x0B        ; Base address middle
        db 10010010b   ; Access rights: writable, non-executable
        db 11001111b   ; Flags: 32-bit, 4K granularity
        db 0x00        ; Base high

gdt_end:
    ; For calculating gdt size

gdt_descriptor:
    dw gdt_end - gdt_start - 1 ; GDT size
    dd gdt_start               ; GDT base address


; =============================================
; Error codes
; =============================================

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



; =============================================
; Protected Mode
; =============================================

[BITS 32] ; In protected mode

protected_mode_init:
    ; Set up segment registers in protected mode
    mov ax, DATA_SEG_OFFSET

    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax


    ; Set up stack pointer in protected mode
    mov ebp, 0x9C00
    mov esp, ebp



    ; Relocate the kernel to its final address (1 MiB)
    extern __kernel_start
    extern __kernel_end

    mov esi, KERNEL_LOAD_SEG       ; Source address
    mov edi, KERNEL_START_ADDRESS  ; Destination address (1 MiB)

    ; Calculate the size of the kernel
    mov eax, __kernel_end
    sub eax, __kernel_start        ; Calculate size (__kernel_end - __kernel_start)
    mov ecx, eax                   ; Set size in ECX for copying

    rep movsb                      ; Copy kernel to final address (EDI)




    ; Set VGA segment descriptor to access VGA memory at 0xB8000
    mov ax, VGA_SEG_OFFSET
    mov es, ax

    ; Far jump to the kernel's entry point
    jmp CODE_SEG_OFFSET:KERNEL_START_ADDRESS



; Filling remainder of space with zeros
times 510 - ($ - $$) db 0
dw 0xAA55                           ; Boot signature