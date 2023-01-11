//
// Created by xuan on 11/01/23.
//
#include "../include/stdarg.h"
#include "../../kernel/include/string.h"
#define is_digit(c)	((c) >= '0' && (c) <= '9')
#define ZEROPAD	1		/* pad with zero */
#define SIGN	2		/* unsigned/signed long */
#define PLUS	4		/* show plus */
#define SPACE	8		/* space if plus */
#define LEFT	16		/* left justified */
#define SPECIAL	32		/* 0x */
#define SMALL	64		/* use 'abcdef' instead of 'ABCDEF' */
#define LONG    128     //if long long

static int skip_atoi(const char **fmtp)
{
    int i=0;
    while (is_digit(**fmtp))
        i = i*10 + *((*fmtp)++) - '0';
    return i;
}

unsigned int do_div_10(unsigned int* n)
{
    unsigned int t = *n % 10;
    *n = *n / 10;
    return t;
}
unsigned int do_div_16_8(unsigned long long *n, int base)
{
    unsigned int t = base==16?28:29;
    unsigned int low = *n;
    unsigned int hign= (*n)>>32;
    unsigned int mod = ((*n)&(base==16?15:7));
    unsigned long long tmp = (unsigned long long)(1<<t) * hign + low / base;
    *n = tmp;
    return mod;
}
static char * number(char * str, long long num, int base, int size, int precision,int type)
{
    char c,sign,tmp[36];
    const char *digits="0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    int i;
    if (type&SMALL) digits="0123456789abcdefghijklmnopqrstuvwxyz";
    if (type&LEFT) type &= ~ZEROPAD;
    if (base<2 || base>36)
        return 0;
    c = (type & ZEROPAD) ? '0' : ' ' ;
    if (type&SIGN && num<0) {
        sign='-';
        num = -num;
    } else
        sign=(type&PLUS) ? '+' : ((type&SPACE) ? ' ' : 0);
    if (sign) size--;
    if (type&SPECIAL) {
        if (base==16) size -= 2;
        else if (base==8) size--;
    }
    i=0;
    if (num==0)
        tmp[i++]='0';
    if(base==16 || base==8)
    {
        if(!(type&LONG))
        {
            int *p = &num;
            *(++p) = 0;
        }
        while (num!=0)
            tmp[i++]=digits[do_div_16_8(&num,base)];
    }
    if(base==10)
    {
        if(!(type&LONG))
        {
            while (num != 0)
                tmp[i++] = digits[do_div_10(&num)];
        }
        else
        {
            unsigned int low = num;
            unsigned int hign= num>>32;
            while(low>0) {
                tmp[i++] = ((hign % 10) * 6 + low % 10) % 10 + '0';
                low = 429496729 * (hign % 10) + low / 10 + ((hign % 10) * 6 + low % 10) / 10;
                hign = hign / 10;
            }
        }
    }
    if (i>precision) precision=i;
    size -= precision;
    if (!(type&(ZEROPAD+LEFT)))
        while(size-->0)
            *str++ = ' ';
    if (sign)
        *str++ = sign;
    if (type&SPECIAL) {
        if (base==8)
            *str++ = '0';
        else if (base==16) {
            *str++ = '0';
            *str++ = digits[33];
        }
    }
    if (!(type&LEFT))
        while(size-->0)
            *str++ = c;
    while(i<precision--)
        *str++ = '0';
    while(i-->0)
        *str++ = tmp[i];
    while(size-->0)
        *str++ = ' ';
    return str;
}

int vsprintf(char *buf, const char *fmt, va_list args) {
    int len;
    char * str;
    char *s;
    int *ip;

    int flags;		/* flags to number() */

    int field_width;	/* width of output field */
    int precision;		/* min. # of digits for integers; max
				   number of chars for from string */
    int qualifier;		/* 'h', 'l', or 'L' for integer fields */

    for (str=buf ; *fmt ; ++fmt)
    {
        if (*fmt != '%')
        {
            *str++ = *fmt;
            continue;
        }

        /* process flags */
        flags = 0;
        repeat:
        ++fmt;		/* this also skips first '%' */
        switch (*fmt) {
            case '-': flags |= LEFT; goto repeat;
            case '+': flags |= PLUS; goto repeat;
            case ' ': flags |= SPACE; goto repeat;
            case '#': flags |= SPECIAL; goto repeat;
            case '0': flags |= ZEROPAD; goto repeat;
        }

        /* get field width */
        field_width = -1;
        if (is_digit(*fmt))
            field_width = skip_atoi(&fmt);
        else if (*fmt == '*')
        {
            ++fmt;
            /* it's the next argument */
            field_width = va_arg(args, int);
            if (field_width < 0)
            {
                field_width = -field_width;
                flags |= LEFT;
            }
        }

        /* get the precision */
        precision = -1;
        if (*fmt == '.')
        {
            ++fmt;
            if (is_digit(*fmt))
                precision = skip_atoi(&fmt);
            else if (*fmt == '*')
            {
                ++fmt;
                /* it's the next argument */
                precision = va_arg(args, int);
            }
            if (precision < 0)
                precision = 0;
        }

        /* get the conversion qualifier */
        qualifier = -1;
        if (*fmt == 'h')
        {
            qualifier = *fmt;
            ++fmt;
        }
        else if(*fmt == 'l')
        {
            qualifier = *fmt;
            fmt++;
            if(*fmt == 'l')
            {
                qualifier = 'm';
                flags |= LONG;
                fmt++;
            }
        }

        switch (*fmt) {
            case 'c':
                if (!(flags & LEFT))
                    while (--field_width > 0)
                        *str++ = ' ';
                *str++ = (unsigned char) va_arg(args, int);
                while (--field_width > 0)
                    *str++ = ' ';
                break;

            case 's':
                s = va_arg(args, char *);
                len = strlen(s);
                if (precision < 0)
                    precision = len;
                else if (len > precision)
                    len = precision;

                if (!(flags & LEFT))
                    while (len < field_width--)
                        *str++ = ' ';
                for (int i = 0; i < len; ++i)
                    *str++ = *s++;
                while (len < field_width--)
                    *str++ = ' ';
                break;

            case 'o':
                if(qualifier=='h')
                    str = number(str, va_arg(args, unsigned short), 8, field_width, precision, flags);
                else if(qualifier=='l')
                    str = number(str, va_arg(args, unsigned long), 8, field_width, precision, flags);
                else if(qualifier=='m')
                    str = number(str, va_arg(args, unsigned long long), 8, field_width, precision, flags);
                else
                    str = number(str, va_arg(args, unsigned int), 8, field_width, precision, flags);
                break;

            case 'p':
                if (field_width == -1) {
                    field_width = 8;
                    flags |= ZEROPAD;
                }
                str = number(str,(unsigned long) va_arg(args, void *), 16,field_width, precision, flags);
                break;

            case 'x':
                flags |= SMALL;
            case 'X':
                if(qualifier=='h')
                    str = number(str, va_arg(args, unsigned short), 16, field_width, precision, flags);
                else if(qualifier=='l')
                    str = number(str, va_arg(args, unsigned long), 16, field_width, precision, flags);
                else if(qualifier=='m')// %llx
                    str = number(str, va_arg(args, unsigned long long), 16, field_width, precision, flags);
                else
                    str = number(str, va_arg(args, unsigned int), 16, field_width, precision, flags);
                break;

            case 'd':
            case 'i':
                flags |= SIGN;
                if(qualifier=='h')
                    str = number(str, va_arg(args, short), 10, field_width, precision, flags);
                else if(qualifier=='l')
                    str = number(str, va_arg(args, long), 10, field_width, precision, flags);
                else if(qualifier=='m')
                    str = number(str, va_arg(args, long long), 10, field_width, precision, flags);
                else
                    str = number(str, va_arg(args, int), 10, field_width, precision, flags);
                break;

            case 'u':
                if(qualifier=='h')
                    str = number(str, va_arg(args, unsigned short), 10, field_width, precision, flags);
                else if(qualifier=='l')
                    str = number(str, va_arg(args, unsigned long), 10, field_width, precision, flags);
                else if(qualifier=='m')
                    str = number(str, va_arg(args, unsigned long long), 10, field_width, precision, flags);
                else
                    str = number(str, va_arg(args, unsigned int), 10, field_width, precision, flags);
                break;

            case 'n':
                ip = va_arg(args, int *);
                *ip = (str - buf);
                break;

            default:
                if (*fmt != '%')
                    *str++ = '%';
                if (*fmt)
                    *str++ = *fmt;
                else
                    --fmt;
                break;
        }
    }
    *str = '\0';
    return str-buf;
}