const std = @import("std");

// Although this function looks imperative, note that its job is to
// declaratively construct a build graph that will be executed by an external
// runner.
pub fn build(b: *std.Build) !void {
    const portable = b.option(bool, "portable", "Is this installation meant to be self-contained, using relative paths") orelse false;

    // options
    const static = b.option(bool, "static", "Build with static dependencies") orelse false;
    const data_dir = b.option([]const u8, "datadir", "Where to put resources") orelse "share";
    // use --prefix-exe-dir to set the output of binaries

    const target = b.standardTargetOptions(.{});
    const optimize = b.standardOptimizeOption(.{});

    const resources_install = b.addInstallDirectory(.{
        .source_dir = b.path("resources/"),
        .install_dir = .{ .custom = data_dir },
        .install_subdir = if (portable) "" else "mar1d",
    });

    const resources_install_dir =
        if (portable) data_dir else b.getInstallPath(.{ .custom = data_dir }, "mar1d");

    const resources_h = b.addConfigHeader(
        .{
            .style = .{ .cmake = b.path("src/resources.h.in") },
        },
        .{ .RESOURCE_DIRECTORY = resources_install_dir },
    );

    const exe = b.addExecutable(.{
        .name = "MAR1D",
        .root_source_file = b.path("src/main.zig"),
        .target = target,
        .optimize = optimize,
    });

    const enemies = b.addObject(.{
        .name = "enemies",
        .root_source_file = b.path("src/enemies.zig"),
        .target = target,
        .optimize = optimize,
    });

    exe.addObject(enemies);

    const source_files: []const []const u8 = &.{
        // "src/main.c",
        "src/audio.c",
        "src/controls.c",
        // "src/enemies.c",
        "src/gamelogic.c",
        "src/graphics.c",
        "src/helpers.c",
        "src/lodepng.c",
        "src/mechanics.c",
        "src/menu.c",
        "src/objects.c",
        "src/parsing.c",
        "src/rendering.c",
        "src/resources.c",
        "src/visual_sounds.c",
        "src/windowing.c",
    };

    var cflags = std.ArrayList([]const u8).init(b.allocator);

    try cflags.appendSlice(&.{ "-Werror", "-Wall", "-Wno-char-subscripts" });
    if (static) {
        try cflags.appendSlice(&.{ "-D LIBCONFIG_STATIC", "-static" });
    }

    exe.addCSourceFiles(.{ .files = source_files, .flags = cflags.items });

    exe.step.dependOn(&resources_install.step);

    // I don't think we're actually linking anything when producing the enemies object, but we need the include files.
    inline for (&.{ exe, enemies }) |c| {
        const link_mode: std.builtin.LinkMode = if (static) std.builtin.LinkMode.static else std.builtin.LinkMode.dynamic;
        const link_settings: std.Build.Module.LinkSystemLibraryOptions = .{ .preferred_link_mode = link_mode };

        c.addIncludePath(b.path("src/"));

        c.addConfigHeader(resources_h);

        c.linkSystemLibrary2("libconfig", link_settings);

        c.linkSystemLibrary2("sdl2", link_settings);

        c.linkSystemLibrary2("SDL2_mixer", link_settings);

        c.linkSystemLibrary2(
            switch (target.result.os.tag) {
                .windows => "glu32",
                else => "glu",
            },
            link_settings,
        );

        c.linkSystemLibrary2(
            switch (target.result.os.tag) {
                .windows => "opengl32",
                else => "opengl",
            },
            link_settings,
        );

        // TODO: not sure why this needs to be added explicitly (only for windows static compilation). it should be implied by SDL2_mixer
        c.linkSystemLibrary2("opusfile", link_settings);

        c.linkLibC();
    }
    b.installArtifact(exe);

    // This *creates* a Run step in the build graph, to be executed when another
    // step is evaluated that depends on it. The next line below will establish
    // such a dependency.
    const run_cmd = b.addRunArtifact(exe);

    // By making the run step depend on the install step, it will be run from the
    // installation directory rather than directly from within the cache directory.
    // This is not necessary, however, if the application depends on other installed
    // files, this ensures they will be present and in the expected location.
    run_cmd.step.dependOn(b.getInstallStep());

    // This allows the user to pass arguments to the application in the build
    // command itself, like this: `zig build run -- arg1 arg2 etc`
    if (b.args) |args| {
        run_cmd.addArgs(args);
    }

    // This creates a build step. It will be visible in the `zig build --help` menu,
    // and can be selected like this: `zig build run`
    // This will evaluate the `run` step rather than the default, which is "install".
    const run_step = b.step("run", "Run the app");
    run_step.dependOn(&run_cmd.step);
}
