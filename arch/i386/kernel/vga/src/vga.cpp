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
    static constexpr uint16_t
	    CRTCAddressRegister = 0x3d4,
	    CRTCDataRegister = 0x3d5;

    namespace CRTC {
	enum Enum : uint8_t {
		CursorLocationHigh = 0x0e,
		CursorLocationLow = 0x0f,
	};
    }

    static inline uint16_t *vgaBuffer()
    {
	    return reinterpret_cast<uint16_t *>(BufferAddress);
    }

    static inline void writeRegister(CRTC::Enum address, uint8_t value)
    {
	    outb(CRTCAddressRegister, address);
	    outb(CRTCDataRegister, value);
    }

    void renderFrameBuffer(const uint16_t *frameBuffer)
    {
	    xos::memcpy(vgaBuffer(), frameBuffer, Total);
    }

    void setCursorPosition(uint16_t position)
    {
	    writeRegister(CRTC::CursorLocationHigh, (position >> 8u) & 0xffu);
	    writeRegister(CRTC::CursorLocationLow, position & 0xffu);
    }
}

