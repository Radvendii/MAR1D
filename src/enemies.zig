const c = @import("c.zig");

extern var s: c.state;

export fn ai_init() void {
    var i: usize = 0;
    while (s.scene[i].type != '\x00') : (i += 1) {
        if ((s.scene[i].type == 'e' or s.scene[i].type == '&') and s.scene[i].x > s.scene[@intCast(s.pli)].x + (16 * 8)) {
            s.scene[i].active = false;
        }
    }
}

export fn ai_killAt(i: c_int) void {
    ai_kill(&s.scene[@intCast(i)]);
}

export fn ai_kill(e: [*c]c.obj) void {
    c.au_play(c.SND_shot);
    e.*.physical = false;
    e.*.vy = 8;
}

export fn ai_update() void {
    var i: usize = 0;
    while (s.scene[i].type != '\x00') : (i += 1) {
        if ((s.scene[i].type == 'e' or s.scene[i].type == '&') and s.scene[i].x < s.scene[@intCast(s.pli)].x + (16 * 8) + 16) {
            ai_activate(s.scene[i].x, s.scene[i].y);
        }
    }
}

export fn ai_activate(x: c_int, y: c_int) void {
    var i: usize = 0;
    while (s.scene[i].type != '\x00') : (i += 1) {
        if (s.scene[i].x == x and s.scene[i].y == y and s.scene[i].active == false) {
            s.scene[i].active = true;
            ai_activate(x - 2 * 16, y);
            ai_activate(x - 1 * 16, y);
            ai_activate(x + 1 * 16, y);
            ai_activate(x + 2 * 16, y);
        }
    }
}
