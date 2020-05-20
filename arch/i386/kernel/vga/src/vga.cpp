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
// Created by xgallom on 5/11/20.
//

#include <xos/vga.h>
#include <xos/port.h>
#include <xos/string.h>

namespace vga {
    static constexpr uintptr_t BufferAddress = 0xb8000;

    static inline uint16_t *vgaBuffer()
    {
	    return reinterpret_cast<uint16_t *>(BufferAddress);
    }

    void renderFrameBuffer(const uint16_t *frameBuffer)
    {
	    xos::memcpy(vgaBuffer(), frameBuffer, Total);
    }

    void setCursorPosition(uint16_t position)
    {
	    const uint8_t
		    low = position & 0xffu,
		    high = (position >> 8u) & 0xffu;

	    outb(0x3d4u, 0x0eu);
	    outb(0x3d5u, high);

	    outb(0x3d4u, 0x0fu);
	    outb(0x3d5u, low);
    }
}

