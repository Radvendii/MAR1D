const std = @import("std");
const builtin = @import("builtin");
const Style = std.Build.ConfigHeaderStep.Style;
const Allocator = std.mem.Allocator;
const NativePaths = std.zig.system.NativePaths;

pub fn getNixFlags(allocator: std.mem.Allocator) ?NativePaths {
    // we don't actually care what this is. it's not used by the nix detection code
    if (std.process.hasEnvVarConstant("NIX_CFLAGS_COMPILE") or std.process.hasEnvVarConstant("NIX_LDFLAGS")) {
        const garbage_nti = std.zig.system.NativeTargetInfo{ .target = builtin.target, .dynamic_linker = std.zig.system.NativeTargetInfo.DynamicLinker.init(null) };
        return NativePaths.detect(allocator, garbage_nti) catch unreachable;
    }
    return null;
}

pub fn build(b: *std.Build) !void {
    var gpa = std.heap.GeneralPurposeAllocator(.{}){};
    var arena = std.heap.ArenaAllocator.init(gpa.allocator());
    defer arena.deinit();

    var cflags = std.ArrayList([]const u8).init(arena.allocator());
    defer cflags.deinit();

    const portable = b.option(bool, "portable", "Is this installation meant to be self-contained, using relative paths") orelse false;
    const static = b.option(bool, "static", "Build with static dependencies") orelse false;
    const data_dir = b.option([]const u8, "datadir", "Where to put resources") orelse "share";
    const bin_dir = b.option([]const u8, "bindir", "Where to put binaries") orelse "bin";
    _ = bin_dir;

    // Standard target options allows the person running `zig build` to choose
    // what target to build for. Here we do not override the defaults, which
    // means any target is allowed, and the default is native. Other options
    // for restricting supported target set are available.
    const target = b.standardTargetOptions(.{});

    // Standard optimization options allow the person running `zig build` to select
    // between Debug, ReleaseSafe, ReleaseFast, and ReleaseSmall. Here we do not
    // set a preferred release mode, allowing the user to decide how to optimize.
    const optimize = b.standardOptimizeOption(.{});

    const resources_install = b.addInstallDirectory(.{
        .source_dir = "resources/",
        .install_dir = .{ .custom = data_dir },
        .install_subdir = if (portable) "" else "mar1d",
    });

    const resources_h = b.addConfigHeader(.{
        .style = .{ .cmake = .{ .path = "src/resources.h.in" } },
    }, .{ .k_resourceDir = if (portable) data_dir else b.getInstallPath(.{ .custom = data_dir }, "mar1d") });

    const exe = b.addExecutable(.{
        .name = "MAR1D",
        // In this case the main source file is merely a path, however, in more
        // complicated build scripts, this could be a generated file.
        .root_source_file = .{ .path = "src/main.c" },
        .target = target,
        .optimize = optimize,
    });

    try cflags.appendSlice(&.{ "-Werror", "-Wall", "-Wno-char-subscripts" });
    if (static) {
        try cflags.appendSlice(&.{ "-D LIBCONFIG_STATIC", "-static" });
    }

    // TODO: complain to zig fmt about this formatting
    exe.addCSourceFiles(&.{ "src/audio.c", "src/controls.c", "src/enemies.c", "src/gamelogic.c", "src/graphics.c", "src/helpers.c", "src/lodepng.c", "src/mechanics.c", "src/menu.c", "src/objects.c", "src/parsing.c", "src/rendering.c", "src/resources.c", "src/visual_sounds.c", "src/windowing.c" }, cflags.items);
    exe.addConfigHeader(resources_h);

    if (false) {
        // exe.addObjectFile("libSDL2.a");
        // exe.addObjectFile("libSDL2_mixer.a");
        // exe.addObjectFile("libconfig.a");
        // exe.addObjectFile("OpenGL.a");
        // exe.addObjectFile("libGLU.a");
        exe.addObjectFile("/nix/store/hrzz0djnxav1r88vx70nacvfddh6nq00-SDL2-static-x86_64-w64-mingw32-2.26.4/lib/libSDL2.a");
        exe.addObjectFile("/nix/store/hrzz0djnxav1r88vx70nacvfddh6nq00-SDL2-static-x86_64-w64-mingw32-2.26.4/lib/libSDL2main.a");
        exe.addObjectFile("/nix/store/216i33q43pzc7dsybjx6s09595mzhilg-SDL2_mixer-static-x86_64-w64-mingw32-2.6.3/lib/libSDL2_mixer.a");
        exe.addObjectFile("/nix/store/wyld9vz7myxl4wsp3pf9dx89i8652m2l-libconfig-static-x86_64-w64-mingw32-1.7.3/lib/libconfig.a");
        exe.linkSystemLibrary("opengl32");
        exe.linkSystemLibrary("glu32");
    } else {
        exe.linkSystemLibrary("sdl2");
        exe.linkSystemLibrary("SDL2_mixer");
        exe.linkSystemLibrary("libconfig");
        exe.linkSystemLibrary(switch (builtin.target.os.tag) {
            .windows => "opengl32",
            else => "opengl",
        });
        exe.linkSystemLibrary(switch (builtin.target.os.tag) {
            .windows => "glu32",
            else => "glu",
        });
    }
    exe.addIncludePath("src/");
    exe.addIncludePath("/nix/store/rwql2h2avh2gvzl60hnlhhib0j65bx15-SDL2-static-x86_64-w64-mingw32-2.26.4-dev/include/SDL2");
    exe.addIncludePath("/nix/store/qlsgvqw2rqkxs9jl4z4gq3s1jl7xzh0q-SDL2_mixer-static-x86_64-w64-mingw32-2.6.3-dev/include/SDL2");

    exe.linkLibC();

    // add nix flags

    if (getNixFlags(arena.allocator())) |paths| {
        for (paths.warnings.items) |warning| {
            std.log.warn("\n\n{s}\n\n", .{warning});
        }
        for (paths.include_dirs.items) |include_dir| {
            exe.addIncludePath(include_dir);
        }
        for (paths.framework_dirs.items) |framework_dir| {
            exe.addFrameworkPath(framework_dir);
        }
        for (paths.lib_dirs.items) |lib_dir| {
            exe.addLibraryPath(lib_dir);
        }
        for (paths.rpaths.items) |rpath| {
            exe.addRPath(rpath);
        }
    }

    // This declares intent for the executable to be installed into the
    // standard location when the user invokes the "install" step (the default
    // step when running `zig build`).
    exe.step.dependOn(&resources_install.step);
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
