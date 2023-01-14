//
// Created by xuan on 10/01/23.
//
#include "../include/syscall.h"
#include "../../kernel/include/system.h"

static uint32_t callno_ret;
static uint32_t arg1;
static uint32_t arg2;
static uint32_t arg3;
uint32_t _syscall0(uint32_t no)
{
   int retval;
   asm volatile
   (
   "int 0x80"
   : "=a" (retval)
   : "a" (no)
   : "memory"
   );
   return retval;
}
uint32_t _syscall1(uint32_t no, uint32_t _arg1)
{
    int retval;
    asm volatile
    ("int 0x80"
    : "=a" (retval)
    : "a" (no), "b" (_arg1)
    : "memory"
    );
    return retval;
}
uint32_t _syscall2(uint32_t no, uint32_t _arg1, uint32_t _arg2)
{
   int retval;
   asm volatile
   (
   "int 0x80"
   : "=a" (retval)
   : "a" (no), "b" (_arg1), "c" (_arg2)
   : "memory"
   );
   return retval;
}
uint32_t _syscall3(uint32_t no, uint32_t _arg1, uint32_t _arg2, uint32_t _arg3)
{
   int retval;
   asm volatile
   (
      "int 0x80"
      : "=a" (retval)
      : "a" (no), "b" (_arg1), "c" (_arg2), "d" (_arg3)
      : "memory"
   );
  return retval;
}
uint32_t write(const char* str)
{
    return _syscall1(SYS_WRITE,(uint32_t)str);
}