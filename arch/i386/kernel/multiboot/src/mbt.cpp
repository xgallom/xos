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

#include <xos/mbt.h>
#include <xos/stdio.h>

namespace mbt {
    static constexpr uint32_t MagicMask = 0x2bad0000;

    static Multiboot::Object s_multiboot;


    bool initialize(uint32_t magic, uint32_t *multiboot)
    {
	    if ((magic ^ MagicMask) >> 16u) {
		    xos::printf("Invalid multiboot: 0x{x} 0x{x}\n",
				magic,
				uintptr_t(multiboot)
		    );
		    return false;
	    }

	    s_multiboot = *reinterpret_cast<Multiboot::Object *>(multiboot);

	    xos::printf(
		    "Initialized multiboot 0x{x} at 0x{x}\n",
		    magic,
		    uintptr_t(multiboot)
	    );
	    return true;
    }

    const Multiboot::Object &entry()
    {
	    return s_multiboot;
    }
}
