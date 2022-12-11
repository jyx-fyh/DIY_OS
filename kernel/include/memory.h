//
// Created by xuan on 4/12/22.
//

#ifndef OSLEARNING_MEMERY_H
#define OSLEARNING_MEMERY_H

#include "type.h"
#include "bitmap.h"

/* 用于虚拟地址管理 */
struct virtual_addr {
    struct bitmap vaddr_bitmap; // 虚拟地址用到的位图结构
    uint32_t vaddr_start;       // 虚拟地址起始地址
};
struct pool {
    struct bitmap pool_bitmap;	 // 本内存池用到的位图结构,用于管理物理内存
    uint32_t phy_addr_start;	 // 本内存池所管理物理内存的起始地址
    uint32_t pool_size;		     // 本内存池字节容量
};

void mem_init(void);

#endif //OSLEARNING_MEMERY_H
