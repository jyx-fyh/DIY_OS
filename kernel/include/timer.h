//
// Created by xuan on 24/11/22.
//
/* PIT8253 */
#ifndef OSLEARNING_TIMER_H
#define OSLEARNING_TIMER_H
#define IRQ0_FREQUENCY	   100
#define INPUT_FREQUENCY	   1193180
#define COUNTER0_VALUE	   (INPUT_FREQUENCY / IRQ0_FREQUENCY)
#define CONTRER0_PORT	   0x40
#define COUNTER0_NO	       (0<<6)
#define COUNTER_MODE	   (2<<1)
#define READ_WRITE_LATCH   (3<<4)
#define PIT_CONTROL_PORT   0x43
void timer_init();
#endif //OSLEARNING_TIMER_H