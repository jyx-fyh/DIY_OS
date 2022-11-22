//
// Created by 极简 on 14/11/22.
//
#include "../include/print.h"
#include "../include/interrupt.h"
#include "../include/system.h"
void kernel_main(void)
{
    idt_init();
    STI;
    asm volatile("int 0x20");
    while(1);

    return;
}