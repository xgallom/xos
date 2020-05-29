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
// Created by xgallom on 5/23/20.
//

#ifndef _XOS_LIBC_INCLUDE_XOS_WRITE_H
#define _XOS_LIBC_INCLUDE_XOS_WRITE_H

#include <stddef.h>

namespace xos {
    constexpr char _TabOffset[] = "\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t";

    inline const char *TabOffset(size_t offset)
    {
	    if (offset >= sizeof(_TabOffset))
		    return "Offset too big";
	    return _TabOffset + sizeof(_TabOffset) - 1 - offset;
    }

    extern void writeHex(char);
    extern void writeHex(unsigned char);
    extern void writeHex(short);
    extern void writeHex(unsigned short);
    extern void writeHex(int);
    extern void writeHex(unsigned int);
    extern void writeHex(long);
    extern void writeHex(unsigned long);
    extern void writeHex(long long);
    extern void writeHex(unsigned long long);

    extern void writeDec(char);
    extern void writeDec(unsigned char);
    extern void writeDec(short);
    extern void writeDec(unsigned short);
    extern void writeDec(int);
    extern void writeDec(unsigned int);
    extern void writeDec(long);
    extern void writeDec(unsigned long);
    extern void writeDec(long long);
    extern void writeDec(unsigned long long);

    extern void writeBin(char);
    extern void writeBin(unsigned char);
    extern void writeBin(short);
    extern void writeBin(unsigned short);
    extern void writeBin(int);
    extern void writeBin(unsigned int);
    extern void writeBin(long);
    extern void writeBin(unsigned long);
    extern void writeBin(long long);
    extern void writeBin(unsigned long long);

    extern bool write(
	    const char *, char, const char *&, size_t);
    extern bool write(
	    const char *, unsigned char, const char *&, size_t);
    extern bool write(
	    const char *, short, const char *&, size_t);
    extern bool write(
	    const char *, unsigned short, const char *&, size_t);
    extern bool write(
	    const char *, int, const char *&, size_t);
    extern bool write(
	    const char *, unsigned int, const char *&, size_t);
    extern bool write(
	    const char *, long, const char *&, size_t);
    extern bool write(
	    const char *, unsigned long, const char *&, size_t);
    extern bool write(
	    const char *, long long, const char *&, size_t);
    extern bool write(
	    const char *, unsigned long long, const char *&, size_t);

    extern bool write(
	    const char *, bool, const char *&, size_t);
    extern bool write(
	    const char *, const char *, const char *&, size_t);

    void write(const char *string);
    void write(const char *string, size_t count);
    void putchar(char c);
}

#endif //_XOS_LIBC_INCLUDE_XOS_WRITE_H
