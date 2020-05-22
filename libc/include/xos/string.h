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
// Created by xgallom on 5/20/20.
//

#ifndef _XOS_LIBC_INCLUDE_XOS_STRING_H
#define _XOS_LIBC_INCLUDE_XOS_STRING_H

#include "../string.h"

namespace xos {
    template<typename T>
    inline T *memcpy(T *_Rstr dest, const T *_Rstr src, size_t count)
    {
	    if (!count)
		    return dest;

	    T *_Rstr destBuf = dest;

	    do
		    *destBuf++ = *src++;
	    while (--count);

	    return dest;
    }

    template<typename T>
    inline T *rmemcpy(T *_Rstr rdest, const T *_Rstr rsrc, size_t count)
    {
	    if (!count)
		    return rdest;

	    do
		    *--rdest = *--rsrc;
	    while (--count);

	    return rdest;
    }

    template<typename T>
    inline T *memmove(T *dest, const T *src, size_t count)
    {
	    if (dest < src)
		    return memcpy<T>(dest, src, count);
	    else
		    return rmemcpy<T>(dest + count, src + count, count);
    }

    template<typename T>
    T *memset(T *_Rstr dest, T value, size_t count)
    {
	    if (!count)
		    return dest;

	    T *_Rstr const start = dest;

	    do
		    *dest++ = value;
	    while (--count);

	    return start;
    }

    template<typename T, size_t N>
    T *memset(T (&_Rstr dest)[N], T value)
    {
	    for (auto &entry : dest)
		    entry = value;

	    return dest;
    }
}

#endif //_XOS_LIBC_INCLUDE_XOS_STRING_H
