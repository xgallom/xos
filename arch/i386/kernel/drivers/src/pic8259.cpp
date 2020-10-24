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

#include <xos/drivers/pic8259.h>
#include <xos/port.h>
#include <xos/descriptors/idt.h>
#include <xos/stdio.h>

namespace pic8259 {
    struct DeviceConfig {
	    uint16_t commandPort, dataPort;
    };

    static constexpr DeviceConfig Config[Device::Count] = {
	    {.commandPort = 0x20, .dataPort = 0x21},
	    {.commandPort = 0xa0, .dataPort = 0xa1},
    };

    namespace PicCommand {
	enum Enum : uint8_t {
		StartInit = 0x11,
		EndOfInterrupt = 0x20,
	};
    }

    namespace PicData {
	static constexpr uint8_t
		Mode8086 = 0x01,
		ModeAuto = 0x02,

		CascadeIdentity = 0x02,
		SlaveAtIrq2 = 0x04;
    }

    [[nodiscard]]
    static constexpr Device::Enum deviceId(uint8_t irq)
    {
	    return irq >= DeviceIrqCount ? Device::Slave : Device::Master;
    }

    template<Device::Enum Device>
    static void writeCommand(PicCommand::Enum command)
    {
	    outb(Config[Device].commandPort, command);
	    ioWait();
    }

    static void writeCommands(PicCommand::Enum command)
    {
	    writeCommand<Device::Master>(command);
	    writeCommand<Device::Slave>(command);
    }

    template<Device::Enum Device>
    static void writeData(uint8_t data)
    {
	    outb(Config[Device].dataPort, data);
	    ioWait();
    }

    static void writeData(uint8_t masterData, uint8_t slaveData)
    {
	    writeData<Device::Master>(masterData);
	    writeData<Device::Slave>(slaveData);
    }

    template<Device::Enum Device>
    static uint8_t readData()
    {
	    return inb(Config[Device].dataPort);
    }

    bool initialize()
    {
	    const uint8_t
		    masterData = readData<Device::Master>(),
		    slaveData = readData<Device::Slave>();

	    writeCommands(PicCommand::StartInit);
	    writeData(IrqBase, IrqBase + DeviceIrqCount);
	    writeData(PicData::SlaveAtIrq2, PicData::CascadeIdentity);
	    writeData(PicData::Mode8086, PicData::Mode8086);

	    writeData(masterData, slaveData);

	    for (auto irq = IrqBase; irq < IrqBase + IrqCount; ++irq)
		    idt::__registerHandlerUnsafe(
			    irq,
			    pic8259_sendEndOfInterrupt
		    );

	    disableInterrupt(IRQ::Timer);

	    xos::printf("Initialized pic8259\n");
	    return true;
    }

    void __registerHandlerUnsafe(IRQ::Enum irq, InterruptHandler *handler)
    {
	    idt::__registerHandlerUnsafe(irq, handler);
    }

    void registerHandler(IRQ::Enum irq, InterruptHandler *handler)
    {
	    idt::registerHandler(irq, handler);
    }

    void enableInterrupt(IRQ::Enum irq)
    {
	    uint8_t offset = irq - IrqBase;

	    const auto device = deviceId(offset);
	    if (device)
		    offset -= 8;

	    rstb(Config[device].dataPort, 1u << offset);
    }

    void disableInterrupt(IRQ::Enum irq)
    {
	    uint8_t offset = irq - IrqBase;

	    const auto device = deviceId(offset);
	    if (device)
		    offset -= 8;

	    setb(Config[device].dataPort, 1u << offset);
    }

    void sendEndOfInterrupt(uint8_t irq)
    {
	    if (deviceId(irq - IrqBase))
		    writeCommand<Device::Slave>(PicCommand::EndOfInterrupt);
	    writeCommand<Device::Master>(PicCommand::EndOfInterrupt);
    }
}

void pic8259_sendEndOfInterrupt(uint32_t irq, uint32_t)
{
	xos::printf("pic[{x}]: default\n", uint8_t(irq));
	pic8259::sendEndOfInterrupt(irq);
}
