//
// Created by 极简 on 14/11/22.
//
#include "../include/print.h"
#include "../include/interrupt.h"
#include "../include/system.h"
#include "../include/timer.h"
#include "../include/debug.h"
#include "../include/string.h"
#include "../include/memory.h"
void kernel_main(void)
{
    idt_init();    // 初始化中断
    timer_init();  // 初始化PIT
    mem_init();	  // 初始化内存管理系统
    void* addr = get_kernel_pages(1);
    put_str("get_kernel_page start vaddr:     ",FT_YELLOW);
    put_uint((uint32_t)addr,FT_YELLOW,HEX);
    while(1);
}