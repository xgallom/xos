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

#ifndef _XOS_LIBC_INCLUDE_STDIO_H
#define _XOS_LIBC_INCLUDE_STDIO_H

#include <sys/cdefs.h>
#include <xos/extern-c.h>
#include <xos/restrict.h>

_EXT_C

#define EOF (-1)

int printf(const char *_Rstr format, ...);
int putchar(int value);
int puts(const char *str);

_EXT_C_END

#endif //_XOS_LIBC_INCLUDE_STDIO_H
