;%include "boot.inc"
;%include "loader.inc"
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
	mov ax,0             ;转移到loader代码后,如果不想立即进入保护模式
	mov ds,ax            ;则最好先将各段寄存器清0
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

    mov byte [gs:160],'p'   ;在保护模式下打印
    mov byte [gs:161],11000010b

	call setup_page
	sgdt [gdt_ptr]          ;store gdt,将GDTR中的信息存到标号gdt_ptr处
	mov  ebx,[gdt_ptr+2]    ;此时ebx中为GDT的起始地址

	;显存段是第3个段描述符，24~31段基址在高4字节
	or dword [ebx+8*3+4],0xc0000000   ;c刚好对应高字节

	;将gdt的基址加上0xc0000000使其位于内核镜像所在的高地址
	add dword [gdt_ptr + 2], 0xc0000000

	add esp, 0xc0000000           ;将栈指针同样映射到内核地址

	mov eax, PAGE_DIR_POS         ;把页目录地址赋给cr3
	mov cr3, eax

	mov eax, cr0                  ;打开cr0的pg位(第31位)
	or eax, 0x80000000
	mov cr0, eax

	;在开启分页后,用gdt新的地址重新加载
	lgdt [gdt_ptr]             ; 重新加载

	mov byte [gs:320], 'V'     ;视频段段基址已经被更新,用字符v表示virtual addr
	mov byte [gs:322], 'i'     ;视频段段基址已经被更新,用字符v表示virtual addr
	mov byte [gs:324], 'r'     ;视频段段基址已经被更新,用字符v表示virtual addr
	mov byte [gs:326], 't'     ;视频段段基址已经被更新,用字符v表示virtual addr
	mov byte [gs:328], 'u'     ;视频段段基址已经被更新,用字符v表示virtual addr
	mov byte [gs:330], 'a'     ;视频段段基址已经被更新,用字符v表示virtual addr
	mov byte [gs:332], 'l'     ;视频段段基址已经被更新,用字符v表示virtual addr
	jmp $

;=======================================================
;页目录将要存放的内存清零,每次清空4bytes
setup_page:
	mov ecx,1024
	mov esi,0
.clear:
	mov dword [PAGE_DIR_POS+esi],0
	add esi,4
	loop .clear

;创建页目录项,向页目录项中安装各个页表的物理地址
.create_PDE:
	mov eax,PAGE_TABLE_POS
	add eax,4096*0               ;此时eax为第0个页表的物理位置,等价于add eax,(1<<12)*0
	mov ebx,eax                  ;备用

;下面在第0和0xc00目录项中安装第0个页表的地址，一个页表可表示4MB内存,
;这样虚拟地址中高1GB空间的起始4MB和低3GB空间的起始4MB都指向相同的页表。详见博客分析
;这是在为内核映射做准备。
	or eax, PG_US_U | PG_RW_W | PG_P         ;写入目录项属性,RW,P位和US都为1
	mov [PAGE_DIR_POS + 0*4], eax            ;安装第0个目录项,对应虚拟地址0~4MB
	mov [PAGE_DIR_POS + 768*4], eax          ;安装第768个目录项,对应虚拟地址3GB~3GB+4MB

	sub eax,4096
	mov dword [PAGE_DIR_POS + 1023*4], eax   ; 使最后一个目录项指向页目录表自己的地址

;下面创建页表项(PTE)
	mov ecx,256				         ;1M低端内存/每页大小4KB=56
	mov esi,0
	mov edx,PG_US_U | PG_RW_W | PG_P ;属性为7,US=1,RW=1,P=1;高12~31位的物理页地址为0,所以直接mov
.create_PTE:
	mov [PAGE_TABLE_POS+4*esi],edx
	add edx,4096                     ;一张页表含1024个页表项,每个项4字节,总大小4KB
	inc esi
	loop .create_PTE

;创建虚拟地址高1GB空间对应的目录项
	mov eax,PAGE_TABLE_POS+4096*1    ;此时eax为第1个页表的物理位置
	or  eax,PG_US_U | PG_RW_W | PG_P ;页目录项的属性US,RW和P位都为1
	mov ecx, 254                     ;范围为第769~1022的所有目录项数量,第1023目录项已经安装
    mov esi, 769                     ;从第769目录项开始
.create_kernel_pde:
	mov ebx,PAGE_DIR_POS
	mov [ebx+esi*4], eax
	inc esi
	add eax, 4096
	loop .create_kernel_pde
	ret
;========================================================
program_end  equ  $-BASE_ADDR