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
// Created by xgallom on 10/24/20.
//

#ifndef _ARCH_I386_KERNEL_ASM_INCLUDE_XOS_ATOMIC_H
#define _ARCH_I386_KERNEL_ASM_INCLUDE_XOS_ATOMIC_H

#include "pause.h"
#include "interrupt-enable.h"

namespace xos {
    struct InterruptAccessFlag {
	    enum Enum : uint8_t {
		    Free = 0,
		    Active = 1,
		    Modified = 2,
	    };

	    volatile Enum value = {};

	    inline void waitForAccess()
	    {
		    Enum expected = Free;
		    while (!__atomic_compare_exchange_n(
			    &value,
			    &expected,
			    Free,
			    true,
			    __ATOMIC_RELAXED,
			    __ATOMIC_RELAXED
		    )) {
			    if (expected == Active)
				    expected = Modified;
			    pause();
		    }

		    __atomic_signal_fence(__ATOMIC_ACQUIRE);

		    if (expected != Free) {
			    disableInterrupts();
			    waitForAccess();
		    }
	    }

	    inline void acquire()
	    {
		    __atomic_store_n(
			    &value,
			    Active,
			    __ATOMIC_RELAXED
		    );
		    __atomic_signal_fence(__ATOMIC_ACQUIRE);
	    }

	    inline void publish()
	    {
		    __atomic_store_n(
			    &value,
			    Modified,
			    __ATOMIC_RELAXED
		    );
		    __atomic_signal_fence(__ATOMIC_RELEASE);
	    }
    };
}

#endif //_ARCH_I386_KERNEL_ASM_INCLUDE_XOS_ATOMIC_H
