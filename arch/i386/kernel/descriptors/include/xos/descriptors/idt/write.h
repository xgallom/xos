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

#ifndef _ARCH_I386_KERNEL_DESCRIPTORS_IDT_INCLUDE_XOS_DESCRIPTORS_IDT_WRITE_H
#define _ARCH_I386_KERNEL_DESCRIPTORS_IDT_INCLUDE_XOS_DESCRIPTORS_IDT_WRITE_H

#include <xos/write.h>
#include "../idt.h"

namespace xos {
    inline bool write(const char *,
		      idt::IdtEntry arg,
		      const char *&errorBuffer,
		      size_t formatLength,
		      size_t offsetIndex = 0)
    {
	    if (formatLength) {
		    errorBuffer = "idt::IdtEntry expects no format";
		    return false;
	    }

	    const auto offset = TabOffset(offsetIndex);
	    const auto newOffset = TabOffset(offsetIndex + 1);

	    const uint32_t argOffset = (uint32_t(arg.offset1) << 16u) |
				       arg.offset0;

	    xos::write("idt::IdtEntry {\n");

	    xos::write(newOffset);
	    xos::write("offset:\t");
	    xos::writeHex(argOffset);
	    xos::write("\n");

	    xos::write(newOffset);
	    xos::write("selector:\t");
	    xos::writeHex(arg.selector);
	    xos::write("\n");

	    xos::write(newOffset);
	    xos::write("type:\t");
	    xos::writeBin(arg.type);
	    xos::write("\n");

	    xos::write(newOffset);
	    xos::write("flags:\t");
	    xos::writeBin(arg.flags);
	    xos::write("\n");

	    xos::write(offset);
	    xos::write("}");

	    return true;
    }
}

#endif //_ARCH_I386_KERNEL_DESCRIPTORS_IDT_INCLUDE_XOS_DESCRIPTORS_IDT_WRITE_H
