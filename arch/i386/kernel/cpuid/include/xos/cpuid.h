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
// Created by xgallom on 5/22/20.
//

#ifndef XOS_CPUID_H
#define XOS_CPUID_H

#include <stdint.h>
#include <stddef.h>

namespace cpuid {
    namespace RequestType {
	enum Enum : uint8_t {
		GetVendorString = 0,
		GetFeatures,
		GetTLB,
		GetSerial,

		IntelExtended,
		IntelFeatures,
		IntelBrandString,
		IntelBrandStringMore,
		IntelBrandStringEnd,
	};

	static constexpr size_t Count = IntelBrandStringEnd + 1;
    }

    static constexpr size_t
	    ResultLength = 4,
	    ResultByteLength = sizeof(uint32_t) * ResultLength;

    bool initialize();

    const uint32_t *cpuid(RequestType::Enum request);
}

#endif //XOS_CPUID_H
