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


cmake_minimum_required(VERSION 3.12)

set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_VERSION 1)

# specify cross compilers and tools
SET(CMAKE_C_COMPILER_WORKS 1)
SET(CMAKE_CXX_COMPILER_WORKS 1)


project(xos C CXX ASM)
set(CMAKE_C_STANDARD gnu99)
set(CMAKE_CXX_STANDARD 17)


set(XOS_ARCHITECTURES i386)

if (XOS_ARCH IN_LIST XOS_ARCHITECTURES)
    message("Found architecture ${XOS_ARCH}")
else ()
    message("Architecture ${XOS_ARCH} is not a valid architecture")
    message("Available architectures: ${XOS_ARCHITECTURES}")
    message(FATAL_ERROR "System architecture specified by XOS_ARCH is not valid")
endif ()


set(COMMON_FLAGS "-fno-asynchronous-unwind-tables -fno-dwarf2-cfi-asm  \
-ffreestanding -Wall -Wextra -Werror")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${COMMON_FLAGS} -nostdlib")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${COMMON_FLAGS}")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${COMMON_FLAGS} -fno-exceptions \
-fno-rtti -Wmultiple-inheritance -Wuseless-cast -Wimplicit-fallthrough=5")


add_subdirectory(core)
add_subdirectory(arch)
add_subdirectory(libc)
add_subdirectory(xos)
add_subdirectory(image_root)


target_link_options(xos PUBLIC "-T${CMAKE_SOURCE_DIR}/arch/${XOS_ARCH}/linker.ld")
