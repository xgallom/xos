const std = @import("std");
const builtin = @import("builtin");
const assert = std.debug.assert;

ihdr: IHDR = .{},
data: []u8,
capacity: usize = 0,

pub const Self = @This();
pub const Data = std.ArrayList(u8);
pub const Crc = u32;

pub const Header = struct {
    magic: u8,
    png: [3]u8,
    crlf: [2]u8,
    eof: u8,
    lf: u8,

    comptime {
        assert(@sizeOf(Header) == 8);
    }
};

pub const Chunk = struct {
    size: u32,
    type: [4]u8,

    pub fn fromSlice(slice: []const u8) Chunk {
        var result: Chunk = undefined;
        @memcpy(std.mem.asBytes(&result), slice[0..@sizeOf(Chunk)]);
        return result;
    }

    pub fn data(self: *const Chunk, slice: []const u8) []const u8 {
        const data_start = @as([*]const u8, @ptrCast(slice.ptr)) + @sizeOf(Chunk);
        return data_start[0..self.nativeSize()];
    }

    pub fn offset(self: *const Chunk) usize {
        return @sizeOf(Chunk) + self.nativeSize() + @sizeOf(Crc);
    }

    pub fn nativeSize(self: *const Chunk) u32 {
        return std.mem.bigToNative(u32, self.size);
    }

    comptime {
        assert(@sizeOf(Chunk) == 8);
    }
};

pub const IHDR = struct {
    width: u32 = 0,
    height: u32 = 0,
    depth: u8 = 0,
    color_type: u8 = 0,
    compression: u8 = 0,
    filter: u8 = 0,
    interlace: u8 = 0,

    pub fn fromSlice(slice: []const u8) IHDR {
        var result: IHDR = undefined;
        @memcpy(std.mem.asBytes(&result)[0..13], slice[0..13]);
        return result;
    }
};

pub fn init(buffer: []u8) Self {
    return .{ .data = buffer[0..0], .capacity = buffer.len };
}

pub fn appendSlice(self: *Self, slice: []const u8) !void {
    const start_idx = self.data.len;
    if (start_idx + slice.len > self.capacity) return error.OutOfMemory;
    self.data.len += slice.len;
    @memcpy(self.data[start_idx..(start_idx + slice.len)], slice);
}

pub fn parseSlice(buffer: []u8, data: []const u8) !Self {
    var result = Self.init(buffer);

    var index: usize = @sizeOf(Header);
    while (index < buffer.len) {
        const chunk = Chunk.fromSlice(data[index..]);
        std.log.debug("chunk: size={} type={s} offset={} index={}", .{ chunk.nativeSize(), chunk.type, chunk.offset(), index });
        try result.appendSlice(chunk.data(data[index..]));
        index += chunk.offset();
    }

    return result;
}

pub usingnamespace if (builtin.target.os.tag == .freestanding) struct {} else struct {
    pub const ArrayList = struct {
        png: Self,
        array_list: Data,

        pub fn deinit(self: *ArrayList) void {
            self.array_list.deinit();
        }

        pub fn appendSlice(self: *ArrayList, slice: []const u8) !void {
            try self.array_list.appendSlice(slice);
            self.png.data = self.array_list.items;
            self.png.capacity = self.array_list.capacity;
        }
    };

    pub fn arrayList(allocator: std.mem.Allocator) ArrayList {
        return .{
            .png = .{ .data = &[_]u8{} },
            .array_list = Data.init(allocator),
        };
    }

    pub fn parseFile(allocator: std.mem.Allocator, file: *const std.fs.File) !ArrayList {
        const file_size = try file.getEndPos();

        var buf_reader = std.io.bufferedReader(file.reader());
        var in_stream = buf_reader.reader();

        var data = Data.init(allocator);
        defer data.deinit();

        try in_stream.readAllArrayList(&data, file_size);
        std.log.debug("data: size={}", .{data.items.len});

        return parseSliceAlloc(allocator, data.items);
    }

    pub fn parseSliceAlloc(allocator: std.mem.Allocator, data: []const u8) !ArrayList {
        var result = arrayList(allocator);

        var index: usize = @sizeOf(Header);
        while (index < data.len) {
            const chunk = Chunk.fromSlice(data[index..]);
            std.log.debug("chunk: size={} type={s} offset={} index={}", .{ chunk.nativeSize(), chunk.type, chunk.offset(), index });

            if (std.mem.eql(u8, &chunk.type, "IHDR")) {
                result.png.ihdr = IHDR.fromSlice(chunk.data(data[index..]));
                result.png.ihdr.width = std.mem.bigToNative(u32, result.png.ihdr.width);
                result.png.ihdr.height = std.mem.bigToNative(u32, result.png.ihdr.height);
            } else if (std.mem.eql(u8, &chunk.type, "IDAT")) {
                try result.appendSlice(chunk.data(data[index..]));
            }
            index += chunk.offset();
        }

        return result;
    }
};
