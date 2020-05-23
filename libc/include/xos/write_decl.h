//
// Created by xgallom on 5/23/20.
//

#ifndef _XOS_LIBC_INCLUDE_XOS_WRITE_DECL_H
#define _XOS_LIBC_INCLUDE_XOS_WRITE_DECL_H

#include <stddef.h>

namespace xos {
    template<typename T>
    void writeHex(T value);

    template<typename T>
    void writeDec(T value);

    template<typename T>
    bool write(
	    const char *formatBuffer,
	    T arg,
	    const char *&errorBuffer,
	    size_t formatLength
    );

    void write(const char *string);
    void write(const char *string, size_t count);
    void putchar(char c);
}

#endif //_XOS_LIBC_INCLUDE_XOS_WRITE_DECL_H
