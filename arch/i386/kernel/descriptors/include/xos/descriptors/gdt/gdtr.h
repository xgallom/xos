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
// Created by xgallom on 10/15/20.
//

#ifndef _ARCH_I386_KERNEL_DESCRIPTORS_INCLUDE_XOS_DESCRIPTORS_GDT_GDTR_H
#define _ARCH_I386_KERNEL_DESCRIPTORS_INCLUDE_XOS_DESCRIPTORS_GDT_GDTR_H

#include "gdt-entry.h"
#include "../descriptor-register.h"

namespace gdt {
    using Gdtr = DescriptorRegister<const GdtEntry>;
    static_assert(sizeof(Gdtr) == Gdtr::Size, "Gdtr size mismatch");
}

#endif //_ARCH_I386_KERNEL_DESCRIPTORS_INCLUDE_XOS_DESCRIPTORS_GDT_GDTR_H
