with import <nixpkgs> { };
with xorg;

let 
  width = 30;
  mute = false;
  effects = false;
  sensitivity = 5;
  reverseY = false;
in

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
      SDL2
      alsaLib
      alsaOss
      cmake
      doxygen
      libao
      libpthreadstubs
      libGLU
    ];

  preConfigure = ''
    cd src
  '';

  meta = with stdenv.lib; {
    description = "First person Super Mario Bros";
    longDescription = ''
      The original Super Mario Bros as you've never seen it. Step into Mario's
      shoes in this first person clone of the classic Mario game. True to the
      original, however, the game still takes place in a two dimensional world.
      You must view the world as mario does, as a one dimensional line.
    '';
    homepage = https://github.com/olynch/fp_mario;
    license = licenses.agpl3;
    maintainers = with maintainers; [ taeer ];
    platforms = platforms.linux;
  };
}
