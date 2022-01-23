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

#include <cctype>

int isalnum(int c)
{
        return isalpha(c) || isdigit(c);
}

int isalpha(int c)
{
        return islower(c) || isupper(c);
}

int isblank(int c)
{
        return c == '\t' || c == ' ';
}

int iscntrl(int c)
{
        return !isprint(c);
}

int isdigit(int c)
{
        return c >= '0' && c <= '9';
}

int isgraph(int c)
{
        return c > 32 && c < 127;
}

int islower(int c)
{
        return c >= 'a' && c <= 'z';
}

int isprint(int c)
{
        return c > 31 && c < 127;
}

int ispunct(int c)
{
        return isgraph(c) && !isalnum(c);
}

int isspace(int c)
{
        return (c > 8 && c < 14) || c == ' ';
}

int isupper(int c)
{
        return c >= 'A' && c <= 'Z';
}

int isxdigit(int c)
{
        return isdigit(c) || (c >= 'a' && c <= 'f') || (c >= 'A' && c <= 'F');
}

int tolower(int c)
{
        return isupper(c) ? (c - 'A' + 'a') : c;
}

int toupper(int c)
{
        return islower(c) ? (c - 'a' + 'A') : c;
}
