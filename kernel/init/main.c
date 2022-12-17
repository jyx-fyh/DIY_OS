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
#include "../include/thread.h"

void k_thread_a(void* arg)
{
    char* para = arg;    //用void*来通用表示参数,被调用的函数知道自己需要什么类型的参数,自己转换再用
    while(1)
        put_str(para,FT_YELLOW);
}
void kernel_main()
{
    idt_init();    // 初始化中断
    timer_init();  // 初始化PIT
    mem_init();	   // 初始化内存管理系统
    thread_start("k_thread_a", 31, k_thread_a, "K-thread-a-argA ");
    while(1);
}