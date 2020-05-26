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

#ifndef _XOS_ARCH_I386_KERNEL_GDT_INCLUDE_XOS_GDT_H
#define _XOS_ARCH_I386_KERNEL_GDT_INCLUDE_XOS_GDT_H

#include <stdint.h>
#include <xos/packed.h>

namespace gdt {
    struct _Pckd GdtEntry {
	    union {
		    uint64_t entry;
		    struct _Pckd {
			    uint16_t limit0;
			    uint16_t base0;
			    uint8_t base1;
			    uint8_t access;
			    uint8_t limit1 : 4,
				    flags : 4;
			    uint8_t base2;
		    };
	    };
    };

    struct _Pckd Gdtr {
	    uint16_t size;
	    GdtEntry *address;
    };

    void storeGdtr();
    const Gdtr &gdtr();
}

#endif //_XOS_ARCH_I386_KERNEL_GDT_INCLUDE_XOS_GDT_H
