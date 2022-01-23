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
// Created by Milan Gallo on 26/12/2021.
//

#ifndef _LIBC_INCLUDE_STDLIB_H
#define _LIBC_INCLUDE_STDLIB_H

#include <sys/cdefs.h>
#include <xos/attributes.h>
#include <cstdint>

_ExtC

int atoi(const char *str);
long atol(const char *str);
long long atoll(const char *str);

long strtol(const char *_Rstr str, char **_Rstr str_end, int base = 0);
long long strtoll(const char *_Rstr str, char **_Rstr str_end, int base = 0);

unsigned long strtoul(const char *_Rstr str, char **_Rstr str_end, int base = 0);
unsigned long long strtoull(const char *_Rstr str, char **_Rstr str_end, int base = 0);

float strtof(const char *_Rstr str, char **_Rstr str_end);
double strtod(const char *_Rstr str, char **_Rstr str_end);
long double strtold(const char *_Rstr str, char **_Rstr str_end);

intmax_t strtoimax(const char *_Rstr nptr, char **_Rstr endptr, int base = 0);
uintmax_t strtoumax(const char *_Rstr nptr, char **_Rstr endptr, int base = 0);

_ExtCEnd

#endif //_LIBC_INCLUDE_STDLIB_H
