//
// Created by xgallom on 5/11/20.
//

#ifndef XOS_ARCH_I386_KERNEL_VGA_INCLUDE_XOS_VGA_H
#define XOS_ARCH_I386_KERNEL_VGA_INCLUDE_XOS_VGA_H

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

	static constexpr uint16_t Width = 80, Height = 25, Total = Width * Height;
	static const uintptr_t BufferAddress = 0xb8000;

	inline uint16_t *Buffer() { return reinterpret_cast<uint16_t *>(BufferAddress); }

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
