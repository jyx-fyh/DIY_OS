//
// Created by xuan on 24/11/22.
//
// PIC8253
#include "../include/timer.h"
#include "../include/system.h"
#include "../include/print.h"
static void frequency_set(unsigned char  counter_port, \
			              unsigned char  counter_no,   \
			              unsigned char  rwl,          \
			              unsigned char  counter_mode, \
			              unsigned short counter_value)
{
/* 往控制字寄存器端口0x43中写入控制字 */
    outb(PIT_CONTROL_PORT, counter_no + rwl + counter_mode);

/* 先写入counter_value的低8位 */
    outb(counter_port, counter_value); //short转char,高8位截断,留下低8位

/* 再写入counter_value的高8位 */
    outb(counter_port, counter_value >> 8);
}

/* 初始化PIT8253 */
void timer_init()
{
    put_str("timer_init start...\n",DEFUALT);
    /* 设置8253的定时周期,也就是发中断的周期 */
    frequency_set(CONTRER0_PORT, COUNTER0_NO, READ_WRITE_LATCH, COUNTER_MODE, COUNTER0_VALUE);
    put_str("timer_init done: Clock interrupt frequency increased to 100Hz\n",DEFUALT);
}