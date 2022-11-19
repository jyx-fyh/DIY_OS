//
// Created by 极简 on 14/11/22.
//
#include "../include/print.h"

void kernel_main(void) {
    char* video = (char*)0xb8000+480;
    short* tmp = video;
    char* str = "This is kernel.";
    for(int i=0;i<15;i++)
    {
        *tmp = str[i];
        *((char*)tmp+1)=BG_BLACK+FT_GREEN;
        tmp += 1;
    }
    put_str("Hello,world.",BG_BLUE+FT_GRAY);
}