pub const c = @cImport({
    @cDefine("LIMINE_API_REVISION", "3");
    @cInclude("limine.h");
});

pub usingnamespace c;

pub const BootloaderInfo = c.limine_bootloader_info_response;
pub const ExeCmdline = c.limine_executable_cmdline_response;

pub const FirmwareType = struct {
    type: c.limine_firmware_type_response,

    const Self = @This();

    pub fn str(self: *const Self) []const u8 {
        return switch (self.type.firmware_type) {
            c.LIMINE_FIRMWARE_TYPE_X86BIOS => "bios32",
            c.LIMINE_FIRMWARE_TYPE_UEFI32 => "uefi32",
            c.LIMINE_FIRMWARE_TYPE_UEFI64 => "uefi64",
            c.LIMINE_FIRMWARE_TYPE_SBI => "sbi",
            else => unreachable,
        };
    }
};

pub const StackSize = c.limine_stack_size_request;
pub const HigherHalfDirectMap = c.limine_hhdm_response;

pub const Framebuffer = struct {
    fb: c.limine_framebuffer,

    const Self = @This();

    pub fn data(self: *const Self) []u8 {
        return @as([*]u8, @ptrCast(self.fb.address))[0..(self.fb.pitch * self.fb.height)];
    }

    pub fn index(self: *const Self, x: u64, y: u64, bytes_per_pixel: usize) usize {
        return y * self.fb.pitch + x * bytes_per_pixel;
    }

    pub fn bytesPerPixel(self: *const Self) usize {
        return self.fb.bpp / @bitSizeOf(u8);
    }

    pub fn videoModes(self: *const Self) []c.limine_video_mode {
        return self.fb.modes.*[0..self.fb.mode_count];
    }

    pub fn sliceFromResponse(response: *const c.limine_framebuffer_response) []const Self {
        return @as([*]const Self, @ptrCast(response.framebuffers.*))[0..response.framebuffer_count];
    }
};

pub const MemMap = struct {
    mm: c.limine_memmap_response,

    const Self = @This();

    pub fn entries(self: *const Self) []const Entry {
        return @as([*]const Entry, @ptrCast(self.mm.entries.*))[0..self.mm.entry_count];
    }

    pub const Entry = struct {
        e: c.limine_memmap_entry,

        pub fn data(self: *const Entry) []u8 {
            return @as([*]u8, @ptrFromInt(self.e.base))[0..self.e.length];
        }

        pub fn entryType(self: *const Entry) Type {
            return @enumFromInt(self.e.type);
        }

        pub const Type = enum(u64) {
            usable = c.LIMINE_MEMMAP_USABLE,
            reserved = c.LIMINE_MEMMAP_RESERVED,
            acpi_recl = c.LIMINE_MEMMAP_ACPI_RECLAIMABLE,
            acpi_nvs = c.LIMINE_MEMMAP_ACPI_NVS,
            bad = c.LIMINE_MEMMAP_BAD_MEMORY,
            boot_recl = c.LIMINE_MEMMAP_BOOTLOADER_RECLAIMABLE,
            exe = c.LIMINE_MEMMAP_EXECUTABLE_AND_MODULES,
            fb = c.LIMINE_MEMMAP_FRAMEBUFFER,

            pub fn str(entry_type: Type) []const u8 {
                return switch (entry_type) {
                    .usable => "usable",
                    .reserved => "reserved",
                    .acpi_recl => "acpi reclaimable",
                    .acpi_nvs => "acpi nvs",
                    .bad => "bad",
                    .boot_recl => "bootloader reclaimable",
                    .exe => "executable and modules",
                    .fb => "framebuffer",
                };
            }
        };
    };
};

pub const File = struct {
    f: c.limine_file,

    const Self = @This();

    pub fn data(self: *const Self) []const u8 {
        return @as([*]u8, @ptrCast(self.f.address))[0..self.f.size];
    }

    pub fn gptDiskUuid(self: *const Self) *const Uuid {
        return @ptrCast(&self.f.gpt_disk_uuid);
    }

    pub fn gptPartUuid(self: *const Self) *const Uuid {
        return @ptrCast(&self.f.gpt_part_uuid);
    }

    pub fn fsPartUuid(self: *const Self) *const Uuid {
        return @ptrCast(&self.f.part_uuid);
    }
};

pub const Uuid = c.limine_uuid;

pub const bootloader_info_request = @extern(*const c.limine_bootloader_info_request, .{ .name = "limine_bootloader_info_request" });
pub const executable_cmdline_request = @extern(*const c.limine_executable_cmdline_request, .{ .name = "limine_executable_cmdline_request" });
pub const firmware_type_request = @extern(*const c.limine_firmware_type_request, .{ .name = "limine_firmware_type_request" });
pub const stack_size_request = @extern(*const c.limine_stack_size_request, .{ .name = "limine_stack_size_request" });
pub const hhdm_request = @extern(*const c.limine_hhdm_request, .{ .name = "limine_hhdm_request" });
pub const framebuffer_request = @extern(*const c.limine_framebuffer_request, .{ .name = "limine_framebuffer_request" });
pub const memmap_request = @extern(*const c.limine_memmap_request, .{ .name = "limine_memmap_request" });
pub const executable_file_request = @extern(*const c.limine_executable_file_request, .{ .name = "limine_executable_file_request" });

pub fn getBootloaderInfo() *const BootloaderInfo {
    return @ptrCast(bootloader_info_request.response);
}

pub fn getExeCmdline() *const ExeCmdline {
    return @ptrCast(executable_cmdline_request.response);
}

pub fn getFirmwareType() *const FirmwareType {
    return @ptrCast(firmware_type_request.response);
}

pub fn getStackSize() *const StackSize {
    return @ptrCast(stack_size_request);
}

pub fn getHHDM() *const HigherHalfDirectMap {
    return @ptrCast(hhdm_request.response);
}

pub fn getFramebuffers() []const Framebuffer {
    return Framebuffer.sliceFromResponse(framebuffer_request.response);
}

pub fn getMemMap() *const MemMap {
    return @ptrCast(memmap_request.response);
}

pub fn getExeFile() *const File {
    return @ptrCast(executable_file_request.response.*.executable_file);
}
