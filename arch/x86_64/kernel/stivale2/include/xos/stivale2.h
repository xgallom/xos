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

#ifndef _ARCH_X86_64_KERNEL_STIVALE2_INCLUDE_XOS_STIVALE2_H
#define _ARCH_X86_64_KERNEL_STIVALE2_INCLUDE_XOS_STIVALE2_H

#include <stdint.h>
#include <xos/packed.h>

namespace stv2 {
    namespace Header {
	struct _Pckd Tag {
		uint64_t id;
		Tag *next;

		constexpr Tag(uint64_t id = 0, Tag *next = nullptr) :
			id{id},
			next{next} {}
	};

	struct _Pckd FramebufferTag : Tag {
		uint16_t
			width,
			height,
			bitsPerPixel;

		static constexpr uint64_t TagId = 0x3ecc1bc43d0f7971;

		constexpr FramebufferTag(
			Tag *next = nullptr,
			uint16_t width = 0,
			uint16_t height = 0,
			uint16_t bitsPerPixel = 0
		) :
			Tag(TagId, next),
			width{width},
			height{height},
			bitsPerPixel{bitsPerPixel} {}
	};

	struct _Pckd FramebufferMtrrTag : Tag {
		static constexpr uint64_t TagId = 0x4c7bb07731282e00;

		constexpr FramebufferMtrrTag(
			Tag *next = nullptr
		) :
			Tag(TagId, next) {}
	};

	struct _Pckd Paging5LevelsTag : Tag {
		static constexpr uint64_t TagId = 0x932f477032007e8f;

		constexpr Paging5LevelsTag(
			Tag *next = nullptr
		) :
			Tag(TagId, next) {}
	};

	struct _Pckd SmpTag : Tag {
		uint64_t flags;               // Flags:
		//   bit 0: 0 = use xAPIC, 1 = use x2APIC (if available)
		// All other bits are undefined and must be 0.

		static constexpr uint64_t TagId = 0x1ab015085f3273df;

		constexpr SmpTag(
			Tag *next = nullptr,
			uint64_t flags = 0
		) :
			Tag(TagId, next),
			flags{flags} {}
	};

	struct _Pckd Header {
		uintptr_t
			entry,
			stack;
		uint64_t flags;         // Bit 0: Formerly used to indicate whether to enable
		//        KASLR, this flag is now reserved as KASLR
		//        is enabled in the bootloader configuration
		//        instead. Presently reserved and unused.
		// All other bits are undefined and must be 0.

		Tag *tags;
	};
    }

    namespace Structure {
	struct _Pckd Tag {
		uint64_t id;
		Tag *next;
	};

	struct _Pckd CommandLineTag : Tag {
		uint64_t commandLine;

		static constexpr uint64_t TagId = 0xe5e76a1b4597a781;
	};

	struct _Pckd MemoryMapTag : Tag {
		// TODO: Replace with MemoryMap
		uint64_t entries;

		static constexpr uint64_t TagId = 0x2187f79e8612de07;
	};

	struct _Pckd FramebufferTag : Tag {
		uint8_t *address;
		uint16_t
			width,
			height,
			pitch,
			bitsPerPixel;
		uint8_t
			memoryModel,
			redMaskSize,
			redMaskShift,
			greenMaskSize,
			greenMaskShift,
			blueMaskSize,
			blueMaskShift;

		static constexpr uint64_t TagId = 0x506461d2950408fa;
	};

	struct _Pckd EdidTag : Tag {
		uint64_t size;
		uint8_t information[];

		static constexpr uint64_t TagId = 0x968609d7af96b845;
	};

	struct _Pckd FramebufferMtrrTag : Tag {
		static constexpr uint64_t TagId = 0x6bc1a78ebe871172;
	};

	struct _Pckd ModulesTag : Tag {
		uint64_t size;
		// TODO: Replace with Module
		uint8_t modules[];

		static constexpr uint64_t TagId = 0x4b6fe466aade04ce;
	};

	struct _Pckd RsdpTag : Tag {
		uint64_t rsdp;

		static constexpr uint64_t TagId = 0x9e1786930a375e78;
	};

	struct _Pckd EpochTag : Tag {
		uint64_t epoch;

		static constexpr uint64_t TagId = 0x566a7bed888e1407;
	};

	struct _Pckd FirmwareTag : Tag {
		uint64_t flags;

		static constexpr uint64_t TagId = 0x359d837855e3858c;
	};

	struct _Pckd EfiSystemTag : Tag {
		// TODO: Replace with SystemTable
		uint64_t systemTable;

		static constexpr uint64_t TagId = 0x4bc5ec15845b558e;
	};

	struct _Pckd SmpTag : Tag {
		uint64_t flags;             // Flags:
		//   bit 0: Set if x2APIC was requested and it
		//          was supported and enabled.
		//  All other bits are undefined and set to 0.
		uint32_t
			bspLapicId,
			unused;
		// LAPIC ID of the BSP (bootstrap processor).
		// Reserved for future use.
		uint64_t size;
		// TODO: Replace with SmpInfo
		uint8_t smpInfos[];

		static constexpr uint64_t TagId = 0x4b6fe466aade04ce;
	};

	struct _Pckd PxeTag : Tag {
		uint32_t serverIp;

		static constexpr uint64_t TagId = 0xb0ed257db18cb58f;
	};

	struct _Pckd Mmio32UartTag : Tag {
		// TODO: Replace with Mmio32Uart
		uintptr_t uart;

		static constexpr uint64_t TagId = 0xb0ed257db18cb58f;
	};

	struct _Pckd DeviceBlobTag : Tag {
		// TODO: Replace with DeviceBlob
		uintptr_t blob;
		uint64_t size;

		static constexpr uint64_t TagId = 0xabb29bd49a2833fa;
	};

	struct _Pckd VMapTag : Tag {
		// TODO: Replace with VMap
		uintptr_t virtualMap;

		static constexpr uint64_t TagId = 0xb0ed257db18cb58f;
	};

	struct _Pckd Structure {
		char
			bootloaderBrand[64],
			bootloaderVersion[64];

		Tag *tags;
	};
    }
}

#endif //_ARCH_X86_64_KERNEL_STIVALE2_INCLUDE_XOS_STIVALE2_H
