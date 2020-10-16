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

#ifndef _ARCH_I386_KERNEL_DESCRIPTORS_INCLUDE_XOS_DESCRIPTORS_WRITE_H
#define _ARCH_I386_KERNEL_DESCRIPTORS_INCLUDE_XOS_DESCRIPTORS_WRITE_H

#include "gdt/write.h"
#include "idt/write.h"

namespace xos {
    template<typename DescriptorType>
    inline bool write(const char *,
		      DescriptorRegister<DescriptorType> arg,
		      const char *&errorBuffer,
		      size_t formatLength,
		      size_t offsetIndex = 0)
    {
	    if (formatLength) {
		    errorBuffer = "gdt::GdtEntry expects no format";
		    return false;
	    }

	    const auto offset = TabOffset(offsetIndex);
	    const auto newOffset = TabOffset(offsetIndex + 1);

	    xos::write("DescriptorRegister {\n");

	    xos::write(newOffset);
	    xos::write("size:\t");
	    xos::writeHex(arg.size);
	    xos::write("\n");

	    xos::write(newOffset);
	    xos::write("address:\t");
	    xos::writeHex(uintptr_t(arg.address));
	    xos::write("\n");

	    xos::write(offset);
	    xos::write("}");

	    return true;
    }
}

#endif //_ARCH_I386_KERNEL_DESCRIPTORS_INCLUDE_XOS_DESCRIPTORS_WRITE_H
