# this doesn't work for some reason, but we have bundle and flatpak working, so whatever.

with import <nixpkgs> { };
let
  bundle = import "${nix-bundle}/share/nix-bundle/appimage-top.nix" { nixpkgs' = <nixpkgs>; };
  nixGL = import ./nixGL/default.nix { };
  mar1d = (import ./default.nix).overrideAttrs (old: {
    buildInputs = [ nixGL.nixGLDefault ] ++ old.buildInputs;
    postInstall = ''
      echo "#!/usr/bin/env bash" > $out/bin/MAR1D-wrapped
      echo "${nixGL.nixGLDefault}/bin/nixGL $out/bin/MAR1D" >> $out/bin/MAR1D-wrapped
      chmod +x $out/bin/MAR1D-wrapped
    '';
  });
in

bundle.appimage (
  bundle.appdir {
    name = "MAR1D";
    target = mar1d; # (import ./default.nix);
  }
)
