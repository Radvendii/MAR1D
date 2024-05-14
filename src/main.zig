const std = @import("std");
const c = @cImport({
    // zig translates bool -> bool
    // but makes true and false of type c_int
    // this makes it impossible to take values defined in C code and assign them to variables defined in C code
    // SEE: https://github.com/ziglang/zig/issues/19950
    @cInclude("stdbool.h");
    @cUndef("bool");
    @cDefine("bool", "int");
    //

    @cInclude("SDL.h");
    @cInclude("libconfig.h");
    @cInclude("helpers.h");
    @cInclude("resources.h");
    @cInclude("objects.h");
    @cInclude("rendering.h");
    @cInclude("gamelogic.h");
    @cInclude("controls.h");
    @cInclude("mechanics.h");
    @cInclude("graphics.h");
    @cInclude("windowing.h");
    @cInclude("parsing.h");
    @cInclude("audio.h");
    @cInclude("visual_sounds.h");
    @cInclude("menu.h");
    @cInclude("stdio.h");
    @cInclude("stdlib.h");
});

pub export var s: c.state = undefined;

pub fn main() !void {
    c.quit = c.false;

    c.rs_init();

    c.conf = c.k_defaultConf;

    c.io_readConfig(&c.conf);

    // Order matters for intializations
    c.mu_init();
    c.ob_init();
    c.gl_init();
    c.wn_init();
    c.au_init();
    c.vs_init();
    c.gr_init();

    c.mu_main(); // start the program with the menu

    // deinit in reverse order
    c.gr_deinit();
    c.vs_deinit();
    c.au_deinit();
    c.wn_deinit();
    c.gl_deinit();
    c.ob_deinit();
    c.mu_deinit();
    c.rs_deinit();

    c.exit(c.EXIT_SUCCESS);
}
