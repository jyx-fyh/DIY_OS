//
// Created by xuan on 31/12/22.
//

#ifndef OSLEARNING_CONSOLE_H
#define OSLEARNING_CONSOLE_H
#include "type.h"
void console_init();
void console_acquire();
void console_release();
void console_put_str(char* str,uint8_t clr);
void console_put_char(uint8_t char_asci,uint8_t clr);
void console_put_int(int32_t num,uint8_t clr,enum radix radix);
void console_put_uint(uint32_t num,uint8_t clr,enum radix radix);

#endif //OSLEARNING_CONSOLE_H
