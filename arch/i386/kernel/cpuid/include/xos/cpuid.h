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

#ifndef _XOS_ARCH_I386_KERNEL_CPUID_INCLUDE_XOS_CPUID_H
#define _XOS_ARCH_I386_KERNEL_CPUID_INCLUDE_XOS_CPUID_H

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

    namespace FeatureType {
	static constexpr uint8_t
		EdxMask = 0x80u,
		BitOffsetMask = uint8_t(~EdxMask);

	enum Enum : uint8_t {
		Sse3 = 0,
		PclMul,
		Dtes64,
		Monitor,
		DsCpl,
		Vmx,
		Smx,
		Est,
		Tm2,
		Ssse3,
		Cid,
		Fma,
		Cx16,
		Etprd,
		Pdcm,
		PciDe,
		Dca,
		Sse4_1,
		Sse4_2,
		X2Apic,
		MovBe,
		PopCnt,
		Aes,
		XSave,
		OsxSave,
		Avx,

		Fpu = EdxMask,
		Vme,
		De,
		Pse,
		Tsc,
		Msr,
		Pae,
		Mce,
		Cx8,
		Apic,
		Sep,
		Mtrr,
		Pge,
		Mca,
		CMov,
		Pat,
		Pse36,
		Psn,
		Clf,
		Dtes,
		Acpi,
		Mmx,
		Fxsr,
		Sse,
		Sse2,
		Ss,
		Htt,
		Tm1,
		IA64,
		Pbe,
	};
    }

    static constexpr size_t
	    ResultLength = 4,
	    ResultByteLength = sizeof(uint32_t) * ResultLength;

    bool initialize();

    const uint32_t *entry(RequestType::Enum request);
    bool feature(FeatureType::Enum feature);
}

#endif //_XOS_ARCH_I386_KERNEL_CPUID_INCLUDE_XOS_CPUID_H
