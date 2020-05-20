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

#ifndef XOS_LIBC_INCLUDE_XOS_STRING_H
#define XOS_LIBC_INCLUDE_XOS_STRING_H

#include "../string.h"

namespace xos {
    template<typename T>
    inline T *memcpy(T *_Rstr dest, const T *_Rstr src, size_t count)
    {
	    return reinterpret_cast<T *>(
		    ::memcpy(dest, src, sizeof(T) * count)
	    );
    }

    template<typename T>
    inline T *memmove(T *dest, const T *src, size_t count)
    {
	    return reinterpret_cast<T *>(
		    ::memmove(dest, src, sizeof(T) * count)
	    );
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
}

#endif //XOS_LIBC_INCLUDE_XOS_STRING_H
