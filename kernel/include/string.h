//
// Created by xuan on 1/12/22.
//

#ifndef OSLEARNING_STRING_H
#define OSLEARNING_STRING_H

void memset(void* dst, unsigned char var, unsigned int size);
void memcpy(void* dst, void* src, unsigned int size);
int  memcmp(const void* mem1, const void* mem2, unsigned int size);
char* strchr(const char * str,char ch);
char* strcpy(char* dst, const char* src);
char* strrchr(const char* str,char ch);
char* strcat(char* dst, const char* src);
unsigned int strchrs(const char* src, char ch);
unsigned int strlen(const char* str);
int strcmp(const char* dst, const char* src);

#endif //OSLEARNING_STRING_H
