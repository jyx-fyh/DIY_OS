//
// Created by 极简 on 14/11/22.
//
#include "../include/console.h"
#include "../include/init.h"
#include "../include/print.h"
#include "../include/system.h"
#include "../include/thread.h"
#include "../include/interrupt.h"
#include "../../userprog/include/process.h"
#include "../../lib/include/syscall.h"
#include "../../lib/include/syscall_init.h"

void k_thread_a(void*);
void k_thread_b(void*);
void u_prog_a(void);
void u_prog_b(void);
int test_var_a = 0, test_var_b = 0;

int kernel_main(void)
{
    init_all();
    syscall_init();
    //write("fuck");
    thread_start("k_thread_a", 31, k_thread_a, "argA ");
    thread_start("k_thread_b", 31, k_thread_b, "argB ");
    process_execute(u_prog_a, "user_prog_a");
    process_execute(u_prog_b, "user_prog_b");
   // asm("int 0xd");
        console_put_str("shitbro\n",DEFUALT);
        int i=0;
        while(1){
            console_put_int(i,DEFUALT,DEC);
            console_put_str("\n",DEFUALT);
        }
    return 0;
}

/* 在线程中运行的函数 */
void k_thread_a(void* arg)
{
    char* para = arg;
    while(1)
    {
        console_put_str("shit\n",FT_GREEN);
    }
}

/* 在线程中运行的函数 */
void k_thread_b(void* arg) {
    char* para = arg;
    while(1) {
        console_put_str("damn\n",FT_RED);
    }
}

void tst(int a,int b){
    for(int i=a;i>b;i--){}
}

/* 测试用户进程 */
void u_prog_a(void) {
    while(1) {
        write("bitch\n");

    }
}

/* 测试用户进程 */
void u_prog_b(void) {
    while(1) {

        write("son\n");
    }
}