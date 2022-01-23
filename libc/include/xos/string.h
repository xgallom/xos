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

#ifndef _LIBC_INCLUDE_XOS_STRING_H
#define _LIBC_INCLUDE_XOS_STRING_H

#include <sys/cdefs.h>
#include <cstddef>

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

        template<typename T, size_t N, size_t M>
        inline T *memcpy(T (&_Rstr dest)[N], const T (&_Rstr src)[M])
        {
                if constexpr (N && M) {
                        auto count = N < M ? N : M;

                        T *_Rstr destBuf = dest;
                        const T *_Rstr srcBuf = src;

                        do
                                *destBuf++ = *srcBuf++;
                        while (--count);
                }

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

        template<typename T, size_t N, size_t M>
        inline T *rmemcpy(T (&_Rstr dest)[N], const T (&_Rstr src)[M])
        {
                if constexpr (N && M) {
                        auto count = N < M ? N : M;

                        T *_Rstr destBuf = dest + count;
                        const T *_Rstr srcBuf = src + count;

                        do
                                *--destBuf = *--srcBuf;
                        while (--count);
                }

                return dest;
        }

        template<typename T>
        inline T *memmove(T *dest, const T *src, size_t count)
        {
                if (dest < src)
                        return memcpy<T>(dest, src, count);
                else
                        return rmemcpy<T>(dest + count, src + count, count);
        }

        template<typename T, size_t N, size_t M>
        inline T *memmove(T (&_Rstr dest)[N], const T (&_Rstr src)[M])
        {
                if (dest < src)
                        return memcpy<T, N, M>(dest, src);
                else
                        return rmemcpy<T, N, M>(dest, src);
        }

        template<typename T>
        inline T *memset(T *_Rstr dest, T value, size_t count)
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
        inline T *memset(T (&_Rstr dest)[N], T value)
        {
                for (auto &entry: dest)
                        entry = value;

                return dest;
        }

        inline size_t strlen(const char *str)
        {
                size_t count = 0;
                if (!*str)
                        return count;

                do {
                        ++count;
                } while (*++str);

                return count;
        }

        template<size_t N>
        inline size_t strlen(const char (&str)[N])
        {
                for (const auto &entry : str)
                        if (!entry)
                                return &entry - str;

                return N;
        }
}
#endif //_LIBC_INCLUDE_XOS_STRING_H
