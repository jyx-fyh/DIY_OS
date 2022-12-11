//
// Created by xuan on 4/12/22.
//

#ifndef OSLEARNING_MEMERY_H
#define OSLEARNING_MEMERY_H
#include "type.h"
#include "bitmap.h"

/* 用于虚拟地址管理 */
enum pool_flags {
    PF_KERNEL = 1,    // 内核内存池
    PF_USER = 2	      // 用户内存池
};
struct virtual_addr {
    struct bitmap vaddr_bitmap; // 虚拟地址用到的位图结构
    uint32_t vaddr_start;       // 虚拟地址起始地址
};
struct pool {
    struct bitmap pool_bitmap;	 // 本内存池用到的位图结构,用于管理物理内存
    uint32_t phy_addr_start;	 // 本内存池所管理物理内存的起始地址
    uint32_t pool_size;		     // 本内存池字节容量
};

#define	 PG_P_1	  1	// 页表项或页目录项存在属性位
#define	 PG_P_0	  0	// 页表项或页目录项存在属性位
#define	 PG_RW_R  0	// R/W 属性位值, 读/执行
#define	 PG_RW_W  2	// R/W 属性位值, 读/写/执行
#define	 PG_US_S  0	// U/S 属性位值, 系统级
#define	 PG_US_U  4	// U/S 属性位值, 用户级

void mem_init(void);
void* get_kernel_pages(uint32_t pg_cnt);
void* malloc_page(enum pool_flags pf, uint32_t pg_cnt);
void  malloc_init();
uint32_t* pte_ptr(uint32_t vaddr);
uint32_t* pde_ptr(uint32_t vaddr);
#endif //OSLEARNING_MEMERY_H
