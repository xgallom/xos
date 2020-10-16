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
// Created by xgallom on 5/25/20.
//

#ifndef _XOS_ARCH_I386_KERNEL_MULTIBOOT_INCLUDE_XOS_MBT_WRITE_H
#define _XOS_ARCH_I386_KERNEL_MULTIBOOT_INCLUDE_XOS_MBT_WRITE_H

#include "../mbt.h"
#include <xos/write.h>

namespace xos {
    bool write(const char *,
	       mbt::Multiboot::Header arg,
	       size_t formatLength,
	       size_t offsetIndex = 0);


    bool write(const char *,
	       mbt::Multiboot::Memory arg,
	       const char *&errorBuffer,
	       size_t formatLength,
	       size_t offsetIndex = 0);

    bool write(const char *,
	       mbt::Multiboot::Boot arg,
	       const char *&errorBuffer,
	       size_t formatLength,
	       size_t offsetIndex = 0);

    bool write(const char *,
	       mbt::Multiboot::Command arg,
	       const char *&errorBuffer,
	       size_t formatLength,
	       size_t offsetIndex = 0);

    bool write(const char *,
	       mbt::Multiboot::Modules arg,
	       const char *&errorBuffer,
	       size_t formatLength,
	       size_t offsetIndex = 0);

    bool write(const char *,
	       mbt::Multiboot::Symbols arg,
	       const char *&errorBuffer,
	       size_t formatLength,
	       size_t offsetIndex = 0);

    bool write(const char *,
	       mbt::Multiboot::MemoryMap arg,
	       const char *&errorBuffer,
	       size_t formatLength,
	       size_t offsetIndex = 0);

    bool write(const char *,
	       mbt::Multiboot::Drives arg,
	       const char *&errorBuffer,
	       size_t formatLength,
	       size_t offsetIndex = 0);

    bool write(const char *,
	       mbt::Multiboot::Config arg,
	       const char *&errorBuffer,
	       size_t formatLength,
	       size_t offsetIndex = 0);

    bool write(const char *,
	       mbt::Multiboot::BootLoader arg,
	       const char *&errorBuffer,
	       size_t formatLength,
	       size_t offsetIndex = 0);

    bool write(const char *,
	       mbt::Multiboot::ApmTable arg,
	       const char *&errorBuffer,
	       size_t formatLength,
	       size_t offsetIndex = 0);

    bool write(const char *,
	       mbt::Multiboot::Vbe arg,
	       const char *&errorBuffer,
	       size_t formatLength,
	       size_t offsetIndex = 0);

    bool write(const char *,
	       mbt::Multiboot::FrameBuffer arg,
	       const char *&errorBuffer,
	       size_t formatLength,
	       size_t offsetIndex = 0);

    bool write(const char *,
	       const mbt::Multiboot::Object *arg,
	       const char *&errorBuffer,
	       size_t formatLength,
	       size_t offsetIndex = 0);
}

#endif //_XOS_ARCH_I386_KERNEL_MULTIBOOT_INCLUDE_XOS_WRITE_MBT_H
