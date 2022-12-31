//
// Created by xuan on 23/12/22.
//
#include "../include/sync.h"
#include "../include/interrupt.h"
#include "../include/print.h"
#include "../include/debug.h"
#include "../include/thread.h"

/* 初始化信号量 */
void sema_init(struct semaphore* psema, uint8_t value)
{
    psema->value = value;
    list_init(&psema->waiters);
}

/* 初始化锁plock */
void lock_init(struct lock* plock)
{
    plock->holder = NULL;
    plock->holder_repeat_nr = 0;
    sema_init(&plock->semaphore, 1);  // 信号量初值为1
}

/* 信号量down操作 */
void sema_down(struct semaphore* psema) {
    enum intr_status old_status = intr_disable();
    if(psema->value == 0) // 若value为0,表示已经被别人持有
    {
/* 若信号量的值等于0,则当前线程把自己加入该锁的等待队列,然后阻塞自己 */
        list_append(&psema->waiters, &running_thread()->general_tag);
        thread_block(TASK_BLOCKED);    // 阻塞线程,直到被唤醒
    }
/* 若value为1或被唤醒后,会执行下面的代码,也就是获得了锁。*/
    psema->value--;
    intr_set_status(old_status);
}

/* 信号量的up操作 */
void sema_up(struct semaphore* psema)
{
    enum intr_status old_status = intr_disable();
    if (!list_empty(&psema->waiters))
    {
        struct task_struct* thread_blocked = elem2entry(struct task_struct, general_tag, list_pop(&psema->waiters));
        thread_unblock(thread_blocked);
    }
    psema->value++;
    intr_set_status(old_status);
}

/* 获取锁plock */
void lock_acquire(struct lock* plock)
{
/* 排除曾经自己已经持有锁但还未将其释放的情况。*/
    if (plock->holder != running_thread())
    {
        sema_down(&plock->semaphore);
        plock->holder = running_thread();
        plock->holder_repeat_nr = 1;
    }
    else
        plock->holder_repeat_nr++;
}

/* 释放锁plock */
void lock_release(struct lock* plock)
{
    if (plock->holder_repeat_nr > 1)
    {
        plock->holder_repeat_nr--;
        return;
    }
    plock->holder = NULL;	   // 把锁的持有者置空放在V操作之前
    plock->holder_repeat_nr = 0;
    sema_up(&plock->semaphore);
}