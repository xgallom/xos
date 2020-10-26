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
// Created by xgallom on 5/29/20.
//

#include <xos/multiboot/write.h>

namespace xos {
    bool write(const char *,
	       mbt::Multiboot::Header arg,
	       const char *&errorBuffer,
	       size_t formatLength,
	       size_t offsetIndex)
    {
	    if (formatLength) {
		    errorBuffer = "mbt::Multiboot::Header expects no format";
		    return false;
	    }

	    const auto offset = TabOffset(offsetIndex);
	    const auto newOffset = TabOffset(offsetIndex + 1);

	    xos::write("xos::Multiboot::Header{\n");

	    xos::write(newOffset);
	    xos::write("flags: ");
	    xos::writeBin(arg.flags);
	    xos::write("\n");

	    xos::write(offset);
	    xos::write("}");

	    return true;
    }

    bool write(const char *,
	       mbt::Multiboot::Memory arg,
	       const char *&errorBuffer,
	       size_t formatLength,
	       size_t offsetIndex)
    {
	    if (formatLength) {
		    errorBuffer = "mbt::Multiboot::Memory expects no format";
		    return false;
	    }

	    const auto offset = TabOffset(offsetIndex);
	    const auto newOffset = TabOffset(offsetIndex + 1);

	    xos::write("mbt::Multiboot::Memory{\n");

	    xos::write(newOffset);
	    xos::write("lower: ");
	    xos::writeHex(arg.lower);
	    xos::write("\n");

	    xos::write(newOffset);
	    xos::write("upper: ");
	    xos::writeHex(arg.upper);
	    xos::write("\n");

	    xos::write(offset);
	    xos::write("}");

	    return true;
    }

    bool write(const char *,
	       mbt::Multiboot::Boot arg,
	       const char *&errorBuffer,
	       size_t formatLength,
	       size_t offsetIndex)
    {
	    if (formatLength) {
		    errorBuffer = "mbt::Multiboot::Boot expects no format";
		    return false;
	    }

	    const auto offset = TabOffset(offsetIndex);
	    const auto newOffset = TabOffset(offsetIndex + 1);

	    xos::write("mbt::Multiboot::Boot{\n");

	    xos::write(newOffset);
	    xos::write("drive: ");
	    xos::writeHex(arg.drive);
	    xos::write("\n");

	    xos::write(newOffset);
	    xos::write("partition: ");

	    auto partition = arg.partition + 2, end = arg.partition - 1;
	    xos::writeHex(*partition--);
	    while (partition > end) {
		    xos::write(":");
		    xos::writeHex(*partition--);
	    }
	    xos::write("\n");

	    xos::write(offset);
	    xos::write("}");

	    return true;
    }

    bool write(const char *,
	       mbt::Multiboot::Command arg,
	       const char *&errorBuffer,
	       size_t formatLength,
	       size_t offsetIndex)
    {
	    if (formatLength) {
		    errorBuffer = "mbt::Multiboot::Command expects no format";
		    return false;
	    }

	    const auto offset = TabOffset(offsetIndex);
	    const auto newOffset = TabOffset(offsetIndex + 1);

	    xos::write("mbt::Multiboot::Command{\n");

	    xos::write(newOffset);
	    xos::write("line: ");
	    xos::write(arg.line);
	    xos::write("\n");

	    xos::write(offset);
	    xos::write("}");

	    return true;
    }

    bool write(const char *,
	       mbt::Multiboot::Modules arg,
	       const char *&errorBuffer,
	       size_t formatLength,
	       size_t offsetIndex)
    {
	    if (formatLength) {
		    errorBuffer = "mbt::Multiboot::Modules expects no format";
		    return false;
	    }

	    const auto offset = TabOffset(offsetIndex);
	    const auto newOffset = TabOffset(offsetIndex + 1);

	    xos::write("mbt::Multiboot::Modules{\n");

	    xos::write(newOffset);
	    xos::write("count: ");
	    xos::writeDec(arg.count);
	    xos::write("\n");

	    xos::write(newOffset);
	    xos::write("address: ");
	    xos::writeHex(arg.address);
	    xos::write("\n");

	    xos::write(offset);
	    xos::write("}");

	    return true;
    }

    bool write(const char *,
	       mbt::Multiboot::Symbols arg,
	       const char *&errorBuffer,
	       size_t formatLength,
	       size_t offsetIndex)
    {
	    if (formatLength) {
		    errorBuffer = "mbt::Multiboot::Symbols expects no format";
		    return false;
	    }

	    const auto offset = TabOffset(offsetIndex);
	    const auto newOffset = TabOffset(offsetIndex + 1);

	    xos::write("mbt::Multiboot::Symbols{\n");

	    xos::write(newOffset);
	    xos::write("number: ");
	    xos::writeDec(arg.number);
	    xos::write("\n");

	    xos::write(newOffset);
	    xos::write("size: ");
	    xos::writeDec(arg.size);
	    xos::write("\n");

	    xos::write(newOffset);
	    xos::write("address: ");
	    xos::writeHex(arg.address);
	    xos::write("\n");

	    xos::write(newOffset);
	    xos::write("sectionHeader: ");
	    xos::writeHex(arg.sectionHeader);
	    xos::write("\n");

	    xos::write(offset);
	    xos::write("}");

	    return true;
    }

    bool write(const char *,
	       mbt::Multiboot::MemoryMap arg,
	       const char *&errorBuffer,
	       size_t formatLength,
	       size_t offsetIndex)
    {
	    if (formatLength) {
		    errorBuffer = "mbt::Multiboot::MemoryMap expects no format";
		    return false;
	    }

	    const auto offset = TabOffset(offsetIndex);
	    const auto newOffset = TabOffset(offsetIndex + 1);

	    xos::write("mbt::Multiboot::MemoryMap{\n");

	    xos::write(newOffset);
	    xos::write("length: ");
	    xos::writeDec(arg.length);
	    xos::write("\n");

	    xos::write(newOffset);
	    xos::write("address: ");
	    xos::writeHex(arg.address);
	    xos::write("\n");

	    xos::write(offset);
	    xos::write("}");

	    return true;
    }

    bool write(const char *,
	       mbt::Multiboot::Drives arg,
	       const char *&errorBuffer,
	       size_t formatLength,
	       size_t offsetIndex)
    {
	    if (formatLength) {
		    errorBuffer = "mbt::Multiboot::Drives expects no format";
		    return false;
	    }

	    const auto offset = TabOffset(offsetIndex);
	    const auto newOffset = TabOffset(offsetIndex + 1);

	    xos::write("mbt::Multiboot::Drives{\n");

	    xos::write(newOffset);
	    xos::write("length: ");
	    xos::writeDec(arg.length);
	    xos::write("\n");

	    xos::write(newOffset);
	    xos::write("address: ");
	    xos::writeHex(arg.address);
	    xos::write("\n");

	    xos::write(offset);
	    xos::write("}");

	    return true;
    }

    bool write(const char *,
	       mbt::Multiboot::Config arg,
	       const char *&errorBuffer,
	       size_t formatLength,
	       size_t offsetIndex)
    {
	    if (formatLength) {
		    errorBuffer = "mbt::Multiboot::Config expects no format";
		    return false;
	    }

	    const auto offset = TabOffset(offsetIndex);
	    const auto newOffset = TabOffset(offsetIndex + 1);

	    xos::write("mbt::Multiboot::Config{\n");

	    xos::write(newOffset);
	    xos::write("table: ");
	    xos::writeHex(arg.table);
	    xos::write("\n");

	    xos::write(offset);
	    xos::write("}");

	    return true;
    }

    bool write(const char *,
	       mbt::Multiboot::BootLoader arg,
	       const char *&errorBuffer,
	       size_t formatLength,
	       size_t offsetIndex)
    {
	    if (formatLength) {
		    errorBuffer = "mbt::Multiboot::BootLoader expects no format";
		    return false;
	    }

	    const auto offset = TabOffset(offsetIndex);
	    const auto newOffset = TabOffset(offsetIndex + 1);

	    xos::write("mbt::Multiboot::BootLoader{\n");

	    xos::write(newOffset);
	    xos::write("name: ");
	    xos::write(arg.name);
	    xos::write("\n");

	    xos::write(offset);
	    xos::write("}");

	    return true;
    }

    bool write(const char *,
	       mbt::Multiboot::ApmTable arg,
	       const char *&errorBuffer,
	       size_t formatLength,
	       size_t offsetIndex)
    {
	    if (formatLength) {
		    errorBuffer = "mbt::Multiboot::ApmTable expects no format";
		    return false;
	    }

	    const auto offset = TabOffset(offsetIndex);
	    const auto newOffset = TabOffset(offsetIndex + 1);

	    xos::write("mbt::Multiboot::ApmTable{\n");

	    xos::write(newOffset);
	    xos::write("address: ");
	    xos::writeHex(arg.address);
	    xos::write("\n");

	    xos::write(offset);
	    xos::write("}");

	    return true;
    }

    bool write(const char *,
	       mbt::Multiboot::Vbe arg,
	       const char *&errorBuffer,
	       size_t formatLength,
	       size_t offsetIndex)
    {
	    if (formatLength) {
		    errorBuffer = "mbt::Multiboot::Vbe expects no format";
		    return false;
	    }

	    const auto offset = TabOffset(offsetIndex);
	    const auto newOffset = TabOffset(offsetIndex + 1);

	    xos::write("mbt::Multiboot::Vbe{\n");

	    xos::write(newOffset);
	    xos::write("controlInfo: ");
	    xos::writeBin(arg.controlInfo);
	    xos::write("\n");

	    xos::write(newOffset);
	    xos::write("modeInfo: ");
	    xos::writeBin(arg.modeInfo);
	    xos::write("\n");

	    xos::write(newOffset);
	    xos::write("mode: ");
	    xos::writeBin(arg.mode);
	    xos::write("\n");

	    xos::write(newOffset);
	    xos::write("interfaceSegment: ");
	    xos::writeHex(arg.interfaceSegment);
	    xos::write("\n");

	    xos::write(newOffset);
	    xos::write("interfaceOffset: ");
	    xos::writeHex(arg.interfaceOffset);
	    xos::write("\n");

	    xos::write(newOffset);
	    xos::write("interfaceLength: ");
	    xos::writeDec(arg.interfaceLength);
	    xos::write("\n");

	    xos::write(offset);
	    xos::write("}");

	    return true;
    }

    bool write(const char *,
	       mbt::Multiboot::FrameBuffer arg,
	       const char *&errorBuffer,
	       size_t formatLength,
	       size_t offsetIndex)
    {
	    if (formatLength) {
		    errorBuffer = "mbt::Multiboot::FrameBuffer expects no format";
		    return false;
	    }

	    const auto offset = TabOffset(offsetIndex);
	    const auto newOffset = TabOffset(offsetIndex + 1);

	    xos::write("mbt::Multiboot::FrameBuffer{\n");

	    xos::write(newOffset);
	    xos::write("address: ");
	    xos::writeHex(arg.address);
	    xos::write("\n");

	    xos::write(newOffset);
	    xos::write("pitch: ");
	    xos::writeDec(arg.pitch);
	    xos::write("\n");

	    xos::write(newOffset);
	    xos::write("width: ");
	    xos::writeDec(arg.width);
	    xos::write("\n");

	    xos::write(newOffset);
	    xos::write("height: ");
	    xos::writeDec(arg.height);
	    xos::write("\n");

	    xos::write(newOffset);
	    xos::write("bpp: ");
	    xos::writeDec(arg.bpp);
	    xos::write("\n");

	    xos::write(newOffset);
	    xos::write("type: ");
	    xos::writeBin(arg.type);
	    xos::write("\n");

	    xos::write(newOffset);
	    xos::write("colorInfo: ");
	    for(const auto colorInfo : arg.colorInfo)
	    	xos::writeHex(colorInfo);
	    xos::write("\n");

	    xos::write(offset);
	    xos::write("}");

	    return true;
    }

    bool write(const char *,
	       const mbt::Multiboot::Object *arg,
	       const char *&errorBuffer,
	       size_t formatLength,
	       size_t offsetIndex)
    {
	    if (formatLength) {
		    errorBuffer = "mbt::Multiboot::Object expects no format";
		    return false;
	    }

	    const auto offset = TabOffset(offsetIndex);
	    const auto newOffsetIndex = offsetIndex + 1;
	    const auto newOffset = TabOffset(newOffsetIndex);

	    xos::write("mbt::Multiboot::Object{\n");

	    xos::write(newOffset);
	    xos::write("header: ");
	    xos::write("", arg->header, errorBuffer, 0, newOffsetIndex);
	    xos::write("\n");

	    const auto flags = arg->header.flags;

	    if (flags & (0x1u << 0u)) {
		    xos::write(newOffset);
		    xos::write("memory: ");
		    xos::write("", arg->memory, errorBuffer, 0, newOffsetIndex);
		    xos::write("\n");
	    }
	    if (flags & (0x1u << 1u)) {
		    xos::write(newOffset);
		    xos::write("boot: ");
		    xos::write("", arg->boot, errorBuffer, 0, newOffsetIndex);
		    xos::write("\n");
	    }
	    if (flags & (0x1u << 2u)) {
		    xos::write(newOffset);
		    xos::write("command: ");
		    xos::write("", arg->command, errorBuffer, 0,
			       newOffsetIndex);
		    xos::write("\n");
	    }
	    if (flags & (0x1u << 3u)) {
		    xos::write(newOffset);
		    xos::write("modules: ");
		    xos::write("", arg->modules, errorBuffer, 0,
			       newOffsetIndex);
		    xos::write("\n");
	    }
	    if (flags & (0x3u << 4u)) {
		    xos::write(newOffset);
		    xos::write("symbols: ");
		    xos::write("", arg->symbols, errorBuffer, 0,
			       newOffsetIndex);
		    xos::write("\n");
	    }
	    if (flags & (0x1u << 6u)) {
		    xos::write(newOffset);
		    xos::write("memoryMap: ");
		    xos::write("", arg->memoryMap, errorBuffer, 0,
			       newOffsetIndex);
		    xos::write("\n");
	    }
	    if (flags & (0x1u << 7u)) {
		    xos::write(newOffset);
		    xos::write("drives: ");
		    xos::write("", arg->drives, errorBuffer, 0,
			       newOffsetIndex);
		    xos::write("\n");
	    }
	    if (flags & (0x1u << 8u)) {
		    xos::write(newOffset);
		    xos::write("config: ");
		    xos::write("", arg->config, errorBuffer, 0,
			       newOffsetIndex);
		    xos::write("\n");
	    }
	    if (flags & (0x1u << 9u)) {
		    xos::write(newOffset);
		    xos::write("bootLoader: ");
		    xos::write("", arg->bootLoader, errorBuffer, 0,
			       newOffsetIndex);
		    xos::write("\n");
	    }
	    if (flags & (0x1u << 10u)) {
		    xos::write(newOffset);
		    xos::write("apmTable: ");
		    xos::write("", arg->apmTable, errorBuffer, 0,
			       newOffsetIndex);
		    xos::write("\n");
	    }
	    if (flags & (0x1u << 11u)) {
		    xos::write(newOffset);
		    xos::write("vbe: ");
		    xos::write("", arg->vbe, errorBuffer, 0,
			       newOffsetIndex);
		    xos::write("\n");
	    }
	    if (flags & (0x1u << 12u)) {
		    xos::write(newOffset);
		    xos::write("frameBuffer: ");
		    xos::write("", arg->frameBuffer, errorBuffer, 0,
			       newOffsetIndex);
		    xos::write("\n");
	    }

	    xos::write(offset);
	    xos::write("}");

	    return true;
    }
}
