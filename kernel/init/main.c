//
// Created by 极简 on 14/11/22.
//
#include "../include/print.h"
#include "../include/interrupt.h"
#include "../include/system.h"
#include "../include/print.h"
void kernel_main(void)
{
    idt_init();
    STI;
    while(1);
}