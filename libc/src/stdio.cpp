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
// Created by xgallom on 5/11/20.
//

#include <stdio.h>
#include <stdarg.h>
#include <stdint.h>
#include <stddef.h>

#ifdef __xos_is_libk

#include <xos/tty.h>

#endif

static void write(const char *string)
{
#ifdef __xos_is_libk
	tty::write(string);
#else
	// TODO: Implement system call
	return EOF;
#endif
}

static inline void writeHexLetter(uint8_t hex)
{
	putchar("0123456789abcdef"[hex & 0x0f]);
}

static inline void writeHex(unsigned int value)
{
	write("0x");

	for (size_t n = 0; n < sizeof(value); ++n) {
		const uint8_t byte = value >> (8u * (sizeof(value) - 1));

		writeHexLetter(byte >> 4);
		writeHexLetter(byte);

		value <<= 8u;
	}
}

static inline int format(char c, va_list args)
{
	switch (c) {
	case '%':
		putchar('%');
		return 0;

	case 'c':
		putchar(va_arg(args, int));
		break;

	case 's':
		write(va_arg(args, const char *));
		break;

	case 'x':
		writeHex(va_arg(args, unsigned int));
		break;

	default:
		return c;
	}

	return 1;
}

int printf(const char *_Rstr string, ...)
{
	char c = *string;

	if (!c)
		return 0;

	va_list args;
	va_start(args, string);

	int count = 0;

	do {
		if (c == '%') {
			const int result = format(*++string, args);

			if (result > 1) {
				write("ERROR: printf: Unsupported format %");
				putchar(result);
				putchar('\n');

				va_end(args);
				return count;
			}

			count += result;
		}
		else
			putchar(c);
	} while ((c = *++string));

	va_end(args);
	return count;
}

int putchar(int c)
{
#ifdef __xos_is_libk
	tty::putchar(static_cast<char>(c));
	return c;
#else
	// TODO: Implement system call
	return EOF;
#endif
}

int puts(const char *string)
{
#ifdef __xos_is_libk
	tty::write(string);
	tty::putchar('\n');
	return 0;
#else
	// TODO: Implement system call
	return EOF;
#endif
}
