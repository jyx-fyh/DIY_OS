//
// Created by 极简 on 20/11/22.
//
#include "../include/interrupt.h"
#include "../include/global.h"
#include "../include/print.h"
#include "../include/system.h"

static struct xdt_ptr idt_ptr ;
static char*  interrupt_name[IDT_DESC_CNT];
static struct gate_desc idt[IDT_DESC_CNT];                        //idt-中断描述符表
extern intr_handler interrupt_entry_table[IDT_DESC_CNT];   //引用interrupt.s中的中断处理函数入口数组,注意，这是一个指针数组
       intr_handler interrupt_handler_table[IDT_DESC_CNT]; //实际中断处理例程的地址
void make_idt_desc(struct gate_desc* p_desc, unsigned char DPL, intr_handler function) {
    p_desc->offset_L = (unsigned int)function & 0x0000FFFF;       //低16位赋值给offset_L，高位丢弃
    p_desc->offset_H = ((unsigned int)function>>16) & 0x0000FFFF; //低16位赋值给offset_L，高位丢弃
    p_desc->selector = SELECTOR_K_CODE;
    p_desc->reserved = 0;
    p_desc->s_type   = IDT_DESC_GATE;
    p_desc->DPL      = DPL;
    p_desc->present  = IDT_DESC_P_ON;
}

void idt_desc_init() {
    for (int i = 0; i < IDT_DESC_CNT; i++)
    {
        make_idt_desc(&idt[i],  IDT_DESC_DPL0, interrupt_entry_table[i]);
    }
    put_str("idt is done\n",BG_BLACK+FT_YELLOW);
}

/* 初始化可编程中断控制器8259A */
void pic_init() {

    /* 初始化主片 */
    outb (PIC_M_CTRL, 0x11);   // ICW1: 边沿触发,级联8259, 需要ICW4.
    outb (PIC_M_DATA, 0x20);   // ICW2: 起始中断向量号为0x20,也就是IR[0-7] 为 0x20 ~ 0x27.
    outb (PIC_M_DATA, 0x04);   // ICW3: IR2接从片.
    outb (PIC_M_DATA, 0x01);   // ICW4: 8086模式, 正常EOI

    /* 初始化从片 */
    outb (PIC_S_CTRL, 0x11);	// ICW1: 边沿触发,级联8259, 需要ICW4.
    outb (PIC_S_DATA, 0x28);	// ICW2: 起始中断向量号为0x28,也就是IR[8-15] 为 0x28 ~ 0x2F.
    outb (PIC_S_DATA, 0x02);	// ICW3: 设置从片连接到主片的IR2引脚
    outb (PIC_S_DATA, 0x01);	// ICW4: 8086模式, 正常EOI
    /* 打开主片上IR0,也就是目前只接受时钟产生的中断 */
    outb (PIC_M_DATA, 0xfe);
    outb (PIC_S_DATA, 0xff);
    put_str("pic_init done\n",BG_BLACK+FT_RED);
}

void idt_init() {
    put_str("idt_init start\n",BG_BLACK+FT_YELLOW);
    idt_desc_init();	         //初始化中断描述符表
    general_handler_regist();    //默认中断函数注册
    pic_init();		             //初始化8259A
    load_xdt(&idt_ptr,IDT_DESC_CNT*8-1,idt); //注意，limit=size-1，书中代码有误
    /* 加载idt */
    asm volatile("lidt idt_ptr");
    put_str("idt_init done\n",BG_BLACK+FT_YELLOW);
}

void general_intr_handler(unsigned char vec_num)
{
    if(vec_num==0x27 || vec_num==0x2f)
        return;
    put_str("\ninterrupt ",BG_BLACK+FT_RED);
    put_int(vec_num, BG_BLACK+FT_RED,HEX);
    put_str(" occur: ",BG_BLACK+FT_RED);
    put_str(interrupt_name[vec_num],BG_BLACK+FT_RED);
}

void general_handler_regist()
{
    for(int i=0;i<IDT_DESC_CNT;i++)
    {
        interrupt_handler_table[i]= general_intr_handler;   //将一般函数的地址安装到中断函数表中
        interrupt_name[i]="unknown";
    }
    interrupt_name[0]    = "Divide Error\n";
    interrupt_name[1]    = "Debug Exception\n";
    interrupt_name[2]    = "NMI Interrupt\n";
    interrupt_name[3]    = "Breakpoint Exception\n";
    interrupt_name[4]    = "Overflow Exception\n";
    interrupt_name[5]    = "BOUND Range Exceeded Exception\n";
    interrupt_name[6]    = "Invalid Opcode Exception\n";
    interrupt_name[7]    = "Device Not Available Exception\n";
    interrupt_name[8]    = "Double Fault Exception\n";
    interrupt_name[9]    = "Coprocessor Segment Overrun\n";
    interrupt_name[0xa]  = "Invalid TSS Exception\n";
    interrupt_name[0xb]  = "Segment Not Present\n";
    interrupt_name[0xc]  = "Stack Fault Exception\n";
    interrupt_name[0xd]  = "General Protection Exception\n";
    interrupt_name[0xe]  = "Page-Fault Exception\n";
    //interrupt_name[15]  第15项是intel保留项，未使用
    interrupt_name[0x10] = "x87 FPU Floating-Point Error\n";
    interrupt_name[0x11] = "Alignment Check Exception\n";
    interrupt_name[0x12] = "Machine-Check Exception\n";
    interrupt_name[0x13] = "SIMD Floating-Point Exception\n";
    interrupt_name[0x14] = "Virtualization Exception\n";
    interrupt_name[0x15] = "Control Protection Exception\n";
    interrupt_name[0x16] = "reserved interrupt-unknown\n";
    interrupt_name[0x17] = "reserved interrupt-unknown\n";
    interrupt_name[0x18] = "reserved interrupt-unknown\n";
    interrupt_name[0x19] = "reserved interrupt-unknown\n";
    interrupt_name[0x1a] = "reserved interrupt-unknown\n";
    interrupt_name[0x1b] = "reserved interrupt-unknown\n";
    interrupt_name[0x1c] = "reserved interrupt-unknown\n";
    interrupt_name[0x1d] = "reserved interrupt-unknown\n";
    interrupt_name[0x1e] = "reserved interrupt-unknown\n";
    interrupt_name[0x1f] = "reserved interrupt-unknown\n";
    interrupt_name[0x20] = "Clock interrupt\n";
    interrupt_name[0x21] = "Keyboard interrupt\n";
    interrupt_name[0x22] = "Clock interrupt\n";
    interrupt_name[0x23] = "Cascade\n";
    interrupt_name[0x24] = "Unknown\n";
    interrupt_name[0x25] = "Unknown\n";
    interrupt_name[0x26] = "Unknown\n";
    interrupt_name[0x27] = "Unknown\n";
    interrupt_name[0x28] = "RTC\n";
    interrupt_name[0x29] = "Relocation\n";
    interrupt_name[0x2a] = "Reserved\n";
    interrupt_name[0x2b] = "Reserved\n";
    interrupt_name[0x2c] = "Unknown\n";
    interrupt_name[0x2d] = "FPU Exception\n";
    interrupt_name[0x2e] = "Disk interrupt\n";
    interrupt_name[0x2f] = "Reserved\n";
}


