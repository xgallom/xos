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

static bool printWelcome()
{
	tty::setColor(vga::ColorAttribute(vga::Color::BrightMagenta,
					  vga::Color::Blue));
	tty::write(" xos v0.0.1 ");

	tty::setColor(vga::ColorAttribute(vga::Color::BrightMagenta));
	tty::write(" greets your bitch ass.\n");

	tty::setColor(vga::ColorAttribute(vga::Color::BrightBlue));
	tty::write("Copyright Milan Gallo 2020.\n\n");

	tty::setColor(vga::ColorAttribute());

	return true;
}

static void getReturn()
{
	while (getchar() != '\n') {}
}

void kernel_main(uint32_t magic, uint32_t *multiboot)
{
	if (!(
		tty::initialize() &&
		printWelcome() &&
		mbt::initialize(magic, multiboot) &&
		gdt::loadGdtr() &&
		idt::loadIdtr() &&
		pic8259::initialize() &&
		ps2::kbd::initialize() &&
		cpuid::initialize()
	))
		return;

	enableInterrupts();
	getReturn();

	for (auto n = 0; n < 256; ++n)
		printf("%d:%c ", n, n);

	for (auto n = 0; (n & 0xff) != 'q'; n = getchar())
		putchar(n);

/*
	const char *tmp = nullptr;
	xos::write("", &mbt::entry(), tmp, 0);
*/
}
