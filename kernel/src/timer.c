//
// Created by xuan on 24/11/22.
//
// PIC8253
#include "../include/system.h"
#include "../include/print.h"
#include "../include/debug.h"
#include "../include/thread.h"
#include "../include/interrupt.h"
#include "../include/list.h"

#define OSLEARNING_TIMER_H
#define IRQ0_FREQUENCY	   100
#define INPUT_FREQUENCY	   1193180
#define COUNTER0_VALUE	   (INPUT_FREQUENCY / IRQ0_FREQUENCY)
#define CONTRER0_PORT	   0x40
#define COUNTER0_NO	       (0<<6)
#define COUNTER_MODE	   (2<<1)
#define READ_WRITE_LATCH   (3<<4)
#define PIT_CONTROL_PORT   0x43

uint32_t ticks;          // ticks是内核自中断开启以来总共的嘀嗒数
static void frequency_set(unsigned char  counter_port, \
			              unsigned char  counter_no,   \
			              unsigned char  rwl,          \
			              unsigned char  counter_mode, \
			              unsigned short counter_value)
{
/* 往控制字寄存器端口0x43中写入控制字 */
    outb(PIT_CONTROL_PORT, counter_no | rwl | counter_mode);

/* 先写入counter_value的低8位 */
    outb(counter_port, counter_value); //short转char,高8位截断,留下低8位

/* 再写入counter_value的高8位 */
    outb(counter_port, counter_value >> 8);
}

/* 初始化PIT8253 */

/* 时钟的中断处理函数 */
static void intr_timer_handler(void)
{
    struct task_struct* cur_thread = running_thread();

    assert(cur_thread->stack_magic == 0x19870916);  // 检查栈是否溢出

    cur_thread->elapsed_ticks++;                    // 记录此线程占用的cpu时间嘀
    ticks++;	  //从内核第一次处理时间中断后开始至今的滴哒数,内核态和用户态总共的嘀哒数
    if (cur_thread->ticks == 0)                     // 若进程时间片用完就开始调度新的进程上cpu
        schedule();
    else                                            // 将当前进程的时间片-1
        cur_thread->ticks--;

}

void timer_init()
{
    put_str("timer_init start...\n",DEFUALT);
    /* 设置8253的定时周期,也就是发中断的周期 */
    frequency_set(CONTRER0_PORT, COUNTER0_NO, READ_WRITE_LATCH, COUNTER_MODE, COUNTER0_VALUE);
    register_handler(0x20, intr_timer_handler);
    put_str("timer_init done: Clock interrupt frequency increased to 100Hz\n",DEFUALT);
}