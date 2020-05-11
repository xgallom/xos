//
// Created by xgallom on 5/11/20.
//

#ifndef _XOS_LIBC_INCLUDE_STDIO_H
#define _XOS_LIBC_INCLUDE_STDIO_H

#include <sys/cdefs.h>
#include <xos/ext_c.h>
#include <xos/restrict.h>

_EXT_C

#define EOF (-1)

int printf(const char *_Rstr format, ...);
int putchar(int value);
int puts(const char *str);

_EXT_C_END

#endif //_XOS_LIBC_INCLUDE_STDIO_H
