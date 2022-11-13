;%include "../inc/loader.inc"
SECTION mbr align=16 vstart=0x7c00
         ;设置堆栈段和栈指针
         mov ax,0
         mov ss,ax
         mov sp,ax

         mov eax,BASE_ADDR               ;eax低4位一定为0
         mov cl ,4                       ;移动多位，必须使用cl
         shr eax,cl
         mov ds,ax                       ;令DS和ES指向该段以进行操作
         mov es,ax

         ;以下读取程序的起始部分
         xor di,di
         mov si,START_SECTOR             ;程序在硬盘上的起始逻辑扇区号
         xor bx,bx                       ;加载到DS:0x0000处
         call read_hard_disk_0

         ;以下判断整个程序有多大
         mov dx,[2]
         mov ax,[0]
         mov bx,512                      ;512字节每扇区
         div bx
         cmp dx,0
         jnz @1                          ;未除尽，因此结果比实际扇区数少1
         dec ax                          ;已经读了一个扇区，扇区总数减1
   @1:
         cmp ax,0                        ;考虑实际长度小于等于512个字节的情况
         jz direct

         ;读取剩余的扇区
         push ds                         ;以下要用到并改变DS寄存器

         mov cx,ax                       ;循环次数（剩余扇区数）
   @2:
         mov ax,ds
         add ax,0x20                     ;得到下一个以512字节为边界的段地址
         mov ds,ax

         xor bx,bx                       ;每次读时，偏移地址始终为0x0000
         inc si                          ;下一个逻辑扇区
         call read_hard_disk_0
         loop @2                         ;循环读，直到读完整个功能程序

         pop ds                          ;恢复数据段基址到用户程序头部段

         ;计算入口点代码段基址
   direct:
         mov eax,[0x06]
         call calc_segment_base
         mov [0x06],ax                   ;回填修正后的入口点代码段基址

         ;开始处理段重定位表
         mov cx,[0x0a]                   ;需要重定位的项目数量
	     cmp cx,0
	     jz  jmpToLoader                 ;如果为0项，直接跳转
         mov bx,0x0c                     ;重定位表首地址

 realloc:
         mov eax,[bx]
         call calc_segment_base
         mov [bx],ax                     ;回填段的基址
         add bx,4                        ;下一个重定位项（每项占4个字节）
         loop realloc

 jmpToLoader:
         jmp far [0x04]                  ;转移到用户程序

;------------------------------------------------------------------------------
read_hard_disk_0:                        ;从硬盘读取一个逻辑扇区
                                         ;输入：DI:SI=起始逻辑扇区号
                                         ;      DS:BX=目标缓冲区地址
         push ax
         push bx
         push cx
         push dx

         mov dx,0x1f2
         mov al,1
         out dx,al                       ;读取的扇区数

         inc dx                          ;0x1f3
         mov ax,si
         out dx,al                       ;LBA地址7~0

         inc dx                          ;0x1f4
         mov al,ah
         out dx,al                       ;LBA地址15~8

         inc dx                          ;0x1f5
         mov ax,di
         out dx,al                       ;LBA地址23~16

         inc dx                          ;0x1f6
         mov al,0xe0                     ;LBA28模式，主盘
         or al,ah                        ;LBA地址27~24
         out dx,al

         inc dx                          ;0x1f7
         mov al,0x20                     ;读命令
         out dx,al

  .waits:
         in al,dx
         and al,0x88
         cmp al,0x08
         jnz .waits                      ;不忙，且硬盘已准备好数据传输

         mov cx,256                      ;总共要读取的字数
         mov dx,0x1f0
  .readw:
         in ax,dx
         mov [bx],ax
         add bx,2
         loop .readw

         pop dx
         pop cx
         pop bx
         pop ax

         ret
;-------------------------------------------------------------------------------
calc_segment_base:                       ;计算16位段地址
                                         ;输入：eax低20位有效,最低4位为0
                                         ;返回：AX=16位段基地址
         add eax,BASE_ADDR
	     mov cl,4
	     shr eax,cl
         ret
;-------------------------------------------------------------------------------
 times 510-($-$$) db 0
 db 0x55,0xaa
