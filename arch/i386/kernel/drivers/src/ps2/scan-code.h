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
// Created by xgallom on 10/24/20.
//

#ifndef _ARCH_I386_KERNEL_DRIVERS_SRC_PS2_SCAN_CODE_H
#define _ARCH_I386_KERNEL_DRIVERS_SRC_PS2_SCAN_CODE_H

#include <stdint.h>

namespace ps2::kbd {
    namespace ScanCode {
	enum Enum : uint8_t {
		Error0,
		Esc,

		Ch1,
		Ch2,
		Ch3,
		Ch4,
		Ch5,
		Ch6,
		Ch7,
		Ch8,
		Ch9,
		Ch0,
		ChMi,
		ChEq,
		Backspace,

		Tab,
		Q,
		W,
		E,
		R,
		T,
		Y,
		U,
		I,
		O,
		P,
		LBracket,
		RBracket,

		Enter,
		LCtrl,

		A,
		S,
		D,
		F,
		G,
		H,
		J,
		K,
		L,
		Colon,
		Quote,

		Tilde,

		LShift,

		Backslash,

		Z,
		X,
		C,
		V,
		B,
		N,
		M,
		Comma,
		Dot,
		Slash,

		RShift,

		KpStar,

		LAlt,
		Space,
		CapsLock,

		F1,
		F2,
		F3,
		F4,
		F5,
		F6,
		F7,
		F8,
		F9,
		F10,

		NumLock,
		ScrollLock,

		Kp7,
		Kp8,
		Kp9,

		Home = Kp7,
		Up,
		PgUp,

		KpMinus,

		Kp4,
		Kp5,
		Kp6,

		Left = Kp4,

		KpPlus = Kp6 + 1,

		Kp1,
		Kp2,
		Kp3,
		Kp0,
		KpDot,

		End = Kp1,
		Down,
		PgDn,
		Ins,
		Del,

		F11 = 57,
		F12,
	};

	static constexpr uint8_t LowerAltCharacterMap[128] = {
		"\0\0"
		"\x01\x02\x03\x04\x05\x06\x07\x08\x09\x0a\x0b\x0c\x0d"
		"\x0e\x0f\x10\x11\x12\x13\x14\x15\x16\x17\x18\x19\x1a"
		"\x27\0"
		"\x1c\x1d\x1e\x1f\x20\x21\x22\x23\x24\x25\x26"
		"\x00\0\x1b"
		"\x28\x29\x2a\x2b\x2c\x2d\x2e\x2f\x30\x31"
	};

	static constexpr uint8_t UpperAltCharacterMap[128] = {
		"\0\0"
		"\x31\x32\x33\x34\x35\x36\x37\x38\x39\x3a\x3b\x3c\x3d"
		"\x3e\x3f\x40\x41\x42\x43\x44\x45\x46\x47\x48\x49\x4a"
		"\x57\0"
		"\x4c\x4d\x4e\x4f\x50\x51\x52\x53\x54\x55\x56"
		"\x30\0\x4b"
		"\x58\x59\x5a\x5b\x5c\x5d\x5e\x5f\x60\x61"
	};

	static constexpr uint8_t LowerCharacterMap[128] = {
		"\0\x1b"
		"1234567890-=\b"
		"\tqwertyuiop[]"
		"\n\0"
		"asdfghjkl;'"
		"`\0\\"
		"zxcvbnm,./"
		"\0*\0 \0"
		"\0\0\0\0\0\0\0\0\0\0"
		"\0\0"
		"789-456+1230."
	};

	static constexpr uint8_t UpperCharacterMap[128] = {
		"\0\x1b"
		"!@#$%^&*()_+\b"
		"\tQWERTYUIOP{}"
		"\n\0"
		"ASDFGHJKL:\""
		"~\0|"
		"ZXCVBNM<>?"
		"\0*\0 \0"
		"\0\0\0\0\0\0\0\0\0\0"
		"\0\0"
		"789-456+1230."
	};

	static const uint8_t *CharacterMap[4] = {
		LowerCharacterMap,
		UpperCharacterMap,
		LowerAltCharacterMap,
		UpperAltCharacterMap
	};

	//static constexpr uint8_t
    }
}

#endif //_ARCH_I386_KERNEL_DRIVERS_SRC_PS2_SCAN_CODE_H
