{ nixpkgs ? import <nixpkgs> {}
, width ? 30
, mute ? false
, effects ? false
, sensitivity ? 5
, reverseY ? false
}:

let

  inherit (nixpkgs) pkgs;

  f = { stdenv
      , cmake
      , libGLU
      , xlibsWrapper
      , xorg
      , xinput_calibrator
      , doxygen
      , libpthreadstubs
      , alsaLib
      , alsaOss
      , libao 
      , emscripten
      }:
      stdenv.mkDerivation rec {
        name = "MAR1D-${version}";
        version = "0.2.0";
        options = "-w${toString width}"
                + " -s${toString sensitivity}"
                + (if mute then " -m" else "")
                + (if effects then " -f" else "")
                + (if reverseY then " -r" else "");

        src = ./.;

        buildInputs =
        [
          alsaLib
          alsaOss
          cmake
          doxygen
          libao
          libpthreadstubs
          libGLU
          xlibsWrapper
          xinput_calibrator
          xorg.libXrandr
          xorg.libXi
          xorg.xinput
          xorg.libXxf86vm
          emscripten
        ];

        preConfigure = ''
          cd src
        '';
      };

  drv = pkgs.callPackage f { };

in

  if pkgs.lib.inNixShell then drv.env else drv
