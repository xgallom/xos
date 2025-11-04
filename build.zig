const std = @import("std");

pub fn build(b: *std.Build) void {
    const host_target = b.resolveTargetQuery(.{});
    const target = b.resolveTargetQuery(.{
        .cpu_arch = .x86_64,
        .os_tag = .freestanding,
        .cpu_features_add = std.Target.x86.featureSet(&[_]std.Target.x86.Feature{.soft_float}),
        .cpu_features_sub = std.Target.x86.featureSet(&[_]std.Target.x86.Feature{.sse}),
    });

    const host_optimize = std.builtin.OptimizeMode.Debug;
    const optimize = b.standardOptimizeOption(.{});

    // std.log.info("host_target: {any}", .{host_target.result});
    // std.log.info("target: {any}", .{target.result});

    // const lib_mod = b.createModule(.{
    //     .root_source_file = b.path("src/root.zig"),
    //     .target = target,
    //     .optimize = optimize,
    // });

    const kernel_mod = b.createModule(.{
        .root_source_file = b.path("src/main.zig"),
        .target = target,
        .optimize = optimize,
        .link_libc = false,
        .code_model = .kernel,
    });

    kernel_mod.addIncludePath(b.path("limine"));
    kernel_mod.addCSourceFile(.{ .file = b.path("src/ext/limine.c") });
    kernel_mod.addCSourceFile(.{ .file = b.path("src/descriptors/isr.S") });

    const build_iso_mod = b.createModule(.{
        .root_source_file = b.path("src/build_iso.zig"),
        .target = host_target,
        .optimize = host_optimize,
    });

    // const lib = b.addLibrary(.{
    //     .linkage = .static,
    //     .name = "xos",
    //     .root_module = lib_mod,
    //     .link_libc = false,
    // });
    //
    // b.installArtifact(lib);

    const kernel = b.addExecutable(.{
        .name = "xos",
        .root_module = kernel_mod,
    });

    kernel.setLinkerScript(b.path("linker.ld"));
    b.installArtifact(kernel);

    const build_iso = b.addExecutable(.{
        .name = "xos-build-iso",
        .root_module = build_iso_mod,
    });

    const run_build_iso = b.addRunArtifact(build_iso);
    run_build_iso.step.dependOn(b.getInstallStep());
    run_build_iso.has_side_effects = true;

    run_build_iso.addArg("--input-dir");
    run_build_iso.addDirectoryArg(b.path(""));
    run_build_iso.addArg("--output-dir");
    _ = run_build_iso.addOutputDirectoryArg("image_root");
    run_build_iso.addArg("--xos-bin-dir");
    run_build_iso.addArg(b.getInstallPath(.bin, ""));

    const build_iso_step = b.step("build-iso", "Build bootable iso image");
    build_iso_step.dependOn(&run_build_iso.step);

    // const exe_unit_tests = b.addTest(.{
    //     .root_module = kernel_mod,
    // });
    //
    // const run_exe_unit_tests = b.addRunArtifact(exe_unit_tests);
    //
    // const test_step = b.step("test", "Run unit tests");
    // test_step.dependOn(&run_lib_unit_tests.step);
    // test_step.dependOn(&run_exe_unit_tests.step);
}
