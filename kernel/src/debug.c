//
// Created by xuan on 29/11/22.
//
#include "../include/debug.h"
#include "../include/interrupt.h"
#include "../include/print.h"
void panic(char* err_msg, char* file_name, int line, char* func)
{
    intr_disable();
    put_str("\n===============================",BG_BLACK+FT_RED);
    put_str("\ndebug error:",BG_BLACK+FT_RED);
    put_str("\nFileName: ",BG_BLACK+FT_RED);
    put_str(file_name,BG_BLACK+FT_RED);
    put_str("\nFunction: ",BG_BLACK+FT_RED);
    put_str(func,BG_BLACK+FT_RED);
    put_str("\nLine:     ",BG_BLACK+FT_RED);
    put_int(line,BG_BLACK+FT_RED,DEC);
    put_str("\nmessage:  ",BG_BLACK+FT_RED);
    put_str(err_msg,BG_BLACK+FT_RED);
    put_str("\n===============================",BG_BLACK+FT_RED);
    while(1);
}