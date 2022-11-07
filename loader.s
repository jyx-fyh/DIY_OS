;文件说明：loader.s
%include "boot.inc"
%include "loader.inc"

SECTION loader vstart=BASE_ADDR              ;定义用户程序头部段
    program_length  dd program_end           ;程序总长度[0x00]
    ;用户程序入口点
    code_entry      dw start-BASE_ADDR       ;偏移地址[0x04]
                    dd section.loader.start  ;段地址[0x06]
    realloc_tbl_len dw 0                     ;段重定位表项个数为0
;=========================================================
;GDT
;第0描述符不可用
    GDT_BASE        dd    0x00000000
                    dd    0x00000000
;第1描述符CODE
    DESC_CODE       dd    0x0000FFFF
                    dd    DESC_CODE_HIGH4
;第2描述符DATA
    DESC_DATA       dd    0x0000FFFF
                    dd    DESC_DATA_HIGH4
;第3描述符VIDEO
    DESC_VIDEO      dd    0x80000007
                    dd    DESC_VIDEO_HIGH4

    GDT_SIZE        equ   $ - GDT_BASE
    GDT_LIMIT       equ   GDT_SIZE - 1
;GDT指针
    gdt_ptr         dw    GDT_LIMIT
                    dd    GDT_BASE

    SELECTOR_CODE   equ   ((DESC_CODE - GDT_BASE)/8)<<3 + TI_GDT + RPL0
    SELECTOR_DATA   equ   ((DESC_DATA - GDT_BASE)/8)<<3 + TI_GDT + RPL0
    SELECTOR_VIDEO  equ   ((DESC_VIDEO- GDT_BASE)/8)<<3 + TI_GDT + RPL0

    loader_msg      db    'r',11000010b,'e',11000010b,'a',11000010b,'l',11000010b,'-',11000010b
                    db    'm',11000010b,'o',11000010b,'d',11000010b,'e',11000010b
;=======================================================

start: ;程序入口
	mov ax,0             ;转移到loader代码后,
	mov ds,ax            ;将各段寄存器清0是头等大事
	mov es,ax
	mov ss,ax
	mov gs,ax
	mov fs,ax
print:
	mov ax,0xb800        ;彩色字符模式视频缓冲区
	mov es,ax
	mov si,loader_msg    ;ds:si
	mov di,0             ;es:di
	mov cx,18            ;9个字符,占18字节
	cld
	rep movsb
;========================================================
;1.打开A20
;2.加载GDT
;3.置PE=1
prepare:
;关中断
    cli
;打开A20
    in   al,0x92
    or   al,0000_0010B
    out  0x92,al
;加载GDT
    mov  ax,0
    mov  ds,ax
    lgdt [gdt_ptr]
;CR0的第0位置1
    mov  eax,cr0
    or   eax,0x0000_0001
    mov  cr0,eax
;此后进入保护模式
    jmp  dword SELECTOR_CODE:p_mode_start ;刷新流水线,装载CODE选择子
;======================================================
[bits 32]
p_mode_start:
    mov ax,SELECTOR_DATA
    mov ds,ax
    mov es,ax
    mov ss,ax
    mov esp,BASE_ADDR       ;可以找其他合适的地方作为栈顶，这里使用BASE_ADDR
    mov ax,SELECTOR_VIDEO
    mov gs,ax

    mov byte [gs:160],'p'
    mov byte [gs:161],11000010b
    jmp $
;========================================================
program_end  equ  $-BASE_ADDR
