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

    static constexpr uint16_t
	    Width = 80,
	    Height = 25,
	    Total = Width * Height,
	    TabLength = 8;

    inline constexpr
    uint8_t ColorAttribute(Color::Enum fg = Color::Gray,
			   Color::Enum bg = Color::Black)
    {
	    return fg | (bg << Color::Offset);
    }

    void renderFrameBuffer(const uint16_t *frameBuffer);
    void setCursorPosition(uint16_t position);
}

#endif //_XOS_ARCH_I386_KERNEL_VGA_INCLUDE_XOS_VGA_H
