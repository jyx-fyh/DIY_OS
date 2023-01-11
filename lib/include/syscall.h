//
// Created by xuan on 10/01/23.
//

#ifndef OSLEARNING_SYSCALL_H
#define OSLEARNING_SYSCALL_H
#include "../../kernel/include/type.h"
enum SYSCALL_NR
{
    SYS_WRITE
};
uint32_t write(const char* str);
#endif //OSLEARNING_SYSCALL_H
