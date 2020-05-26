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
