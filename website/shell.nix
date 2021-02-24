# shell.nix
with { pkgs = import ./nix {}; };
pkgs.mkShell {
  buildInputs = [ pkgs.niv pkgs.nix pkgs.snack pkgs.poppler_utils ];
}
