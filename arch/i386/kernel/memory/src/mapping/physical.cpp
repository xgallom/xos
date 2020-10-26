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

#include <xos/memory/mapping/physical.h>
#include <xos/multiboot.h>

namespace mem::map::phy {
    static List s_memoryMap;

    bool initialize()
    {
	    const auto memoryMap = mbt::memoryMap();

	    if (!memoryMap)
		    return false;

	    s_memoryMap = {
		    .head = {reinterpret_cast<const Entry *>(
				     memoryMap->address
			     )
		    },
		    .tail = {reinterpret_cast<const Entry *>(
				     memoryMap->address + memoryMap->length
			     )
		    },
	    };

	    return true;
    }

    List get()
    {
	    return s_memoryMap;
    }


}
