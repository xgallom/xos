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

//#include "compiler_checks.h"

/*
#include <xos/descriptors/write.h>
#include <xos/multiboot/write.h>
#include <xos/extern-c.h>
#include <xos/tty.h>
#include <xos/vga.h>
#include <xos/drivers/pic8259.h>
#include <xos/drivers/ps2/keyboard.h>
#include <xos/memory/mapping/physical/write.h>
#include <xos/cpuid.h>
#include <xos/stdio.h>
#include <stdio.h>
*/

#include <xos/attributes.h>
#include <cstdint>
#include <cstddef>
#include "stivale2.h"


// We need to tell the stivale bootloader where we want our stack to be.
// We are going to allocate our stack as an array in .bss.
static uint8_t stack[8192];

using namespace Stivale_v2;

// stivale2 uses a linked list of tags for both communicating TO the
// bootloader, or receiving info FROM it. More information about these tags
// is found in the stivale2 specification.

// stivale2 offers a runtime terminal service which can be ditched at any
// time, but it provides an easy way to print out to graphical terminal,
// especially during early boot.
// Read the notes about the requirements for using this feature below this
// code block.
static struct Stivale::Header::Terminal terminal_hdr_tag = {
        .tag = {
                .id = Stivale::Header::TagId::Terminal,
                .next = nullptr
        },
        .flags = 0
};

// We are now going to define a framebuffer header tag.
// This tag tells the bootloader that we want a graphical framebuffer instead
// of a CGA-compatible text mode. Omitting this tag will make the bootloader
// default to text mode, if available.
static struct Stivale::Header::Framebuffer framebuffer_hdr_tag = {
        .tag = {
                .id = Stivale::Header::TagId::Framebuffer,
                .next = &terminal_hdr_tag.tag,
        },
        .framebufferWidth  = 0,
        .framebufferHeight = 0,
        .framebufferBitsPerPixel = 0
};

// The stivale2 specification says we need to define a "header structure".
// This structure needs to reside in the .stivale2hdr ELF section in order
// for the bootloader to find it. We use this __attribute__ directive to
// tell the compiler to put the following structure in said section.
__attribute__((section(".stivale2hdr"), used))
static struct Stivale::Header::Struct stivale_hdr = {
        .entryPoint = 0,
        .stack = uintptr_t(stack) + sizeof(stack),
        // Bit 1, if set, causes the bootloader to return to us pointers in the
        // higher half, which we likely want since this is a higher half kernel.
        // Bit 2, if set, tells the bootloader to enable protected memory ranges,
        // that is, to respect the ELF PHDR mandated permissions for the executable's
        // segments.
        // Bit 3, if set, enables fully virtual kernel mappings, which we want as
        // they allow the bootloader to pick whichever *physical* memory address is
        // available to load the kernel, rather than relying on us telling it where
        // to load it.
        // Bit 4 disables a deprecated feature and should always be set.
        .flags = (0 << 1) | (1 << 2) | (1 << 3) | (1 << 4),
        .tags = &framebuffer_hdr_tag.tag,
};

// We will now write a helper function which will allow us to scan for tags
// that we want FROM the bootloader (structure tags).
const Stivale::Struct::Tag *stivaleGetTag(const Stivale::Struct::Struct *stivale, uint64_t id) {
        for (auto walk = stivale->tags; walk; walk = walk->next) {
                if (walk->id == id)
                        return walk;
        }

        return nullptr;
}

_EXT_C
void _start(const Stivale::Struct::Struct *stivale);
_EXT_C_END

size_t strlen(const char *data) {
        if (!*data)
                return 0;

        size_t length = 0;
        do {
                ++length;
        } while (*data++);

        return length;
}

void _start(const Stivale::Struct::Struct *stivale) {
        auto terminal = reinterpret_cast<const Stivale::Struct::Terminal *>(
                stivaleGetTag(stivale, Stivale::Struct::TagId::Terminal)
        );

        if (!terminal) {
                for (;;)
                        asm ("hlt");
        }

        auto write = terminal->write;

        // We should now be able to call the above function pointer to print out
        // a simple "Hello World" to screen.
        const auto message =
                "\x1b[31mxos operating system v0.1.0\n"
                "Copyright (C) 2020 Milan Gallo <gallo.milan.jr@gmail.com>\n";
        write(message, strlen(message));

        // We're done, just hang...
        for (;;)
                asm ("hlt");
}
