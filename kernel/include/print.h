//
// Created by xuan on 19/11/22.
//

#ifndef OSLEARNING_PRINT_H
#define OSLEARNING_PRINT_H

#define BG_BLACK  (0<<4) //务必加括号
#define BG_BLUE   (1<<4)
#define BG_GREEN  (2<<4)
#define BG_RED    (5<<4)
#define BG_BROWN  (6<<4)
#define BG_GRAY   (7<<4)
#define FT_BLACK  0
#define FT_BLUE   1
#define FT_GREEN  2
#define FT_RED    5
#define FT_BROWN  6
#define FT_GRAY   7
#define FT_YELLOW 14
#define FT_WHITE  15
#define DEFAULT   BG_BLACK+FT_GRAY

void  put_char(char,char);
void  put_str(char*,char);
void  put_int(int,char);

#endif //OSLEARNING_PRINT_H
