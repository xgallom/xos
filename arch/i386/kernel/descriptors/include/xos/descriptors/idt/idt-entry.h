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

#ifndef _ARCH_I386_KERNEL_DESCRIPTORS_INCLUDE_XOS_DESCRIPTORS_IDT_IDT_ENTRY_H
#define _ARCH_I386_KERNEL_DESCRIPTORS_INCLUDE_XOS_DESCRIPTORS_IDT_IDT_ENTRY_H

#include <stdint.h>
#include <xos/packed.h>

namespace idt {
    struct _Pckd IdtEntryFlags {
	    uint8_t _padding: 4 = 0,
		    storageSegment: 1 = false,
		    privilege: 2,
		    present: 1 = true;

	    constexpr IdtEntryFlags(uint8_t privilege)
		    : privilege{privilege} {}

	    explicit constexpr operator uint8_t() const
	    {
		    return
			    ((uint8_t(storageSegment) & 0x01u) << 0u) |
			    ((uint8_t(privilege) & 0x03u) << 1u) |
			    ((uint8_t(present) & 0x01u) << 3u);
	    }

	    static constexpr size_t Size = sizeof(uint8_t);
    };

    static_assert(sizeof(IdtEntryFlags) == IdtEntryFlags::Size,
		  "IdtEntryFlags size mismatch"
    );


    struct _Pckd IdtEntry {
	    enum Type : uint8_t {
		    TypeTaskGate = 0x5,
		    Type16InterruptGate = 0x6,
		    Type16TrapGate = 0x7,
		    Type32InterruptGate = 0xe,
		    Type32TrapGate = 0xf,
	    };

	    union {
		    uint64_t entry = 0;
		    struct _Pckd {
			    uint16_t offset0;
			    uint16_t selector;
			    uint8_t _padding;
			    uint8_t type: 4,
				    flags: 4;
			    uint16_t offset1;
		    };
	    };

	    IdtEntry() = default;

	    constexpr IdtEntry(
		    uint16_t selector,
		    uint32_t offset,
		    Type type,
		    IdtEntryFlags flags
	    ) :
		    offset0{uint16_t(offset & 0xffffu)},
		    selector{selector},
		    _padding{0},
		    type{type},
		    flags{uint8_t(flags)},
		    offset1{uint16_t((offset >> 16u) & 0xffffu)} {}

	    static constexpr size_t Size = sizeof(uint64_t);
    };

    static_assert(sizeof(IdtEntry) == IdtEntry::Size, "IdtEntry size mismatch");
}

#endif //_ARCH_I386_KERNEL_DESCRIPTORS_INCLUDE_XOS_DESCRIPTORS_IDT_IDT_ENTRY_H
