{
  description = "First person clone of Super Mario Bros.";

  inputs = {
    nixpkgs.url = "github:nixos/nixpkgs";
    flake-utils.url = "github:numtide/flake-utils";
    nix-bundle.url = "github:radvendii/nix-bundle";
  };

  outputs = { self, nixpkgs, flake-utils, nix-bundle }:
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
          game = import ./. { inherit nixpkgs system; };
          darwin-app = import ./darwin-app.nix { inherit nixpkgs system; };
          windows = import ./windows.nix { inherit nixpkgs system; };
          # no way to bundle flatpak with nix yet
          # flatpak = import ./flatpak.nix { inherit nixpkgs system; };
          appimage = nix-bundle.bundlers.appimage {
            inherit system;
            target = (oldGlibcPkgs.callPackage ./package.nix { }).overrideAttrs (old: {
              mesonFlags = [ "-Dportable=true" ];
            });

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
          };
        };
        defaultApp = {
          type = "app";
          program = "${packages.game}/bin/MAR1D";
        };
      }
    );
}
