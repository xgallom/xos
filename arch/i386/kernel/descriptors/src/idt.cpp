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

#include <xos/descriptors/idt.h>
#include <xos/descriptors/gdt.h>
#include <xos/string.h>
#include <xos/interrupt-enable.h>

_EXT_C
extern const uintptr_t __isr_handler_table[];
_EXT_C_END

namespace idt {
    static IdtEntry s_idt[IdtEntryCount];

    static Idtr s_idtr;

    void storeIdtr()
    {
	    asm volatile(
	    "sidt %[idtr]"
	    :
	    : [idtr]"m"(s_idtr)
	    );
    }

    void loadIdtr()
    {
	    const auto *handler = __isr_handler_table;
	    for (auto &entry : s_idt)
		    entry = IdtEntry(
			    gdt::selector(gdt::EntryIndex::KernelCode),
			    *handler++,
			    IdtEntry::Type32InterruptGate,
			    IdtEntryFlags(0)
		    );

	    s_idtr.set(s_idt);

	    asm volatile(
	    "lidt %[idtr]"
	    :
	    : [idtr]"m"(s_idtr)
	    );

	    enableInterrupts();
    }

    const Idtr &idtr()
    {
	    return s_idtr;
    }
}
