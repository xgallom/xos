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

#include <cstring>

char *strcpy(char *_Rstr dest, const char *_Rstr src)
{
        char *_Rstr start = dest;
        char value = *src;

        if (!value)
                return start;

        do {
                *dest++ = value;
        } while ((value = *++src));

        *dest = '\0';
        return start;
}

char *strncpy(char *_Rstr dest, const char *_Rstr src, size_t count)
{
        char *_Rstr start = dest;
        char value = *src;

        if (!value || !count)
                return start;

        do {
                *dest++ = value;
        } while ((value = *++src) && --count);

        *dest = '\0';
        return start;
}

char *strcat(char *_Rstr dest, const char *_Rstr src)
{
        auto *_Rstr const start = dest;

        if (*dest)
                do {} while (*++dest);

        strcpy(dest, src);
        return start;
}

char *strncat(char *_Rstr dest, const char *_Rstr src, size_t count)
{
        auto *_Rstr const start = dest;

        if (!count)
                return start;
        else if (*dest)
                do {} while (*++dest && --count);

        strncpy(dest, src, count);
        return start;
}

size_t strlen(const char *str)
{
        if (!*str)
                return 0;

        size_t length = 0;

        do {
                ++length;
        } while (*++str);

        return length;
}

int strcmp(const char *lhsArg, const char *rhsArg)
{
        auto
                lhs = reinterpret_cast<const unsigned char *>(lhsArg),
                rhs = reinterpret_cast<const unsigned char *>(rhsArg);

        int l, r;
        do {
                l = *lhs++;
                r = *rhs++;
                const int difference = l - r;

                if (difference)
                        return difference;
        } while (l && r);

        return 0;
}

int strncmp(const char *lhsArg, const char *rhsArg, size_t count)
{
        auto
                lhs = reinterpret_cast<const unsigned char *>(lhsArg),
                rhs = reinterpret_cast<const unsigned char *>(rhsArg);

        if (!count)
                return 0;

        int l, r;
        do {
                l = *lhs++;
                r = *rhs++;
                const int difference = l - r;

                if (difference)
                        return difference;
        } while (l && r && --count);

        return 0;
}

char *strchr(const char *str, int ch)
{
        char value;

        do {
                value = *str++;
                if (value == ch)
                        return const_cast<char *>(str);
        } while (value);

        return nullptr;
}

char *strrchr(const char *str, int ch)
{
        // walk = rbegin, str = rend
        const char *walk = str + strlen(str);
        --str;

        do {
                if (*walk == ch)
                        return const_cast<char *>(str);
        } while (--walk != str);

        return nullptr;
}

static inline bool isIn(char ch, const char *src, size_t count)
{
        do {
                if (ch == *src++)
                        return true;
        } while (--count);

        return false;
}

size_t strspn(const char *dest, const char *src)
{
        auto value = *dest;
        if (!value || !*src)
                return 0;

        const auto length = strlen(src);
        size_t result = 0;

        do {
                if (!isIn(value, src, length))
                        return result;
                ++result;
        } while ((value = *++dest));

        return result;
}

size_t strcspn(const char *dest, const char *src)
{
        auto value = *dest;
        if (!value || !*src)
                return 0;

        const auto length = strlen(src);
        size_t result = 0;

        do {
                if (isIn(value, src, length))
                        return result;
                ++result;
        } while ((value = *++dest));

        return result;
}

char *strpbrk(const char *dest, const char *src)
{
        auto value = *dest;
        if (!value || !*src)
                return nullptr;

        const auto length = strlen(src);

        do {
                if (isIn(value, src, length))
                        return const_cast<char *>(dest);
        } while ((value = *++dest));

        return nullptr;
}

char *strstr(const char *str, const char *subStr)
{
        const auto subLength = strlen(subStr);

        if (!subLength)
                return const_cast<char *>(str);
        else if (!*str)
                return nullptr;

        const char *walk = str;
        do {
                auto count = subLength;
                const auto *subWalk = subStr;

                do {
                        if (*walk++ != *subWalk++)
                                break;
                } while (--count);

                if (!count)
                        return const_cast<char *>(walk);
        } while (*(walk = ++str));

        return nullptr;
}

char *strtok(char *_Rstr strArg, const char *_Rstr delim)
{
        static char *_Rstr str = nullptr;

        if (strArg)
                // First run return first token and find delim
                str = strArg;
        else {
                // Subsequent run skip delim and set new token
                str += strspn(str, delim);
                strArg = str;
        }

        // Isolate token
        str += strcspn(str, delim);;
        *str++ = '\0';

        return strArg;
}

void *memchr(const void *ptrArg, int chArg, size_t count)
{
        if (!count)
                return nullptr;

        auto ptr = reinterpret_cast<const unsigned char *>(ptrArg);
        unsigned char ch = chArg;

        do {
                if (*ptr++ == ch)
                        return const_cast<void *>(
                                reinterpret_cast<const void *>(ptr)
                        );
        } while (--count);

        return nullptr;
}

int memcmp(const void *lhsArg, const void *rhsArg, size_t count)
{
        auto
                lhs = reinterpret_cast<const unsigned char *>(lhsArg),
                rhs = reinterpret_cast<const unsigned char *>(rhsArg);

        if (!count)
                return 0;

        int difference;
        do {
                difference = int(*lhs++) - int(*rhs++);
        } while (!difference && --count);

        return difference;
}

void *memset(void *dest, int ch, size_t count)
{
        if (!count)
                return dest;

        auto walk = reinterpret_cast<unsigned char *>(dest);
        unsigned char value = ch;

        do {
                *walk++ = value;
        } while (--count);

        return dest;
}

static inline void unsafeMemcpy(unsigned char *_Rstr dest, const unsigned char *_Rstr src, size_t byteLength)
{
        do
                *dest++ = *src++;
        while (--byteLength);
}

void *memcpy(void *_Rstr dest, const void *_Rstr src, size_t count)
{
        if (!count)
                return dest;

        unsafeMemcpy(
                reinterpret_cast<unsigned char *_Rstr>(dest),
                reinterpret_cast<const unsigned char *_Rstr>(src),
                count
        );

        return dest;
}

static inline void unsafeMemcpyUp(unsigned char *_Rstr rdest, const unsigned char *_Rstr rsrc, size_t byteLength)
{
        do
                *rdest-- = *rsrc--;
        while (--byteLength);
}

void *memmove(void *dest, const void *src, size_t count)
{
        if (!count)
                return dest;

        if (dest < src)
                unsafeMemcpy(
                        reinterpret_cast<unsigned char *_Rstr>(dest),
                        reinterpret_cast<const unsigned char *_Rstr>(src),
                        count
                );
        else {
                const size_t offset = count - 1;

                unsafeMemcpyUp(
                        reinterpret_cast<unsigned char *_Rstr>(dest) + offset,
                        reinterpret_cast<const unsigned char *_Rstr>(src) + offset,
                        count
                );
        }

        return dest;
}

void *memccpy(void *_Rstr dest, const void *_Rstr src, int c, size_t count)
{
        auto *_Rstr walk = reinterpret_cast<unsigned char *>(dest);
        const auto *_Rstr srcWalk = reinterpret_cast<const unsigned char *>(src);
        const unsigned char delim = c;

        auto value = *srcWalk;
        if (value == delim || !count)
                return dest;

        do {
                *walk++ = value;
        } while ((value = *++srcWalk) != delim && --count);

        *walk = delim;
        return dest;
}
