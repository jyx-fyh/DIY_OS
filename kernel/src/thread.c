//
// Created by xuan on 15/12/22.
//
#include "../include/thread.h"
#include "../include/string.h"
#include "../include/memory.h"
#include "../include/type.h"
#define PG_SIZE 4096
static uint32_t* stack;

static void kernel_thread(thread_func* function, void* func_arg)
{
    function(func_arg);
}

void thread_create(struct task_struct* pthread, thread_func function, void* func_arg)
{
    pthread->self_kstack -= sizeof(struct intr_stack);
    pthread->self_kstack -= sizeof(struct thread_stack);
    struct thread_stack* kthread_stack = (struct thread_stack*)pthread->self_kstack;
    kthread_stack->eip = kernel_thread;
    kthread_stack->function = function;
    kthread_stack->func_arg = func_arg;
    kthread_stack->ebp = kthread_stack->ebx = kthread_stack->esi = kthread_stack->edi = 0;
}

void init_thread(struct task_struct* pthread, char* name, int prio)
{
    memset(pthread, 0, sizeof(*pthread));
    strcpy(pthread->name, name);
    pthread->status = TASK_RUNNING;
    pthread->priority = prio;
    pthread->self_kstack = (uint32_t*)((uint32_t)pthread + PG_SIZE);
    pthread->stack_magic = 0x19870916;	  // 自定义的魔数
}

/* 创建一优先级为prio的线程,线程名为name,线程所执行的函数是function(func_arg) */
/*
*  name:线程名
*  prio:线程优先级
*  function:执行函数
*  func_arg:函数参数
* */
struct task_struct* thread_start(char* name, int prio, thread_func function, void* func_arg)
{
    struct task_struct* thread = get_kernel_pages(1); //pcb都位于内核空间,包括用户进程的pcb也是在内核空间
    init_thread(thread, name, prio);
    thread_create(thread, function, func_arg);
    stack = thread->self_kstack;
    asm volatile("mov esp,stack");
    asm volatile("pop ebp");
    asm volatile("pop ebx");
    asm volatile("pop edi");
    asm volatile("pop esi");
    asm volatile("ret");
    return thread;
}