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
#include <xos/multiboot.h>

namespace vga {
    static constexpr uint16_t
	    CRTCAddressRegister = 0x3d4,
	    CRTCDataRegister = 0x3d5;

    static Config s_config = {};

    namespace CRTC {
	enum Enum : uint8_t {
		CursorLocationHigh = 0x0e,
		CursorLocationLow = 0x0f,
	};
    }

    static inline void writeRegister(CRTC::Enum address, uint8_t value)
    {
	    outb(CRTCAddressRegister, address);
	    outb(CRTCDataRegister, value);
    }

    bool initialize()
    {
	    if (const auto mbtInfo = mbt::frameBuffer(); mbtInfo) {
		    s_config = {
			    .width = uint16_t(mbtInfo->width),
			    .height = uint16_t(mbtInfo->height),
			    .total = uint16_t(mbtInfo->width * mbtInfo->height),
			    .tabLength = Default::TabLength,
			    .frameBuffer = reinterpret_cast<uint16_t *>(
				    uintptr_t(mbtInfo->address)
			    )
		    };
	    }

	    return true;
    }

    void renderFrameBuffer(const uint16_t *frameBuffer)
    {
	    xos::memcpy(s_config.frameBuffer, frameBuffer, s_config.total);
    }

    void setCursorPosition(uint16_t position)
    {
	    writeRegister(CRTC::CursorLocationHigh, (position >> 8u) & 0xffu);
	    writeRegister(CRTC::CursorLocationLow, position & 0xffu);
    }

    const Config &config()
    {
	    return s_config;
    }
}

