# windows.nix
# compile using `nix-build windows.nix` to cross-compile for windows.

{ nixpkgs ? <nixpkgs>
, system ? builtins.currentSystem
, overlays ? []
}:

with import ./windows-pkgs.nix { inherit nixpkgs system overlays; };

(callPackage ./package.nix {}).overrideAttrs (old: {
  nativeBuildInputs = old.nativeBuildInputs ++ [(
    # somebody's done something wrong here. I don't know if zig should be
    # looking for pkg-config somewhere else, or if nix should be providing
    # a pkg-config binary with the appropriate name. Either way this is a
    # compatibility hack
    # TODO: work it out upstream
    buildPackages.runCommand "rename-pkg-config" {} ''
      mkdir -p $out/bin
      ln -s ${buildPackages.pkg-config}/bin/x86_64-w64-mingw32-pkg-config $out/bin/pkg-config
    ''
  )];
  dontAutoPatchelf = true;
  NIX_LDFLAGS = "-lssp";
  zigFlags = old.zigFlags ++ [
    "--prefix-exe-dir ."
    "-Ddatadir=resources"
    "-Dportable=true"
    "-Dtarget=x86_64-windows-gnu"
  ];
  meta = old.meta // {
    platforms = old.meta.platforms ++ lib.platforms.windows;
  };
})
