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
// Created by xgallom on 5/25/20.
//

#ifndef _XOS_ARCH_I386_KERNEL_MULTIBOOT_INCLUDE_XOS_MBT_H
#define _XOS_ARCH_I386_KERNEL_MULTIBOOT_INCLUDE_XOS_MBT_H

#include <stdint.h>

namespace mbt {
    namespace Multiboot {
	struct Header {
		uint32_t flags;
	};

	struct Memory {
		uint32_t lower, upper;
	};

	struct Boot {
		union {
			uint32_t device;
			struct {
				uint8_t partition[3], drive;
			};
		};
	};

	struct Command {
		const char *line;
	};

	struct Modules {
		uint32_t count, address;
	};

	struct Symbols {
		uint32_t number, size, address, sectionHeader;
	};

	struct MemoryMap {
		uint32_t length, address;
	};

	struct Drives {
		uint32_t length, address;
	};

	struct Config {
		uint32_t table;
	};

	struct BootLoader {
		const char *name;
	};

	struct ApmTable {
		uint32_t address;
	};

	struct Vbe {
		uint32_t controlInfo, modeInfo;
		uint16_t
			mode,
			interfaceSegment,
			interfaceOffset,
			interfaceLength;
	};

	struct FrameBuffer {
		uint64_t address;
		uint32_t pitch, width, height;
		uint8_t bpp, type, colorInfo[6];
	};

	struct Object {
		Header header;
		Memory memory;
		Boot boot;
		Command command;
		Modules modules;
		Symbols symbols;
		MemoryMap memoryMap;
		Drives drives;
		Config config;
		BootLoader bootLoader;
		ApmTable apmTable;
		Vbe vbe;
		FrameBuffer frameBuffer;
	};
    }

    bool initialize(uint32_t magic, uint32_t *multiboot);
    const Multiboot::Object &entry();
}

#endif //_XOS_ARCH_I386_KERNEL_MULTIBOOT_INCLUDE_XOS_MBT_H
