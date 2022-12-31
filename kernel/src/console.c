//
// Created by xuan on 31/12/22.
//
#include "../include/sync.h"
#include "../include/print.h"
#include "../include/type.h"
static struct lock console_lock;    // 控制台锁

/* 初始化终端 */
void console_init() {
    lock_init(&console_lock);
}

/* 获取终端 */
void console_acquire() {
    lock_acquire(&console_lock);
}

/* 释放终端 */
void console_release() {
    lock_release(&console_lock);
}

/* 终端中输出字符串 */
void console_put_str(char* str,uint8_t clr) {
    console_acquire();
    put_str(str,clr);
    console_release();
}

/* 终端中输出字符 */
void console_put_char(uint8_t char_asci,uint8_t clr) {
    console_acquire();
    put_char(char_asci,clr);
    console_release();
}

void console_put_int(int32_t num,uint8_t clr,enum radix radix) {
    console_acquire();
    put_int(num,clr,radix);
    console_release();
}


void console_put_uint(uint32_t num,uint8_t clr,enum radix radix) {
    console_acquire();
    put_uint(num,clr,radix);
    console_release();
}