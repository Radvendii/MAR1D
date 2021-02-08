with import <nixpkgs> { };
let
  bundle = import "${nix-bundle}/share/nix-bundle/appimage-top.nix" { nixpkgs' = <nixpkgs>; };
in

bundle.appimage (
  bundle.appdir {
    name = "MAR1D";
    target = (import ./default.nix);
  }
)
