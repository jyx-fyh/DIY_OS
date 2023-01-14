//
// Created by 极简 on 14/11/22.
//
#include "../include/console.h"
#include "../include/init.h"
#include "../include/system.h"
#include "../include/thread.h"
#include "../../userprog/include/process.h"
#include "../../lib/include/syscall.h"
#include "../../lib/include/printf.h"

void k_thread_a(void*);
void k_thread_b(void*);
void u_prog_a(void);
void u_prog_b(void);

int kernel_main(void)
{
    init_all();
    process_execute(u_prog_a,"a");
    while(1)
    {
        printf("kernel\n");
    }
    return 0;
}

/* 在线程中运行的函数 */
void k_thread_a(void* arg)
{
    char* para = arg;
    while(1)
        printf("hello,bro\n");
}

/* 在线程中运行的函数 */
void k_thread_b(void* arg)
{
    char* para = arg;
    while(1)
        console_put_str("system\n",FT_RED);
}



/* 测试用户进程 */
void u_prog_a(void)
{
    char arr[20]="user_progress\n";
    int cnt = write(arr);;

    while(1);
}

/* 测试用户进程 */
void u_prog_b(void)
{
    while(1)
        write("world\n");
}