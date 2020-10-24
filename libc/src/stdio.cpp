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
#include <xos/write.h>

static constexpr size_t FormatBufferLength = 3;


static inline int format(
	const char *_Rstr &string,
	va_list &args,
	char *_Rstr buffer)
{
	uint8_t count = 0;

	for (; count < FormatBufferLength; (void) (++buffer, ++count)) {
		switch (*buffer = *++string) {
		case '%':
			if (count > 0)
				return -1;
			putchar('%');
			return 0;

		case 'c':
			if (count > 0)
				return -1;
			putchar(va_arg(args, int));
			return 1;

		case 's':
			if (count > 0)
				return -1;
			xos::write(va_arg(args, const char *));
			return 1;

		case 'x': {
			switch (count) {
			case 0:
				xos::writeHex(va_arg(args, unsigned int));
				return 1;

			case 1:
				xos::writeHex(va_arg(args, unsigned long));
				return 1;

			case 2:
				xos::writeHex(
					va_arg(args, unsigned long long)
				);
				return 1;

			default:
				__builtin_unreachable();
			}
		}

		case 'd':
			switch (count) {
			case 0:
				xos::writeDec(va_arg(args, int));
				return 1;

			case 1:
				xos::writeDec(va_arg(args, long));
				return 1;

			case 2:
				xos::writeDec(va_arg(args, long long));
				return 1;

			default:
				__builtin_unreachable();
			}

		case 'u':
			switch (count) {
			case 0:
				xos::writeDec(va_arg(args, unsigned int));
				return 1;

			case 1:
				xos::writeDec(va_arg(args, unsigned long));
				return 1;

			case 2:
				xos::writeDec(
					va_arg(args, unsigned long long)
				);
				return 1;

			default:
				__builtin_unreachable();
			}

			break;

		case 'b':
			xos::write(va_arg(args, unsigned int) ?
				   "true" :
				   "false");
			return 1;

		case 'l':
			break;

		default:
			return -1;
		}
	}

	return -1;
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
			char buffer[FormatBufferLength] = "";

			const int result = format(string, args, buffer);

			if (__builtin_expect(result >= 0, 1)) {
				count += result;
				continue;
			}

			xos::write(
				"ERROR: printf: Unsupported format %"
			);
			xos::write(buffer);
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
	xos::putchar(c);
	return 0;
}

int puts(const char *string)
{
	xos::write(string);
	xos::putchar('\n');
	return 0;
}

int getchar()
{
	return xos::getchar();
}
