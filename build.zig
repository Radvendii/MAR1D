const std = @import("std");
const Style = std.Build.ConfigHeaderStep.Style;

// Although this function looks imperative, note that its job is to
// declaratively construct a build graph that will be executed by an external
// runner.
pub fn build(b: *std.Build) void {
    // Standard target options allows the person running `zig build` to choose
    // what target to build for. Here we do not override the defaults, which
    // means any target is allowed, and the default is native. Other options
    // for restricting supported target set are available.
    const target = b.standardTargetOptions(.{});

    // Standard optimization options allow the person running `zig build` to select
    // between Debug, ReleaseSafe, ReleaseFast, and ReleaseSmall. Here we do not
    // set a preferred release mode, allowing the user to decide how to optimize.
    const optimize = b.standardOptimizeOption(.{});

    const resources_dir = b.addInstallDirectory(.{
        .source_dir = "resources/",
        .install_dir = .{ .custom = "share" },
        .install_subdir = "resources",
    });

    const resources_h = b.addConfigHeader(.{
        .style = .{ .cmake = .{ .path = "src/resources.h.in" } },
    }, .{ .k_resourceDir = b.getInstallPath(.{ .custom = "share" }, "resources") });

    const exe = b.addExecutable(.{
        .name = "MAR1D",
        // In this case the main source file is merely a path, however, in more
        // complicated build scripts, this could be a generated file.
        .root_source_file = .{ .path = "src/main.c" },
        .target = target,
        .optimize = optimize,
    });
    exe.addCSourceFiles(&.{ "src/audio.c", "src/controls.c", "src/enemies.c", "src/gamelogic.c", "src/graphics.c", "src/helpers.c", "src/lodepng.c", "src/mechanics.c", "src/menu.c", "src/objects.c", "src/parsing.c", "src/rendering.c", "src/resources.c", "src/visual_sounds.c", "src/windowing.c" }, &.{ "-Werror", "-Wall", "-Wno-char-subscripts" });
    exe.addConfigHeader(resources_h);
    exe.linkSystemLibrary("opengl");
    exe.linkSystemLibrary("sdl2");
    exe.linkSystemLibrary("SDL2_mixer");
    exe.linkSystemLibrary("glu");
    exe.linkSystemLibrary("libconfig");
    exe.addIncludePath("src/");
    exe.linkLibC();

    // This declares intent for the executable to be installed into the
    // standard location when the user invokes the "install" step (the default
    // step when running `zig build`).
    exe.step.dependOn(&resources_dir.step);
    exe.install();

    // This *creates* a RunStep in the build graph, to be executed when another
    // step is evaluated that depends on it. The next line below will establish
    // such a dependency.
    const run_cmd = exe.run();

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
