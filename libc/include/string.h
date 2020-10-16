// xos
// Copyright (C) 2020 Milan Gallo <gallo.milan.jr@gmail.com>
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, version 2.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <https://www.gnu.org/licenses/>.
//
//
// Created by xgallom on 5/11/20.
//

#ifndef _XOS_LIBC_INCLUDE_STRING_H
#define _XOS_LIBC_INCLUDE_STRING_H

#include <sys/cdefs.h>
#include <xos/extern-c.h>
#include <xos/restrict.h>

#include <stddef.h>

_EXT_C

int memcmp(const void *left, const void *right, size_t byteLength);
void *memcpy(void *_Rstr dest, const void *_Rstr src, size_t byteLength);
void *memmove(void *dest, const void *src, size_t byteLength);
void *memset(void *dest, int value, size_t byteLength);
char *strcpy(char *_Rstr dest, const char *_Rstr src);
char *strncpy(char *_Rstr dest, const char *_Rstr src, size_t length);
size_t strlen(const char *str);

_EXT_C_END

#endif //_XOS_LIBC_INCLUDE_STRING_H
