const std = @import("std");
const assert = std.debug.assert;

const limine = @import("ext/limine.zig");

fb_data: []u8,
text_buf: []u8,
screen_buf: []u8,

fb_res: Coords = .{},
char_res: Coords = .{},
screen_size: Coords = .{},
screen_buf_size: Coords = .{},
fb_offset: Coords = .{},

screen_offset: Coords = .{},
cursor_pos: Coords = .{},

text_buf_start: usize = 0,
text_buf_end: usize = 0,

pitch: usize,
bytes_per_pixel: usize,

const Self = @This();

pub const Coords = struct {
    x: u64 = 0,
    y: u64 = 0,
};

const max_tty = 16;
const text_buf_len = 64 << 10;
const text_buf_idx_mask = text_buf_len - 1;
const screen_buf_len = 64 << 10;

var state_buf: [max_tty]Self = undefined;
var states: []Self = undefined;
var text_bufs: [max_tty][text_buf_len]u8 = undefined;
var screen_bufs: [max_tty][screen_buf_len]u8 = undefined;

pub const default_char_res = Coords{ .x = 13, .y = 28 };

pub fn initSystem() void {
    const framebuffers = limine.getFramebuffers();

    const end = @min(framebuffers.len, max_tty);
    states = state_buf[0..end];
    for (0..end) |n| {
        const state = &states[n];
        const framebuffer = &framebuffers[n];

        state.fb_data = framebuffer.data();
        state.text_buf = &text_bufs[n];
        state.screen_buf = &screen_bufs[n];

        state.fb_res = .{ .x = framebuffer.fb.width, .y = framebuffer.fb.height };
        state.char_res.x = default_char_res.x;
        state.char_res.y = default_char_res.y;
        state.screen_size = .{
            .x = state.fb_res.x / state.char_res.x,
            .y = state.fb_res.y / state.char_res.y,
        };
        state.screen_buf_size = .{
            .x = state.screen_size.x,
            .y = screen_buf_len / state.screen_size.x,
        };
        state.fb_offset = .{
            .x = state.fb_res.x - (state.char_res.x * state.screen_size.x) / 2,
            .y = state.fb_res.y - (state.char_res.y * state.screen_size.y) / 2,
        };

        state.screen_offset.x = 0;
        state.screen_offset.y = 0;
        state.cursor_pos.x = 0;
        state.cursor_pos.y = 0;

        state.text_buf_start = 0;
        state.text_buf_end = 0;

        state.pitch = framebuffer.fb.pitch;
        state.bytes_per_pixel = framebuffer.bytesPerPixel();
    }
}

pub fn get(handle: usize) *Self {
    return &states[handle];
}

pub fn putStr(self: *Self, str: []const u8) void {
    for (str) |c| self.putChar(c);
}

pub fn putChar(self: *Self, c: u8) void {
    self.text_buf[self.text_buf_end] = c;

    self.text_buf_end = textIdxInc(self.text_buf_end);
    if (self.text_buf_end == self.text_buf_start) self.text_buf_start = textIdxInc(self.text_buf_start);

    const screen_index = self.screenIndex(self.cursor_pos);
    self.screen_buf[screen_index] = c;

    switch (c) {
        '\n' => self.cursor_pos = .{ .x = 0, .y = (self.cursor_pos.y + 1) },
        else => self.cursor_pos.x += 1,
    }

    self.wrapCursorPos();
}

pub fn update(self: *const Self) usize {
    for (0..self.screen_size.y) |screen_y| {
        for (0..self.char_res.y) |char_y| {
            for (0..self.screen_size.x) |screen_x| {
                const screen_pos = Coords{ .x = screen_x, .y = screen_y };
                const screen_idx = self.screenIndex(screen_pos);
                const c = self.screen_buf[screen_idx];
                const font_offset = self.fontOffset(default_font_map.get(c));
                const fb_offset = self.fbOffset(self.charPos(screen_pos));

                for (0..self.char_res.x) |char_x| {
                    const idx = self.fbIndex(.{ .x = fb_offset.x + char_x, .y = fb_offset.y + char_y });
                    const color = self.fontPixelAt(.{ .x = font_offset.x + char_x, .y = font_offset.y + char_y }).applyAlpha();
                    _ = idx;
                    _ = color;
                    return fb_offset.y;
                    // self.fb_data[idx] = color.b;
                    // self.fb_data[idx + 1] = color.g;
                    // self.fb_data[idx + 2] = color.r;
                }
            }
        }
    }
    return 0;
}

fn textIdxInc(idx: usize) usize {
    return (idx +% 1) & text_buf_idx_mask;
}

fn textIdxDec(idx: usize) usize {
    return (idx -% 1) & text_buf_idx_mask;
}

fn screenPosDec(self: *const Self, pos: Coords) Coords {
    if (pos.x == 0) return .{ .x = self.screen_buf_size.x - 1, .y = pos.y - 1 };
    return .{ .x = pos.x - 1, .y = pos.y };
}

fn screenIndex(self: *const Self, pos: Coords) usize {
    return (self.screen_offset.y + pos.y) * self.screen_buf_size.x + pos.x;
}

fn charPos(self: *const Self, pos: Coords) Coords {
    return .{
        .x = pos.x * self.char_res.x,
        .y = pos.y * self.char_res.y,
    };
}

fn fbOffset(self: *const Self, pos: Coords) Coords {
    return .{
        .x = self.fb_offset.x + pos.x,
        .y = self.fb_offset.y + pos.y,
    };
}

fn fbIndex(self: *const Self, pos: Coords) usize {
    return pos.y * self.pitch + pos.x * self.bytes_per_pixel;
}

fn fontOffset(self: *const Self, font_idx: u8) Coords {
    return self.charPos(.{ .x = font_idx & 0x0f, .y = font_idx >> 4 });
}

fn wrapCursorPos(self: *Self) void {
    if (self.cursor_pos.x >= self.screen_buf_size.x) self.cursor_pos = .{ .x = 0, .y = self.cursor_pos.y + 1 };
    if (self.cursor_pos.y >= self.screen_buf_size.y) self.cursor_pos.y = self.screen_buf_size.y - 1;
}

pub const Color = struct {
    b: u8 = 0,
    g: u8 = 0,
    r: u8 = 0,
    a: u8 = 0,

    pub fn fromRgba(pixel_value: u32) Color {
        const value = std.mem.bigToNative(u32, pixel_value);
        return .{
            .r = @intCast(value >> 24),
            .g = @intCast((value >> 16) & 0xff),
            .b = @intCast((value >> 8) & 0xff),
            .a = @intCast(value & 0xff),
        };
    }

    pub fn applyAlpha(color: Color) Color {
        const b: u16 = @intCast(color.b);
        const g: u16 = @intCast(color.g);
        const r: u16 = @intCast(color.r);
        const a: u16 = @intCast(color.a);
        return .{
            .b = @intCast(b * a / 0xff),
            .g = @intCast(g * a / 0xff),
            .r = @intCast(r * a / 0xff),
            .a = 0xff,
        };
    }
};

pub const FontMap = struct {
    data: Data = [1]u8{0} ** data_len,

    pub const data_len = std.math.maxInt(u8);
    pub const Data = [data_len]u8;

    pub fn generateDefault() FontMap {
        var result = FontMap{};

        result.set('!', 1, 0);
        result.set('"', 2, 0);
        result.set('#', 3, 0);
        result.set('$', 4, 0);
        result.set('%', 5, 0);
        result.set('&', 6, 0);
        result.set('\'', 7, 0);
        result.set('(', 8, 0);
        result.set(')', 9, 0);
        result.set('*', 10, 0);
        result.set('+', 11, 0);
        result.set(',', 12, 0);
        result.set('-', 13, 0);
        result.set('.', 14, 0);
        result.set('/', 15, 0);

        var idx = 0;
        while (idx <= '9' - '0') : (idx += 1) result.set(idx + '0', idx, 1);

        result.set(';', 10, 1);
        result.set(';', 11, 1);
        result.set('<', 12, 1);
        result.set('=', 13, 1);
        result.set('>', 14, 1);
        result.set('?', 15, 1);
        result.set('@', 0, 2);

        idx = 0;
        while (idx <= 'Z' - 'A') : (idx += 1) result.set(idx + 'A', 1 + idx, 2);

        result.set('[', 11, 3);
        result.set('\\', 12, 3);
        result.set(']', 13, 3);
        result.set('^', 14, 3);
        result.set('_', 15, 3);
        result.set('`', 0, 4);

        idx = 0;
        while (idx <= 'z' - 'a') : (idx += 1) result.set(idx + 'a', 1 + idx, 4);

        result.set(';', 11, 5);
        result.set('<', 12, 5);
        result.set('=', 13, 5);
        result.set('>', 14, 5);

        return result;
    }

    pub fn get(self: *const FontMap, c: u8) u8 {
        return self.data[c];
    }

    pub fn set(self: *FontMap, c: u8, x: u8, y: u8) void {
        assert(y * 16 + x < 16 * 6);
        self.data[c] = y * 16 + x;
    }
};

pub const default_font_map = FontMap.generateDefault();
pub const font_file_data = @embedFile("asset/font.tga");

pub fn fontPixelAt(self: *const Self, pos: Coords) Color {
    var idx: usize = 18 + pos.y * 4 * 16 * self.char_res.x + pos.x * 4;

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
