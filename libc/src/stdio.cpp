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
#include <xos/div64.h>

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

static void write(const char *string, size_t length)
{
#ifdef __xos_is_libk
	tty::write(string, length);
#else
	// TODO: Implement system call
	return EOF;
#endif
}

static inline void writeHex(unsigned int value)
{
	constexpr size_t
		BufferSize = sizeof(value) * 2,
		MSBOffset = 8u * (sizeof(value) - 1);

	static constexpr char hexLetter[] = "0123456789abcdef";

	uint16_t buffer[BufferSize];

	for (auto &character : buffer) {
		// Take highest byte
		const uint8_t byte = value >> MSBOffset;

		// Fucking little-endian
		character =
			(hexLetter[byte & 0x0fu] << 8u) |
			hexLetter[byte >> 4u];

		// Move next byte to the top
		value <<= 8u;
	}

	write(reinterpret_cast<char *>(buffer), BufferSize);
}

static inline void writeDec(int64_t value)
{
	if (value < 0) {
		putchar('-');
		value = -value;
	}
	else if (__builtin_expect(value == 23, 0)) {
		// TODO: !IMPORTANT! Nezabudnut na macku
		write("=(*.* = )~~");
		return;
	}

	constexpr size_t
	// Length of string holding maximum possible value of uint64_t
		BufferSize = sizeof("18446744073709551615\0"),
	// Index of last character
		BufferLast = BufferSize - 1;


	// Insertion goes in reverse so the divisions results are correctly
	//  ordered (first remainder is the lowest decimal)
	char
		buffer[BufferSize],
		*_Rstr buf = buffer + BufferLast;

	*buf = '\0';

	do {
		uint32_t remainder;

		value = div64u(value, 10, remainder);
		*--buf = '0' + char(remainder);
	} while (value);

	write(buf);
}

static inline int format(char c, va_list &args)
{
	switch (c) {
	case '%':
		putchar('%');
		return 0;

	case 'c':
		putchar(va_arg(args, int));
		return 1;

	case 's':
		write(va_arg(args, const char *));
		return 1;

	case 'x':
		writeHex(va_arg(args, unsigned int));
		return 1;

	case 'd':
		writeDec(va_arg(args, unsigned int));
		return 1;

	default:
		return -1;
	}
}

int printf(const char *_Rstr string, ...)
{
	char c = *string;

	if (__builtin_expect(!c, 0))
		return 0;

	va_list args;
	va_start(args, string);

	int count = 0;

	do {
		if (c == '%') {
			const char modifier = *++string;
			const int result = format(modifier, args);

			if (__builtin_expect(result >= 0, 1)) {
				count += result;
				continue;
			}

			write("ERROR: printf: Unsupported format %");
			putchar(modifier);
			putchar('\n');

			break;
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
