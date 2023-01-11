//
// Created by xuan on 23/12/22.
//
#include "../include/console.h"
#include "../include/sync.h"
#include "../include/print.h"
#include "../include/system.h"
static struct lock console_lock;    // 控制台锁

/* 初始化终端 */
void console_init()
{
    lock_init(&console_lock);
}

void console_acquire()
{
    lock_acquire(&console_lock);
}

void console_release()
{
    lock_release(&console_lock);
}

void console_put_str(char* str, uint8_t clr)
{
    //console_acquire();
    CLI;
    put_str(str,clr);
    console_release();
}

void console_put_char(uint8_t char_asci,uint8_t clr)
{
    console_acquire();
    put_char(char_asci,clr);
    console_release();
}


void console_put_int(uint32_t num,uint8_t clr,uint8_t radix)
{
    console_acquire();
    put_int(num,clr,radix);
    console_release();
}

void console_put_uint(uint32_t num,uint8_t clr,uint8_t radix)
{
    console_acquire();
    put_uint(num,clr,radix);
    console_release();
}