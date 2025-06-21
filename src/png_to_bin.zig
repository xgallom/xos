const std = @import("std");
const assert = std.debug.assert;
const fatal = std.zig.fatal;

const usage =
    \\Usage: ./png_to_bin [options]
    \\
    \\Options:
    \\  --input INPUT_FILE
    \\  --output OUTPUT_FILE
    \\
;

const Arguments = struct {
    input_filename: []const u8,
    output_filename: []const u8,
};

const Png = @import("Png.zig");

fn parseArguments(allocator: std.mem.Allocator) !?Arguments {
    const args = try std.process.argsAlloc(allocator);

    var input_filename: ?[]const u8 = null;
    var output_filename: ?[]const u8 = null;

    {
        var n: usize = 1;
        while (n < args.len) : (n += 1) {
            const arg = args[n];
            if (std.mem.eql(u8, "-h", arg) or std.mem.eql(u8, "--help", arg)) {
                try std.io.getStdOut().writeAll(usage);
                return null;
            } else if (std.mem.eql(u8, "--input", arg)) {
                n += 1;
                if (n >= args.len) fatal("expected argument after '{s}'", .{arg});
                if (input_filename != null) fatal("duplicated argument {s}", .{arg});
                input_filename = args[n];
            } else if (std.mem.eql(u8, "--output", arg)) {
                n += 1;
                if (n >= args.len) fatal("expected argument after '{s}'", .{arg});
                if (output_filename != null) fatal("duplicated argument {s}", .{arg});
                output_filename = args[n];
            } else {
                fatal("unrecognized argument: {s}", .{arg});
            }
        }
    }

    return .{
        .input_filename = input_filename orelse fatal("missing argument --input", .{}),
        .output_filename = output_filename orelse fatal("missing argument --output", .{}),
    };
}

const FileConfig = struct {
    input_filename: []const u8,
    output_filename: []const u8,

    const Self = @This();

    fn init(input_filename: []const u8, output_filename: []const u8) Self {
        return .{
            .input_filename = input_filename,
            .output_filename = output_filename,
        };
    }
};

pub fn main() !void {
    var arena_state = std.heap.ArenaAllocator.init(std.heap.page_allocator);
    defer arena_state.deinit();
    const arena = arena_state.allocator();

    const arguments = try parseArguments(arena) orelse return;

    std.log.info(
        \\running with:
        \\  --input:  {s}
        \\  --output: {s}
    , .{ arguments.input_filename, arguments.output_filename });

    const input_file = std.fs.cwd().openFile(arguments.input_filename, .{}) catch |err| {
        fatal("failed opening input file {s}: {s}", .{ arguments.input_filename, @errorName(err) });
    };
    defer input_file.close();

    const output_file = std.fs.cwd().createFile(arguments.output_filename, .{ .truncate = true }) catch |err| {
        fatal("failed opening output file {s}: {s}", .{ arguments.output_filename, @errorName(err) });
    };
    defer output_file.close();

    const png = try Png.parseFile(arena, &input_file);
    std.log.info("parsed input png file {s}: {any}", .{ arguments.input_filename, png.png.ihdr });

    const bytes_written = try output_file.write(png.png.data);
    std.log.info("successfully written {} bytes into binary file {s}", .{ bytes_written, arguments.output_filename });
}
