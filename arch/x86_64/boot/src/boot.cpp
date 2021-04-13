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
// Created by xgallom on 3/21/21.
//

#include <xos/stivale2.h>
#include <xos/extern-c.h>

static uint8_t stack[16384];

static auto smpTag = stv2::Header::SmpTag();
static auto framebufferMtrrTag = stv2::Header::FramebufferMtrrTag(&smpTag);
static auto framebufferTag = stv2::Header::FramebufferTag(&framebufferMtrrTag);

__attribute__((section(".stivale2hdr"), used))
stv2::Header::Header stivale_hdr = {
	.entry = 0,
	.stack = reinterpret_cast<uintptr_t>(stack) + sizeof(stack),
	.flags = 0,
	.tags = &framebufferTag,
};

_EXT_C
void _start(stv2::Structure::Structure *);
_EXT_C_END

void _start(stv2::Structure::Structure *structure)
{
	const auto *tag = structure->tags;

	while (tag && tag->id != stv2::Structure::FramebufferTag::TagId)
		tag = tag->next;

	if (tag) {
		const auto *const frameBufferTag =
			reinterpret_cast<const stv2::Structure::FramebufferTag *>(tag);

		if(frameBufferTag->address) {
			for (uint64_t n = 0; n < 64; n++)
				frameBufferTag->address[n] = 0xff;
		}
	}

	for (;;) {
		asm volatile("pause");
	}
}
