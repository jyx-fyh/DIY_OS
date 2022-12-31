//
// Created by xuan on 30/12/22.
//

#ifndef OSLEARNING_PROCESS_H
#define OSLEARNING_PROCESS_H
#include "../../kernel/include/type.h"
#include "../../kernel/include/thread.h"
#define DEFUALT_PRIO 31
#define USER_STACK3_VADDR  (0xc0000000 - 0x1000)
#define USER_VADDR_START 0x8048000
void process_execute(void* filename, char* name);
void start_process(void* filename_);
void process_activate(struct task_struct* p_thread);
void page_dir_activate(struct task_struct* p_thread);
uint32_t* create_page_dir(void);
void create_user_vaddr_bitmap(struct task_struct* user_prog);
#endif //OSLEARNING_PROCESS_H
