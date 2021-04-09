{
  description = "First person clone of Super Mario Bros.";

  inputs = {
    nixpkgs.url = "github:nixos/nixpkgs";
    flake-utils.url = "github:numtide/flake-utils";
    nix-bundle.url = "github:radvendii/nix-bundle";
    website.url = "path:./website";
  };

  outputs = { self, nixpkgs, flake-utils, nix-bundle, website }:
    flake-utils.lib.eachDefaultSystem (
      system:
      let
        pkgs = nixpkgs.legacyPackages."${system}";
        oldGlibcPkgs = import nixpkgs {
          inherit system;
          overlays = [
            nix-bundle.overlays.glibc_2_24
          ];
        };
      in rec {
        defaultPackage = packages.game;
        packages = {
          website = website.defaultPackage.${system};
          game = import ./. { inherit nixpkgs system; };
          darwin-app = import ./darwin-app.nix { inherit nixpkgs system; };
          windows = import ./windows.nix { inherit nixpkgs system; };
          appimage = nix-bundle.bundlers.appimage {
            inherit system;
            # trial and error removing them from the AppDir
            excludePkgs = with oldGlibcPkgs;
              [ llvm_11
                mesa
                glibc
                glibcInfo
                glibcLocales
                systemd
                systemdMinimal
                glib
                ncurses
                xorg.xkeyboardconfig
                util-linuxMinimal
                bashInteractive
                kbd
                shadow
              ];
            target = (oldGlibcPkgs.callPackage ./package.nix { }).overrideAttrs (old: {
              mesonFlags = [ "-Dportable=true" ];
            });
          };
        };
      }
    );
}
