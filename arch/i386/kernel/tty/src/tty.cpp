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
#include <xos/drivers/ps2/keyboard.h>
#include <stdint.h>
#include <xos/always-inline.h>

namespace tty {
    static constexpr size_t FrameBufferSize = 1u << (sizeof(uint16_t) * 8);

    struct State {
	    uint16_t
		    frameStart = 0,
		    position = 0,
		    attributeMask = 0;
    } static s_state = {};

    static uint16_t s_frameBuffer[FrameBufferSize + vga::Default::Total] = {};


    [[nodiscard]] static _Inln
    uint16_t *frame() { return s_frameBuffer + s_state.frameStart; }

    [[nodiscard]] static _Inln
    uint16_t &frame(int offset) { return frame()[offset]; }

    [[nodiscard]] static _Inln
    uint16_t &frameStart() { return s_state.frameStart; }

    [[nodiscard]] static _Inln
    uint16_t &position() { return s_state.position; }

    [[nodiscard]] static _Inln
    uint16_t &attributeMask() { return s_state.attributeMask; }


    static _Inln uint16_t align(uint16_t value, uint16_t alignment)
    {
	    return value % alignment;
    }

    static _Inln uint16_t alignMissing(uint16_t value, uint16_t alignment)
    {
	    return alignment - align(value, alignment);
    }

    static _Inln void alignAdd(uint16_t &value, uint16_t alignment)
    {
	    value += alignMissing(value, alignment);
    }

    static _Inln uint16_t index(uint16_t x, uint16_t y)
    {
	    return y * vga::config().width + x;
    }

    static void updateOverlay()
    {
	    const uint16_t
		    col = position() % vga::config().width,
		    row = (frameStart() + position()) / vga::config().width,
		    attr = vga::ColorAttribute(vga::Color::White,
					       vga::Color::Red) << 8u,
		    overlay[] = {
		    attr | ' ',
		    uint16_t(attr | ('0' + col / 10)),
		    uint16_t(attr | ('0' + col % 10)),
		    attr | ':',
		    uint16_t(attr | ('0' + row / 10)),
		    uint16_t(attr | ('0' + row % 10)),
		    attr | ' '};

	    constexpr auto size = sizeof(overlay) / sizeof(uint16_t);

	    xos::memcpy(
		    vga::config().frameBuffer + vga::config().total - size,
		    overlay,
		    size);
    }

    static void renderFrameBuffer()
    {
	    vga::renderFrameBuffer(frame());
	    updateOverlay();
    }

    static void setCursorPosition()
    {
	    updateOverlay();
	    vga::setCursorPosition(position());
    }

    static void setCursorPosition(uint16_t value)
    {
	    position() = value;
	    setCursorPosition();
    }

    static void internal_putchar(int c)
    {
	    switch (uint8_t(c)) {
	    case '\n':
		    alignAdd(position(), vga::config().width);
		    break;

	    case '\t':
		    alignAdd(position(), vga::config().tabLength);
		    break;

	    case '\b':
		    frame(--position()) = attributeMask();
		    break;

	    default:
		    frame(position()++) = attributeMask() | uint8_t(c);
		    break;
	    }

	    if (position() >= vga::config().total) {
		    frameStart() += vga::config().width;
		    position() -= vga::config().width;

		    if (uint16_t(frameStart() + position()) < frameStart())
			    frameStart() = 0;
	    }
    }

    bool initialize()
    {
	    setColor(vga::ColorAttribute());
	    clear();

	    return true;
    }

    void clear()
    {
	    frameStart() = 0;
	    xos::memset(frame(), attributeMask(), FrameBufferSize);

	    renderFrameBuffer();
	    setCursorPosition(0);
    }

    int getchar()
    {
	    using ps2::kbd::State;

	    for (;;) {
		    const auto key = ps2::kbd::getchar();
		    bool update = false;

		    switch (key) {
		    case '7' | State::CharMask<State::E0>():
			    position() -= align(position(),
						vga::config().width);
			    break;
		    case '1' | State::CharMask<State::E0>():
			    position() += alignMissing(position(),
						       vga::config().width) - 1;
			    break;

		    case '9' | State::CharMask<State::E0>():
			    frameStart() -= vga::config().total;
			    update = true;
			    break;
		    case '3' | State::CharMask<State::E0>():
			    frameStart() += vga::config().total;
			    update = true;
			    break;

		    case '8' | State::CharMask<State::E0>():
			    if (position() < vga::config().width) {
				    frameStart() -= vga::config().width;
				    update = true;
			    }
			    else
				    position() -= vga::config().width;
			    break;
		    case '2' | State::CharMask<State::E0>():
			    if (position() >= vga::config().total -
					      vga::config().width) {
				    frameStart() += vga::config().width;
				    update = true;
			    }
			    else
				    position() += vga::config().width;
			    break;

		    case '6' | State::CharMask<State::E0>():
			    if (++position() >= vga::config().total) {
				    frameStart() += vga::config().width;
				    position() -= vga::config().width;
				    update = true;
			    }
			    break;
		    case '4' | State::CharMask<State::E0>():
			    if (!position()--) {
				    frameStart() -= vga::config().width;
				    position() += vga::config().width;
				    update = true;
			    }
			    break;
		    case '.' | State::CharMask<State::E0>():
			    frame(position()++) = attributeMask();
			    update = true;
		            break;

		    default:
			    return key;
		    }

		    if (uint16_t(frameStart() +
				 vga::config().total +
				 vga::config().width) < frameStart()
			    ) {
			    frameStart() = position() = 0;
			    update = true;
		    }

		    if (update)
			    renderFrameBuffer();
		    setCursorPosition();
	    }
    }

    void putchar(int c)
    {
	    internal_putchar(c);

	    renderFrameBuffer();
	    setCursorPosition();
    }

    void write(const char *data, size_t size)
    {
	    if (!size)
		    return;

	    do
		    internal_putchar(*data++);
	    while (--size);

	    renderFrameBuffer();
	    setCursorPosition();
    }

    void write(const char *data)
    {
	    char character = *data;

	    if (!character)
		    return;

	    do
		    internal_putchar(character);
	    while ((character = *++data));

	    renderFrameBuffer();
	    setCursorPosition();
    }

    void setColor(uint8_t color)
    {
	    attributeMask() = uint16_t(color) << 8u;
    }

    void setCursor(uint16_t newPosition)
    {
	    setCursorPosition(newPosition);
    }

    void setCursor(uint8_t x, uint8_t y)
    {
	    setCursorPosition(index(x, y));
    }
}
