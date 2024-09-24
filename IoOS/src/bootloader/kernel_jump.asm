; ============================================
; kernel_jump.asm
; The file that connects the bootlader to the kernel.
;
; Author: Ioane Baidoshvili
; Date: 06-Sep-24
; ============================================


[BITS 32] ; We are in protected mode

global _start 

extern kernel_main ; External symbol of kernel_main.cpp

_start:
    ; Setting up VGA segment
    mov ax, 0x18
    mov es, ax


    ; Print directly to VGA buffer in protected mode
    mov eax, 0xB8000
    mov byte [eax], 'C'        ; Character
    mov byte [eax + 1], 0x4F       ; Attribute byte


    call kernel_main ; Connecting to kernel_main.cpp (void kernel_main())

    jmp $ ; Infinite loop
    
times 512 - ($ - $$) db 0 ; Filling remainder of 512 bytes with 0