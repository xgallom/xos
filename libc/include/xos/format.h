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
	    const T &arg,
	    const char *&errorBuffer)
    {
	    char
		    formatBuffer[_format::MaxBufferLength + 1] = "",
		    *formatPtr = formatBuffer,
		    c;

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
