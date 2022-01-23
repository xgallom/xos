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
// Created by Milan Gallo on 24/12/2021.
//

#ifndef _LIBC_INCLUDE_XOS_ATTRIBUTES_H
#define _LIBC_INCLUDE_XOS_ATTRIBUTES_H

#include <sys/cdefs.h>

#define _Section(SECTION) __attribute__((section(#SECTION), used))
#define _Inln inline __attribute__((__always_inline__, __artificial__))
#define _NoRet __attribute__((__noreturn__))
#define _Pckd __attribute__((packed))


#ifdef __cplusplus

#define _ExtC    extern "C" {
#define _ExtCEnd }

#define _Rstr __restrict

#else

#define _ExtC
#define _ExtCEnd

#define _Rstr restrict

#endif

#endif //_LIBC_INCLUDE_XOS_ATTRIBUTES_H
