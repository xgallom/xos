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

#include <xos/cpuid.h>
#include "isAvailable.h"

namespace cpuid {
    static constexpr uint32_t RequestTypeMap[RequestType::Count] = {
	    0x00000000,
	    0x00000001,
	    0x00000002,
	    0x00000003,

	    0x80000000,
	    0x80000001,
	    0x80000002,
	    0x80000003,
    };

    static uint32_t s_result[RequestType::Count][ResultLength] = {};

    static void loadCpuid(RequestType::Enum request)
    {
	    uint32_t *result = s_result[request];

	    asm volatile(
	    "cpuid"
	    : "=a"(*result), "=b"(*(result + 1)), "=c"(*(result + 2)),
	    "=d"(*(result + 3))
	    : "a"(RequestTypeMap[request])
	    );
    }

    bool initialize()
    {
	    if (!isAvailable())
		    return false;

	    for(uint8_t request = 0; request < RequestType::Count; ++request)
	    	loadCpuid(static_cast<RequestType::Enum>(request));

	    return true;
    }

    const uint32_t *cpuid(RequestType::Enum request)
    {
	    return s_result[request];
    }
}
