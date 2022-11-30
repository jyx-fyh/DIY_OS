//
// Created by xuan on 29/11/22.
//

#ifndef OSLEARNING_DEBUG_H
#define OSLEARNING_DEBUG_H
void panic(char* err_msg, char* file_name, int line, char* func);
#ifdef NDEBUG
#define assert(expression) ((void)0)
#else
#define assert(expr) \
    if(expr){}       \
    else{            \
    panic(#expr, __FILE__, __LINE__, __func__);}
#endif
#endif //OSLEARNING_DEBUG_H
