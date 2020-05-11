//
// Created by xgallom on 5/11/20.
//

#ifndef _ARCH_i386_KERNEL_TTY_INCLUDE_XOS_TTY_H
#define _ARCH_i386_KERNEL_TTY_INCLUDE_XOS_TTY_H

#include <stddef.h>
#include <stdint.h>

namespace tty {
	void initialize();
	void clear();

	void putchar(char c);
	void write(const char *data, size_t size);
	void write(const char *data);

	void setColor(uint8_t color);
}

#endif //_ARCH_i386_KERNEL_TTY_INCLUDE_XOS_TTY_H
