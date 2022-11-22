//
// Created by xuan on 20/11/22.
//

#ifndef OSLEARNING_GLOBAL_H
#define OSLEARNING_GLOBAL_H

#define	 RPL0  0
#define	 RPL1  1
#define	 RPL2  2
#define	 RPL3  3

#define TI_GDT 0
#define TI_LDT 1

#define SELECTOR_K_CODE	   ((1 << 3) + (TI_GDT << 2) + RPL0)
#define SELECTOR_K_DATA	   ((2 << 3) + (TI_GDT << 2) + RPL0)
#define SELECTOR_K_STACK   SELECTOR_K_DATA
#define SELECTOR_K_VIDEO   ((3 << 3) + (TI_GDT << 2) + RPL0)

//================IDT描述符P================
#define	 IDT_DESC_P_ON	 1
#define	 IDT_DESC_P_OFF	 0
//================IDT描述符DPL==============
#define	 IDT_DESC_DPL0   0       //为什么只有0和3？
#define	 IDT_DESC_DPL3   3
//==========中断门的s位与type位=================
#define	 IDT_DESC_GATE   0xE     // S=0(系统段),TYPE=1110(32位中断门)
//============================================
struct gate_desc
{
    short offset_L;       // 段内偏移 0 ~ 15 位
    short selector;       // 代码段选择子
    char reserved ;       // 保留不用
    char s_type :5;       // 系统段:任务门/中断门/陷阱门/调用门
    char DPL    :2;       // 使用 int 指令访问的最低权限
    char present:1;       // 是否有效
    short offset_H;       // 段内偏移 16 ~ 31 位
} __attribute__((packed));//声明不要进行对齐
//=================GDT/IDT指针=================
struct xdt_ptr
{
    short   limit;
    int     base;
}__attribute__((packed));
//======加载GDT/LDT指针的函数，直接内联===========
static inline void load_xdt(struct xdt_ptr* p, short limit, int base)
{
    p->base=base;
    p->limit=limit;
}
#endif //OSLEARNING_GLOBAL_H
