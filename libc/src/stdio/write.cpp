//
// Created by xgallom on 5/23/20.
//

#include <xos/write_decl.h>
#include <xos/restrict.h>

#ifdef __xos_is_libk

#include <xos/tty.h>
#include <xos/div64.h>

#endif

namespace xos {
    template<typename T>
    void writeHex(T value)
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

    template void writeHex(char);
    template void writeHex(unsigned char);
    template void writeHex(short);
    template void writeHex(unsigned short);
    template void writeHex(int);
    template void writeHex(unsigned int);
    template void writeHex(long);
    template void writeHex(unsigned long);
    template void writeHex(long long);
    template void writeHex(unsigned long long);

    template<typename T>
    inline void writeDec(T value)
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
    void writeDec(long long value)
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
		    uint32_t remainder = value % 10;
		    value = div64u(value, 10, remainder);
		    *--buf = '0' + char(remainder);
	    } while (value);

	    write(buf);
    }

    template<>
    void writeDec(unsigned long long value)
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
		    uint32_t remainder = value % 10;
		    value = div64u(value, 10, remainder);
		    *--buf = '0' + char(remainder);
	    } while (value);

	    write(buf);
    }

    template void writeDec(char);
    template void writeDec(unsigned char);
    template void writeDec(short);
    template void writeDec(unsigned short);
    template void writeDec(int);
    template void writeDec(unsigned int);
    template void writeDec(long);
    template void writeDec(unsigned long);

    template<typename T>
    bool write(
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
		    else if (*formatBuffer != 'd') {
			    errorBuffer =
				    "Invalid format supplied for short";
			    return false;
		    }
	    }

	    writeDec(arg);
	    return true;
    }

    template<>
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

    template<>
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

    template bool write(
	    const char *, char, const char *&, size_t);
    template bool write(
	    const char *, unsigned char, const char *&, size_t);
    template bool write(
	    const char *, short, const char *&, size_t);
    template bool write(
	    const char *, unsigned short, const char *&, size_t);
    template bool write(
	    const char *, int, const char *&, size_t);
    template bool write(
	    const char *, unsigned int, const char *&, size_t);
    template bool write(
	    const char *, long, const char *&, size_t);
    template bool write(
	    const char *, unsigned long, const char *&, size_t);
    template bool write(
	    const char *, long long, const char *&, size_t);
    template bool write(
	    const char *, unsigned long long, const char *&, size_t);

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
