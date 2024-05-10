# windows.nix
# compile using `nix-build windows.nix` to cross-compile for windows.

{ nixpkgs ? <nixpkgs>
, system ? builtins.currentSystem
, overlays ? []
}:

with import ./windows-pkgs.nix { inherit nixpkgs system overlays; };

(callPackage ./package.nix {}).overrideAttrs (old: {
  NIX_LDFLAGS = "-lssp";
  zigFlags = old.zigFlags ++ [
    # $prefix doesn't get expanded here
    "--prefix-exe-dir ."
    "-Ddatadir=resources"
    "-Dportable=true"
    "-Dtarget=x86_64-windows-gnu"
  ];
  meta = old.meta // {
    platforms = old.meta.platforms ++ lib.platforms.windows;
  };
})
