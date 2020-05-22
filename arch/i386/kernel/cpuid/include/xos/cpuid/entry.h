//
// Created by xgallom on 5/22/20.
//

#ifndef _XOS_ARCH_I386_KERNEL_CPUID_INCLUDE_XOS_CPUID_ENTRY_H
#define _XOS_ARCH_I386_KERNEL_CPUID_INCLUDE_XOS_CPUID_ENTRY_H

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

    const uint32_t *entry(RequestType::Enum request);
    const char *entryName(RequestType::Enum request);
    uint32_t entryRequest(RequestType::Enum request);
}

#endif //_XOS_ARCH_I386_KERNEL_CPUID_INCLUDE_XOS_CPUID_ENTRY_H
