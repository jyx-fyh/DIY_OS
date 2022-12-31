//
// Created by xuan on 23/12/22.
//

#ifndef OSLEARNING_CONSOLE_H
#define OSLEARNING_CONSOLE_H
#include "../include/type.h"
#include "../include/print.h"
void console_init(void);
void console_put_str(char* str, uint8_t clr);
void console_put_char(uint8_t char_asci, uint8_t clr);
void console_put_int(uint32_t num, uint8_t clr, uint8_t radix);
void console_put_uint(uint32_t num, uint8_t clr, uint8_t radix);

#endif //OSLEARNING_CONSOLE_H
