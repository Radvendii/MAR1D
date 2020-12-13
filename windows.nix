with import <nixpkgs> {
  crossSystem = {
    config = "x86_64-w64-mingw32";
    libc = "msvcrt";
  };

  overlays = [
    (self: super: {
      libconfig = (super.libconfig.override {
      }).overrideAttrs (old: {
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
        configureFlags = [ "--enable-static" ];
        autoreconfFlags = [ "--include=./acinclude" ];
        dontDisableStatic = 1;
      });
      flac = super.flac.overrideAttrs (old: {
        configureFlags = [ "--enable-static" "--disable-shared" ];
        dontDisableStatic = 1;
      });
      mpg123 = super.mpg123.overrideAttrs (old: {
        buildInputs = []; # get rid of alsaLib (like for darwin)
      });
      libGL = super.libGL.overrideAttrs (old:  {
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

callPackage ./package.nix {}
# (callPackage ./package.nix {}).overrideAttrs (old : {
#   postInstall = ''
#     cp ${SDL2_mixer}/bin/SDL2_mixer.dll \
#        ${SDL2}/bin/SDL2.dll ${libconfig}/bin/libconfig-11.dll \
#        ${libmodplug}/bin/libmodplug-1.dll \
#        ${libvorbis}/bin/libvorbis-0.dll \
#        ${libvorbis}/bin/libvorbisfile-3.dll \
#        ${libogg}/bin/libogg-0.dll \
#        ${libopus}/bin/libopus-0.dll \
#        ${opusfile}/bin/libopusfile-0.dll \
#        ${stage-final-gcc}/x86_64-w64-mingw32/lib/libstdc++-6.dll \
#        $out/bin
#   '';
# })
