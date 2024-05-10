{ nixpkgs ? <nixpkgs>
, system ? builtins.currentSystem
, overlays ? []
}:

import nixpkgs {
  inherit system overlays;
  crossSystem = {
    isStatic = true;
    config = "x86_64-w64-mingw32";
    libc = "msvcrt";
  };

  config.allowUnsupportedSystem = true;

  crossOverlays = [(self: super: {

    libconfig = super.libconfig.overrideAttrs (old: {
      # there's some conflict between the way nix builds libconfig (with msvc?)
      # and the way zig expects it (not depending on msvc) that causes a linker
      # error
      patches = [ ./patches/libconfig-no-msvc.patch ];
    });

    flac = self.callPackage ({cmake, pkg-config, doxygen}: super.flac.overrideAttrs (old: {
      # remove graphviz, which doesn't seem necessary, and fails to build even thought it's in nativeBuildInputs
      nativeBuildInputs = [ cmake pkg-config doxygen ];
    })) {};

    SDL2 = super.SDL2.overrideAttrs (old: {
      # this is for dynamic linking. 
      # UPSTREAM: checking whether dynamic linking is happening
      postFixup = "";
    });


    SDL2_mixer = (super.SDL2_mixer.override {
      fluidsynth = null; # fluidsynth is broken in like a dozen different ways.
      libmodplug = null; # SDL2_mixer can't find libmodplug for some reason
      smpeg2 = null; # this can be upstreamed; smpeg isn't a dependency anymore

      timidity = null; # didn't work; didn't really look into why; don't need it
      mpg123 = null; # didn't work; didn't really look into why; don't need it
    }).overrideAttrs (old: rec {
      # UPSTREAM: using SDL2.dev in buildInputs rather than SDL2 in nativeBuildInputs
      nativeBuildInputs = [ self.pkg-config self.SDL2.dev ];

      configureFlags = [ # copied from upstream (remove timidity)
        "--disable-music-ogg-shared"
        "--disable-music-flac-shared"
        "--disable-music-mod-modplug-shared"
        "--disable-music-mp3-mpg123-shared"
        "--disable-music-opus-shared"
        "--disable-music-midi-fluidsynth-shared"
      ] ++ [
        "--disable-sdltest" # like darwin UPSTREAM:
        "--disable-music-mod-modplug"
      ];
      meta = old.meta // {
        platforms = old.meta.platforms ++ super.lib.platforms.windows;
      };
    });
  })];
}
