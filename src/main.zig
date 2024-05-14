const std = @import("std");
const c = @import("c.zig");

pub export var s: c.state = undefined;

pub fn main() !void {
    c.quit = false;

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
