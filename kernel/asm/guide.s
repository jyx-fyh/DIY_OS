[BITS 32]
extern kernel_main
global _start ;可要可不要

section .text
_start:
    mov esp, 0xc009f000
    call kernel_main
    jmp $