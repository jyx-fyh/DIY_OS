//
// Created by xuan on 11/01/23.
//
#include "../include/syscall_init.h"
#include "../include/syscall.h"
#include "../../kernel/include/console.h"
#include "../../kernel/include/string.h"
#include "../../kernel/include/system.h"
#define syscall_nr 32
typedef void* syscall_handler;
syscall_handler syscall_table[syscall_nr];

uint32_t sys_write(char* str);

/* 初始化系统调用 */
void syscall_init(void)
{
    console_put_str("syscall_init start\n",DEFUALT);
    syscall_table[SYS_WRITE] = sys_write;
    console_put_str("syscall_init done\n",DEFUALT);
}


/* 打印字符串str(未实现文件系统前的版本) */
uint32_t sys_write(char* str)
{
    console_put_str(str,DEFUALT);
    return strlen(str);
}