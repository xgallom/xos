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
