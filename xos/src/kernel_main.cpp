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
	tty::setColor(vga::ColorAttribute(vga::Color::LightMagenta,
					  vga::Color::LightBlue));
	tty::write(" xos v0.0.1 ");

	tty::setColor(vga::ColorAttribute(vga::Color::LightMagenta));
	tty::write(" greets your bitch ass.\n\n");

	tty::setColor(vga::ColorAttribute(vga::Color::LightBlue));
	tty::write("Copyright Milan Gallo 2020.");
}

_EXT_C

void kernel_main(void)
{
	tty::initialize();
	printWelcome();

	tty::setColor(vga::ColorAttribute(vga::Color::LightBrown));
	printf("\n\nTesting printf:\n");

	printf("\tHex \"%x\" \tString \"%s\"\n\tChar \'%c\'\n",
	       0xd34db33f,
	       "This is echoed as well.",
	       35
	);

	printf("printf worked until now:\t%a");
}

_EXT_C_END
