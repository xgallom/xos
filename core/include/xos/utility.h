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

#ifndef _XOS_CORE_INCLUDE_XOS_UTILITY_H
#define _XOS_CORE_INCLUDE_XOS_UTILITY_H

namespace xos {
    constexpr size_t log2(size_t x)
    {
	    size_t y = 0;
	    while (!(x & 1u)) {
		    x >>= 1u;
		    ++y;
	    }

	    return y;
    }
}

#endif //_XOS_CORE_INCLUDE_XOS_UTILITY_H
