const std = @import("std");
const assert = std.debug.assert;

const limine = @import("ext/limine.zig");

const tty = @import("tty.zig");

pub const std_options = std.Options{
    .logFn = logFn,
};

var logFn_buf: [64 << 10]u8 = undefined;

var logFn_y: usize = 0;
pub fn logFn(
    comptime message_level: std.log.Level,
    comptime scope: @TypeOf(.enum_literal),
    comptime format: []const u8,
    args: anytype,
) void {
    _ = scope;
    const buf = &logFn_buf;
    const result = std.fmt.bufPrint(buf, "[" ++ message_level.asText() ++ "]: " ++ format ++ "\n", args) catch return;
    putStr(.{ .x = 0, .y = logFn_y }, result);
    logFn_y += 1;
}

const dummy_allocator = std.mem.Allocator{
    .ptr = undefined,
    .vtable = &std.mem.Allocator.VTable{
        .alloc = alloc,
        .resize = resize,
        .remap = remap,
        .free = free,
    },
};

const Coords = tty.Coords;
const Color = tty.Color;
const default_font_map = tty.default_font_map;
const font_file_data = tty.font_file_data;
const getFramebuffers = limine.getFramebuffers;

const rect = tty.default_char_res;
const blue = Color{ .b = 0xff };
const magenta = Color{ .b = 0xff, .r = 0xff };
const default_color = magenta;

export fn _start() callconv(.Naked) noreturn {
    // asm volatile (
    //     \\ bl %[start:P]
    //     :
    //     : [start] "X" (&main),
    // );
    asm volatile (
        \\ pushq %%rbp
        \\ jmp %[start:P]
        :
        : [start] "X" (&main),
    );
}

const Scope = enum { scope0 };
fn main() noreturn {
    tty.initSystem();
    const t = tty.get(0);

    t.putStr("XOS\n");

    t.putStr("bootloader: ");
    t.putStrz(limine.getBootloaderInfo().name);
    t.putStr(" v");
    t.putStrz(limine.getBootloaderInfo().version);
    t.putStr("\n");

    t.putStr("cmdline: \"");
    t.putStrz(limine.getExeCmdline().cmdline);
    t.putStr("\"\n");

    t.putStr("firmware type: ");
    t.putStr(limine.getFirmwareType().str());
    t.putStr("\n");

    t.putStr("stack size: ");
    t.putMemoryUnits(u64, limine.getStackSize().stack_size);
    t.putStr("\n");

    t.putStr("kernel virtual address: 0x");
    t.putHexAlign(u64, limine.getHHDM().offset);
    t.putStr("\n");

    t.putStr("framebuffers:\n");
    for (limine.getFramebuffers(), 0..) |*fb, n| {
        t.putStr(" [");
        t.putNumber(usize, n);
        t.putStr("] ");
        t.putNumber(u64, fb.fb.width);
        t.putStr("x");
        t.putNumber(u64, fb.fb.height);
        t.putStr(" @ ");
        t.putNumber(u64, fb.fb.bpp);
        t.putStr("bpp, tty ");
        t.putNumber(u64, t.screen_size.x);
        t.putStr("x");
        t.putNumber(u64, t.screen_size.y);
        t.putStr("\n");

        // for (fb.videoModes()) |*vm| {
        //     if (vm.width == 0 or vm.height == 0) continue;
        //     t.putStr("  - ");
        //     t.putNumber(u64, vm.width);
        //     t.putStr("x");
        //     t.putNumber(u64, vm.height);
        //     t.putStr(" @ ");
        //     t.putNumber(u64, vm.bpp);
        //     t.putStr("bpp\n");
        // }
    }

    const exe_file = limine.getExeFile();
    t.putStr("executable: ");
    t.putStrz(exe_file.f.path);
    t.putStr(" at 0x");
    t.putHexAlign(usize, @intFromPtr(exe_file.f.address));
    t.putStr(", size ");
    t.putMemoryUnits(u64, exe_file.f.size);
    t.putStr("\n");
    t.putStr(" mbr_disk=");
    t.putNumber(u32, exe_file.f.mbr_disk_id);
    t.putStr(" mbr_part=");
    t.putNumber(u32, exe_file.f.partition_index);
    t.putStr("\n");
    t.putStr(" gpt_disk=");
    t.putUuid(limine.Uuid, exe_file.gptDiskUuid());
    t.putStr("\n");
    t.putStr(" gpt_part=");
    t.putUuid(limine.Uuid, exe_file.gptPartUuid());
    t.putStr("\n");
    t.putStr(" fs_part=");
    t.putUuid(limine.Uuid, exe_file.fsPartUuid());
    t.putStr("\n");

    t.putStr("memory mapping:\n");
    for (limine.getMemMap().entries(), 0..) |*entry, n| {
        _ = &entry;
        t.putStr(" [");
        t.putNumberAlign(usize, n, 2);
        t.putStr("] {");
        t.putHexAlign(u64, entry.e.base);
        t.putStr("-");
        t.putHexAlign(u64, entry.e.base + entry.e.length);
        t.putStr("} size ");
        t.putMemoryUnits(u64, entry.e.length);
        t.putStr(", ");
        t.putStr(entry.entryType().str());
        t.putStr("\n");
    }

    const scroll = true;

    if (scroll) {
        const max_screen_y = t.screen_offset.y + 1;
        t.screen_offset.y = 0;

        while (true) {
            t.update();
            for (0..(1 << 24)) |_| asm volatile ("pause");
            t.screen_offset.y = (t.screen_offset.y + 1) % max_screen_y;
            // t.screen_offset.y = if (t.screen_offset.y == 0) max_screen_y - 1 else t.screen_offset.y - 1;
        }
    } else {
        t.update();
    }

    while (true) {
        asm volatile ("hlt");
    }
}

fn rectPos(pos: Coords) Coords {
    return .{
        .x = pos.x * rect.x,
        .y = pos.y * rect.y,
    };
}

fn fontPos(font_idx: u8) Coords {
    return rectPos(.{ .x = font_idx & 0x0f, .y = font_idx >> 4 });
}

pub fn fontPixelAt(pos: Coords) Color {
    var idx: usize = 18 + pos.y * 4 * 16 * rect.x + pos.x * 4;

    var color: Color = undefined;
    color.b = font_file_data[idx];
    idx += 1;
    color.g = font_file_data[idx];
    idx += 1;
    color.r = font_file_data[idx];
    idx += 1;
    color.a = font_file_data[idx];

    return color;
}

fn putPixel(pos: Coords, color: Color) void {
    for (getFramebuffers()) |*framebuffer| {
        const data = framebuffer.data();
        const bytes_per_pixel = framebuffer.bytesPerPixel();

        const index = framebuffer.index(pos.x, pos.y, bytes_per_pixel);
        data[index] = color.b;
        data[index + 1] = color.g;
        data[index + 2] = color.r;
    }
}

fn putRect(pos: Coords, size: Coords, color: Color) void {
    for (0..size.y) |y_offset| {
        for (0..size.x) |x_offset| {
            putPixel(.{ .x = pos.x + x_offset, .y = pos.y + y_offset }, color);
        }
    }
}

fn putChar(pos: Coords, c: u8) void {
    const rect_pos = rectPos(pos);

    for (getFramebuffers()) |*framebuffer| {
        const data = framebuffer.data();
        const bytes_per_pixel = framebuffer.bytesPerPixel();

        const font_pos = fontPos(default_font_map.get(c));

        for (0..rect.y) |y| {
            for (0..rect.x) |x| {
                const index = framebuffer.index(rect_pos.x + x, rect_pos.y + y, bytes_per_pixel);
                const color = fontPixelAt(.{ .x = font_pos.x + x, .y = font_pos.y + y }).applyAlpha();

                data[index] = color.b;
                data[index + 1] = color.g;
                data[index + 2] = color.r;
            }
        }
    }
}

fn putStr(pos: Coords, buf: []const u8) usize {
    for (getFramebuffers()) |*framebuffer| {
        const data = framebuffer.data();
        const bytes_per_pixel = framebuffer.bytesPerPixel();

        for (0..rect.y) |y| {
            for (0..buf.len) |n| {
                const c = buf[n];
                const font_pos = fontPos(default_font_map.get(c));
                const rect_pos = rectPos(.{ .x = pos.x + n, .y = pos.y });

                for (0..rect.x) |x| {
                    const index = framebuffer.index(rect_pos.x + x, rect_pos.y + y, bytes_per_pixel);
                    const color = fontPixelAt(.{ .x = font_pos.x + x, .y = font_pos.y + y }).applyAlpha();

                    data[index] = color.b;
                    data[index + 1] = color.g;
                    data[index + 2] = color.r;
                }
            }
        }
    }

    return buf.len;
}

var putNumber_buf: [20]u8 = undefined;

fn putNumber(comptime T: type, pos: Coords, value: T) usize {
    const buf = &putNumber_buf;
    var v = value;
    var start: usize = buf.len;

    for (0..buf.len) |_| {
        start -= 1;
        buf[start] = @intCast(v % 10 + '0');
        v /= 10;
        if (v == 0) break;
    }

    return putStr(pos, buf[start..]);
}

fn putBinSlice(comptime T: type, pos: Coords, slice: []const T, color: Color) void {
    const bit_size = @bitSizeOf(T);
    const framebuffers = getFramebuffers();
    const fb_width = framebuffers[0].width;
    const fb_size = fb_width / rect.x;
    for (0..slice.len) |n| {
        const item = slice[n];
        var mask: T = 1;
        for (0..bit_size) |x| {
            const offset = pos.x + n * bit_size + x;
            const offset_pos = Coords{ .x = offset % fb_size, .y = pos.y + offset / fb_size };
            if (item & mask != 0) putRect(rectPos(offset_pos), rect, color);
            mask <<= 1;
        }
    }
}

// fn putBinRect(comptime T: type, pos: Coords, slices: [][]const T, color: Color) void {}

fn alloc(context: *anyopaque, n: usize, alignment: std.mem.Alignment, ra: usize) ?[*]u8 {
    _ = context;
    _ = n;
    _ = alignment;
    _ = ra;
    return null;
}

fn resize(
    context: *anyopaque,
    memory: []u8,
    alignment: std.mem.Alignment,
    new_len: usize,
    return_address: usize,
) bool {
    _ = context;
    _ = memory;
    _ = alignment;
    _ = new_len;
    _ = return_address;
    return false;
}

fn remap(
    context: *anyopaque,
    memory: []u8,
    alignment: std.mem.Alignment,
    new_len: usize,
    return_address: usize,
) ?[*]u8 {
    _ = context;
    _ = memory;
    _ = alignment;
    _ = new_len;
    _ = return_address;
    return null;
}

fn free(
    context: *anyopaque,
    memory: []u8,
    alignment: std.mem.Alignment,
    return_address: usize,
) void {
    _ = context;
    _ = memory;
    _ = alignment;
    _ = return_address;
}
