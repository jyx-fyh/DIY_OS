//
// Created by xuan on 14/12/22.
//
#include "../include/print.h"
#include "../include/interrupt.h"
#include "../include/timer.h"
#include "../include/memory.h"
#include "../include/thread.h"

void init_all()
{
    put_str("init_all\n",DEFUALT);
    idt_init();    // 初始化中断
    timer_init();  // 初始化PIT
    thread_init(); // 初始化线程相关结构
    mem_init();	   // 初始化内存管理系统
}