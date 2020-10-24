# xos
# Copyright (C) 2020 Milan Gallo <gallo.milan.jr@gmail.com>
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, version 2.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <https://www.gnu.org/licenses/>.


# Declare constants for the Multiboot header
.set ALIGN,    1<<0             # align loaded modules on page boundaries
.set MEMINFO,  1<<1             # provide memory map
.set VIDMODE,  1<<2             #
                                #
.set TEXTMODE, 1                #
.set WIDTH,    80               #
.set HEIGHT,   50               #
                                # this is the Multiboot 'flag' field
.set FLAGS,    ALIGN | MEMINFO | VIDMODE
.set MAGIC,    0x1BADB002       # 'magic number' lets bootloader find the header
.set CHECKSUM, -(MAGIC + FLAGS) # checksum of above, to prove we are multiboot

# Declare a header as in the Multiboot Standard
.section .multiboot
.align 4
.long MAGIC
.long FLAGS
.long CHECKSUM
.rept 5
    .long 0
.endr
.long TEXTMODE
.long WIDTH
.long HEIGHT
.long 0

# Reserve a stack for the initial thread
.section .bss

.align 16
stack_bottom:
.skip 16384 # 16 kB
stack_top:

# The kernel entry point
.section .text
.global _start
.type _start, @function
_start:
    cli

    # Set up stack
	movl $stack_top, %esp

	# Call the global constructors
	# Ignored for now
	# call _init

	# Transfer control to the main kernel
	push %ebx
	push %eax
	call kernel_main

	# Hang if kernel_main unexpectedly returns
	cli
1:	hlt
	jmp 1b
.size _start, . - _start
