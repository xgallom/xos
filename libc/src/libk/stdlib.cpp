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

#include <cstdlib>
#include <limits.h>
#include "cctype"

static const auto NaN = 0.0 / 0.0;

int atoi(const char *str)
{
        return atoll(str);
}

long atol(const char *str)
{
        return atoll(str);
}

long long atoll(const char *str)
{
        return strtoll(str, nullptr, 10);
}

long strtol(const char *_Rstr str, char **_Rstr str_end, int base)
{
        return strtoll(str, str_end, base);
}

long long strtoll(const char *_Rstr strArg, char **_Rstr str_end, int base)
{
        int c, neg = 0;
        auto *_Rstr str = reinterpret_cast<const unsigned char *_Rstr>(strArg);

        do {
                c = *str++;
        } while (isspace(c));

        if (c == '-') {
                neg = 1;
                c = *str++;
        } else if (c == '+')
                c = *str++;

        if ((base == 0 || base == 16) && c == '0' && (*str == 'x' || *str == 'X')) {
                c = str[1];
                str += 2;
                base = 16;
        }
        if (base == 0)
                base = c == '0' ? 8 : 10;

        const auto limit = neg ?
                           -static_cast<unsigned long long>(LONG_LONG_MIN) :
                           static_cast<unsigned long long>(LONG_LONG_MAX);
        const auto cutoff = limit / static_cast<unsigned long long>(base);
        const auto cutLimit = static_cast<int>(limit % static_cast<unsigned long long>(base));

        unsigned long long acc = 0;
        int any = 0;
        _forever {
                if (isdigit(c))
                        c -= '0';
                else if (isupper(c))
                        c -= 'A' - 10;
                else if (islower(c))
                        c -= 'a' - 10;
                else
                        break;

                if (c >= base)
                        break;

                if (any < 0 || acc > cutoff || (acc == cutoff && c > cutLimit))
                        any = -1;
                else {
                        any = 1;
                        acc *= base;
                        acc += c;
                }

                c = *str++;
        }

        if (any < 0)
                acc = limit;
        else if (neg)
                acc = -acc;

        if (str_end) {
                *str_end = const_cast<char *>(
                        any ? reinterpret_cast<const char *>(str - 1) : strArg
                );
        }

        return static_cast<long long>(acc);
}

unsigned long strtoul(const char *_Rstr str, char **_Rstr str_end, int base)
{
        return strtoull(str, str_end, base);
}

unsigned long long strtoull(const char *_Rstr strArg, char **_Rstr str_end, int base)
{
        int c, neg = 0;
        auto *_Rstr str = reinterpret_cast<const unsigned char *_Rstr>(strArg);

        do {
                c = *str++;
        } while (isspace(c));

        if (c == '-') {
                neg = 1;
                c = *str++;
        } else if (c == '+')
                c = *str++;

        if ((base == 0 || base == 16) && c == '0' && (*str == 'x' || *str == 'X')) {
                c = str[1];
                str += 2;
                base = 16;
        }
        if (base == 0)
                base = c == '0' ? 8 : 10;

        const auto limit = ULONG_LONG_MAX;
        const auto cutoff = limit / static_cast<unsigned long long>(base);
        const auto cutLimit = static_cast<int>(limit % static_cast<unsigned long long>(base));

        unsigned long long acc = 0;
        int any = 0;
        _forever {
                if (isdigit(c))
                        c -= '0';
                else if (isupper(c))
                        c -= 'A' - 10;
                else if (islower(c))
                        c -= 'a' - 10;
                else
                        break;

                if (c >= base)
                        break;

                if (any < 0 || acc > cutoff || (acc == cutoff && c > cutLimit))
                        any = -1;
                else {
                        any = 1;
                        acc *= base;
                        acc += c;
                }

                c = *str++;
        }

        if (any < 0)
                acc = limit;
        else if (neg)
                acc = -acc;

        if (str_end) {
                *str_end = const_cast<char *>(
                        any ? reinterpret_cast<const char *>(str - 1) : strArg
                );
        }

        return static_cast<long long>(acc);
}

float strtof(const char *_Rstr str, char **_Rstr str_end)
{
        return strtold(str, str_end);
}

double strtod(const char *_Rstr str, char **_Rstr str_end)
{
        return strtold(str, str_end);
}

long double strtold(const char *_Rstr /*str*/, char **_Rstr /*str_end*/)
{
        // TODO: Implament
        return NaN;
}

intmax_t strtoimax(const char *_Rstr nptr, char **_Rstr endptr, int base)
{
        return strtoll(nptr, endptr, base);
}

uintmax_t strtoumax(const char *_Rstr nptr, char **_Rstr endptr, int base)
{
        return strtoull(nptr, endptr, base);
}
