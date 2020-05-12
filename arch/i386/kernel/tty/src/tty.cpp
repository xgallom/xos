//
// Created by xgallom on 5/11/20.
//

#include <xos/tty.h>
#include <xos/vga.h>
#include <xos/port.h>
#include <string.h>
#include <stdint.h>

namespace tty {
	namespace {
		uint16_t s_position;
		uint8_t s_color;

		inline void updateCursorPosition(uint16_t position)
		{
			const uint8_t low = position & 0xffu, high = (position >> 8u) & 0xffu;

			outb(0x3d4u, 0x0eu);
			outb(0x3d5u, high);

			outb(0x3d4u, 0x0fu);
			outb(0x3d5u, low);
		}

		inline void unsafe_write(const char *begin, const char *end)
		{
			const auto position = s_position;
			uint16_t *buffer = vga::Buffer() + position;
			const uint8_t color = s_color;

			s_position = position + (end - begin);

			do {
				*buffer++ = vga::Char(*begin++, color);
			} while(begin < end);
		}

		inline uint16_t unsafe_newLine()
		{
			return (s_position += vga::Width - (s_position % vga::Width));
		}
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
		uint16_t *buffer = vga::Buffer();

		do {
			*buffer++ = empty;
		} while(--count);

		setCursor(0);
	}

	void putchar(char c)
	{
		if(c == '\n')
			unsafe_newLine();
		else
			vga::Buffer()[s_position++] = vga::Char(c, s_color);

		updateCursorPosition(s_position);
	}

	void newLine()
	{
		updateCursorPosition(unsafe_newLine());
	}

	void write(const char *str, size_t length)
	{
		if(!length)
			return;

		do {
			bool addNewLine = false;
			size_t toWrite = 0;

			for(; toWrite < length; ++toWrite) {
				if(str[toWrite] == '\n') {
					addNewLine = true;
					break;
				}
			}

			if(toWrite) {
				const char *const end = str + toWrite;
				unsafe_write(str, end);

				str = end;
				length -= toWrite;
			}

			if(addNewLine) {
				unsafe_newLine();

				++str;
				--length;
			}
		} while(length);

		updateCursorPosition(s_position);
	}

	void write(const char *data)
	{
		write(data, strlen(data));
	}

	void setColor(uint8_t color)
	{
		s_color = color;
	}

	void setCursor(uint16_t position)
	{
		updateCursorPosition((s_position = position));
	}
}
