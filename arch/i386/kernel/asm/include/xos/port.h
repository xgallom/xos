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
// Created by xgallom on 5/12/20.
//

#ifndef XOS_ARCH_I386_KERNEL_ASM_INCLUDE_XOS_PORT_H
#define XOS_ARCH_I386_KERNEL_ASM_INCLUDE_XOS_PORT_H

#include <stdint.h>

inline void outb(uint16_t port, uint8_t value)
{
	asm volatile(
	"outb %[value], %[port]\n"
	:
	: [value]"a"(value), [port]"Nd"(port)
	:
	);
}

#endif //XOS_ARCH_I386_KERNEL_ASM_INCLUDE_XOS_PORT_H
