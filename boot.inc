;文件说明：boot.inc ,包含保护模式中要用到的段选择子,描述符等内容
;以下为段描述符的子属性
DESC_G_4K         equ   1000_0000_0000_0000_0000_0000B
DESC_DB_32        equ    100_0000_0000_0000_0000_0000B
DESC_L            equ     00_0000_0000_0000_0000_0000B
DESC_AVL          equ      0_0000_0000_0000_0000_0000B
DESC_LIMIT_CODE2  equ        1111_0000_0000_0000_0000B
DESC_LIMIT_DATA2  equ        DESC_LIMIT_CODE2
DESC_LIMIT_VIDEO2 equ        0000_0000_0000_0000_0000B
DESC_P            equ             1000_0000_0000_0000B
DESC_DPL_0        equ              000_0000_0000_0000B
DESC_DPL_1        equ              010_0000_0000_0000B
DESC_DPL_2        equ              100_0000_0000_0000B
DESC_DPL_3        equ              110_0000_0000_0000B
DESC_S_SYS        equ                0_0000_0000_0000B
DESC_S_DATA       equ                1_0000_0000_0000B
DESC_TYPE_CODE    equ                  1000_0000_0000B;只执行1000
DESC_TYPE_DATA    equ                  0010_0000_0000B;可读写0010

;以下为段描述符的高四位(低四位在loader.s中定义)
DESC_CODE_HIGH4   equ   (0x00<<24) + DESC_G_4K + DESC_DB_32 + DESC_L + DESC_AVL +\
                        DESC_LIMIT_CODE2 + DESC_P + DESC_DPL_0 + DESC_S_DATA +\
                        DESC_TYPE_CODE + 0X00
DESC_DATA_HIGH4   equ   (0x00<<24) + DESC_G_4K + DESC_DB_32 + DESC_L + DESC_AVL +\
                        DESC_LIMIT_DATA2 + DESC_P + DESC_DPL_0 + DESC_S_DATA +\
                        DESC_TYPE_DATA + 0X00
DESC_VIDEO_HIGH4  equ   (0x00<<24) + DESC_G_4K + DESC_DB_32 + DESC_L + DESC_AVL +\
                        DESC_LIMIT_VIDEO2 + DESC_P + DESC_DPL_0 + DESC_S_DATA +\
                        DESC_TYPE_DATA + 0X0b
;========段选择子属性=============
RPL0   equ   00B
RPL1   equ   01B
RPL2   equ   10B
RPL3   equ   11B
TI_GDT equ   000B
TI_LDT equ   100B
