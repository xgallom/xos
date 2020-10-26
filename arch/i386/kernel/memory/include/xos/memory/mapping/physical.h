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

#ifndef _ARCH_I386_KERNEL_MEMORY_INCLUDE_XOS_MEMORY_MAPPING_PHYSICAL_H
#define _ARCH_I386_KERNEL_MEMORY_INCLUDE_XOS_MEMORY_MAPPING_PHYSICAL_H

#include <stddef.h>
#include <stdint.h>
#include <xos/always-inline.h>

namespace mem::map::phy {
    struct Entry {
	    size_t _next;
	    uint32_t base, baseHigh;
	    uint32_t size, sizeHigh;
	    uint32_t type;

	    [[nodiscard]] inline const Entry *next() const
	    {
		    return reinterpret_cast<const Entry *>(
			    uintptr_t(this) + _next + sizeof(_next)
		    );
	    }
    };

    struct List {
	    struct Iterator {
		    const Entry *ptr;

		    _Inln Iterator &operator++()
		    {
			    ptr = ptr->next();
			    return *this;
		    }

		    _Inln bool operator!=(const Iterator &other) const
		    {
			    return ptr != other.ptr;
		    }

		    _Inln const Entry &operator*() const
		    {
			    return *ptr;
		    }
	    };

	    Iterator head, tail;

	    [[nodiscard]] _Inln Iterator begin() const { return head; }

	    [[nodiscard]] _Inln Iterator end() const { return tail; }
    };

    bool initialize();
    List get();

}

#endif //_ARCH_I386_KERNEL_MEMORY_INCLUDE_XOS_MEMORY_MAPPING_PHYSICAL_H
