# windows.nix
# compile using `nix-build windows.nix` to cross-compile for windows. This file does 3 main things
#
# 1. Make all the libraries static. DLL hell is real, and I don't want to deal with it.
# 2. Disable libraries / features wherever they were causing problems and could be disabled.
# 3. Copy stuff that was enabled for Darwin that's actually needed on all non-linux systems (e.g. windows)
#
# This was accomplished through a boatload of trial-and-error. There may well be a better way of going about this, and I encourage someone to send a pull request.

with import <nixpkgs> {
  crossSystem = {
    config = "x86_64-w64-mingw32";
    libc = "msvcrt";
  };

  overlays = [
    (self: super: {
      libconfig = super.libconfig.overrideAttrs (old: {
        configureFlags = [ "--disable-examples --enable-static" ];
        dontDisableStatic = 1;
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
        dontDisableStatic = 1;
      });
      SDL2_mixer = (super.SDL2_mixer.override {
        fluidsynth = null;
      }).overrideAttrs (old: {
        configureFlags = old.configureFlags
                         ++ [ "--disable-sdltest" "--disable-smpegtest" ] # like darwin
                         ++ [ "--disable-music-mod-modplug"
                              "--disable-music-ogg"
                              "--disable-music-opus"
                            ] # disable libraries that were causing problems compiling statically
                         ++ [ "--enable-static" "--disable-shared" ];
        autoreconfFlags = [ "--include=./acinclude" ];
        dontDisableStatic = 1;
      });
      flac = super.flac.overrideAttrs (old: {
        configureFlags = [ "--enable-static" "--disable-shared" ];
        dontDisableStatic = 1;
      });
      mpg123 = super.mpg123.overrideAttrs (old: {
        configureFlags = old.configureFlags
                         ++ [ "--enable-static" "--disable-shared" ];
        dontDisableStatic = 1;
        buildInputs = []; # get rid of alsaLib (like for darwin)
      });
      libGL = super.libGL.overrideAttrs (old:  {
        # like darwin
        buildCommand = ''
          mkdir -p $out/nix-support $dev
          echo ${self.mesa} >> $out/nix-support/propagated-build-inputs
          ln -s ${self.mesa.out}/lib $out/lib
          mkdir -p $dev/lib/pkgconfig $dev/nix-support
          echo "$out" > $dev/nix-support/propagated-build-inputs
          ln -s ${self.mesa.dev}/include $dev/include
          cat <<EOF >$dev/lib/pkgconfig/gl.pc
  Name: gl
  Description: gl library
  Version: ${self.mesa.version}
  Libs: -L${self.mesa.out}/lib -lGL
  Cflags: -I${self.mesa.dev}/include
  EOF
    cat <<EOF >$dev/lib/pkgconfig/glesv1_cm.pc
  Name: glesv1_cm
  Description: glesv1_cm library
  Version: ${self.mesa.version}
  Libs: -L${self.mesa.out}/lib -lGLESv1_CM
  Cflags: -I${self.mesa.dev}/include
  EOF
    cat <<EOF >$dev/lib/pkgconfig/glesv2.pc
  Name: glesv2
  Description: glesv2 library
  Version: ${self.mesa.version}
  Libs: -L${self.mesa.out}/lib -lGLESv2
  Cflags: -I${self.mesa.dev}/include
  EOF
        '';
      });
      mesa = super.mesa.overrideAttrs (old: {
        buildInputs = [ self.zlib ];
        propagatedBuildInputs = [];
      });
    })
  ];
};

(callPackage ./package.nix {}).overrideAttrs (old: {
  # package it up nice and tidy for transfering to windows
  postInstall = ''
    cd $out
    ${buildPackages.zip}/bin/zip -r MAR1D.zip *
  '';
})
