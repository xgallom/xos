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
    template<typename T>
    struct remove_reference {
	    using type = T;
    };

    template<typename T>
    struct remove_reference<T &> {
	    using type = T;
    };

    template<typename T>
    struct remove_reference<T &&> {
	    using type = T;
    };

    struct false_type {
	    static constexpr bool value = false;
    };

    struct true_type {
	    static constexpr bool value = true;
    };

    template<typename T>
    struct is_reference : false_type {};
    template<typename T>
    struct is_reference<T &> : true_type {};

    template<typename T>
    T forward(T arg)
    {
	    if constexpr(is_reference<T>::value)
		    return static_cast<const T &>(arg);
	    return arg;
    }
}

#endif //_XOS_CORE_INCLUDE_XOS_UTILITY_H
