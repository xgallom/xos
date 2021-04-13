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

//#include <xos/extern-c.h>
//#include <xos/tty.h>
//#include <xos/vga.h>
//#include <xos/drivers/pic8259.h>
//#include <xos/drivers/ps2/keyboard.h>
//#include <xos/memory/mapping/physical/write.h>
//#include <xos/cpuid.h>
//#include <xos/stdio.h>
//#include <stdio.h>

/*
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
		mem::map::phy::initialize() &&
		cpuid::initialize()
	))
		return;

	putchar('\n');

	for(const auto entry : mem::map::phy::get())
		xos::printf("{}\n", entry);

	xos::printf("\n{}\n\n", &mbt::entry());

	for (int n = getchar(); ; n = getchar())
		putchar(n);
}
*/
