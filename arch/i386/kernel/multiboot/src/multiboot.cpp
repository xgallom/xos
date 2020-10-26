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

#include <xos/multiboot.h>
#include <xos/stdio.h>

namespace mbt {
    static constexpr uint32_t MagicMask = 0x2bad0000;

    static Multiboot::Object s_multiboot;


    bool initialize(uint32_t magic, uint32_t *multiboot)
    {
	    if ((magic ^ MagicMask) >> 16u)
		    return false;

	    s_multiboot = *reinterpret_cast<Multiboot::Object *>(multiboot);

	    return true;
    }

    const Multiboot::Object &entry()
    {
	    return s_multiboot;
    }

    const Multiboot::MemoryMap *memoryMap()
    {
	    return s_multiboot.header.flags & (0x1u << 6u) ?
		   &s_multiboot.memoryMap :
		   nullptr;
    }

    const Multiboot::FrameBuffer *frameBuffer()
    {
	    return s_multiboot.header.flags & (0x1u << 12u) ?
		   &s_multiboot.frameBuffer :
		   nullptr;
    }
}
