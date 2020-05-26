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

#ifndef _XOS_ARCH_I386_KERNEL_GDT_INCLUDE_XOS_GDT_WRITE_H
#define _XOS_ARCH_I386_KERNEL_GDT_INCLUDE_XOS_GDT_WRITE_H

#include "../gdt.h"
#include <xos/write.h>

namespace xos {
    inline bool write(const char *,
		      gdt::GdtEntry arg,
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

	    const uint32_t
		    base = (uint32_t(arg.base2) << 24u) |
			   (uint32_t(arg.base1) << 16u) |
			   arg.base0,
		    limit = (uint32_t(arg.limit1) << 16u) |
			    arg.limit0;

	    xos::write("gdt::GdtEntry {{\n");

	    xos::write(newOffset);
	    xos::write("base:\t");
	    xos::writeHex(base);
	    xos::write("\n");

	    xos::write(newOffset);
	    xos::write("limit:\t");
	    xos::writeHex(limit);
	    xos::write("\n");

	    xos::write(newOffset);
	    xos::write("access:\t");
	    xos::writeBin(arg.access);
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

#endif //_XOS_ARCH_I386_KERNEL_GDT_INCLUDE_XOS_GDT_WRITE_H
