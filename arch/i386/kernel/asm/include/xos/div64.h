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
// Created by xgallom on 5/21/20.
//

#ifndef _XOS_ARCH_I386_KERNEL_ASM_INCLUDE_XOS_DIV64_H
#define _XOS_ARCH_I386_KERNEL_ASM_INCLUDE_XOS_DIV64_H

/** TODO: WHAT THE FUCK TORVALDS */
/*
 * do_div() is NOT a C function. It wants to return
 * two values (the quotient and the remainder), but
 * since that doesn't work very well in C, what it
 * does is:
 *
 * - modifies the 64-bit dividend _in_place_
 * - returns the 32-bit remainder
 *
 * This ends up being the most efficient "calling
 * convention" on x86.
 */
/*
#define do_div(n, base)                                                \
({                                                                \
        unsigned long __upper, __low, __high, __mod, __base;        \
        __base = (base);                                        \
        if (__builtin_constant_p(__base) && is_power_of_2(__base)) { \
                __mod = n & (__base - 1);                        \
                n >>= ilog2(__base);                                \
        } else {                                                \
                asm("" : "=a" (__low), "=d" (__high) : "A" (n));\
                __upper = __high;                                \
                if (__high) {                                        \
                        __upper = __high % (__base);                \
                        __high = __high / (__base);                \
                }                                                \
                asm("divl %2" : "=a" (__low), "=d" (__mod)        \
                        : "rm" (__base), "0" (__low), "1" (__upper));        \
                asm("" : "=A" (n) : "a" (__low), "d" (__high));        \
        }                                                        \
        __mod;                                                        \
})
*/

inline uint64_t div64u(
	uint64_t x,
	uint32_t y,
	uint32_t &remainder
)
{
	union {
		uint64_t x;
		struct {
			// Fucking little-endian
			uint32_t low, high;
		};
	} buffer = {.x = x};

	uint32_t high = buffer.high;
	buffer.high = 0;

	if (high >= y) {
		buffer.high = high / y;
		high %= y;
	}

	asm(
	"divl %2"
	: "=a"(buffer.low), "=d"(remainder)
	: "rm"(y), "0"(buffer.low), "1"(high)
	:
	);

	return buffer.x;
}

#endif //_XOS_ARCH_I386_KERNEL_ASM_INCLUDE_XOS_DIV64_H
