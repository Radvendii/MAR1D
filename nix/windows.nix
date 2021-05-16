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

  crossOverlays = [
    # statically compile everything same as pkgsStatic
    (import "${nixpkgs}/pkgs/top-level/static.nix")

    (self: super: {

      # nixpkgs PR #120973
      libconfig = super.libconfig.overrideAttrs (old: {
        nativeBuildInputs = (old.nativeBuildInputs or []) ++ [ super.autoconf super.automake115x ];
        configureFlags = (old.configureFlags or []) ++ [ "--disable-tests" ];
        # libconfig PR #190
        patches = (old.patches or []) ++ [ ./patches/libconfig-disable-tests.patch ];
        cmakeFlags = (old.cmakeFlags or []) ++ [ "-DBUILD_TESTS:BOOL=OFF" ];
        doCheck = false;
      });

      SDL2_mixer = (super.SDL2_mixer.override {
        fluidsynth = null; # fluidsynth is broken in like a dozen different ways.
        libmodplug = null; # SDL2_mixer can't find libmodplug for some reason
        smpeg2 = null; # this can be upstreamed; smpeg isn't a dependency anymore
      }).overrideAttrs (old: {
        # src = super.fetchFromGitHub {
        #   owner = "libsdl-org";
        #   repo = "SDL_mixer";
        #   rev = "4d2fec78b8deca24ce6c7f7a9e3725cf31f75896";
        #   sha256 = "sha256-Z0uIGEoSrsBhZeF859mcEK+7OCVdtlx5mQaLvKyahLg=";
        # };
        # https://github.com/libsdl-org/SDL_mixer/pull/313
        patches = (old.patches or []) ++ [ ./patches/pkgconfig_requires.patch ];
        # patches = (old.patches or []) ++ [ ./patches/sdl2_mixer-pkgconfig.patch ];
        # patches = (old.patches or []) ++ [ ./patches/sdl2-mixer-new.patch ];
        # # dontAddDisableDepTrack = true;
        # # dontFixLibtool = true;
        # # dontPruneLibtoolFiles = true;
        # nativeBuildInputs = old.nativeBuildInputs
        #   ++ (with super.buildPackages; [ autoconf automake which ]);

        preConfigure = ''
          ./autogen.sh
        '';

        # this doesn't help the modplug issue
        # NIX_LDFLAGS = [ "-L${super.libmodplug}/lib -lmodplug" ];

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
  mesonFlags = old.mesonFlags ++ [
    "--bindir=."
    "--datadir=resources"
    "-Dportable=true"
  ];
  meta = old.meta // {
    platforms = old.meta.platforms ++ lib.platforms.windows;
  };
})
