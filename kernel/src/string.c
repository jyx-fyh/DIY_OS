//
// Created by xuan on 1/12/22.
//
#include "../include/string.h"
#include "../include/debug.h"
#include "../include/type.h"

void memset(void* dst, char var, unsigned int size)
{
    assert(dst != NULL);
    unsigned char* tmp = dst;
    while((size--) > 0)
        *tmp++ = var;
}

void memcpy(void* dst, void* src, unsigned int size)
{
    assert(dst != NULL);
    assert(src != NULL);
    src = (char *)src + size - 1;
    dst = (char *)dst + size - 1;
    while(size--)
    {
        *(char *)dst = *(char *)src;
        dst = (char *)dst - 1;
        src = (char *)src - 1;
    }
}

int memcmp(const void* mem1, const void* mem2, unsigned int size)
{
    assert(mem1 != NULL);
    assert(mem2 != NULL);
    const char* tmp1 = mem1;
    const char* tmp2 = mem2;
    while(size > 0)
    {
        if(*tmp1 != *tmp2)
            return *tmp1 > *tmp2 ? 1 : -1;
        tmp1++;
        tmp2++;
        size++;
    }
    return 0;
}

char* strcpy(char* dst, const char* src)
{
    assert(dst != NULL);
    assert(src != NULL);
    if(dst == src)
        return src;
    memcpy(dst,src,strlen(src)+1);
    return dst;
}

unsigned int strlen(const char* str)
{
    assert(str != NULL);
    const char* tmp = str;
    while(*tmp++);
    return tmp-str-1;
}

int strcmp(const char* dst, const char* src)
{
    assert(dst != NULL);
    assert(src != NULL);
    while(*dst != '\0' && *dst==*src)
    {
        dst++;
        src++;
    }
    return *dst < *src ? -1 : *dst > *src;
}

char* strchr(const char * str,char ch)
{
    assert(str != NULL);
    while (*str && *str != ch)
        str++;
    if (*str == ch)
        return str;
    return NULL;
}

char* strrchr(const char* str,char ch)
{
    assert(str != NULL);
    const char* last_char = NULL;
    while (*str != 0)
    {
        if (*str == ch)
            last_char = str;
        str++;
    }
    return (char*)last_char;
}

char* strcat(char* dst, const char* src)
{
    assert(dst != NULL);
    assert(src != NULL);
    //通过strcpy来实现strcat函数
    strcpy (dst + strlen (dst), src);
    return dst;
}

unsigned int strchrs(const char* src, char ch)
{
    assert(src != NULL);
    unsigned int cnt = 0;
    const char* tmp = src;
    while(*tmp)
    {
        if(*tmp == ch)
            cnt++;
        tmp++;
    }
    return cnt;
}









