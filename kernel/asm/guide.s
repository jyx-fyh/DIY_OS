[BITS 32]
extern kernel_main
global _start

section .text
_start:
    mov esp, 0xc009f000
    jmp kernel_main