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
// Created by xgallom on 10/23/20.
//

#ifndef _ARCH_I386_KERNEL_ASM_INCLUDE_XOS_INTERRUPT_HANDLER_H
#define _ARCH_I386_KERNEL_ASM_INCLUDE_XOS_INTERRUPT_HANDLER_H

#include <stdint.h>

using InterruptHandler = void (uint32_t error, uint32_t irq);

#endif //_ARCH_I386_KERNEL_ASM_INCLUDE_XOS_INTERRUPT_HANDLER_H
