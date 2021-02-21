with import <nixpkgs> { };
let
  bundle = import "${nix-bundle}/share/nix-bundle" { nixpkgs = import <nixpkgs> { }; };
  # nixGL is not a part of nixpkgs (yet) so we have to grab it separately
  nixGL = import (fetchFromGitHub {
    owner = "guibou";
    repo = "nixGL";
    rev = "7d6bc1b21316bab6cf4a6520c2639a11c25a220e";
    sha256 = "02y38zmdplk7a9ihsxvnrzhhv7324mmf5g8hmxqizaid5k5ydpr3";
  }) { };
  mar1d = (import ./default.nix).overrideAttrs (old: {
    # make a wrapper to call it with nixGL because something's weird with nix-bundle and OpenGL.
    # this is a super janky solution and will probably fail e.g. on AMD systems
    buildInputs = [ nixGL.nixGLDefault ] ++ old.buildInputs;
    postInstall = ''
      echo "#!/usr/bin/env bash" > $out/bin/MAR1D-wrapped
      echo "${nixGL.nixGLDefault}/bin/nixGL $out/bin/MAR1D" >> $out/bin/MAR1D-wrapped
      chmod +x $out/bin/MAR1D-wrapped
    '';
  });
in
bundle.nix-bootstrap {
  target = mar1d;
  run = "/bin/MAR1D-wrapped";
}
