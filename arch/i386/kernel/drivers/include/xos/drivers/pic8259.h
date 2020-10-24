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
// Created by xgallom on 10/19/20.
//

#ifndef _ARCH_I386_KERNEL_DRIVERS_PIC_INCLUDE_XOS_DRIVERS_PIC_H
#define _ARCH_I386_KERNEL_DRIVERS_PIC_INCLUDE_XOS_DRIVERS_PIC_H

#include <stdint.h>
#include <xos/extern-c.h>
#include <xos/interrupt-handler.h>

namespace pic8259 {
    static constexpr uint8_t
	    IrqBase = 0x20,
	    DeviceIrqCount = 8,
	    IrqCount = 2 * DeviceIrqCount;

    namespace IRQ {
	enum Enum : uint8_t {
		Timer = IrqBase,
		Keyboard,
		Cascade,
		SerialPort2,
		SerialPort1,
		ParallelPort2,
		Floppy,
		ParallelPort1,

		RTC,
		ACPI,
		Open10,
		Open11,
		Mouse,
		CoProcessor,
		ATA,
		ATA2
	};
    }

    namespace Device {
	enum Enum : uint8_t {
		Master,
		Slave,

		Count
	};
    }

    bool initialize();

    void __registerHandlerUnsafe(IRQ::Enum irq, InterruptHandler *handler);
    void registerHandler(IRQ::Enum irq, InterruptHandler *handler);

    void enableInterrupt(IRQ::Enum irq);
    void disableInterrupt(IRQ::Enum irq);

    void sendEndOfInterrupt(uint8_t irq);
}

_EXT_C
void pic8259_sendEndOfInterrupt(uint32_t irq, uint32_t error);
_EXT_C_END

#endif //_ARCH_I386_KERNEL_DRIVERS_PIC_INCLUDE_XOS_DRIVERS_PIC_H
