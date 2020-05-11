//
// Created by xgallom on 5/11/20.
//

#include <stdio.h>
#include <stdarg.h>

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

int printf(const char *_Rstr string, ...)
{
	va_list args;
	va_start(args, string);

	int count = 0;

	char c = *string++;
	while(c) {
		if(c == '%') {
			switch((c = *string++)) {
				case '%':
					putchar('%');
					break;

				case 's':
					write(va_arg(args, const char *));
					++count;
					break;

				case 'x': {
					write("0x");

					const unsigned int arg = va_arg(args, unsigned int);

					for(size_t n = 1; n <= sizeof(arg); ++n) {
						const uint8_t byte = arg >> ((sizeof(arg) - n) << 3);

						const auto printHex = [](uint8_t hex) {
							putchar("0123456789abcdef"[hex & 0x0f]);
						};

						printHex(byte >> 4);
						printHex(byte);
					}

					++count;
					break;
				}

				default:
					write("ERROR: printf: Unsupported format %");
					putchar(c);

					va_end(args);
					return count;
			}
		}
		else
			putchar(c);

		c = *string++;
	}

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
