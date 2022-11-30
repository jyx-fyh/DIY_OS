//
// Created by xuan on 29/11/22.
//
#include "../include/interrupt.h"
#include "../include/system.h"
static int EFLAGS=0;

void get_eflags(int* efgs)
{
    asm volatile("pushf; pop EFLAGS"); //获取eflags的值
    *efgs = EFLAGS;
}

enum intr_status intr_enable()
{
    if(INTR_ON==intr_get_status())
        return INTR_ON;
    else
    {
        STI;
        return INTR_OFF;
    }
}

enum intr_status intr_disable()
{
    if(INTR_OFF==intr_get_status())
        return INTR_OFF;
    else
    {
        CLI;
        return INTR_ON;
    }
}

enum intr_status intr_get_status()
{
    get_eflags(EFLAGS);
    return (EFLAGS_IF & EFLAGS) ? INTR_ON:INTR_OFF;
}

enum intr_status intr_set_status(enum intr_status st)
{
    return INTR_ON & st ? intr_enable() : intr_disable();
}
