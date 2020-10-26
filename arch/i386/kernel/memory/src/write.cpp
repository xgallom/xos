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
// Created by xgallom on 10/26/20.
//

#include <xos/memory/write.h>

namespace xos {
    bool write(const char *,
	       mem::map::phy::Entry arg,
	       const char *&errorBuffer,
	       size_t formatLength,
	       size_t )
    {
	    if (formatLength) {
		    errorBuffer = "mem::map::phy::Entry expects no format";
		    return false;
	    }

	    xos::write("mem::map::phy::Entry{ ");

	    xos::write("base: ");
	    xos::writeHex(arg.base);

	    xos::write(", size: ");
	    xos::writeHex(arg.size);

	    xos::write(", type: ");
	    xos::writeHex(arg.type);

	    xos::write(" }");

	    return true;
    }
}
