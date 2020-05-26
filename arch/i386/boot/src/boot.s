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
.set FLAGS,    ALIGN | MEMINFO  # this is the Multiboot 'flag' field
.set MAGIC,    0x1BADB002       # 'magic number' lets bootloader find the header
.set CHECKSUM, -(MAGIC + FLAGS) # checksum of above, to prove we are multiboot

# Declare a header as in the Multiboot Standard
.section .multiboot
.align 4
.long MAGIC
.long FLAGS
.long CHECKSUM

# Reserve a stack for the initial thread
.section .bss

.align 2
.global xos_reg_dump
xos_reg_dump:
.skip 16
xos_reg_dump_top:

.align 16
stack_bottom:
.skip 16384 # 16 kB
stack_top:

# The kernel entry point
.section .text
.global _start
.type _start, @function
_start:
    # Dump all segment registers for examination
    movl $xos_reg_dump_top, %esp
    movw %gs, %ax
    pushw %ax
    movw %fs, %ax
    pushw %ax
    movw %ss, %ax
    pushw %ax
    movw %es, %ax
    pushw %ax
    movw %ds, %ax
    pushw %ax
    movw %cs, %ax
    pushw %ax

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
