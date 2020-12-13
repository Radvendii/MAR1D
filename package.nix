{ stdenv
, SDL2
, SDL2_mixer
, libGLU
, libconfig
, cmake
# necessary for dragging dlls to the right places for windows cross-compilation
# , glibc # for ldconfig
# , dos2unix
, meson
, ninja
, pkgconfig
}:

stdenv.mkDerivation rec {
  name = "MAR1D-${version}";
  version = "0.2.0";

  src = ./.;

  nativeBuildInputs = [ meson ninja pkgconfig ];
  # nativeBuildInputs = [ cmake glibc dos2unix ];

  buildInputs =
    [
      SDL2
      SDL2_mixer
      libGLU
      libconfig
    ];

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
    platforms = [ platforms.linux platforms.windows ];
  };
}
