# windows.nix
# compile using `nix-build windows.nix` to cross-compile for windows.

{ nixpkgs ? <nixpkgs>
, system ? builtins.currentSystem
}:

with import nixpkgs {
  inherit system;
  crossSystem = {
    isStatic = true;
    config = "x86_64-w64-mingw32";
    libc = "msvcrt";
  };

  config.allowUnsupportedSystem = true;

  crossOverlays = [

    (self: super: {

      flac = self.callPackage ({cmake, pkg-config, doxygen}: super.flac.overrideAttrs (old: {
        # remove graphviz, which doesn't seem necessary, and fails to build even thought it's in nativeBuildInputs
        nativeBuildInputs = [ cmake pkg-config doxygen ];
      })) {};

      libopus = super.libopus.overrideAttrs (old: {
        NIX_LDFLAGS = "-lssp";
      });

      SDL2_mixer = (super.SDL2_mixer.override {
        fluidsynth = null; # fluidsynth is broken in like a dozen different ways.
        libmodplug = null; # SDL2_mixer can't find libmodplug for some reason
        smpeg2 = null; # this can be upstreamed; smpeg isn't a dependency anymore

        timidity = null; # didn't work; didn't really look into why; don't need it
        mpg123 = null; # didn't work; didn't really look into why; don't need it
      }).overrideAttrs (old: rec {

        # SDL2_mixer is woefully outdated in nixpkgs
        # TODO: upstream
        version = "2.6.2";
        src = self.fetchFromGitHub {
          owner = "libsdl-org";
          repo = "SDL_mixer";
          rev = "release-${version}";
          sha256 = "sha256-ZdPqNyRI9QnZ2qyrz1W63VHzJXO6n1STrT+TS9gNweM=";
        };

        NIX_LDFLAGS = "-lssp";
        # remove timidity
        postPatch = "";

        configureFlags = old.configureFlags
          ++ [ "--disable-sdltest" # like darwin (can be upstreamed)
               "--disable-music-mod-modplug"
             ];
        meta = old.meta // {
          platforms = old.meta.platforms ++ super.lib.platforms.windows;
        };
      });
    })
  ];
};

(callPackage ./package.nix {}).overrideAttrs (old: {
  NIX_LDFLAGS = "-lssp";
  mesonFlags = old.mesonFlags ++ [
    "--bindir=."
    "--datadir=resources"
    "-Dportable=true"
  ];
  meta = old.meta // {
    platforms = old.meta.platforms ++ lib.platforms.windows;
  };
})
