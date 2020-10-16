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

#ifndef _XOS_LIBC_INCLUDE_XOS_FORMAT_H
#define _XOS_LIBC_INCLUDE_XOS_FORMAT_H

#include "write.h"
#include <stddef.h>

namespace xos {
    namespace _format {
	static constexpr size_t
		MaxErrorLength = 256,
		MaxFormatCount = 1,
		MaxBufferLength = MaxFormatCount + 1;
    }

    template<typename T>
    bool format(
	    const char *&string,
	    T arg,
	    const char *&errorBuffer)
    {
	    char
		    formatBuffer[_format::MaxBufferLength + 1] = "",
		    *formatPtr = formatBuffer,
		    c;

	    restart:

	    while ((c = *string++)) {
		    if (c == '{') {
			    size_t formatLength = 0;

			    do {
				    if ((c = *string++) == '}')
					    return write(
						    formatBuffer,
						    arg,
						    errorBuffer,
						    formatLength
					    );
				    else if (c == '{') {
					    if (formatLength) {
						    errorBuffer =
							    "Duplicate { with "
							    "content between";
						    return false;
					    }

					    putchar('{');
					    goto restart;
				    }

				    *formatPtr++ = c;
			    } while (
				    ++formatLength < _format::MaxBufferLength
				    && c
				    );

			    errorBuffer =
				    "Argument missing } or format too long";
			    return false;
		    }
		    else
			    xos::putchar(c);
	    }

	    errorBuffer = "Argument missing {";
	    return false;
    }
}

#endif //_XOS_LIBC_INCLUDE_XOS_FORMAT_H
