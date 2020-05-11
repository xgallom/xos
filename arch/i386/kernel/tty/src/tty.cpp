//
// Created by xgallom on 5/11/20.
//

#include <xos/tty.h>
#include <xos/vga.h>
#include <string.h>
#include <stdint.h>

namespace vga {
	namespace {
		static constexpr size_t Width = 80, Height = 25, Total = Width * Height;
		static uint16_t *const Buffer = reinterpret_cast<uint16_t *>(0xb8000);
	}
}

namespace tty {
	namespace {
		size_t s_position;
		uint8_t s_color;
	}

	void initialize()
	{
		s_color = vga::ColorCombination(vga::Color::LightGray, vga::Color::Black);

		clear();
	}

	void clear()
	{
		const auto empty = vga::Char(' ', s_color);

		size_t count = vga::Total;
		uint16_t *buffer = vga::Buffer;

		do {
			*buffer++ = empty;
		} while(--count);

		s_position = 0;
	}

	void putchar(char c)
	{
		write(&c, 1);
	}

	void write(const char *str, size_t length)
	{
		if(!length)
			return;

		uint16_t *buffer = vga::Buffer + s_position;
		const auto color = s_color;

		s_position += length;

		do {
			*buffer++ = vga::Char(*str++, color);
		} while(--length);
	}

	void write(const char *data)
	{
		write(data, strlen(data));
	}

	void setColor(uint8_t color)
	{
		s_color = color;
	}
}
