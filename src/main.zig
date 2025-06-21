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

    t.putChar('C');
    t.putChar('h');
    t.putChar('a');
    t.putChar('r');
    t.putChar('s');

    const rsp = asm volatile (
        \\ movq %rsp, %rax
        : [ret] "={rax}" (-> u64),
        :
        : "rsp"
    );

    const rbp = asm volatile (
        \\ movq %rbp, %rax
        : [ret] "={rax}" (-> u64),
        :
        : "rbp"
    );

    _ = putNumber(u64, .{ .x = 0, .y = 0 }, rbp - rsp);
    const width: u64 = t.fb_res.x;
    const height = t.fb_res.y;

    var n: usize = 0;
    // var pos: Coords = undefined;
    // _ = &pos;

    // _ = &pos;
    n += putNumber(u64, .{ .x = n, .y = 1 }, width);
    putChar(.{ .x = n, .y = 1 }, 'x');
    n += 1;
    n += putNumber(u64, .{ .x = n, .y = 1 }, height);

    // for (limine.getFramebuffers()[0].videoModes(), 0..) |video_mode, video_mode_idx| {
    //     if (video_mode_idx > 4) break;
    //     n = 0;
    //     n += putNumber(usize, .{ .x = n, .y = 2 + n }, video_mode_idx);
    //     n += 1;
    //     n += putNumber(u64, .{ .x = n, .y = 2 + n }, video_mode.width);
    //     putChar(.{ .x = n, .y = 2 + n }, 'x');
    //     n += 1;
    //     n += putNumber(u64, .{ .x = n, .y = 2 + n }, video_mode.height);
    // }

    // pos.x += putNumber(u64, pos, t.fb_res.x);
    // putChar(pos, 'x');
    // pos.x += 1;
    // pos.x += putNumber(u64, pos, t.fb_res.y);
    //
    // pos.x = 0;
    // pos.y += 1;

    // pos.x += putNumber(u64, pos, t.screen_size.x);
    // putChar(pos, 'x');
    // pos.x += 1;
    // pos.x += putNumber(u64, pos, t.screen_size.y);

    // logFn(.info, Scope, "debug", .{});

    // t.updateScreen();
    // _ = putNumber(usize, .{ .x = 0, .y = 5 }, t.updateFramebuffer());

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
