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

#include <xos/extern-c.h>
#include <xos/tty.h>
#include <xos/vga.h>
#include <xos/descriptors/write.h>
#include <xos/mbt/write.h>
#include <xos/stdio.h>
#include <xos/port.h>
#include <xos/pause.h>
#include <xos/cpuid.h>
#include <stdio.h>

_EXT_C

extern uint16_t xos_reg_dump[6];
void kernel_main(uint32_t magic, uint32_t *multiboot);

_EXT_C_END

static void printWelcome()
{
	tty::setColor(vga::ColorAttribute(vga::Color::BrightMagenta,
					  vga::Color::BrightBlue));
	tty::write(" xos v0.0.1 ");

	tty::setColor(vga::ColorAttribute(vga::Color::BrightMagenta));
	tty::write(" greets your bitch ass.\n\n");

	tty::setColor(vga::ColorAttribute(vga::Color::BrightBlue));
	tty::write("Copyright Milan Gallo 2020.\n");
}

// TODO: Move to arch/kernel/ps2
static uint8_t getchar()
{
	uint8_t
		read,
		firstRead = 0,
		oldRead = inb(0x60u);

	for (;;) {
		// Spin-loop
		pause();

		// Read PS-2 output register
		if ((read = inb(0x60u)) != oldRead) {
			// Wait for release
			if (firstRead)
				return firstRead;
			else
				firstRead = read;
		}

		oldRead = read;
	}
}

static void getReturn()
{
	//while (getchar() != 28) {}
}

void kernel_main(uint32_t magic, uint32_t *multiboot)
{
	tty::initialize();

	printWelcome();

	if (!mbt::initialize(magic, multiboot)) {
		xos::printf("Invalid multiboot: 0x{x} 0x{x}\n",
			    magic,
			    uintptr_t(multiboot)
		);
		return;
	}

	xos::printf("\nMultiboot: ");
	tty::setColor(vga::ColorAttribute());
	xos::printf("{}\n", &mbt::entry());

	getReturn();

	const uint16_t *regDump = xos_reg_dump;

	xos::printf("\ncs: 0x{x}\nds: 0x{x}\nes: 0x{x}\n"
		    "ss: 0x{x}\nfs: 0x{x}\ngs: 0x{x}\n",
		    regDump[0],
		    regDump[1],
		    regDump[2],
		    regDump[3],
		    regDump[4],
		    regDump[5]
	);

	getReturn();

	tty::setColor(vga::ColorAttribute(vga::Color::BrightGreen));
	xos::printf("\nGrub-created gdtr:\n");

	gdt::storeGdtr();
	const auto &gdtr = gdt::gdtr();

	tty::setColor(vga::ColorAttribute(vga::Color::BrightBlue));
	xos::printf("Size:\t\t {x}\nAddress:\t {x}\n",
		    gdtr.size,
		    uintptr_t(gdtr.address)
	);

	for (size_t n = 0; n < (gdtr.size + 1u) / 8u; ++n) {
		tty::setColor(vga::ColorAttribute(vga::Color::BrightBlue));
		xos::printf("{}: ", n);
		tty::setColor(vga::ColorAttribute());
		xos::printf("{}\n", gdtr.address[n]);
		getReturn();
	}

	tty::setColor(vga::ColorAttribute(vga::Color::BrightGreen));
	xos::printf("\nOur gdtr:\n");

	gdt::loadGdtr();

	tty::setColor(vga::ColorAttribute(vga::Color::BrightBlue));
	xos::printf("Size:\t\t {x}\nAddress:\t {x}\n",
		    gdtr.size,
		    uintptr_t(gdtr.address)
	);

	for (size_t n = 0; n < (gdtr.size + 1u) / 8u; ++n) {
		tty::setColor(vga::ColorAttribute(vga::Color::BrightBlue));
		xos::printf("{}: ", n);
		tty::setColor(vga::ColorAttribute());
		xos::printf("{}\n", gdtr.address[n]);
		getReturn();
	}


	const auto &idtr = idt::idtr();

	tty::setColor(vga::ColorAttribute(vga::Color::BrightGreen));
	xos::printf("\nOur idtr:\n");

	idt::loadIdtr();
	for (;;) { pause(); }

	tty::setColor(vga::ColorAttribute(vga::Color::BrightBlue));
	xos::printf("Size:\t\t {x}\nAddress:\t {x}\n",
		    idtr.size,
		    uintptr_t(idtr.address)
	);

	for (size_t n = 0; n < (idtr.size + 1u) / 8u; ++n) {
		tty::setColor(vga::ColorAttribute(vga::Color::BrightBlue));
		xos::printf("{}: ", n);
		tty::setColor(vga::ColorAttribute());
		xos::printf("{}\n", idtr.address[n]);
		getReturn();
	}


	tty::setColor(vga::ColorAttribute(vga::Color::BrightGreen));
	xos::printf("\nTesting printf:\n");
	tty::setColor(vga::ColorAttribute());

	int processed = xos::printf(
		"\t Hex\t\t 0x{x}\n"
		"\t String\t\t \"{}\"\n"
		"\t Char\t\t \'{}\'\n"
		"\t Int\t\t {}\n"
		"\t 23\t\t {}\n"
		"\t Invalid\t {a}",
		0xdeadb33f,
		"This is echoed as well.",
		'#',
		103000,
		23,
		'x'
	);

	tty::setColor(vga::ColorAttribute(vga::Color::BrightGreen));
	xos::printf(
		"\nprintf: {} entries processed (should be -1)\n",
		processed
	);

	getReturn();


	bool hasCpuid = cpuid::initialize();

	xos::printf(
		"\nInitializing Cpuid: {}",
		hasCpuid ?
		"available\nResults:\n" :
		"not available\n"
	);

	if (hasCpuid) {
		for (uint8_t request = 0;
		     request < cpuid::RequestType::Count;
		     ++request) {
			const auto requestType =
				static_cast<cpuid::RequestType::Enum>(
					request
				);

			const auto *const result = cpuid::entry(requestType);

			tty::setColor(
				vga::ColorAttribute(vga::Color::BrightBlue)
			);
			xos::printf(
				"{x} {}:\n",
				cpuid::entryRequest(requestType),
				cpuid::entryName(requestType)
			);

			tty::setColor(vga::ColorAttribute());
			xos::printf(
				"\t  {x} {x} {x} {x}\t\"",
				result[0],
				result[1],
				result[2],
				result[3]
			);
			xos::write(
				reinterpret_cast<const char *>(result),
				cpuid::ResultByteLength
			);
			xos::printf("\"\n");
		}

		getReturn();


		tty::setColor(vga::ColorAttribute(vga::Color::BrightGreen));
		xos::printf("\nCpuid Features:\n");
		for (uint8_t feature = 0;
		     feature < 32;
		     ++feature) {
			if (!(feature & 0x07u))
				getReturn();

			const auto featureType =
				static_cast<cpuid::FeatureType::Enum>(
					feature
				);

			const auto result = cpuid::feature(featureType);

			tty::setColor(
				vga::ColorAttribute(vga::Color::BrightBlue)
			);
			xos::printf("{x} {}:\n",
				    uint8_t(featureType),
				    cpuid::featureName(featureType)

			);
			tty::setColor(vga::ColorAttribute());
			xos::printf("\t  {}\n", result);
		}

		for (uint8_t feature = 0;
		     feature < 32;
		     ++feature) {
			if (!(feature & 0x07u))
				getReturn();

			const auto featureType =
				static_cast<cpuid::FeatureType::Enum>(
					cpuid::FeatureType::EdxMask | feature
				);

			const auto result = cpuid::feature(featureType);

			tty::setColor(
				vga::ColorAttribute(vga::Color::BrightBlue)
			);
			xos::printf("{x} {}:\n",
				    uint8_t(featureType),
				    cpuid::featureName(featureType)

			);
			tty::setColor(vga::ColorAttribute());
			xos::printf("\t  {}\n", result);
		}
	}


	tty::setColor(vga::ColorAttribute(vga::Color::BrightGreen));
	xos::printf("\nTesting PS-2 interface:\n");

	tty::setColor(vga::ColorAttribute(vga::Color::Gray));
	uint8_t character;
	while ((character = getchar()) != 16)
		xos::printf("{} ", character);

	tty::clear();
	tty::setColor(vga::ColorAttribute(vga::Color::BrightRed));
	xos::printf("q pressed, returning to hlt\n");
}
