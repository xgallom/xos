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
// Created by xgallom on 10/23/20.
//

#ifndef _ARCH_I386_KERNEL_CONTAINERS_INCLUDE_XOS_CONTAINERS_CIRCULAR_BUFFER_H
#define _ARCH_I386_KERNEL_CONTAINERS_INCLUDE_XOS_CONTAINERS_CIRCULAR_BUFFER_H

#include <stddef.h>
#include <stdint.h>
#include <xos/atomic.h>
#include "result.h"

namespace xos {
    template<typename T, size_t BitSize>
    struct CircularBuffer {
	    static_assert(BitSize <= sizeof(size_t) * 8u,
			  "Index offset overflow");

	    static constexpr size_t
		    Offset = BitSize,
		    Count = (size_t(1) << Offset) - 1;
	    static constexpr uint32_t
		    Mask = Count;


	    size_t front = 0, back = 0;
	    T buffer[Count] = {};

	    Result<T> pop()
	    {
		    T result = {};
		    if (front != back) {
			    result = buffer[front++];
			    front &= Mask;
		    }

		    return result;
	    }

	    bool push(T value)
	    {
		    const auto hasSpace = size() != Count;
		    if (hasSpace) {
			    buffer[back++] = value;
			    back &= Mask;
		    }

		    return hasSpace;
	    }

	    [[nodiscard]] inline size_t size() const
	    {
		    return back > front ? back - front : front - back;
	    }
    };
}

#endif //_ARCH_I386_KERNEL_CONTAINERS_INCLUDE_XOS_CONTAINERS_CIRCULAR_BUFFER_H
