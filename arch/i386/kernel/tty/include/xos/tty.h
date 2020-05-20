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

#ifndef _ARCH_i386_KERNEL_TTY_INCLUDE_XOS_TTY_H
#define _ARCH_i386_KERNEL_TTY_INCLUDE_XOS_TTY_H

#include <stddef.h>
#include <stdint.h>

namespace tty {
	void initialize();
	void clear();

	void putchar(char c);
	void newLine();
	void write(const char *data, size_t size);
	void write(const char *data);

	void setColor(uint8_t color);
	void setCursor(uint16_t position);
}

#endif //_ARCH_i386_KERNEL_TTY_INCLUDE_XOS_TTY_H
