//
// Created by xgallom on 5/11/20.
//

#ifndef XOS_ARCH_I386_KERNEL_VGA_INCLUDE_XOS_VGA_H
#define XOS_ARCH_I386_KERNEL_VGA_INCLUDE_XOS_VGA_H

#include <stdint.h>

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
			LightGray,

			DarkGray,
			LightBlue,
			LightGreen,
			LightCyan,
			LightRed,
			LightMagenta,
			LightBrown,
			White
		};

		static constexpr uint8_t
				Offset = 4u,
				CombinationOffset = Offset << 1u;
	}

	inline constexpr uint8_t ColorCombination(Color::Enum fg, Color::Enum bg)
	{
		return fg | (bg << Color::Offset);
	}

	inline constexpr uint16_t Char(unsigned char character, uint8_t color)
	{
		return uint16_t(character) | (uint16_t(color) << Color::CombinationOffset);
	}
}

#endif //XOS_ARCH_I386_KERNEL_VGA_INCLUDE_XOS_VGA_H
