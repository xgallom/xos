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
// Created by xgallom on 5/23/20.
//

#ifndef _XOS_LIBC_INCLUDE_XOS_STDIO_H
#define _XOS_LIBC_INCLUDE_XOS_STDIO_H

#include <stddef.h>
#include "format.h"

namespace xos {
    template<typename ... Args>
    size_t printf(const char *string, Args ... args)
    {
	    const char *errorBuffer = "";

	    if (!(
		    format<Args>(
			    string,
			    args,
			    errorBuffer
		    ) && ...
	    )) {
		    write(errorBuffer);
		    return -1;
	    }

	    write(string);
	    return sizeof...(args);
    }
}

#endif //_XOS_LIBC_INCLUDE_XOS_FORMAT_H
