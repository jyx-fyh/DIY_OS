//
// Created by xuan on 20/11/22.
//

#ifndef OSLEARNING_SYSTEM_H
#define OSLEARNING_SYSTEM_H

#define BOCHS_DEBUG_MAGIC   __asm__("xchg bx, bx");
#define STI   __asm__("sti");
#define CLI   __asm__("cli");
void outb(unsigned char port, char data);
void inb(unsigned char port);
void outw(unsigned char port, short data);
void inw(unsigned char port);
#endif //OSLEARNING_SYSTEM_H
