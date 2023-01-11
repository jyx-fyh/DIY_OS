//
// Created by xuan on 11/01/23.
//

#ifndef OSLEARNING_STDARG_H
#define OSLEARNING_STDARG_H
typedef char* va_list;
#define __va_rounded_size(TYPE)  \
  (((sizeof (TYPE) + sizeof (int) - 1) / sizeof (int)) * sizeof (int))
#define va_start(p, fmt) (p = (char*)&fmt + sizeof(char*))
#define va_end(p)        (p = 0)
#define va_arg(p, type)  (p += __va_rounded_size(type), *((type*)(p - __va_rounded_size (type))))
#endif //OSLEARNING_STDARG_H
