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
// Created by xgallom on 10/15/20.
//

#ifndef _ARCH_I386_KERNEL_DESCRIPTORS_INCLUDE_XOS_DESCRIPTORS_DESCRIPTOR_REGISTER_H
#define _ARCH_I386_KERNEL_DESCRIPTORS_INCLUDE_XOS_DESCRIPTORS_DESCRIPTOR_REGISTER_H

#include <stdint.h>
#include <xos/packed.h>

template<typename DescriptorType_>
struct _Pckd DescriptorRegister {
	using DescriptorType = DescriptorType_;

	uint16_t size;
	const DescriptorType *address;

	[[nodiscard]] inline
	const DescriptorType *begin() const { return address; }

	[[nodiscard]] inline
	const DescriptorType *end() const
	{
		return reinterpret_cast<const DescriptorType *>(
			reinterpret_cast<uintptr_t>(address) + size + 1
		);
	}

	template<size_t N>
	void set(const DescriptorType (&descriptors)[N])
	{
		size = sizeof(descriptors) - 1;
		address = descriptors;
	}

	static constexpr size_t Size = sizeof(uint16_t) + sizeof(uintptr_t);
};

#endif //_ARCH_I386_KERNEL_DESCRIPTORS_INCLUDE_XOS_DESCRIPTORS_DESCRIPTOR_REGISTER_H
