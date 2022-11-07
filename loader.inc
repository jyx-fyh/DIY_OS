BASE_ADDR     equ 0x900         ;将用户程序加载到物理位置BASE_ADDR处
START_SECTOR  equ 2             ;BASE_ADDR的末位必须为0
                                ;声明常数（用户程序起始逻辑扇区号）
                                ;常数的声明不会占用汇编地址