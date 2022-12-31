//
// Created by 极简 on 14/11/22.
//
#include "../include/print.h"
#include "../include/init.h"
#include "../include/system.h"
#include "../include/thread.h"
#include "../include/interrupt.h"
#include "../include/console.h"
void k_thread_a(void*);
void k_thread_b(void*);
void k_thread_c(void*);
void k_thread_d(void*);
extern struct list thread_ready_list;	    // 就绪队列
extern struct list thread_all_list;	    // 所有任务队列
int kernel_main() {
    for(int i=100000000;i>0;i--);
    init_all();
    thread_start("k_thread_a", 31, k_thread_a, NULL);
    thread_start("k_thread_b", 21, k_thread_b, NULL);
    thread_start("k_thread_c", 11, k_thread_c, NULL);
    thread_start("k_thread_d", 5, k_thread_d, NULL);
    intr_enable();	// 打开中断,使时钟中断起作用
    while(1)
    {
        console_acquire();
        //CLI;
        put_uint((uint32_t)running_thread(),FT_RED,HEX);
        console_release();
        //STI;
    };
    return 0;
}

/* 在线程中运行的函数 */
void k_thread_a(void* arg) {
/* 用void*来通用表示参数,被调用的函数知道自己需要什么类型的参数,自己转换再用 */
    while(1)
    {
        console_acquire();
        //CLI;
        put_uint((uint32_t)running_thread(),FT_YELLOW,HEX);
        //STI;
        console_release();

    }
}

/* 在线程中运行的函数 */
void k_thread_b(void* arg) {
/* 用void*来通用表示参数,被调用的函数知道自己需要什么类型的参数,自己转换再用 */
    char* para = arg;
    while(1)
    {
        console_acquire();
        //CLI;
        put_uint((uint32_t)running_thread(),FT_GREEN,HEX);
        //STI;
        console_release();
    }
}
void k_thread_c(void* arg) {
/* 用void*来通用表示参数,被调用的函数知道自己需要什么类型的参数,自己转换再用 */
    while(1)
    {
        console_acquire();
        //CLI;
        put_uint((uint32_t)running_thread(),FT_BROWN,HEX);
        //STI;
        console_release();

    }
}
void k_thread_d(void* arg) {
/* 用void*来通用表示参数,被调用的函数知道自己需要什么类型的参数,自己转换再用 */
    while(1)
    {
        console_acquire();
        //CLI;
        put_uint((uint32_t)running_thread(),FT_GRAY,HEX);
        //STI;
        console_release();

    }
}