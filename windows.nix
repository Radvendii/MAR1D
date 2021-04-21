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

      # nixpkgs PR #120017
      libconfig = super.libconfig.overrideAttrs (old: {
        configureFlags = [ "--disable-examples" ];
      });

      # nixpkgs PR #120013
      SDL2 = (super.SDL2.override {
        x11Support = false;
      }).overrideAttrs (old: {
        configureFlags = old.configureFlags
                         ++ [ "--disable-video-opengles" ];
      });

      SDL2_mixer = (super.SDL2_mixer.override {
        fluidsynth = null;
        mpg123 = null;
        opusfile = null;
        libogg = null;
        libmodplug = null;
        smpeg2 = null;
      }).overrideAttrs (old: {
        configureFlags = old.configureFlags
                         ++ [ "--disable-sdltest" "--disable-smpegtest" ] # like darwin
                         ++ [ "--disable-music-mod-modplug"
                              "--disable-music-ogg"
                              "--disable-music-opus"
                              "--disable-music-mp3"
                              "--disable-music-mp3-mpg123"
                            ]; # disable libraries that were causing problems
        autoreconfFlags = [ "--include=./acinclude" ];
      });
    })
  ];
};

(pkgsStatic.callPackage ./package.nix {}).overrideAttrs (old: {
  nativeBuildInputs =
    old.nativeBuildInputs ++ [
      buildPackages.zip
      buildPackages.perl
    ];

  # windows can't handle certain characters in file names, so we encode in ascii
  # TODO: really, this should happen in the meson.build file, so the build
  #       process is more flexible
  # TODO: really really, i shouldn't be using special characters in file names.
  #       I couldn't even figure out how to deal with them in bash and had to
  #       shell out to perl.
  postPatch = ''
    pushd resources/2D
    # dealing with these files in bash is a mess. maybe i shouldn't be using weird characters in file names to begin with...
    ls -A | perl -MFile::Copy -ne 'chomp && move($_, s/^(.)/ord($1)/re) or die "move failed $_: $!"'
    popd
  '';

  mesonFlags = [
    "--bindir=."
    "--datadir=resources"
    "-Dportable=true"
    "-Dstatic=true"
  ];

  # package it up nice and tidy for transfering to windows
  postInstall = ''
    cd $out
    zip -r MAR1D.zip *
  '';
})
