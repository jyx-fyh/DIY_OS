//
// Created by 极简 on 20/11/22.
//
#include "../include/interrupt.h"
#include "../include/global.h"
#include "../include/print.h"
#include "../include/system.h"

struct xdt_ptr idt_ptr ;
struct gate_desc idt[IDT_DESC_CNT];                        //idt-中断描述符表
extern intr_handler interrupt_entry_table[IDT_DESC_CNT];   //引用interrupt.s中的中断处理函数入口数组,注意，这是一个指针数组
                                                           //中断例程的引导
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
    idt_desc_init();	   //初始化中断描述符表
    pic_init();		       //初始化8259A
    load_xdt(&idt_ptr,IDT_DESC_CNT*8-1,idt); //注意，limit=size-1，书中代码有误
    /* 加载idt */
    asm volatile("lidt idt_ptr");
    put_str("idt_init done\n",BG_BLACK+FT_YELLOW);
}

void general_intr_handler(unsigned char vec_num)
{
    if(vec_num==0x27 || vec_num==0x2f)
        return;
    put_str("interrupt ",BG_BLACK+FT_RED);
/////////////////......////////////////////////////
    put_str(" occur!",BG_BLACK+FT_RED);
}

