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
// Created by xgallom on 5/25/20.
//

#include <xos/gdt.h>
#include <xos/stdio.h>

namespace gdt {
    static Gdtr s_gdtr;

    void storeGdtr()
    {
	    asm volatile(
	    "sgdt %0"
	    :
	    : "m"(s_gdtr)
	    );
    }

    const Gdtr &gdtr()
    {
	    return s_gdtr;
    }
}

