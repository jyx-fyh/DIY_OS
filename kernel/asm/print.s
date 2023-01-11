TI_GDT equ  0
RPL0  equ   0
SELECTOR_VIDEO equ (0x0003<<3) + TI_GDT + RPL0
[bits 32]
section .data
buffer times 12 db 0
sign db 0

section .text
;--------------------------------------------
;put_str 通过put_char来打印以0字符结尾的字符串
;--------------------------------------------
;输入：栈中参数为打印的字符串
;输出：无
global put_str
put_str:
;由于本函数中只用到了ebx和ecx,只备份这两个寄存器
   push ebx
   push ecx
   push edx
   xor ecx, ecx           ; 准备用ecx存储参数,清空
   mov ebx, [esp + 16]    ; 从栈中得到待打印的字符串地址
   mov edx, [esp + 20]    ;获取字符属性
.goon:
   mov cl, [ebx]
   cmp cl, 0              ; 如果处理到了字符串尾,跳到结束处返回
   jz .str_over
   push edx               ; 传递字符属性参数
   push ecx               ; 为put_char函数传递参数
   call put_char
   add esp, 8             ; 回收参数所占的栈空间
   inc ebx                ; 使ebx指向下一个字符
   jmp .goon
.str_over:
   pop edx
   pop ecx
   pop ebx
   nop
   nop
   ret


;====================put_int===========================================
;参数1：数字  参数2：字符属性 ;参数3:进制
global put_int

put_int:
    push ebp
    mov ebp,esp
    pushad

    mov eax,[ebp+8]
    mov ebx,eax
    mov byte [sign],1
    mov edi,11
    mov esi,buffer
    mov cl,31
    shr eax,cl
    cmp eax,1
    jne  .positive

.negative:
    mov byte [sign],0
    not ebx
    inc ebx

.positive:
    mov ax,bx
    mov cl,16
    shr ebx,cl
    mov dx,bx
.loop:
    mov cx,[ebp+16]
    call divdw
    sub edi,1
    cmp cl,10
    jb  .dec
.hex:
    add cl,'a'-10
    jmp .@2
.dec:
    add cl,'0'
.@2:
    mov [esi+edi],cl
    mov cl,16
    mov bx,dx
    shl ebx,cl
    mov bx,ax
    cmp ebx,0
    jne .loop

.@1:
    mov cx,[ebp+16]
    cmp cx,16
    jne .sign
    sub edi,1
    mov byte [esi+edi],'x'
    sub edi,1
    mov byte [esi+edi],'0'

.sign:
    mov al,[sign]
    cmp al,0
    jne .@3
    sub edi,1
    mov byte [esi+edi],'-'
.@3:
    push dword [ebp+12]
    add  esi,edi
    push esi
    call put_str
    add esp,8

    popad
    pop ebp
    ret
divdw:
    push ax
    mov ax,dx
    mov dx,0
    div cx
    mov bx,ax
    pop ax
    div cx
    mov cx,dx;余数
    mov dx,bx;高八位的商
    ;此时ax中存储的就是低八位的商
    ret
;=====put_uint===============================
global put_uint
put_uint:
    push ebp
    mov ebp,esp
    pushad

    mov eax,[ebp+8]
    mov ebx,eax
    mov edi,11
    mov esi,buffer

    mov ax,bx
    mov cl,16
    shr ebx,cl
    mov dx,bx
.loop:
    mov cx,[ebp+16]
    call divdw
    sub edi,1
    cmp cl,10
    jb  .dec
.hex:
    add cl,'a'-10
    jmp .@2
.dec:
    add cl,'0'
.@2:
    mov [esi+edi],cl
    mov cl,16
    mov bx,dx
    shl ebx,cl
    mov bx,ax
    cmp ebx,0
    jne .loop

.@1:
    mov cx,[ebp+16]
    cmp cx,16
    jne .@3
    sub edi,1
    mov byte [esi+edi],'x'
    sub edi,1
    mov byte [esi+edi],'0'

.@3:
    push dword [ebp+12]
    add  esi,edi
    push esi
    call put_str
    add esp,8

    popad
    pop ebp
    ret
;------------------------   put_char   -----------------------------
;功能描述:把栈中的1个字符写入光标所在处
;-------------------------------------------------------------------
global put_char
global set_cursor
put_char:
   pushad	              ;备份32位寄存器环境
   mov ax, SELECTOR_VIDEO ;需要保证gs中为正确的视频段选择子,为保险起见,每次打印时都为gs赋值
   mov gs, ax

;;;;;;;;;  获取当前光标位置 ;;;;;;;;;
   ;先获得高8位
   mov dx, 0x03d4  ;索引寄存器
   mov al, 0x0e	   ;用于提供光标位置的高8位
   out dx, al
   mov dx, 0x03d5  ;通过读写数据端口0x3d5来获得或设置光标位置
   in al, dx	   ;得到了光标位置的高8位
   mov ah, al

   ;再获取低8位
   mov dx, 0x03d4
   mov al, 0x0f
   out dx, al
   mov dx, 0x03d5
   in al, dx

   mov bx, ax                 ;将光标存入bx
   ;下面这行是在栈中获取待打印的字符
   mov ecx, [esp + 36]	      ;pushad压入4×8＝32字节,加上主调函数的返回地址4字节,故esp+36字节
   mov edx, [esp + 40]        ;获取字符属性
   cmp cl, 0xd                ;CR是0x0d,LF是0x0a
   jz .is_carriage_return
   cmp cl, 0xa
   jz .is_line_feed

   cmp cl, 0x8                ;BS(backspace)的asc码是8
   jz .is_backspace
   jmp .put_other
;backspace的一点说明:
;当为backspace时,本质上只要将光标移向前一个显存位置即可.后面再输入的字符自然会覆盖此处的字符
;但有可能在键入backspace后并不再键入新的字符,这时在光标已经向前移动到待删除的字符位置,但字符还在原处,
;这就显得好怪异,所以此处添加了空格或空字符0
.is_backspace:
   dec bx
   shl bx,1
   mov byte [gs:bx], 0x20     ;将待删除的字节补为0或空格皆可
   inc bx
   mov byte [gs:bx], 0x07     ;黑底白字
   shr bx,1
   jmp set_cursor
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
 .put_other:
   shl bx, 1				  ; 光标位置是用2字节表示,将光标值乘2,表示对应显存中的偏移字节
   mov [gs:bx], cl			  ; ascii字符本身
   inc bx
   mov byte [gs:bx],dl        ; 字符属性
   shr bx, 1				  ; 恢复老的光标值
   inc bx                     ; 下一个光标值
   cmp bx, 2000
   jl set_cursor             ; 若光标值小于2000,表示未写到显存的最后,则去设置新的光标值
                              ; 若超出屏幕字符数大小(2000)则换行处理
 .is_line_feed:               ; 是换行符LF(\n)
 .is_carriage_return:         ; 是回车符CR(\r)
                              ; 如果是CR(\r),只要把光标移到行首就行了。
   xor dx, dx                 ; dx是被除数的高16位,清0.
   mov ax, bx                 ; ax是被除数的低16位.
   mov si, 80                 ; 由于是效仿linux，linux中\n便表示下一行的行首，所以本系统中，
   div si                     ; 把\n和\r都处理为linux中\n的意思，也就是下一行的行首。
   sub bx, dx                 ; 光标值减去除80的余数便是取整
                              ; 以上4行处理\r的代码

 .is_carriage_return_end:     ; 回车符CR处理结束
   add bx, 80
   cmp bx, 2000
 .is_line_feed_end:           ; 若是LF(\n),将光标移+80便可。
   jl set_cursor

;屏幕行范围是0~24,滚屏的原理是将屏幕的1~24行搬运到0~23行,再将第24行用空格填充
 .roll_screen:                ; 若超出屏幕大小，开始滚屏
   cld
   mov ecx, 960               ; 一共有2000-80=1920个字符要搬运,共1920*2=3840字节.一次搬4字节,共3840/4=960次
   mov esi, 0xc00b80a0	          ; 第1行行首
   mov edi, 0xc00b8000            ; 第0行行首
   rep movsd

;将最后一行填充为空白
   mov ebx, 3840              ; 最后一行首字符的第一个字节偏移= 1920 * 2
   mov ecx, 80                ;一行是80字符(160字节),每次清空1字符(2字节),一行需要移动80次
 .cls:
   mov word [gs:ebx], 0x0720  ;0x0720是黑底白字的空格键
   add ebx, 2
   loop .cls
   mov bx,1920                ;将光标值重置为1920,最后一行的首字符.

 set_cursor:
;将光标设为bx值
;;;;;;; 1 先设置高8位 ;;;;;;;;
   mov dx, 0x03d4             ;索引寄存器
   mov al, 0x0e               ;用于提供光标位置的高8位
   out dx, al
   mov dx, 0x03d5             ;通过读写数据端口0x3d5来获得或设置光标位置
   mov al, bh
   out dx, al

;;;;;;; 2 再设置低8位 ;;;;;;;;;
   mov dx, 0x03d4
   mov al, 0x0f
   out dx, al
   mov dx, 0x03d5
   mov al, bl
   out dx, al
 .put_char_done:
   popad
   ret