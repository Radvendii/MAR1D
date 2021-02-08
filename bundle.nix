with import <nixpkgs> { };
let
  bundle = import "${nix-bundle}/share/nix-bundle" { nixpkgs = import <nixpkgs> { }; };
in
bundle.nix-bootstrap {
  target = import ./default.nix;
  run = "/bin/MAR1D";
}
