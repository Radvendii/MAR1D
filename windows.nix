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
        configureFlags = [ "--disable-examples" ];
      });
      SDL2 = (super.SDL2.override {
        x11Support = false;
        alsaSupport = false;
        waylandSupport = false;
        dbusSupport = false;
        pulseaudioSupport = false;
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
  mesonFlags = [
    "--bindir=."
    "--datadir=resources"
    "-Dportable=true"
    "-Dstatic=true"
  ];
  # package it up nice and tidy for transfering to windows
  postInstall = ''
    cd $out
    ${buildPackages.zip}/bin/zip -r MAR1D.zip *
  '';
})
