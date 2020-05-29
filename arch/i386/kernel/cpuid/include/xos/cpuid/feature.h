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

#ifndef _XOS_ARCH_I386_KERNEL_CPUID_INCLUDE_XOS_CPUID_FEATURE_H
#define _XOS_ARCH_I386_KERNEL_CPUID_INCLUDE_XOS_CPUID_FEATURE_H

#include <stdint.h>

namespace cpuid {
    namespace FeatureType {
	static constexpr uint8_t
		EdxMask = 0x80u,
		BitOffsetMask = uint8_t(~EdxMask);

	enum Enum : uint8_t {
		Sse3 = 0,
		Pclmulqdq,
		Dtes64,
		Monitor,
		DsCpl,
		Vmx,
		Smx,
		Est,
		Tm2,
		Ssse3,
		CnxtId,
		Sdbg,
		Fma,
		Cx16,
		Xtpr,
		Pdcm,
		_reserved_ecx16,
		Pcid,
		Dca,
		Sse4p1,
		Sse4p2,
		X2apic,
		Movbe,
		Popcnt,
		Aes,
		Xsave,
		Osxsave,
		Avx,
		F16c,
		Rdrnd,
		Hypervisor,

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
		_reserved_edx10,
		Sep,
		Mtrr,
		Pge,
		Mca,
		Cmov,
		Pat,
		Pse36,
		Psn,
		Clfsh,
		_reserved_edx20,
		Ds,
		Acpi,
		Mmx,
		Fxsr,
		Sse,
		Sse2,
		Ss,
		Htt,
		Tm,
		Ia64,
		Pbe,
	};
    }

    bool feature(FeatureType::Enum feature);
    const char *featureName(FeatureType::Enum feature);
}

#endif //_XOS_ARCH_I386_KERNEL_CPUID_INCLUDE_XOS_CPUID_FEATURE_H
