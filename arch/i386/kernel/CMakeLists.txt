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


add_subdirectory(asm)
add_subdirectory(containers)
add_subdirectory(cpuid)
add_subdirectory(descriptors)
add_subdirectory(drivers)
add_subdirectory(memory)
add_subdirectory(multiboot)
add_subdirectory(tty)
add_subdirectory(vga)

add_library(arch_kernel INTERFACE)

target_link_libraries(arch_kernel INTERFACE
        arch_kernel_asm
        arch_kernel_containers
        arch_kernel_cpuid
        arch_kernel_descriptors
        arch_kernel_drivers
        arch_kernel_memory
        arch_kernel_multiboot
        arch_kernel_tty
        arch_kernel_vga
        )
