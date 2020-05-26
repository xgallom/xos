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
    inline bool write(const char *,
		      mbt::Multiboot::Header arg,
		      const char *&errorBuffer,
		      size_t formatLength,
		      size_t offsetIndex = 0)
    {
	    if (formatLength) {
		    errorBuffer = "mbt::Multiboot::Header expects no format";
		    return false;
	    }

	    const auto offset = TabOffset(offsetIndex);
	    const auto newOffset = TabOffset(offsetIndex + 1);

	    xos::write("xos::Multiboot::Header{\n");

	    xos::write(newOffset);
	    xos::write("flags:\t");
	    xos::writeBin(arg.flags);
	    xos::write("\n");

	    xos::write(offset);
	    xos::write("}");

	    return true;
    }

    inline bool write(const char *,
		      mbt::Multiboot::Memory arg,
		      const char *&errorBuffer,
		      size_t formatLength,
		      size_t offsetIndex = 0)
    {
	    if (formatLength) {
		    errorBuffer = "mbt::Multiboot::Memory expects no format";
		    return false;
	    }

	    const auto offset = TabOffset(offsetIndex);
	    const auto newOffset = TabOffset(offsetIndex + 1);

	    xos::write("mbt::Multiboot::Memory{\n");

	    xos::write(newOffset);
	    xos::write("lower:\t");
	    xos::writeHex(arg.lower);
	    xos::write("\n");

	    xos::write(newOffset);
	    xos::write("upper:\t");
	    xos::writeHex(arg.upper);
	    xos::write("\n");

	    xos::write(offset);
	    xos::write("}");

	    return true;
    }

    inline bool write(const char *,
		      const mbt::Multiboot::Boot arg,
		      const char *&errorBuffer,
		      size_t formatLength,
		      size_t offsetIndex = 0)
    {
	    if (formatLength) {
		    errorBuffer = "mbt::Multiboot::Boot expects no format";
		    return false;
	    }

	    const auto offset = TabOffset(offsetIndex);
	    const auto newOffset = TabOffset(offsetIndex + 1);

	    xos::write("mbt::Multiboot::Boot{\n");

	    xos::write(newOffset);
	    xos::write("device:\t");
	    xos::writeHex(arg.device);
	    xos::write("\n");

	    xos::write(offset);
	    xos::write("}");

	    return true;
    }

    inline bool write(const char *,
		      const mbt::Multiboot::Object *arg,
		      const char *&errorBuffer,
		      size_t formatLength,
		      size_t offsetIndex = 0)
    {
	    if (formatLength) {
		    errorBuffer = "mbt::Multiboot::Object expects no format";
		    return false;
	    }


	    const auto offset = TabOffset(offsetIndex);
	    const auto newOffsetIndex = offsetIndex + 1;
	    const auto newOffset = TabOffset(newOffsetIndex);

	    xos::write("mbt::Multiboot::Object{\n");

	    xos::write(newOffset);
	    xos::write("header:\t");
	    xos::write("", arg->header, errorBuffer, 0, newOffsetIndex);
	    xos::write("\n");

	    xos::write(newOffset);
	    xos::write("memory:\t");
	    xos::write("", arg->memory, errorBuffer, 0, newOffsetIndex);
	    xos::write("\n");

	    xos::write(newOffset);
	    xos::write("boot:\t");
	    xos::write("", arg->boot, errorBuffer, 0, newOffsetIndex);
	    xos::write("\n");

	    xos::write(offset);
	    xos::write("}");

	    return true;
    }
}

#endif //_XOS_ARCH_I386_KERNEL_MULTIBOOT_INCLUDE_XOS_WRITE_MBT_H
