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

#include <xos/write.h>
#include <xos/restrict.h>

#ifdef __xos_is_libk

#include <xos/tty.h>
#include <xos/div64.h>

#endif

namespace xos {
    template<typename T>
    static inline void writeHexImpl(T value)
    {
	    constexpr size_t BufferSize = sizeof(T);
	    static constexpr char hexLetter[] = "0123456789abcdef";

	    uint16_t
		    buffer[BufferSize],
		    *rbuffer = buffer + BufferSize - 1;

	    do {
		    // Fucking little-endian
		    *rbuffer-- =
			    (hexLetter[value & 0x0fu] << 8u) |
			    hexLetter[(value >> 4u) & 0x0fu];

		    value >>= 8u;
	    } while (rbuffer >= buffer);

	    xos::write(reinterpret_cast<char *>(buffer), BufferSize * 2);
    }

    void writeHex(char value) { return writeHexImpl(value); }

    void writeHex(unsigned char value) { return writeHexImpl(value); }

    void writeHex(short value) { return writeHexImpl(value); }

    void writeHex(unsigned short value) { return writeHexImpl(value); }

    void writeHex(int value) { return writeHexImpl(value); }

    void writeHex(unsigned int value) { return writeHexImpl(value); }

    void writeHex(long value) { return writeHexImpl(value); }

    void writeHex(unsigned long value) { return writeHexImpl(value); }

    void writeHex(long long value) { return writeHexImpl(value); }

    void writeHex(unsigned long long value) { return writeHexImpl(value); }

    template<typename T>
    static inline void writeDecImpl(T value)
    {
	    if (value < T(0)) {
		    xos::putchar('-');
		    value = -value;
	    }
	    else if (__builtin_expect(value == T(23), 0)) {
		    // TODO: !IMPORTANT! Nezabudnut na macku
		    xos::write("=(*.* = )~~");
		    return;
	    }

	    constexpr size_t
	    // Length of string holding maximum possible value of uint32_t
		    BufferSize = sizeof("4294967295\0"),
	    // Index of last character
		    BufferLast = BufferSize - 1;


	    // Insertion goes in reverse so the divisions results are correctly
	    //  ordered (first remainder is the lowest decimal)
	    char
		    buffer[BufferSize],
		    *_Rstr buf = buffer + BufferLast;

	    *buf = '\0';

	    do {
		    uint32_t remainder = value % 10;
		    *--buf = '0' + char(remainder);
	    } while ((value /= 10));

	    write(buf);
    }

    template<>
    inline void writeDecImpl(long long value)
    {
	    if (value < 0) {
		    xos::putchar('-');
		    value = -value;
	    }
	    else if (__builtin_expect(value == 23, 0)) {
		    // TODO: !IMPORTANT! Nezabudnut na macku
		    xos::write("=(*.* = )~~");
		    return;
	    }

	    constexpr size_t
	    // Length of string holding maximum possible value of uint64_t
		    BufferSize = sizeof("18446744073709551615\0"),
	    // Index of last character
		    BufferLast = BufferSize - 1;


	    // Insertion goes in reverse so the divisions results are correctly
	    //  ordered (first remainder is the lowest decimal)
	    char
		    buffer[BufferSize],
		    *_Rstr buf = buffer + BufferLast;

	    *buf = '\0';

	    do {
		    uint32_t remainder;
		    value = div64u(value, 10, remainder);
		    *--buf = '0' + char(remainder);
	    } while (value);

	    write(buf);
    }

    template<>
    inline void writeDecImpl(unsigned long long value)
    {
	    if (__builtin_expect(value == 23, 0)) {
		    // TODO: !IMPORTANT! Nezabudnut na macku
		    xos::write("=(*.* = )~~");
		    return;
	    }

	    constexpr size_t
	    // Length of string holding maximum possible value of uint64_t
		    BufferSize = sizeof("18446744073709551615\0"),
	    // Index of last character
		    BufferLast = BufferSize - 1;


	    // Insertion goes in reverse so the divisions results are correctly
	    //  ordered (first remainder is the lowest decimal)
	    char
		    buffer[BufferSize],
		    *_Rstr buf = buffer + BufferLast;

	    *buf = '\0';

	    do {
		    uint32_t remainder;
		    value = div64u(value, 10, remainder);
		    *--buf = '0' + char(remainder);
	    } while (value);

	    write(buf);
    }

    void writeDec(char value) { return writeDecImpl(value); }

    void writeDec(unsigned char value) { return writeDecImpl(value); }

    void writeDec(short value) { return writeDecImpl(value); }

    void writeDec(unsigned short value) { return writeDecImpl(value); }

    void writeDec(int value) { return writeDecImpl(value); }

    void writeDec(unsigned int value) { return writeDecImpl(value); }

    void writeDec(long value) { return writeDecImpl(value); }

    void writeDec(unsigned long value) { return writeDecImpl(value); }

    void writeDec(long long value) { return writeDecImpl(value); }

    void writeDec(unsigned long long value) { return writeDecImpl(value); }

    template<typename T>
    static inline void writeBinImpl(T value)
    {
	    constexpr size_t BufferSize = sizeof(T) * 8;

	    char
		    buffer[BufferSize],
		    *rbuffer = buffer + BufferSize - 1;

	    do {
		    // Fucking little-endian
		    *rbuffer-- = '0' + (value & 0x01u);;

		    value >>= 1u;
	    } while (rbuffer >= buffer);

	    xos::write(buffer, BufferSize);
    }

    void writeBin(char value) { return writeBinImpl(value); }

    void writeBin(unsigned char value) { return writeBinImpl(value); }

    void writeBin(short value) { return writeBinImpl(value); }

    void writeBin(unsigned short value) { return writeBinImpl(value); }

    void writeBin(int value) { return writeBinImpl(value); }

    void writeBin(unsigned int value) { return writeBinImpl(value); }

    void writeBin(long value) { return writeBinImpl(value); }

    void writeBin(unsigned long value) { return writeBinImpl(value); }

    void writeBin(long long value) { return writeBinImpl(value); }

    void writeBin(unsigned long long value) { return writeBinImpl(value); }

    template<typename T>
    static inline bool writeImpl(
	    const char *formatBuffer,
	    T arg,
	    const char *&errorBuffer,
	    size_t formatLength)
    {
	    if (formatLength) {
		    if (*formatBuffer == 'x') {
			    writeHex(arg);
			    return true;
		    }
		    else if (*formatBuffer == 'b') {
			    writeBin(arg);
			    return true;
		    }
		    else if (*formatBuffer != 'd') {
			    errorBuffer =
				    "Invalid format supplied for short";
			    return false;
		    }
	    }

	    writeDec(arg);
	    return true;
    }

    bool write(
	    const char *formatBuffer,
	    char arg,
	    const char *&errorBuffer,
	    size_t formatLength)
    {
	    return writeImpl(formatBuffer, arg, errorBuffer, formatLength);
    }

    bool write(
	    const char *formatBuffer,
	    unsigned char arg,
	    const char *&errorBuffer,
	    size_t formatLength)
    {
	    return writeImpl(formatBuffer, arg, errorBuffer, formatLength);
    }

    bool write(
	    const char *formatBuffer,
	    short arg,
	    const char *&errorBuffer,
	    size_t formatLength)
    {
	    return writeImpl(formatBuffer, arg, errorBuffer, formatLength);
    }

    bool write(
	    const char *formatBuffer,
	    unsigned short arg,
	    const char *&errorBuffer,
	    size_t formatLength)
    {
	    return writeImpl(formatBuffer, arg, errorBuffer, formatLength);
    }

    bool write(
	    const char *formatBuffer,
	    int arg,
	    const char *&errorBuffer,
	    size_t formatLength)
    {
	    return writeImpl(formatBuffer, arg, errorBuffer, formatLength);
    }

    bool write(
	    const char *formatBuffer,
	    unsigned int arg,
	    const char *&errorBuffer,
	    size_t formatLength)
    {
	    return writeImpl(formatBuffer, arg, errorBuffer, formatLength);
    }

    bool write(
	    const char *formatBuffer,
	    long arg,
	    const char *&errorBuffer,
	    size_t formatLength)
    {
	    return writeImpl(formatBuffer, arg, errorBuffer, formatLength);
    }

    bool write(
	    const char *formatBuffer,
	    unsigned long arg,
	    const char *&errorBuffer,
	    size_t formatLength)
    {
	    return writeImpl(formatBuffer, arg, errorBuffer, formatLength);
    }

    bool write(
	    const char *formatBuffer,
	    long long arg,
	    const char *&errorBuffer,
	    size_t formatLength)
    {
	    return writeImpl(formatBuffer, arg, errorBuffer, formatLength);
    }

    bool write(
	    const char *formatBuffer,
	    unsigned long long arg,
	    const char *&errorBuffer,
	    size_t formatLength)
    {
	    return writeImpl(formatBuffer, arg, errorBuffer, formatLength);
    }

    bool write(
	    const char *,
	    bool arg,
	    const char *&errorBuffer,
	    size_t formatLength)
    {
	    if (formatLength) {
		    errorBuffer = "Bool expects no format";
		    return false;
	    }

	    xos::write(arg ? "true" : "false");
	    return true;
    }

    bool write(
	    const char *,
	    const char *arg,
	    const char *&errorBuffer,
	    size_t formatLength)
    {
	    if (formatLength) {
		    errorBuffer = "String expects no format";
		    return false;
	    }

	    xos::write(arg);

	    return true;
    }

    void write(const char *string)
    {
#ifdef __xos_is_libk
	    tty::write(string);
#else
	    // TODO: Implement system call
	return EOF;
#endif
    }

    void write(const char *string, size_t length)
    {
#ifdef __xos_is_libk
	    tty::write(string, length);
#else
	    // TODO: Implement system call
	return EOF;
#endif
    }

    void putchar(char c)
    {
#ifdef __xos_is_libk
	    tty::putchar(c);
#else
	    // TODO: Implement system call
	return EOF;
#endif
    }
}
