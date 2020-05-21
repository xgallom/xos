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

#include <xos/ext_c.h>
#include <xos/tty.h>
#include <xos/vga.h>
#include <stdio.h>

static void printWelcome()
{
	tty::setColor(vga::ColorAttribute(vga::Color::BrightMagenta,
					  vga::Color::BrightBlue));
	tty::write(" xos v0.0.1 ");

	tty::setColor(vga::ColorAttribute(vga::Color::BrightMagenta));
	tty::write(" greets your bitch ass.\n\n");

	tty::setColor(vga::ColorAttribute(vga::Color::BrightBlue));
	tty::write("Copyright Milan Gallo 2020.");
}

_EXT_C

void kernel_main(void)
{
	tty::initialize();
	printWelcome();

	tty::setColor(vga::ColorAttribute(vga::Color::BrightGreen));
	printf("\n\nTesting printf:\n");
	tty::setColor(vga::ColorAttribute(vga::Color::Gray));

	int processed = printf(
		"\t Hex\t\t \"%x\"\n"
		"\t String\t\t \"%s\"\n"
		"\t Char\t\t \'%c\'\n"
		"\t Int\t\t %d\n"
		"\t 23\t\t %d\n"
		"\t Invalid\t %a\n",
		0xdeadb33f,
		"This is echoed as well.",
		'#',
		103000,
		23,
		'x'
	);

	tty::setColor(vga::ColorAttribute(vga::Color::BrightGreen));
	printf(
		"printf: %d entries processed (should be 5)\n",
		processed
	);
}

_EXT_C_END
