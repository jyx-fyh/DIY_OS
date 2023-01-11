//
// Created by xuan on 11/01/23.
//
#include "../include/stdarg.h"
#include "../include/syscall.h"
#include "../include/vsprintf.h"
static char buf[1024];

int printf(const char * fmt, ...)
{
    va_list args;
    int i;
    va_start(args, fmt);
    i = vsprintf(buf, fmt, args);
    va_end(args);
    write(buf);
    return i;
}