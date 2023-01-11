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
    int num;
    printf("1.  %x\n",-1);
    printf("2.  %llX\n",(long long)-1);
    printf("3.  %llx\n",(long long)0xfffffffffff);
    printf("4.  %#llx\n",(long long)0xfffffffff);
    printf("5.  %lld\n",0xffffffffff);
    printf("6.  %lld\n",(long long)-1);
    printf("7.  %d\n",-1);
    printf("8.  %u\n",-1);
    printf("9.  %hd\n",0xffff); //-1,越界，变为负
    printf("10. %hx\n",0xffff); //0xffff,即为-1的补码
    printf("11. %10d\n",12345);
    printf("12. %*.*d\n",8,5,1234);
    printf("13. %*.*s\n",10,5,"hello,sun");
    printf("14. %s,%nworld\n","hello",&num);
    printf("15. %d\n",num);
    printf("16. %v\n",&num);
    printf("17. %%\n");
    printf("18. %\n");
    thread_start("a",32,k_thread_a,NULL);
    while(1)
    {
        printf("Hi,man\n");
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
    while(1)
        write("hello\n");
}

/* 测试用户进程 */
void u_prog_b(void)
{
    while(1)
        write("world\n");
}