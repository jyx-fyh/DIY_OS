[bits 32]
%define ERROR_CODE nop     ; 若在相关的异常中cpu已经自动压入了错误码,为保持栈中格式统一,这里不做操作.
%define ZERO push 0        ; 若在相关的异常中cpu没有压入错误码,为了统一栈中格式,就手工压入一个0

extern put_char
extern put_str                 ;必须先声明外部函数
extern interrupt_handler_table ;声明中断处理函数的指针数组

%macro VECTOR 2
INTERRUPT_ENTRY_%1:        ;中断处理entry
    %2
    push ds
    push es
    push fs
    push gs
    pushad

    mov al,0x20            ;中断结束命令EOI
    out 0xa0,al            ;向从片发送
    out 0x20,al            ;向主片发送

    push dword %1
    call [interrupt_handler_table + %1*4]
    add esp, 4             ;外平栈

    popad
    pop gs
    pop fs
    pop es
    pop ds



    add esp,4			   ;跨过error_code,以保持堆栈平衡
    iret				   ;从中断返回,32位下等同指令iretd
%endmacro

;;;;;;;;;以下代码利用宏来定义函数;;;;;;;;;;;;;;;
VECTOR 0x00, ZERO        ;divide by zero
VECTOR 0x01, ZERO        ;debug
VECTOR 0x02, ZERO        ;non maskable interrupt
VECTOR 0x03, ZERO        ;breakpoint
VECTOR 0x04, ZERO        ;overflow
VECTOR 0x05, ZERO        ;bound range exceeded
VECTOR 0x06, ZERO        ;invalid opcode
VECTOR 0x07, ZERO        ;device not avilable
VECTOR 0x08, ERROR_CODE  ;double fault
VECTOR 0x09, ZERO        ;coprocessor segment overrun
VECTOR 0x0a, ERROR_CODE  ;invalid TSS
VECTOR 0x0b, ERROR_CODE  ;segment not present
VECTOR 0x0c, ZERO        ;stack segment fault
VECTOR 0x0d, ERROR_CODE  ;general protection fault
VECTOR 0x0e, ERROR_CODE  ;page fault
VECTOR 0x0f, ZERO        ;reserved
VECTOR 0x10, ZERO        ;x87 floating point exception
VECTOR 0x11, ERROR_CODE  ;alignment check
VECTOR 0x12, ZERO        ;machine check
VECTOR 0x13, ZERO        ;SIMD Floating - Point Exception
VECTOR 0x14, ZERO        ;Virtualization Exception
VECTOR 0x15, ZERO        ;Control Protection Exception
VECTOR 0x16, ZERO        ;reserved
VECTOR 0x17, ZERO        ;reserved
VECTOR 0x18, ERROR_CODE  ;reserved
VECTOR 0x19, ZERO        ;reserved
VECTOR 0x1a, ERROR_CODE  ;reserved
VECTOR 0x1b, ERROR_CODE  ;reserved
VECTOR 0x1c, ZERO        ;reserved
VECTOR 0x1d, ERROR_CODE  ;reserved
VECTOR 0x1e, ERROR_CODE  ;reserved
VECTOR 0x1f, ZERO        ;reserved
VECTOR 0x20, ZERO        ;clock 时钟中断
VECTOR 0x21, ZERO        ;键盘中断
VECTOR 0x22, ZERO        ;级联用的
VECTOR 0x23, ZERO        ;串口2对应的入口
VECTOR 0x24, ZERO        ;串口1对应的入口
VECTOR 0x25, ZERO        ;并口2对应的入口
VECTOR 0x26, ZERO        ;软盘对应的入口
VECTOR 0x27, ZERO        ;并口1对应的入口
VECTOR 0x28, ZERO        ;rtc实时时钟
VECTOR 0x29, ZERO        ;重定向
VECTOR 0x2a, ZERO        ;保留
VECTOR 0x2b, ZERO        ;保留
VECTOR 0x2c, ZERO        ;ps/2鼠标
VECTOR 0x2d, ZERO        ;fpu浮点单元异常
VECTOR 0x2e, ZERO        ;硬盘
VECTOR 0x2f, ZERO        ;保留

;;;;;;;;;中断函数地址表;;;;;;;;;;
global interrupt_entry_table
interrupt_entry_table:
    dd INTERRUPT_ENTRY_0x00
    dd INTERRUPT_ENTRY_0x01
    dd INTERRUPT_ENTRY_0x02
    dd INTERRUPT_ENTRY_0x03
    dd INTERRUPT_ENTRY_0x04
    dd INTERRUPT_ENTRY_0x05
    dd INTERRUPT_ENTRY_0x06
    dd INTERRUPT_ENTRY_0x07
    dd INTERRUPT_ENTRY_0x08
    dd INTERRUPT_ENTRY_0x09
    dd INTERRUPT_ENTRY_0x0a
    dd INTERRUPT_ENTRY_0x0b
    dd INTERRUPT_ENTRY_0x0c
    dd INTERRUPT_ENTRY_0x0d
    dd INTERRUPT_ENTRY_0x0e
    dd INTERRUPT_ENTRY_0x0f
    dd INTERRUPT_ENTRY_0x10
    dd INTERRUPT_ENTRY_0x11
    dd INTERRUPT_ENTRY_0x12
    dd INTERRUPT_ENTRY_0x13
    dd INTERRUPT_ENTRY_0x14
    dd INTERRUPT_ENTRY_0x15
    dd INTERRUPT_ENTRY_0x16
    dd INTERRUPT_ENTRY_0x17
    dd INTERRUPT_ENTRY_0x18
    dd INTERRUPT_ENTRY_0x19
    dd INTERRUPT_ENTRY_0x1a
    dd INTERRUPT_ENTRY_0x1b
    dd INTERRUPT_ENTRY_0x1c
    dd INTERRUPT_ENTRY_0x1d
    dd INTERRUPT_ENTRY_0x1e
    dd INTERRUPT_ENTRY_0x1f
    dd INTERRUPT_ENTRY_0x20
    dd INTERRUPT_ENTRY_0x21
    dd INTERRUPT_ENTRY_0x22
    dd INTERRUPT_ENTRY_0x23
    dd INTERRUPT_ENTRY_0x24
    dd INTERRUPT_ENTRY_0x25
    dd INTERRUPT_ENTRY_0x26
    dd INTERRUPT_ENTRY_0x27
    dd INTERRUPT_ENTRY_0x28
    dd INTERRUPT_ENTRY_0x29
    dd INTERRUPT_ENTRY_0x2a
    dd INTERRUPT_ENTRY_0x2b
    dd INTERRUPT_ENTRY_0x2c
    dd INTERRUPT_ENTRY_0x2d
    dd INTERRUPT_ENTRY_0x2e
    dd INTERRUPT_ENTRY_0x2f