pub usingnamespace @cImport({
    // zig translates bool -> bool
    // but makes true and false of type c_int
    // this makes it impossible to take values defined in C code and assign them to variables defined in C code
    // SEE: https://github.com/ziglang/zig/issues/19950
    @cDefine("true", "(_Bool)1");
    @cDefine("false", "(_Bool)0");
    @cInclude("stdbool.h");
    @cUndef("true");
    @cUndef("false");
    @cDefine("true", "(_Bool)1");
    @cDefine("false", "(_Bool)0");
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
