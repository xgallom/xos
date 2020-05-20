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
// Created by xgallom on 5/11/20.
//

#include <string.h>

static inline int unsafe_memcmp(const unsigned char *left, const unsigned char *right, size_t byteLength)
{
	do {
		const int distance = *right++ - *left++;

		if(distance)
			return distance;
	} while(--byteLength);

	return 0;
}

int memcmp(const void *left, const void *right, size_t byteLength)
{
	if(!byteLength)
		return 0;

	return unsafe_memcmp(
			reinterpret_cast<const unsigned char *>(left),
			reinterpret_cast<const unsigned char *>(right),
			byteLength
	);
}

static inline void unsafe_memcpy(unsigned char *_Rstr dest, const unsigned char *_Rstr src, size_t byteLength)
{
	do {
		*dest++ = *src++;
	} while(--byteLength);
}

void *memcpy(void *_Rstr dest, const void *_Rstr src, size_t byteLength)
{
	if(!byteLength)
		return dest;

	unsafe_memcpy(
			reinterpret_cast<unsigned char *_Rstr>(dest),
			reinterpret_cast<const unsigned char *_Rstr>(src),
			byteLength
	);

	return dest;
}

static inline void unsafe_memmove_up(unsigned char *rdest, const unsigned char *rsrc, size_t byteLength)
{
	do {
		*rdest-- = *rsrc--;
	} while(--byteLength);
}

void *memmove(void *dest, const void *src, size_t byteLength)
{
	if(!byteLength)
		return dest;

	if(dest < src)
		unsafe_memcpy(
				reinterpret_cast<unsigned char *_Rstr>(dest),
				reinterpret_cast<const unsigned char *_Rstr>(src),
				byteLength
		);
	else {
		const size_t offset = byteLength - 1;

		unsafe_memmove_up(
				reinterpret_cast<unsigned char *>(dest) + offset,
				reinterpret_cast<const unsigned char *>(src) + offset,
				byteLength
		);
	}

	return dest;
}

static inline void unsafe_memset(unsigned char *dest, unsigned char value, size_t byteLength)
{
	do {
		*dest++ = value;
	} while(--byteLength);
}

void *memset(void *dest, int value, size_t byteLength)
{
	if(!byteLength)
		return dest;

	unsafe_memset(
			reinterpret_cast<unsigned char *>(dest),
			static_cast<unsigned char>(value),
			byteLength
	);

	return dest;
}

char *strcpy(char *_Rstr dest, const char *_Rstr src)
{
	char *_Rstr destBuf = dest;

	do {
		*destBuf++ = *src;
	} while(*src++);

	return dest;
}

char *strncpy(char *_Rstr dest, const char *_Rstr src, size_t length)
{
	if(!length)
		return dest;

	do {
		*dest++ = *src++;
	} while(--length);

	return dest;
}

size_t strlen(const char *str)
{
	size_t result = 0;

	while(*str++)
		++result;

	return result;
}
