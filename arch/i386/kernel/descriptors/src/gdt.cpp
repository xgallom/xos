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

#include <xos/descriptors/gdt.h>
#include <xos/stdio.h>

namespace gdt {
    static constexpr GdtEntry s_gdt[] = {
	    GdtEntry::Null(),
	    GdtEntry(
		    0x00000000u,
		    0x000fffffu,
		    GdtEntryAccess::Executable(
			    0,
			    GdtEntryAccess::ConformingNo,
			    GdtEntryAccess::ReadableYes
		    ),
		    GdtEntryFlags(
			    GdtEntryFlags::Size32b,
			    GdtEntryFlags::GranularityPage
		    )
	    ),
	    GdtEntry(
		    0x00000000u,
		    0x000fffffu,
		    GdtEntryAccess::Data(
			    0,
			    GdtEntryAccess::DirectionUp,
			    GdtEntryAccess::WritableYes
		    ),
		    GdtEntryFlags(
			    GdtEntryFlags::Size32b,
			    GdtEntryFlags::GranularityPage
		    )
	    ),
    };

    static Gdtr s_gdtr;

    void storeGdtr()
    {
	    asm volatile(
	    "sgdt %[gdtr]"
	    :
	    : [gdtr]"m"(s_gdtr)
	    );
    }

    bool loadGdtr()
    {
	    s_gdtr.set(s_gdt);

	    asm volatile (
	    "lgdt %[gdtr]"
	    :
	    : [gdtr]"m"(s_gdtr)
	    );

	    constexpr auto
		    KernelCodeSelector = selector(EntryIndex::KernelCode),
		    KernelDataSelector = selector(EntryIndex::KernelData);

	    asm volatile (
	    "ljmp %[code], $.reload_cs\n"
	    "\n"
	    ".reload_cs:\n"
	    "mov %[data], %%ax\n"
	    "mov %%ax, %%ds\n"
	    "mov %%ax, %%es\n"
	    "mov %%ax, %%fs\n"
	    "mov %%ax, %%gs\n"
	    "mov %%ax, %%ss"
	    :
	    : [code]"i"(KernelCodeSelector), [data]"i"(KernelDataSelector)
	    );

	    xos::printf("Loaded gdt at 0x{x}\n", uintptr_t(s_gdtr.address));
	    return true;
    }

    const Gdtr &gdtr()
    {
	    return s_gdtr;
    }
}

