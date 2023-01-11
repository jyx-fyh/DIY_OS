//
// Created by xuan on 10/01/23.
//
#include "../include/syscall.h"
#include "../../kernel/include/system.h"

static uint32_t callno_ret;
static uint32_t arg1;
static uint32_t arg2;
static uint32_t arg3;
uint32_t _syscall0(uint32_t  no)
{
    callno_ret = no;
    asm("mov eax,callno_ret");
    asm("int 0x80");
    asm("mov callno_ret,eax");
    return callno_ret;
}
uint32_t _syscall1(uint32_t no, uint32_t _arg1)
{
    callno_ret = no;
    arg1 = _arg1;
    asm("mov eax,callno_ret");
    asm("mov ebx,arg1");
    asm("int 0x80");
    asm("mov callno_ret,eax");
    return callno_ret;
}
uint32_t _syscall2(uint32_t no, uint32_t _arg1, uint32_t _arg2)
{
    callno_ret = no;
    arg1 = _arg1;
    arg2 = _arg2;
    asm("mov eax,callno_ret");
    asm("mov ebx,arg1");
    asm("mov ecx,arg2");
    asm("int 0x80");
    asm("mov callno_ret,eax");
    return callno_ret;
}
uint32_t _syscall3(uint32_t no, uint32_t _arg1, uint32_t _arg2, uint32_t _arg3)
{
    callno_ret = no;
    arg1 = _arg1;
    arg2 = _arg2;
    arg3 = _arg3;
    asm("mov eax,callno_ret");
    asm("mov ebx,arg1");
    asm("mov ecx,arg2");
    asm("mov edx,arg3");
    asm("int 0x80");
    asm("mov callno_ret,eax");
    return callno_ret;
}
uint32_t write(const char* str)
{
    return _syscall1(SYS_WRITE,(uint32_t)str);
}