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
// Created by Milan Gallo on 25/12/2021.
//

#ifndef _LIBC_INCLUDE_STRING_H
#define _LIBC_INCLUDE_STRING_H

#include <sys/cdefs.h>
#include <xos/attributes.h>
#include <cstddef>

_ExtC

char *strcpy(char *_Rstr dest, const char *_Rstr src);
char *strncpy(char *_Rstr dest, const char *_Rstr src, size_t count);
char *strcat(char *_Rstr dest, const char *_Rstr src);
char *strncat(char *_Rstr dest, const char *_Rstr src, size_t count);

size_t strlen(const char *str);
int strcmp(const char *lhs, const char *rhs);
int strncmp(const char *lhs, const char *rhs, size_t count);
char *strchr(const char *str, int ch);
char *strrchr(const char *str, int ch);
size_t strspn(const char *dest, const char *src);
size_t strcspn(const char *dest, const char *src);
char *strpbrk(const char *dest, const char *breakSet);
char *strstr(const char *str, const char *subStr);
char *strtok(char *_Rstr str, const char *_Rstr delim);

void *memchr(const void *ptr, int ch, size_t count);
int memcmp(const void *lhs, const void *rhs, size_t count);
void *memset(void *dest, int ch, size_t count);
void *memcpy(void *_Rstr dest, const void *_Rstr src, size_t count);
void *memmove(void *dest, const void *src, size_t count);
void *memccpy(void *_Rstr dest, const void *_Rstr src, int c, size_t count);

_ExtCEnd

#endif //_LIBC_INCLUDE_STRING_H
