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

#ifndef _ARCH_I386_KERNEL_DRIVERS_INCLUDE_XOS_DRIVERS_PS2_KEYBOARD_H
#define _ARCH_I386_KERNEL_DRIVERS_INCLUDE_XOS_DRIVERS_PS2_KEYBOARD_H

#include <stdint.h>
#include <stddef.h>

namespace ps2::kbd {
    struct State {
	    enum Flags : size_t {
		    LShift,
		    RShift,
		    LCtrl,
		    RCtrl,
		    LAlt,
		    RAlt,
		    CapsLock,
		    E0,

		    FlagsCount
	    };

	    template<Flags ... Flag>
	    static constexpr uint32_t Mask()
	    {
		    return ((1u << Flag) | ...);
	    }

	    template<Flags ... Flag>
	    static constexpr uint32_t CharMask()
	    {
		    return Mask<Flag...>() << 8u;
	    }

	    template<Flags ... Flag>
	    static constexpr bool get(uint32_t value)
	    {
		    return (value & Mask<Flag...>()) != 0;
	    }

	    template<Flags ... Flag>
	    static constexpr void set(uint32_t &value)
	    {
		    value |= Mask<Flag...>();
	    }

	    template<Flags ... Flag>
	    static constexpr void rst(uint32_t &value)
	    {
		    value &= ~Mask<Flag...>();
	    }

	    template<Flags ... Flag>
	    static constexpr void tgl(uint32_t &value)
	    {
		    value ^= Mask<Flag...>();
	    }
    };

    bool initialize();
    void clear();
    int getchar();
}

#endif //_ARCH_I386_KERNEL_DRIVERS_INCLUDE_XOS_DRIVERS_PS2_KEYBOARD_H
