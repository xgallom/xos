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
// Created by Milan Gallo on 24/12/2021.
//

#ifndef _XOS_SRC_STIVALE2_H
#define _XOS_SRC_STIVALE2_H

#include <cstdint>

namespace Stivale_v2::Stivale {
        // Anchor for non ELF kernels
        struct Anchor {
                uint8_t anchor[15];
                uint8_t bits;
                uintptr_t physicalLoadAddress,
                        physicalBssStart,
                        physicalBssEnd,
                        physicalHeader;
        };

        template<typename Id>
        struct Tag {
                Id id;
                Tag<Id> *next;
        };

        /* --- Header --------------------------------------------------------------- */
        /*  Information passed from the kernel to the bootloader                      */
        namespace Header {
                namespace TagId {
                        enum Enum : uint64_t {
                                Null = 0u,
                                AnyVideo = 0xc75c9fa92a44c4dbu,
                                Framebuffer = 0x3ecc1bc43d0f7971u,
                                MTRR = 0x4c7bb07731282e00u,
                                SlideHHDM = 0xdc29269c2af53d1du,
                                Terminal = 0xa85d499b1823be72u,
                                SMP = 0x1ab015085f3273dfu,
                                Paging5Lvl = 0x932f477032007e8fu,
                                UnmapNull = 0x92919432b16fe7e7u,
                        };
                }

                using Tag = Stivale::Tag<TagId::Enum>;

                struct Struct {
                        uintptr_t entryPoint;
                        uintptr_t stack;
                        uint64_t flags;
                        Tag *tags;
                };

                struct AnyVideo {
                        Tag tag;
                        uint64_t preference;
                };

                struct Framebuffer {
                        Tag tag;
                        uint16_t framebufferWidth;
                        uint16_t framebufferHeight;
                        uint16_t framebufferBitsPerPixel;
                        uint16_t unused = 0;
                };

                using MTRR = Tag;

                struct SlideHHDM {
                        Tag tag;
                        uint64_t flags;
                        uint64_t alignment;
                };

                struct Terminal {
                        enum CallbackType : uint64_t {
                                DEC = 10u,
                                Bell = 20u,
                                PrivateId = 30u,
                                StatusReport = 40u,
                                PositionReport = 50u,
                                KeyboardLeds = 60u,
                                Mode = 70u,
                                Linux = 80u,
                        };

                        using Callback = void (CallbackType, uint64_t, uint64_t, uint64_t);

                        Tag tag;
                        uint64_t flags;
                        Callback *callback = nullptr;
                };

                struct SMP {
                        Tag tag;
                        uint64_t flags;
                };

                using Paging5Lvl = Tag;
                using UnmapNull = Tag;
        }

        /* --- Struct --------------------------------------------------------------- */
        /*  Information passed from the bootloader to the kernel                      */
        namespace Struct {
                namespace TagId {
                        enum Enum : uint64_t {
                                Null = 0u,
                                PMRs = 0x5df266a64047b6bdu,
                                KernelBaseAddress = 0x060d78874a2a8af0u,
                                CommandLine = 0xe5e76a1b4597a781u,
                                MemoryMap = 0x2187f79e8612de07u,
                                Framebuffer = 0x506461d2950408fau,
                                EDID = 0x968609d7af96b845u,
                                TextMode = 0x38d74c23e0dca893u,
                                MTRR = 0x6bc1a78ebe871172u,
                                Terminal = 0xc2b3f4c3233b0974u,
                                Modules = 0x4b6fe466aade04ceu,
                                RSDP = 0x9e1786930a375e78u,
                                Epoch = 0x566a7bed888e1407u,
                                Firmware = 0x359d837855e3858cu,
                                EFISystemTable = 0x4bc5ec15845b558eu,
                                KernelFile = 0xe599d90c2975584au,
                                KernelFile_v2 = 0x37c13018a02c6ea2u,
                                BootVolume = 0x9b4358364c19ee62u,
                                KernelSlide = 0xee80847d01506c57u,
                                SMBIOS = 0x274bd246c62bf7d1u,
                                SMP = 0x34d1d96339647025u,
                                PXEServerInfo = 0x29d1e96239247032u,
                                MMIO32UART = 0xb813f9b8dbc78797u,
                                DeviceTreeBlob = 0xabb29bd49a2833fau,
                                HHDM = 0xb0ed257db18cb58f,
                        };
                }

                using Tag = Stivale::Tag<TagId::Enum>;

                struct Struct {
                        static constexpr size_t
                                BrandLength = 64u,
                                VersionLength = 64u;

                        char
                                brand[BrandLength],
                                version[VersionLength];
                        Tag *tags;
                };

                struct PMR {
                        enum Permission : uint64_t {
                                Executable = 1u << 0u,
                                Writable = 1u << 1u,
                                Readable = 1u << 2u,
                        };

                        uintptr_t base;
                        size_t length;
                        uint64_t permissions;
                };
                struct PMRs {
                        Tag tag;
                        size_t length;
                        PMR items[];
                };

                struct KernelBaseAddress {
                        Tag tag;
                        uintptr_t physicalBaseAddress;
                        uintptr_t virtualBaseAddress;
                };

                struct CommandLine {
                        Tag tag;
                        uint64_t commandLine;
                };

                struct MemoryMapEntry {
                        enum Type : uint32_t {
                                Usable = 1,
                                Reserved = 2,
                                ACPIReclaimable = 3,
                                ACPINvs = 4,
                                BadMemory = 5,
                                BootloaderReclaimable = 0x1000,
                                Kernel = 0x1001,
                                Framebuffer = 0x1002,
                        };

                        uintptr_t base;
                        size_t length;
                        Type type;
                        uint32_t unused;
                };

                struct MemoryMap {
                        Tag tag;
                        size_t length;
                        MemoryMapEntry items[];
                };

                struct Framebuffer {
                        enum MemoryModel : uint8_t {
                                RGB = 1u,
                        };

                        Tag tag;
                        uint64_t framebufferAddress;
                        uint16_t framebufferWidth;
                        uint16_t framebufferHeight;
                        uint16_t framebufferPitch;
                        uint16_t framebufferBitsPerPixel;
                        MemoryModel memoryModel;
                        uint8_t redMaskSize;
                        uint8_t redMaskShift;
                        uint8_t greenMaskSize;
                        uint8_t greenMaskShift;
                        uint8_t blueMaskSize;
                        uint8_t blueMaskShift;
                        uint8_t unused;
                };

                struct EDID {
                        Tag tag;
                        size_t length;
                        uint8_t data[];
                };

                struct TextMode {
                        Tag tag;
                        uintptr_t address;
                        uint16_t unused;
                        uint16_t rows;
                        uint16_t cols;
                        uint16_t bytesPerChar;
                };

                using MTRR = Tag;

                struct Terminal {
                        using TermWrite = void(const char *string, size_t length);

                        Tag tag;
                        uint32_t flags;
                        uint16_t cols;
                        uint16_t rows;
                        TermWrite *write;
                        uint64_t maxLength;
                };

                struct Module {
                        static constexpr size_t StringSize = 128u;

                        uintptr_t begin;
                        uintptr_t end;
                        char string[StringSize];
                };
                struct Modules {
                        Tag tag;
                        size_t length;
                        Module items[];
                };

                struct RSDP {
                        Tag tag;
                        uint64_t rsdp;
                };

                struct Epoch {
                        Tag tag;
                        uint64_t epoch;
                };

                struct Firmware {
                        enum Flags : uint64_t {
                                UEFI = 0u,
                                Bios,
                        };

                        Tag tag;
                        Flags flags;
                };

                struct EFISystemTable {
                        Tag tag;
                        uintptr_t systemTable;
                };

                struct KernelFile {
                        Tag tag;
                        uintptr_t kernelFile;
                };
                struct KernelFile_v2 {
                        Tag tag;
                        uintptr_t kernelFile;
                        size_t kernelSize;
                };

                struct GUID {
                        uint32_t a;
                        uint16_t b;
                        uint16_t c;
                        uint8_t d[8];
                };
                struct BootVolume {
                        Tag tag;
                        uint64_t flags;
                        GUID guid;
                        GUID part_guid;
                };

                struct KernelSlide {
                        Tag tag;
                        uint64_t kernelSlide;
                };

                struct SMBIOS {
                        Tag tag;
                        uint64_t flags;
                        uint64_t smbiosEntry32;
                        uint64_t smbiosEntry64;
                };

                struct SMPInfo {
                        uint32_t processorId;
                        uint32_t lapicId;
                        uint64_t targetStack;
                        uintptr_t gotoAddress;
                        uint64_t extraArgument;
                };
                struct SMP {
                        Tag tag;
                        uint64_t flags;
                        uint32_t bspLapicId;
                        uint32_t unused;
                        uint64_t cpuCount;
                        SMPInfo smpInfo[];
                };

                struct PXEServerInfo {
                        Tag tag;
                        uint32_t serverIp;
                };

                struct MMIO32UART {
                        Tag tag;
                        uintptr_t address;
                };

                struct DeviceTreeBlob {
                        Tag tag;
                        uintptr_t address;
                        size_t size;
                };

                struct HHDM {
                        Tag tag;
                        uintptr_t address;
                };
        }
}

#define STIVALE2_TERM_CTX_SIZE ((uint64_t)(-1))
#define STIVALE2_TERM_CTX_SAVE ((uint64_t)(-2))
#define STIVALE2_TERM_CTX_RESTORE ((uint64_t)(-3))
#define STIVALE2_TERM_FULL_REFRESH ((uint64_t)(-4))

#endif //_XOS_SRC_STIVALE2_H
