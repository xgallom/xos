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
// Created by xgallom on 10/24/20.
//

#ifndef _ARCH_I386_KERNEL_CONTAINERS_INCLUDE_XOS_CONTAINERS_BITSET_H
#define _ARCH_I386_KERNEL_CONTAINERS_INCLUDE_XOS_CONTAINERS_BITSET_H

#include <stddef.h>
#include <stdint.h>
#include <xos/utility.h>

namespace xos {
    template<size_t Count_>
    struct Bitset {
	    static constexpr size_t
		    Count = Count_,
		    BitsPerEntry = sizeof(uint32_t) * 8u,
		    EntryCount = Count / BitsPerEntry;

	    static constexpr uint32_t
		    EntryOffsetMask = BitsPerEntry - 1,
		    EntryIndexOffset = xos::log2(BitsPerEntry);

	    uint32_t entries[EntryCount] = {};

	    inline uint32_t &entry(size_t id)
	    {
		    return entries[id >> EntryIndexOffset];
	    }

	    inline const uint32_t &entry(size_t id) const
	    {
		    return entries[id >> EntryIndexOffset];
	    }

	    [[nodiscard]] inline size_t mask(size_t id) const
	    {
		    return 1u << (id & EntryOffsetMask);
	    }


	    inline void set(size_t id)
	    {
		    entry(id) |= mask(id);
	    }

	    inline void rst(size_t id)
	    {
		    entry(id) &= ~mask(id);
	    }

	    [[nodiscard]] inline bool operator[](size_t id) const
	    {
		    return entry(id) & mask(id);
	    }
    };
}

#endif //_ARCH_I386_KERNEL_CONTAINERS_INCLUDE_XOS_CONTAINERS_BITSET_H
