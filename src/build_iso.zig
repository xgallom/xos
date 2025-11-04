const std = @import("std");
const fatal = std.process.fatal;

const usage =
    \\Usage: ./build_iso [options]
    \\
    \\Options:
    \\  --input-dir INPUT_DIRECTORY
    \\  --output-dir OUTPUT_DIRECTORY
    \\  --xos-bin-dir XOS_BIN_DIRECTORY
    \\
;

const Arguments = struct {
    input_directory: []const u8,
    output_directory: []const u8,
    xos_bin_directory: []const u8,
};

fn parseArguments(allocator: std.mem.Allocator) !?Arguments {
    const args = try std.process.argsAlloc(allocator);

    var input_directory: ?[]const u8 = null;
    var output_directory: ?[]const u8 = null;
    var xos_bin_directory: ?[]const u8 = null;

    {
        var n: usize = 1;
        while (n < args.len) : (n += 1) {
            const arg = args[n];
            if (std.mem.eql(u8, "-h", arg) or std.mem.eql(u8, "--help", arg)) {
                var stdout_buf: [256]u8 = undefined;
                var stdout_writer = std.fs.File.stdout().writer(&stdout_buf);
                const stdout = &stdout_writer.interface;
                try stdout.writeAll(usage);
                try stdout.flush();
                return null;
            } else if (std.mem.eql(u8, "--input-dir", arg)) {
                n += 1;
                if (n >= args.len) fatal("expected argument after '{s}'", .{arg});
                if (input_directory != null) fatal("duplicated argument {s}", .{arg});
                input_directory = args[n];
            } else if (std.mem.eql(u8, "--output-dir", arg)) {
                n += 1;
                if (n >= args.len) fatal("expected argument after '{s}'", .{arg});
                if (output_directory != null) fatal("duplicated argument {s}", .{arg});
                output_directory = args[n];
            } else if (std.mem.eql(u8, "--xos-bin-dir", arg)) {
                n += 1;
                if (n >= args.len) fatal("expected argument after '{s}'", .{arg});
                if (xos_bin_directory != null) fatal("duplicated argument {s}", .{arg});
                xos_bin_directory = args[n];
            } else {
                fatal("unrecognized argument: {s}", .{arg});
            }
        }
    }

    return .{
        .input_directory = input_directory orelse fatal("missing argument --input-dir", .{}),
        .output_directory = output_directory orelse fatal("missing argument --output-dir", .{}),
        .xos_bin_directory = xos_bin_directory orelse fatal("missing argument --xos-bin-dir", .{}),
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
        \\  --input-dir:   {s}
        \\  --output-dir:  {s}
        \\  --xos-bin-dir: {s}
    , .{ arguments.input_directory, arguments.output_directory, arguments.xos_bin_directory });

    std.fs.makeDirAbsolute(arguments.output_directory) catch |err| {
        switch (err) {
            error.PathAlreadyExists => {},
            else => fatal("failed creating output_directory: {s}", .{@errorName(err)}),
        }
    };

    var efi_output_directory = try std.fs.path.join(arena, &.{ arguments.output_directory, "EFI" });
    std.fs.makeDirAbsolute(efi_output_directory) catch |err| {
        switch (err) {
            error.PathAlreadyExists => {},
            else => fatal("failed creating efi_output_directory: {s}", .{@errorName(err)}),
        }
    };

    efi_output_directory = try std.fs.path.join(arena, &.{ efi_output_directory, "BOOT" });
    std.fs.makeDirAbsolute(efi_output_directory) catch |err| {
        switch (err) {
            error.PathAlreadyExists => {},
            else => fatal("failed creating efi_output_directory: {s}", .{@errorName(err)}),
        }
    };

    const limine_path = try std.fs.path.join(arena, &.{ arguments.input_directory, "limine", "limine" });
    const output_path = try std.fs.path.join(arena, &.{ arguments.xos_bin_directory, "xos.iso" });

    const file_configs = [_]FileConfig{
        FileConfig.init(try std.fs.path.join(arena, &.{ "limine", "limine-bios-cd.bin" }), "limine-bios-cd.bin"),
        FileConfig.init(try std.fs.path.join(arena, &.{ "limine", "limine-uefi-cd.bin" }), "limine-uefi-cd.bin"),
        FileConfig.init(try std.fs.path.join(arena, &.{ "limine", "limine-bios.sys" }), "limine-bios.sys"),
        FileConfig.init("limine.conf", "limine.conf"),
    };

    const efi_file_configs = [_]FileConfig{
        FileConfig.init("limine/BOOTX64.EFI", "BOOTX64.EFI"),
    };

    const xos_bin_file_configs = [_]FileConfig{
        FileConfig.init("xos", "xos"),
    };

    for (file_configs) |file_config| try updateFile(arena, arguments.input_directory, arguments.output_directory, file_config);
    for (efi_file_configs) |file_config| try updateFile(arena, arguments.input_directory, efi_output_directory, file_config);
    for (xos_bin_file_configs) |file_config| try updateFile(arena, arguments.xos_bin_directory, arguments.output_directory, file_config);

    {
        std.log.info("running xorriso", .{});
        const result = std.process.Child.run(.{
            .allocator = std.heap.c_allocator,
            .argv = &.{
                "xorriso",
                "-as",
                "mkisofs",
                "-R",
                "-r",
                "-J",
                "-b",
                "limine-bios-cd.bin",
                "-no-emul-boot",
                "-boot-load-size",
                "4",
                "-boot-info-table",
                "-hfsplus",
                "-apm-block-size",
                "2048",
                "--efi-boot",
                "limine-uefi-cd.bin",
                "-efi-boot-part",
                "--efi-boot-image",
                "--protective-msdos-label",
                arguments.output_directory,
                "-o",
                output_path,
            },
        }) catch |err| {
            fatal("failed running xorriso: {s}", .{@errorName(err)});
        };

        switch (result.term) {
            .Exited => {
                if (result.term.Exited != 0) {
                    fatal("failed xorriso: {s}", .{result.stderr});
                } else {
                    std.log.info("xorriso successfully run", .{});
                }
            },
            else => fatal("running xorriso failed", .{}),
        }
    }

    {
        std.log.info("running limine bios-install", .{});
        const result = std.process.Child.run(.{
            .allocator = std.heap.c_allocator,
            .argv = &.{
                limine_path,
                "bios-install",
                output_path,
            },
        }) catch |err| {
            fatal("failed running limine bios-install: {s}", .{@errorName(err)});
        };

        switch (result.term) {
            .Exited => {
                if (result.term.Exited != 0) {
                    fatal("failed limine bios-install: {s}", .{result.stderr});
                } else {
                    std.log.info("limine bios-install successfully run", .{});
                }
            },
            else => fatal("running limine bios-install failed", .{}),
        }
    }
}

fn updateFile(
    arena: std.mem.Allocator,
    input_directory: []const u8,
    output_directory: []const u8,
    file_config: FileConfig,
) !void {
    const input_path = try std.fs.path.join(arena, &.{ input_directory, file_config.input_filename });
    const output_path = try std.fs.path.join(arena, &.{ output_directory, file_config.output_filename });
    const update_stat = std.fs.updateFileAbsolute(input_path, output_path, .{}) catch |err| {
        fatal("failed installing for {s}: {s}\n- copy\n  from: {s}\n  to: {s}", .{
            file_config.output_filename,
            @errorName(err),
            input_path,
            output_path,
        });
    };

    switch (update_stat) {
        .stale => std.log.info("updated file {s}", .{file_config.output_filename}),
        .fresh => std.log.info("skipping file {s}", .{file_config.output_filename}),
    }
}
