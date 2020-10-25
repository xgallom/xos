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

    void writeHex(char);
    void writeHex(unsigned char);
    void writeHex(short);
    void writeHex(unsigned short);
    void writeHex(int);
    void writeHex(unsigned int);
    void writeHex(long);
    void writeHex(unsigned long);
    void writeHex(long long);
    void writeHex(unsigned long long);

    void writeDec(char);
    void writeDec(unsigned char);
    void writeDec(short);
    void writeDec(unsigned short);
    void writeDec(int);
    void writeDec(unsigned int);
    void writeDec(long);
    void writeDec(unsigned long);
    void writeDec(long long);
    void writeDec(unsigned long long);

    void writeBin(char);
    void writeBin(unsigned char);
    void writeBin(short);
    void writeBin(unsigned short);
    void writeBin(int);
    void writeBin(unsigned int);
    void writeBin(long);
    void writeBin(unsigned long);
    void writeBin(long long);
    void writeBin(unsigned long long);

    bool write(const char *, char, const char *&, size_t);
    bool write(const char *, unsigned char, const char *&, size_t);
    bool write(const char *, short, const char *&, size_t);
    bool write(const char *, unsigned short, const char *&, size_t);
    bool write(const char *, int, const char *&, size_t);
    bool write(const char *, unsigned int, const char *&, size_t);
    bool write(const char *, long, const char *&, size_t);
    bool write(const char *, unsigned long, const char *&, size_t);
    bool write(const char *, long long, const char *&, size_t);
    bool write(const char *, unsigned long long, const char *&, size_t);

    bool write(const char *, bool, const char *&, size_t);
    bool write(const char *, const char *, const char *&, size_t);

    void write(const char *string);
    void write(const char *string, size_t count);
    void putchar(int c);
    int getchar();
}

#endif //_XOS_LIBC_INCLUDE_XOS_WRITE_H
