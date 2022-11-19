BASE_ADDR     equ 0x900         ;将用户程序(loader)加载到物理位置BASE_ADDR处,BASE_ADDR的末位必须为0
START_SECTOR  equ 2             ;用户程序所在的起始扇区
KERNEL_ADDR   equ 0x1500        ;内核被加载到的内存位置
KERNEL_SECTOR equ 50            ;内核所在的起始扇区


