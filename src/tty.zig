const std = @import("std");
const assert = std.debug.assert;

const limine = @import("ext.zig").limine;

fb_data: []u8,
text_buf: []u8,
screen_buf: []u8,
dbl_buf: []u8,

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

const max_tty = 1;
const text_buf_len = 64 << 10;
const text_buf_idx_mask = text_buf_len - 1;
const screen_buf_len = 64 << 10;
const dbl_buf_len = 64 << 20;

var states: []Self = undefined;
var state_buf: [max_tty]Self = undefined;
var text_bufs: [max_tty][text_buf_len]u8 = undefined;
var screen_bufs: [max_tty][screen_buf_len]u8 = undefined;
var dbl_bufs: [max_tty][dbl_buf_len]u8 = undefined;

pub const default_char_res = Coords{ .x = 13, .y = 28 };

const default_fg_color = Color{ .b = 0xdd, .g = 0xb3, .r = 0xa8 };
const default_bg_color = Color{ .b = 0x27, .g = 0x1b, .r = 0x1a };

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
        state.dbl_buf = dbl_bufs[n][0..state.fb_data.len];

        state.fb_res = .{ .x = framebuffer.fb.width, .y = framebuffer.fb.height };
        state.char_res = default_char_res;
        state.screen_size = .{
            .x = state.fb_res.x / state.char_res.x,
            .y = state.fb_res.y / state.char_res.y,
        };
        state.screen_buf_size = .{
            .x = state.screen_size.x,
            .y = screen_buf_len / state.screen_size.x,
        };
        state.fb_offset = .{
            .x = (state.fb_res.x - state.char_res.x * state.screen_size.x) / 2,
            .y = (state.fb_res.y - state.char_res.y * state.screen_size.y) / 2,
        };

        state.screen_offset = .{};
        state.cursor_pos = .{};

        state.text_buf_start = 0;
        state.text_buf_end = 0;

        state.pitch = framebuffer.fb.pitch;
        state.bytes_per_pixel = framebuffer.bytesPerPixel();

        state.clear();
    }

    premultiplyFontData();
}

pub fn get(handle: usize) *Self {
    return &states[handle];
}

var putNumber_buf: [32]u8 = undefined;

pub fn putNumber(self: *Self, comptime T: type, value: T) void {
    const buf = &putNumber_buf;
    var v = value;
    var start: usize = buf.len;

    for (0..buf.len) |_| {
        start -= 1;
        buf[start] = @intCast(v % 10 + '0');
        v /= 10;
        if (v == 0) break;
    }

    self.putStr(buf[start..]);
}

pub fn putNumberAlign(self: *Self, comptime T: type, value: T, digits: usize) void {
    const buf = &putNumber_buf;
    var v = value;
    var start: usize = buf.len;

    for (0..buf.len) |_| {
        start -= 1;
        buf[start] = @intCast(v % 10 + '0');
        v /= 10;
        if (v == 0) break;
    }

    for (0..(digits - (buf.len - start))) |_| {
        start -= 1;
        buf[start] = ' ';
    }

    self.putStr(buf[start..]);
}

pub fn putHexAlign(self: *Self, comptime T: type, value: T) void {
    const buf_len = 2 * @sizeOf(T);
    const buf = putNumber_buf[0..buf_len];
    var v = value;
    var start: usize = buf.len;

    const hex_table = "0123456789abcdef";

    for (0..buf.len) |_| {
        start -= 1;
        buf[start] = hex_table[v & 0x0f];
        v >>= 4;
    }

    self.putStr(buf[start..]);
}

pub fn putHexSlice(self: *Self, slice: []const u8) void {
    for (slice) |c| self.putHexAlign(u8, c);
}

pub fn putMemoryUnits(self: *Self, comptime T: type, value: T) void {
    const unit_table = [_][]const u8{
        " B",
        " KB",
        " MB",
        " GB",
        " TB",
    };

    var unit: u6 = 0;
    for (1..unit_table.len) |n_size| {
        const n: u6 = @intCast(n_size);
        if (value >> (10 * n) > 9) unit = n;
    }

    self.putNumber(T, value >> (10 * unit));
    self.putStr(unit_table[unit]);
}

pub fn putUuid(self: *Self, comptime T: type, uuid: *const T) void {
    // self.putStr("{");
    self.putHexAlign(u32, uuid.a);
    self.putStr("-");
    self.putHexAlign(u16, uuid.b);
    self.putStr("-");
    self.putHexAlign(u16, uuid.c);
    self.putStr("-");
    self.putHexSlice(&uuid.d);
    // self.putStr("}");
}

pub fn putStr(self: *Self, str: []const u8) void {
    for (str) |c| self.putChar(c);
}

pub fn putStrz(self: *Self, str: [*:0]const u8) void {
    var n: usize = 0;
    while (str[n] != 0) : (n += 1) self.putChar(str[n]);
}

pub fn putChar(self: *Self, c: u8) void {
    self.text_buf[self.text_buf_end] = c;

    self.text_buf_end = textIdxInc(self.text_buf_end);
    if (self.text_buf_end == self.text_buf_start) self.text_buf_start = textIdxInc(self.text_buf_start);

    const screen_index = self.screenIndex(self.cursor_pos);
    self.screen_buf[screen_index] = c;

    switch (c) {
        '\n' => self.cursor_pos = .{ .x = 0, .y = self.cursor_pos.y + 1 },
        else => self.cursor_pos.x += 1,
    }

    self.wrapCursorPos();
    self.screenFollowCursor();
}

pub fn clear(self: *const Self) void {
    for (0..self.fb_res.y) |y| {
        for (0..self.fb_res.x) |x| {
            const idx = self.fbIndex(.{ .x = x, .y = y });
            self.dbl_buf[idx] = default_bg_color.b;
            self.dbl_buf[idx + 1] = default_bg_color.g;
            self.dbl_buf[idx + 2] = default_bg_color.r;
        }
    }
}

pub fn update(self: *const Self) void {
    for (0..self.screen_size.y) |screen_y| {
        for (0..self.char_res.y) |char_y| {
            for (0..self.screen_size.x) |screen_x| {
                const screen_pos = Coords{ .x = screen_x, .y = screen_y };
                const screen_offset = self.screenOffset(screen_pos);
                const screen_idx = self.screenIndex(screen_offset);

                const c = self.screen_buf[screen_idx];
                const is_cursor = Coords.eql(screen_offset, self.cursor_pos);

                const font_offset = self.fontOffset(default_font_map.get(c));
                const fb_offset = self.fbOffset(self.charPos(screen_pos));

                for (0..self.char_res.x) |char_x| {
                    const idx = self.fbIndex(.{ .x = fb_offset.x + char_x, .y = fb_offset.y + char_y });
                    const color = self.fontPixelAt(.{ .x = font_offset.x + char_x, .y = font_offset.y + char_y }, is_cursor);

                    self.dbl_buf[idx] = color.b;
                    self.dbl_buf[idx + 1] = color.g;
                    self.dbl_buf[idx + 2] = color.r;
                }
            }
        }
    }

    @memcpy(self.fb_data, self.dbl_buf);
    // for (self.fb_data, self.dbl_buf) |*fb, dbl| {
    //     fb.* = dbl;
    // }
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

fn screenOffset(self: *const Self, pos: Coords) Coords {
    return .{
        .x = self.screen_offset.x + pos.x,
        .y = self.screen_offset.y + pos.y,
    };
}

fn screenIndex(self: *const Self, pos: Coords) usize {
    return pos.y * self.screen_buf_size.x + pos.x;
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

fn screenFollowCursor(self: *Self) void {
    if (self.cursor_pos.y < self.screen_offset.y) self.screen_offset.y = self.cursor_pos.y;
    if (self.cursor_pos.y >= self.screen_offset.y + self.screen_size.y - 1) self.screen_offset.y = self.cursor_pos.y + 1 - self.screen_size.y;
}

pub const Coords = struct {
    x: u64 = 0,
    y: u64 = 0,

    pub fn eql(lhs: Coords, rhs: Coords) bool {
        return lhs.x == rhs.x and lhs.y == rhs.y;
    }
};

pub const Color = struct {
    b: u8 = 0,
    g: u8 = 0,
    r: u8 = 0,
    a: u8 = 0,

    pub fn mul(l: u16, r: u16) u16 {
        return (l * r) / 0xff;
    }

    pub fn neg(x: anytype) @TypeOf(x) {
        return 0xff - x;
    }

    pub fn applyAlpha(color: Color) Color {
        const b: u16 = @intCast(color.b);
        const g: u16 = @intCast(color.g);
        const r: u16 = @intCast(color.r);
        const a: u16 = @intCast(color.a);
        return .{
            .b = @intCast(mul(b, a)),
            .g = @intCast(mul(g, a)),
            .r = @intCast(mul(r, a)),
            .a = 0xff,
        };
    }

    fn applyBetweenChannel(self: u16, min: u16, max: u16) u8 {
        return @intCast(mul(self, max) + mul(neg(self), min));
    }

    pub fn applyBetween(self: Color, min: Color, max: Color) Color {
        return .{
            .b = applyBetweenChannel(self.b, min.b, max.b),
            .g = applyBetweenChannel(self.g, min.g, max.g),
            .r = applyBetweenChannel(self.r, min.r, max.r),
            .a = 0xff,
        };
    }

    pub fn invert(color: Color) Color {
        return .{
            .b = neg(color.b),
            .g = neg(color.g),
            .r = neg(color.r),
            .a = color.a,
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

        result.set(':', 10, 1);
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

        result.set('{', 11, 5);
        result.set('|', 12, 5);
        result.set('}', 13, 5);
        result.set('~', 14, 5);

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
pub var font_data: [(font_file_data.len - 18):0]u8 = undefined;
pub var inv_font_data: [(font_file_data.len - 18):0]u8 = undefined;

pub fn fontPixelAt(self: *const Self, pos: Coords, is_inverted: bool) Color {
    const idx: usize = pos.y * 4 * 16 * self.char_res.x + pos.x * 4;

    var color: Color = undefined;
    if (is_inverted) {
        color.b = inv_font_data[idx];
        color.g = inv_font_data[idx + 1];
        color.r = inv_font_data[idx + 2];
        color.a = inv_font_data[idx + 3];
    } else {
        color.b = font_data[idx];
        color.g = font_data[idx + 1];
        color.r = font_data[idx + 2];
        color.a = font_data[idx + 3];
    }

    return color;
}

pub fn premultiplyFontData() void {
    var idx: usize = 0;
    const end = 6 * 16 * default_char_res.y * default_char_res.x * 4;
    while (idx < end) : (idx += 4) {
        var color = Color{
            .b = font_file_data[idx + 18],
            .g = font_file_data[idx + 1 + 18],
            .r = font_file_data[idx + 2 + 18],
            .a = font_file_data[idx + 3 + 18],
        };

        color = color.applyAlpha();
        var inv_color = color.invert();
        color = color.applyBetween(default_bg_color, default_fg_color);
        inv_color = inv_color.applyBetween(default_bg_color, default_fg_color);

        font_data[idx] = color.b;
        font_data[idx + 1] = color.g;
        font_data[idx + 2] = color.r;
        font_data[idx + 3] = color.a;

        inv_font_data[idx] = inv_color.b;
        inv_font_data[idx + 1] = inv_color.g;
        inv_font_data[idx + 2] = inv_color.r;
        inv_font_data[idx + 3] = inv_color.a;
    }
}
