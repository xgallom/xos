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

#ifndef _XOS_ARCH_I386_KERNEL_VGA_INCLUDE_XOS_VGA_H
#define _XOS_ARCH_I386_KERNEL_VGA_INCLUDE_XOS_VGA_H

#include <stdint.h>
#include <stddef.h>

namespace vga {
    namespace Color {
	enum Enum : uint8_t {
		Black = 0,
		Blue,
		Green,
		Cyan,
		Red,
		Magenta,
		Brown,
		Gray,

		DarkGray,
		BrightBlue,
		BrightGreen,
		BrightCyan,
		BrightRed,
		BrightMagenta,
		Yellow,
		White
	};

	static constexpr uint8_t
		Offset = 4u,
		CombinationOffset = Offset << 1u;
    }

    namespace Default {
	static constexpr uint16_t
		Width = 80,
		Height = 25,
		Total = Width * Height,
		TabLength = 8;
	static constexpr uintptr_t FrameBuffer = 0xb8000;
    }

    struct Config {
	    uint16_t
		    width = Default::Width,
		    height = Default::Height,
		    total = Default::Total,
		    tabLength = Default::TabLength;
	    uint16_t
		    *frameBuffer =
		    reinterpret_cast<uint16_t *>(
			    Default::FrameBuffer
		    );
    };

    inline constexpr
    uint8_t ColorAttribute(Color::Enum fg = Color::Gray,
			   Color::Enum bg = Color::Black)
    {
	    return fg | (bg << Color::Offset);
    }

    bool initialize();

    void renderFrameBuffer(const uint16_t *frameBuffer);
    void setCursorPosition(uint16_t position);

    const Config &config();
}

#endif //_XOS_ARCH_I386_KERNEL_VGA_INCLUDE_XOS_VGA_H
