//
// Created by xgallom on 5/11/20.
//

#ifndef _XOS_LIBC_INCLUDE_STRING_H
#define _XOS_LIBC_INCLUDE_STRING_H

#include <sys/cdefs.h>
#include <xos/ext_c.h>
#include <xos/restrict.h>

#include <stddef.h>

_EXT_C

int memcmp(const void *left, const void *right, size_t byteLength);
void *memcpy(void *_Rstr dest, const void* _Rstr src, size_t byteLength);
void *memmove(void *dest, const void *src, size_t byteLength);
void *memset(void *dest, int value, size_t byteLength);
char *strcpy(char *_Rstr dest, const char *_Rstr src);
char *strncpy(char *_Rstr dest, const char *_Rstr src, size_t length);
size_t strlen(const char *str);

_EXT_C_END

#endif //_XOS_LIBC_INCLUDE_STRING_H
