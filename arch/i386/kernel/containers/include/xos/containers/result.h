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

#ifndef _ARCH_I386_KERNEL_CONTAINERS_INCLUDE_XOS_CONTAINERS_RESULT_H
#define _ARCH_I386_KERNEL_CONTAINERS_INCLUDE_XOS_CONTAINERS_RESULT_H

namespace xos {
    template<typename T>
    struct Result {
	    bool valid = {};
	    T result = {};

	    Result() = default;

	    Result(T result) : valid{true}, result{result} {}

	    T operator()() const { return result; }

	    explicit operator bool() const { return valid; }
    };
}

#endif //_ARCH_I386_KERNEL_CONTAINERS_INCLUDE_XOS_CONTAINERS_RESULT_H
