# windows.nix
# compile using `nix-build windows.nix` to cross-compile for windows. This file does 3 main things
#
# 1. Make all the libraries static. DLL hell is real, and I don't want to deal with it.
# 2. Disable libraries / features wherever they were causing problems and could be disabled.
# 3. Copy stuff that was enabled for Darwin that's actually needed on all non-linux systems (e.g. windows)
#
# This was accomplished through a boatload of trial-and-error. There may well be a better way of going about this, and I encourage someone to send a pull request.

{ nixpkgs ? <nixpkgs>
, system ? builtins.currentSystem
}:

with import nixpkgs {
  inherit system;
  crossSystem = {
    config = "x86_64-w64-mingw32";
    libc = "msvcrt";
  };

  config = {
    allowUnsupportedSystem = true;
    permittedInsecurePackages = [
      "openssl-1.0.2u"
    ];
  };

  crossOverlays = [
    (self: super: {

      libconfig = super.libconfig.overrideAttrs (old: {
        nativeBuildInputs = (old.nativeBuildInputs or []) ++ [ super.autoconf super.automake115x ];
        configureFlags = (old.configureFlags or []) ++ [ "--disable-tests" ];
        # libconfig PR #190
        patches = (old.patches or []) ++ [ ./libconfig-disable-tests.patch ];
        cmakeFlags = (old.cmakeFlags or []) ++ [ "-DBUILD_TESTS:BOOL=OFF" ];
        doCheck = false;
      });

      SDL2_mixer = (super.SDL2_mixer.override {
        fluidsynth = null;
        opusfile = null;
        libmodplug = null;
        smpeg2 = null;
      }).overrideAttrs (old: {
        configureFlags = old.configureFlags
                         ++ [ "--disable-sdltest" "--disable-smpegtest" ] # like darwin
                         ++ [ "--disable-music-mod-modplug"
                              "--disable-music-opus"
                            ]; # disable libraries that were causing problems

        # TODO: shouldn't need to specify transitive dependencies manually
        NIX_LDFLAGS = [ "-logg" ];

        autoreconfFlags = [ "--include=./acinclude" ];
      });
    })
  ];
};

(pkgsStatic.callPackage ./package.nix {}).overrideAttrs (old: {

  # TODO: shouldn't need to specify transitive dependencies manually
  NIX_LDFLAGS = [
    "-lvorbisfile"
    "-lvorbisenc"
    "-lvorbis"
    "-lFLAC"
    "-logg"
  ];

  mesonFlags = [
    "--bindir=."
    "--datadir=resources"
    "-Dportable=true"
    "-Dstatic=true"
  ];
})
