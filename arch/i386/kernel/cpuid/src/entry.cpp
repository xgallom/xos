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

#include <stdio.h>
#include "entry.h"

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
	    0x80000004,
    };

    static const char *RequestTypeNameMap[RequestType::Count] = {
	    "Highest Parameter and Manufacturer ID",
	    "Processor Info and Feature Bits",
	    "Cache and TLB Descriptor Information",
	    "Processor Serial Number",

	    "Highest Extended Function Implemented",
	    "Extended Processor Info and Feature Bits",
	    "Processor Brand String [0]",
	    "Processor Brand String [1]",
	    "Processor Brand String [2]",
    };

    static uint32_t s_result[RequestType::Count][ResultLength] = {};

    void requestVendorString(RequestType::Enum request)
    {
	    uint32_t *result = s_result[request];

	    asm volatile(
	    "cpuid"
	    : "=a"(*(result)), "=b"(*(result + 1)), "=c"(*(result + 3)),
	    "=d"(*(result + 2))
	    : "a"(RequestTypeMap[request])
	    :
	    );
    }

    void requestOther(RequestType::Enum request)
    {
	    uint32_t *result = s_result[request];

	    asm volatile(
	    "cpuid"
	    : "=a"(*(result)), "=b"(*(result + 1)), "=c"(*(result + 2)),
	    "=d"(*(result + 3))
	    : "a"(RequestTypeMap[request])
	    :
	    );
    }

    const uint32_t *entry(RequestType::Enum request)
    {
	    return s_result[request];
    }

    const char *stringEntry(RequestType::Enum request)
    {
	    return reinterpret_cast<const char *>(
			   s_result[request]
		   ) + sizeof(uint32_t);
    }

    const char *entryName(RequestType::Enum request)
    {
	    return RequestTypeNameMap[request];
    }

    uint32_t entryRequest(RequestType::Enum request)
    {
	    return RequestTypeMap[request];
    }
}

