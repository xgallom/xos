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

#include <xos/tty.h>
#include <xos/vga.h>
#include <xos/string.h>
#include <stdint.h>

namespace tty {
    // Frame buffer is uint16_t, because vga maps attribute plane
    //  interwoven with character plane.
    // Even byte is attribute, and odd byte is character of specific
    //  screen position, so screen characters can map to words that
    //  have attribute in high byte and character in low byte.

    // Frame buffer has one more empty row.
    // On having full buffer, whole screen is copied one row up,
    //  along with the spare row, which fills the last row with
    //  empty characters
    static uint16_t s_frameBuffer[vga::Total + vga::Width] = {};

    static uint16_t s_position = 0;

    // Attributes are stored in the higher byte so they don't have
    //  to be shifted on every write, only on every setColor.
    static uint16_t s_attributeMask = 0;

    static void internal_putchar(char c)
    {
	    switch (c) {
	    case '\n':
		    // Add remaining offset to end of current row to position:
		    s_position += vga::Width - s_position % vga::Width;
		    break;

	    case '\t':
		    s_position += vga::TabLength - s_position % vga::TabLength;
		    break;

	    default:
		    s_frameBuffer[s_position++] = s_attributeMask | uint16_t(c);
	    }

	    if (s_position == vga::Total) {
		    // On overflow we move over next row of the
		    //  frame buffer along with the spare empty row.
		    s_position -= vga::Width;

		    xos::memmove(
			    s_frameBuffer,
			    s_frameBuffer + vga::Width,
			    vga::Total
		    );
	    }
    }

    void initialize()
    {
	    setColor(vga::ColorAttribute());
	    clear();

	    vga::renderFrameBuffer(s_frameBuffer);
    }

    void clear()
    {
	    // Clear whole frame buffer, along with the extended row
	    for (auto &character : s_frameBuffer)
		    character = s_attributeMask;

	    vga::setCursorPosition((s_position = 0));
	    vga::renderFrameBuffer(s_frameBuffer);
    }

    void putchar(char c)
    {
	    internal_putchar(c);

	    vga::setCursorPosition(s_position);
	    vga::renderFrameBuffer(s_frameBuffer);
    }

    void write(const char *data, size_t size)
    {
	    if (!size)
		    return;

	    do
		    internal_putchar(*data++);
	    while (--size);

	    vga::setCursorPosition(s_position);
	    vga::renderFrameBuffer(s_frameBuffer);
    }

    void write(const char *data)
    {
	    char character = *data;

	    if (!character)
		    return;

	    do
		    internal_putchar(character);
	    while ((character = *++data));

	    vga::setCursorPosition(s_position);
	    vga::renderFrameBuffer(s_frameBuffer);
    }

    void setColor(uint8_t color)
    {
	    s_attributeMask = uint16_t(color) << 8u;
    }

    void setCursor(uint16_t position)
    {
	    vga::setCursorPosition((s_position = position));
    }

    void setCursor(uint8_t x, uint8_t y)
    {
	    vga::setCursorPosition((s_position = y * vga::Width + x));
    }
}
