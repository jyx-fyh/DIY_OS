//
// Created by 极简 on 14/11/22.
//
#include "../include/print.h"
#include "../include/init.h"
#include "../include/system.h"
#include "../include/thread.h"
#include "../include/interrupt.h"
void k_thread_a(void*);
void k_thread_b(void*);
int kernel_main() {
    init_all();
    thread_start("k_thread_a", 31, k_thread_a, "argA ");
    thread_start("k_thread_b", 8, k_thread_b, "argB ");
    intr_enable();	// 打开中断,使时钟中断起作用
    while(1)
    {
        CLI;
        put_str("Main ",FT_YELLOW);
        STI;
    };
    return 0;
}

/* 在线程中运行的函数 */
void k_thread_a(void* arg) {
/* 用void*来通用表示参数,被调用的函数知道自己需要什么类型的参数,自己转换再用 */
    char* para = arg;
    while(1)
    {
        CLI;
        put_uint(intr_get_status(),FT_RED,DEC);
        STI;

    }
}

/* 在线程中运行的函数 */
void k_thread_b(void* arg) {
/* 用void*来通用表示参数,被调用的函数知道自己需要什么类型的参数,自己转换再用 */
    char* para = arg;
    while(1)
    {
        CLI;
        put_str(para,FT_GREEN);
        STI;
    }
}