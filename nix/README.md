MAR1D - /nix
===

``` sh
$ tree -L 1
.                  # nix build files
├── darwin-app.nix # build a .app bundle for macOS
├── default.nix    # build the usual nix outputs
├── flatpak.nix    # build a flatpak using nix
├── package.nix    # a .nix file suited for nixpkgs
├── windows.nix    # cross-compile to windows
└── patches/       # patches for upstream packages
```
