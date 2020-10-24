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

#ifndef _ARCH_I386_KERNEL_DESCRIPTORS_INCLUDE_XOS_DESCRIPTORS_GDT_GDT_ENTRY_H
#define _ARCH_I386_KERNEL_DESCRIPTORS_INCLUDE_XOS_DESCRIPTORS_GDT_GDT_ENTRY_H

#include <stdint.h>
#include <stddef.h>
#include <xos/packed.h>

namespace gdt {
    struct _Pckd GdtEntryAccess {
	    uint8_t accessed: 1 = false,
		    readWrite: 1,
		    directionConforming: 1,
		    executable: 1,
		    type: 1,
		    privilege: 2,
		    present: 1 = true;

	    enum Direction : bool {
		    DirectionUp,
		    DirectionDown
	    };

	    enum Conforming : bool {
		    ConformingNo,
		    ConformingYes
	    };

	    enum Readable : bool {
		    ReadableNo,
		    ReadableYes
	    };

	    enum Writable : bool {
		    WritableNo,
		    WritableYes
	    };

	    static constexpr GdtEntryAccess Data(
		    uint8_t privilege,
		    Direction direction,
		    Writable writable)
	    {
		    return {
			    .readWrite = writable,
			    .directionConforming = direction,
			    .executable = false,
			    .type = true,
			    .privilege = privilege,
		    };
	    }

	    static constexpr GdtEntryAccess Executable(
		    uint8_t privilege,
		    Conforming conforming,
		    Readable readable)
	    {
		    return {
			    .readWrite = readable,
			    .directionConforming = conforming,
			    .executable = true,
			    .type = true,
			    .privilege = privilege,
		    };
	    }

	    explicit constexpr operator uint8_t() const
	    {
		    return
			    ((uint8_t(accessed) & 0x01u) << 0u) |
			    ((uint8_t(readWrite) & 0x01u) << 1u) |
			    ((uint8_t(directionConforming) & 0x01u) << 2u) |
			    ((uint8_t(executable) & 0x01u) << 3u) |
			    ((uint8_t(type) & 0x01u) << 4u) |
			    ((uint8_t(privilege) & 0x03u) << 5u) |
			    ((uint8_t(present) & 0x01u) << 7u);
	    }

	    static constexpr size_t Size = sizeof(uint8_t);
    };

    static_assert(sizeof(GdtEntryAccess) == GdtEntryAccess::Size,
		  "GdtEntryAccess size mismatch");


    struct _Pckd GdtEntryFlags {
	    uint8_t _padding: 4 = 0,
		    _reserved: 2 = 0,
		    size: 1,
		    granularity: 1;

	    enum Size : bool {
		    Size16b,
		    Size32b
	    };

	    enum Granularity : bool {
		    GranularityByte,
		    GranularityPage
	    };

	    constexpr GdtEntryFlags(
		    Size size,
		    Granularity granularity
	    ) : size{size}, granularity{granularity} {}

	    explicit constexpr operator uint8_t() const
	    {
		    return
			    ((uint8_t(size) & 0x01u) << 2) |
			    ((uint8_t(granularity) & 0x01u) << 3u);
	    }

	    static constexpr size_t Size = sizeof(uint8_t);
    };

    static_assert(sizeof(GdtEntryFlags) == GdtEntryFlags::Size,
		  "GdtEntryFlags size mismatch");


    struct _Pckd GdtEntry {
	    union {
		    uint64_t entry;
		    struct _Pckd {
			    uint16_t limit0;
			    uint16_t base0;
			    uint8_t base1;
			    uint8_t access;
			    uint8_t limit1: 4,
				    flags: 4;
			    uint8_t base2;
		    };
	    };

	    constexpr GdtEntry(
		    uint32_t base,
		    uint32_t limit,
		    uint8_t access,
		    uint8_t flags
	    ) :
		    limit0{uint16_t(limit & 0xffffu)},
		    base0{uint16_t(base & 0xffffu)},
		    base1{uint8_t((base >> 16u) & 0xffu)},
		    access{access},
		    limit1{uint8_t((limit >> 16u) & 0x0fu)},
		    flags{flags},
		    base2{uint8_t((base >> 24u) & 0xffu)} {}

	    constexpr GdtEntry(
		    uint32_t base,
		    uint32_t limit,
		    GdtEntryAccess access,
		    GdtEntryFlags flags
	    ) : GdtEntry(base, limit, uint8_t(access), uint8_t(flags)) {}

	    static constexpr GdtEntry Null() { return GdtEntry(0, 0, 0, 0); }

	    static constexpr size_t Size = sizeof(uint64_t);
    };

    static_assert(sizeof(GdtEntry) == GdtEntry::Size, "GdtEntry size mismatch");
}

#endif //_ARCH_I386_KERNEL_DESCRIPTORS_INCLUDE_XOS_DESCRIPTORS_GDT_GDT_ENTRY_H
