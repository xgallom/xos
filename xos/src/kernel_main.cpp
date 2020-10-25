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
// Created by xgallom on 5/10/20.
//

#include "compiler_checks.h"

#include <xos/descriptors/write.h>
#include <xos/mbt/write.h>
#include <xos/extern-c.h>
#include <xos/tty.h>
#include <xos/vga.h>
#include <xos/drivers/pic8259.h>
#include <xos/drivers/ps2/keyboard.h>
#include <xos/interrupt-enable.h>
#include <xos/cpuid.h>
#include <xos/stdio.h>
#include <stdio.h>

_EXT_C

void kernel_main(uint32_t magic, uint32_t *multiboot);

_EXT_C_END

static constexpr char
	Name[] =
	"\nxos operating system v0.1.0\n",
	Copyright[] =
	"Copyright (C) 2020 Milan Gallo <gallo.milan.jr@gmail.com>\n"
	"\n",
	License[] =
	"This program is free software: you can redistribute it and/or modify\n"
	"it under the terms of the GNU General Public License as published by\n"
	"the Free Software Foundation, version 2.\n"
	"\n"
	"This program is distributed in the hope that it will be useful,\n"
	"but WITHOUT ANY WARRANTY; without even the implied warranty of\n"
	"MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the\n"
	"GNU General Public License for more details.\n"
	"\n"
	"You should have received a copy of the GNU General Public License\n"
	"along with this program.  If not, see <https:www.gnu.org/licenses/>.\n"
	"\n"
	"\n";

static bool printWelcome()
{
	tty::setColor(vga::ColorAttribute(vga::Color::BrightRed));
	printf(Name);

	tty::setColor(vga::ColorAttribute(vga::Color::BrightBlue));
	printf(Copyright);

	tty::setColor(vga::ColorAttribute());
	printf(License);

	return true;
}

struct MemoryMapEntry {
	size_t size;
	uint64_t baseAddress;
	uint64_t length;
	uint32_t type;

	static const MemoryMapEntry *Get(uintptr_t address)
	{
		return reinterpret_cast<const MemoryMapEntry *>(address);
	}

	[[nodiscard]] const MemoryMapEntry *next(uintptr_t end) const
	{
		const auto next = uintptr_t(this) + size + 4;
		return next < end ? Get(next) : nullptr;
	}
};

void kernel_main(uint32_t magic, uint32_t *multiboot)
{
	if (!(
		mbt::initialize(magic, multiboot) &&
		vga::initialize() &&
		tty::initialize() &&
		printWelcome() &&
		gdt::loadGdtr() &&
		idt::loadIdtr() &&
		pic8259::initialize() &&
		ps2::kbd::initialize() &&
		cpuid::initialize()
	))
		return;

	putchar('\n');

	const auto end = mbt::memoryMap()->address + mbt::memoryMap()->length;
	for (auto entry = MemoryMapEntry::Get(mbt::memoryMap()->address);
	     entry; entry = entry->next(end)) {
		getchar();
		xos::printf(
			"size: {}, baseAddress: {x}, length: {x}, type: {}\n",
			entry->size,
			uintptr_t(entry->baseAddress),
			size_t(entry->length),
			entry->type
		);
	}

	xos::printf("\n{}\n\n", &mbt::entry());

	for (int n = 0; (n & 0xff) != 'q';) {
		n = getchar();
		putchar(n);
	}
}
