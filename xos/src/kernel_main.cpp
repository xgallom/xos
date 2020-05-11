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
	tty::setColor(vga::ColorCombination(vga::Color::LightMagenta, vga::Color::LightBlue));
	tty::write(" xos");

	tty::setColor(vga::ColorCombination(vga::Color::LightMagenta, vga::Color::LightBlue));
	tty::write(" v0.0.1 ");

	tty::setColor(vga::ColorCombination(vga::Color::LightBlue, vga::Color::Black));
	tty::write(" greets your bitch ass.");

	tty::setColor(vga::ColorCombination(vga::Color::DarkGray, vga::Color::Black));
	tty::write(" Copyright Milan Gallo 2020.     ");
}

_EXT_C

void kernel_main(void)
{
	tty::initialize();
	printWelcome();
}

_EXT_C_END
