{ stdenv
, lib
, SDL2
, SDL2_mixer
, libGLU
, libconfig
, cmake
, meson
, ninja
, pkgconfig
}:

with lib;

stdenv.mkDerivation rec {
  pname = "MAR1D";
  version = "0.2.0";

  src = ./.;

  nativeBuildInputs = [ meson ninja pkgconfig ];

  buildInputs =
    [
      SDL2
      SDL2_mixer
      libconfig
    ]
    ++ optional (!stdenv.targetPlatform.isWindows) libGLU;

  meta = with lib; {
    description = "First person Super Mario Bros";
    longDescription = ''
        The original Super Mario Bros as you've never seen it. Step into Mario's
        shoes in this first person clone of the classic Mario game. True to the
        original, however, the game still takes place in a two dimensional world.
        You must view the world as mario does, as a one dimensional line.
      '';
    homepage = https://mar1d.com;
    license = licenses.agpl3;
    maintainers = with maintainers; [ taeer ];
    platforms = platforms.linux ++ platforms.windows ++ platforms.unix;
  };
}
