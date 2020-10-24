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
// Created by xgallom on 10/23/20.
//

#include <xos/drivers/ps2/keyboard.h>
#include <xos/containers/circular-buffer.h>
#include <xos/drivers/pic8259.h>
#include <xos/stdio.h>
#include <xos/interrupt-enable.h>
#include <xos/port.h>
#include <xos/atomic.h>
#include "scan-code.h"

namespace ps2::kbd {
    static xos::CircularBuffer<uint8_t, 10> s_scanCodeBuffer;
    static xos::InterruptAccessFlag s_accessFlag;
    static uint32_t s_state = 0;

    static uint8_t popScanCode()
    {
	    InterruptGuard interruptGuard;

	    s_accessFlag.waitForAccess();
	    const auto result = s_scanCodeBuffer.pop();

	    if (result) {
		    __atomic_signal_fence(__ATOMIC_RELEASE);
		    return result();
	    }

	    return 0;
    }

    static uint8_t pollScanCode()
    {
	    uint8_t result;

	    while (!(result = popScanCode()))
		    halt();

	    return result;
    }

    static int processScanCode(uint8_t scanCode)
    {
	    const bool
		    isReleased = scanCode & 0x80u,
		    e0 = State::get<State::GotE0>(s_state);
	    const uint8_t
		    alt = State::get<State::LAlt, State::RAlt>(s_state),
		    shift = State::get<State::LShift, State::RShift>(s_state),
		    scanIndex = scanCode & ~0x80u;

	    int result = 0;

	    if (scanCode == 0xe0)
		    State::set<State::GotE0>(s_state);
	    else if (!isReleased) {
		    switch (scanIndex) {
		    case ScanCode::LShift:
			    State::set<State::LShift>(s_state);
			    break;
		    case ScanCode::RShift:
			    State::set<State::RShift>(s_state);
			    break;
		    case ScanCode::LAlt:
			    if (e0)
				    State::set<State::RAlt>(s_state);
			    else
				    State::set<State::LAlt>(s_state);
			    break;
		    case ScanCode::LCtrl:
			    if (State::get<State::GotE0>(s_state))
				    State::set<State::RCtrl>(s_state);
			    else
				    State::set<State::LCtrl>(s_state);
			    break;
		    default:
			    result = ScanCode::CharacterMap[
				    (alt << 1u) | shift
			    ][scanIndex];

			    if (result)
				    result |= s_state << 8u;
			    break;
		    }

		    State::rst<State::GotE0>(s_state);
	    }
	    else {
		    switch (scanIndex) {
		    case ScanCode::LShift:
			    State::rst<State::LShift>(s_state);
			    break;
		    case ScanCode::RShift:
			    State::rst<State::RShift>(s_state);
			    break;
		    case ScanCode::LAlt:
			    if (e0)
				    State::rst<State::RAlt>(s_state);
			    else
				    State::rst<State::LAlt>(s_state);
			    break;
		    case ScanCode::LCtrl:
			    if (State::get<State::GotE0>(s_state))
				    State::rst<State::RCtrl>(s_state);
			    else
				    State::rst<State::LCtrl>(s_state);
			    break;
		    }

		    State::rst<State::GotE0>(s_state);
	    }

//	    xos::printf("{x} {b} {x}\n", scanCode, s_state, result);
	    return result;
    }

    static void keyboardInterrupt(uint32_t irq, uint32_t)
    {
	    if (irq != pic8259::IRQ::Keyboard)
		    return;

	    s_accessFlag.acquire();
	    {
		    InterruptGuard interruptGuard;

		    const auto scanCode = inb(0x60);
		    if (!s_scanCodeBuffer.push(scanCode))
			    xos::write("KBI: Scan code buffer full\n");
		    /*else
			    xos::printf(" {x} ", scanCode);*/
	    }
	    s_accessFlag.publish();

	    pic8259::sendEndOfInterrupt(irq);
    }

    bool initialize()
    {
	    pic8259::__registerHandlerUnsafe(pic8259::IRQ::Keyboard,
					     keyboardInterrupt);

	    xos::printf("Initialized ps2/keyboard\n");
	    return true;
    }

    int getchar()
    {
	    char result;
	    while (!(result = processScanCode(pollScanCode()))) {}
	    return result;
    }

}
