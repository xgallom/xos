//
// Created by xgallom on 5/23/20.
//

#ifndef _XOS_LIBC_INCLUDE_XOS_WRITE_H
#define _XOS_LIBC_INCLUDE_XOS_WRITE_H

#include "write_decl.h"

namespace xos {
    extern template void writeHex(char);
    extern template void writeHex(unsigned char);
    extern template void writeHex(short);
    extern template void writeHex(unsigned short);
    extern template void writeHex(int);
    extern template void writeHex(unsigned int);
    extern template void writeHex(long);
    extern template void writeHex(unsigned long);
    extern template void writeHex(long long);
    extern template void writeHex(unsigned long long);

    extern template void writeDec(char);
    extern template void writeDec(unsigned char);
    extern template void writeDec(short);
    extern template void writeDec(unsigned short);
    extern template void writeDec(int);
    extern template void writeDec(unsigned int);
    extern template void writeDec(long);
    extern template void writeDec(unsigned long);
    extern template void writeDec(long long);
    extern template void writeDec(unsigned long long);

    extern template bool write(
	    const char *, char, const char *&, size_t);
    extern template bool write(
	    const char *, unsigned char, const char *&, size_t);
    extern template bool write(
	    const char *, short, const char *&, size_t);
    extern template bool write(
	    const char *, unsigned short, const char *&, size_t);
    extern template bool write(
	    const char *, int, const char *&, size_t);
    extern template bool write(
	    const char *, unsigned int, const char *&, size_t);
    extern template bool write(
	    const char *, long, const char *&, size_t);
    extern template bool write(
	    const char *, unsigned long, const char *&, size_t);
    extern template bool write(
	    const char *, long long, const char *&, size_t);
    extern template bool write(
	    const char *, unsigned long long, const char *&, size_t);

    extern template bool write(
	    const char *, bool, const char *&, size_t);
    extern template bool write(
	    const char *, const char *, const char *&, size_t);
}

#endif //_XOS_LIBC_INCLUDE_XOS_WRITE_H
