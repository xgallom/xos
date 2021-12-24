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
                        uintptr_t entry_point;
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
                        uint16_t framebuffer_width;
                        uint16_t framebuffer_height;
                        uint16_t framebuffer_bpp;
                        uint16_t unused = 0;
                };

                using MTRR = Tag;

                struct SlideHHDM {
                        Tag tag;
                        uint64_t flags;
                        uint64_t alignment;
                };

                struct Terminal {
                        Tag tag;
                        uint64_t flags;
                        uintptr_t callback = 0;
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
                        uint64_t base;
                        uint64_t length;
                        uint64_t permissions;
                };
                struct PMRs {
                        Tag tag;
                        uint64_t entries;
                        PMR pmrs[];
                };

                struct KernelBaseAddress {
                        Tag tag;
                        uint64_t physicalBaseAddress;
                        uint64_t virtualBaseAddress;
                };

                struct CommandLine {
                        Tag tag;
                        uint64_t commandLine;
                };

                struct MemoryMapEntry {
                        uint64_t base;
                        uint64_t length;
                        uint32_t type;
                        uint32_t unused;
                };

                struct MemoryMap {
                        Tag tag;
                        uint64_t entries;
                        MemoryMapEntry memmap[];
                };

                struct Framebuffer {
                        Tag tag;
                        uint64_t framebuffer_addr;
                        uint16_t framebuffer_width;
                        uint16_t framebuffer_height;
                        uint16_t framebuffer_pitch;
                        uint16_t framebuffer_bpp;
                        uint8_t memory_model;
                        uint8_t red_mask_size;
                        uint8_t red_mask_shift;
                        uint8_t green_mask_size;
                        uint8_t green_mask_shift;
                        uint8_t blue_mask_size;
                        uint8_t blue_mask_shift;
                        uint8_t unused;
                };

                struct EDID {
                        Tag tag;
                        uint64_t edid_size;
                        uint8_t edid_information[];
                };

                struct TextMode {
                        Tag tag;
                        uint64_t address;
                        uint16_t unused;
                        uint16_t rows;
                        uint16_t cols;
                        uint16_t bytes_per_char;
                };

                using MTRR = Tag;

                struct Terminal {
                        using TermWrite = void (const char *string, size_t length);

                        Tag tag;
                        uint32_t flags;
                        uint16_t cols;
                        uint16_t rows;
                        TermWrite *term_write;
                        uint64_t max_length;
                };

                struct Module {
                        static constexpr size_t StringSize = 128u;

                        uint64_t begin;
                        uint64_t end;
                        char string[StringSize];
                };
                struct Modules {
                        Tag tag;
                        uint64_t module_count;
                        Module modules[];
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
                        Tag tag;
                        uint64_t flags;
                };

                struct EFISystemTable {
                        Tag tag;
                        uint64_t system_table;
                };

                struct KernelFile {
                        Tag tag;
                        uint64_t kernel_file;
                };
                struct KernelFile_v2 {
                        Tag tag;
                        uint64_t kernel_file;
                        uint64_t kernel_size;
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
                        uint64_t kernel_slide;
                };

                struct SMBIOS {
                        Tag tag;
                        uint64_t flags;
                        uint64_t smbios_entry_32;
                        uint64_t smbios_entry_64;
                };

                struct SMPInfo {
                        uint32_t processor_id;
                        uint32_t lapic_id;
                        uint64_t target_stack;
                        uint64_t goto_address;
                        uint64_t extra_argument;
                };
                struct SMP {
                        Tag tag;
                        uint64_t flags;
                        uint32_t bsp_lapic_id;
                        uint32_t unused;
                        uint64_t cpu_count;
                        SMPInfo smp_info[];
                };

                struct PXEServerInfo {
                        Tag tag;
                        uint32_t server_ip;
                };

                struct MMIO32UART {
                        Tag tag;
                        uint64_t addr;
                };

                struct DeviceTreeBlob {
                        Tag tag;
                        uint64_t addr;
                        uint64_t size;
                };

                struct HHDM {
                        Tag tag;
                        uint64_t addr;
                };
        }
}

#define STIVALE2_TERM_CB_DEC 10
#define STIVALE2_TERM_CB_BELL 20
#define STIVALE2_TERM_CB_PRIVATE_ID 30
#define STIVALE2_TERM_CB_STATUS_REPORT 40
#define STIVALE2_TERM_CB_POS_REPORT 50
#define STIVALE2_TERM_CB_KBD_LEDS 60
#define STIVALE2_TERM_CB_MODE 70
#define STIVALE2_TERM_CB_LINUX 80

#define STIVALE2_TERM_CTX_SIZE ((uint64_t)(-1))
#define STIVALE2_TERM_CTX_SAVE ((uint64_t)(-2))
#define STIVALE2_TERM_CTX_RESTORE ((uint64_t)(-3))
#define STIVALE2_TERM_FULL_REFRESH ((uint64_t)(-4))


#define STIVALE2_PMR_EXECUTABLE ((uint64_t)1 << 0)
#define STIVALE2_PMR_WRITABLE   ((uint64_t)1 << 1)
#define STIVALE2_PMR_READABLE   ((uint64_t)1 << 2)

#define STIVALE2_MMAP_USABLE                 1
#define STIVALE2_MMAP_RESERVED               2
#define STIVALE2_MMAP_ACPI_RECLAIMABLE       3
#define STIVALE2_MMAP_ACPI_NVS               4
#define STIVALE2_MMAP_BAD_MEMORY             5
#define STIVALE2_MMAP_BOOTLOADER_RECLAIMABLE 0x1000
#define STIVALE2_MMAP_KERNEL_AND_MODULES     0x1001
#define STIVALE2_MMAP_FRAMEBUFFER            0x1002

#define STIVALE2_FBUF_MMODEL_RGB 1

#define STIVALE2_FIRMWARE_BIOS (1 << 0)

#endif //_XOS_SRC_STIVALE2_H
