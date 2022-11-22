[bits 32]
[SECTION .text]

global inb
inb:
    push ebp;
    mov ebp, esp

    xor eax, eax

    mov edx, [ebp + 8]      ; port
    in al, dx

    leave
    ret

global outb
outb:
    push ebp;
    mov ebp, esp

    mov edx, [ebp + 8]      ; port
    mov eax, [ebp + 12]     ; value
    out dx, al

    leave
    ret

global inw
inw:
    push ebp;
    mov ebp, esp

    xor eax, eax

    mov edx, [ebp + 8]      ; port
    in ax, dx

    leave
    ret

global outw
outw:
    push ebp;
    mov ebp, esp

    mov edx, [ebp + 8]      ; port
    mov eax, [ebp + 12]     ; value
    out dx, ax

    leave
    ret