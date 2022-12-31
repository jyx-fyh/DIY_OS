//
// Created by xuan on 23/12/22.
//

#ifndef OSLEARNING_SYSC_H
#define OSLEARNING_SYSC_H
#include "../include/type.h"
#include "../include/list.h"

struct semaphore  //信号量
{
    uint8_t  value;
    struct list waiters;
};

struct lock       //锁结构
{
    struct   task_struct* holder;	    // 锁的持有者
    struct   semaphore semaphore;	    // 用二元信号量实现锁
    uint32_t holder_repeat_nr;		    // 锁的持有者重复申请锁的次数
};

void sema_init(struct semaphore* psema, uint8_t value);
void sema_down(struct semaphore* psema);
void sema_up(struct semaphore* psema);
void lock_init(struct lock* plock);
void lock_acquire(struct lock* plock);
void lock_release(struct lock* plock);

#endif //OSLEARNING_SYSC_H
