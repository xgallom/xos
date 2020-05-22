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

#ifndef _XOS_XOS_SRC_COMPILER_CHECKS_H
#define _XOS_XOS_SRC_COMPILER_CHECKS_H

/* Check if the compiler thinks you are targeting the wrong operating system. */
#if defined(__linux__) || \
        defined(unix) || defined(__unix) || defined(__unix__) || \
        defined(_WIN32) || defined(WIN32) || \
        defined(__APPLE__) || defined(__MACH__) || \
        defined(__FreeBSD__) || \
        defined(__ANDROID__)

#error "Can't use the system compiler for compiling xos"

#endif

/* This tutorial will only work for the 32-bit ix86 targets. */
#if !defined(__i386__)

#error "xos requires an ix86-elf compiler"

#endif

#endif //_XOS_XOS_SRC_COMPILER_CHECKS_H
