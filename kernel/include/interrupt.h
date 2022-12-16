//
// Created by xuan on 20/11/22.
//

#ifndef OSLEARNING_INTERRUPT_H
#define OSLEARNING_INTERRUPT_H
#include "./global.h"
#include "./type.h"
//============================================
#define IDT_DESC_CNT 0x30    //IDT项的个数
typedef void* intr_handler;  //指向中断处理函数的指针
//============================================
void make_idt_desc(struct gate_desc* p_desc, uint8_t DPL, intr_handler function);
void general_handler_regist();
static void general_intr_handler(uint8_t vec_num);
void idt_init();
void register_handler(uint8_t vector_no, intr_handler function);

//============================================
#define PIC_M_CTRL 0x20	       // 这里用的可编程中断控制器是8259A,主片的控制端口是0x20
#define PIC_M_DATA 0x21	       // 主片的数据端口是0x21
#define PIC_S_CTRL 0xa0	       // 从片的控制端口是0xa0
#define PIC_S_DATA 0xa1	       // 从片的数据端口是0xa1
//=============================================
#define EFLAGS_IF  (1<<9)      //eflags中的if位
enum intr_status{INTR_OFF,INTR_ON};

enum intr_status intr_get_status();
enum intr_status intr_set_status(enum intr_status);
enum intr_status intr_enable();
enum intr_status intr_disable();
#endif //OSLEARNING_INTERRUPT_H
