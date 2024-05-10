{
  description = "First person clone of Super Mario Bros.";

  inputs = {
    # XXX: zig links against the wrong glibc if we use nixpkgs master
    nixpkgs.url = "github:nixos/nixpkgs";
    flake-utils.url = "github:numtide/flake-utils";
    # my personal nix-bundle branch
    # can be switched to the main repo when PR #76 is merged
    nix-bundle.url = "github:radvendii/nix-bundle";

    zig-in = {
      url = "github:mitchellh/zig-overlay";
      inputs.nixpkgs.follows = "nixpkgs";
    };
    zls-in = {
      url = "github:zigtools/zls";
      inputs = {
        nixpkgs.follows = "nixpkgs";
        zig-overlay.follows = "zig-in";
      };
    };
  };

  outputs = { self, nixpkgs, flake-utils, nix-bundle, zig-in, zls-in }:
    flake-utils.lib.eachDefaultSystem (
      system:
      let
        overlays = [
            (final: prev: {
              zig = zig-in.packages.${system}.master;
              zls = (zls-in.packages.${system}.zls.overrideAttrs { 
                # zls is obnoxious to build, every bit of speed helps
                doCheck = false;
              });
              zigBuild = final.callPackage ./nix/zig-build { };
            })
          ];
        pkgs = import nixpkgs {
          inherit system overlays;
        };
        windows-pkgs = import ./nix/windows-pkgs.nix { inherit nixpkgs system overlays; };
        oldGlibcPkgs = import nixpkgs {
          inherit system;
          overlays = [
            nix-bundle.overlays.glibc_2_24
          ];
        };
      in rec {
        packages = {
          default = packages.game;
          game = import ./nix { inherit pkgs system; };
          darwin-app = import ./nix/darwin-app.nix { inherit nixpkgs system; };
          windows = import ./nix/windows.nix { inherit nixpkgs system overlays; };
          windows-zip = pkgs.runCommandLocal "MAR1D-windows.zip" {} ''
            mkdir -p $out
            {
              cd ${packages.windows}
              ${pkgs.zip}/bin/zip -r $out/MAR1D.zip MAR1D.exe resources
            }
          '';
          flatpak = import ./nix/flatpak.nix { inherit nixpkgs system; };
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
        apps.default = {
          type = "app";
          program = "${packages.game}/bin/MAR1D";
        };
        devShells.default = pkgs.mkShell {
          buildInputs = with pkgs; [
            zig
            zls
            SDL2
            SDL2_mixer
            libGLU
            libconfig
            pkg-config
          ];
        };
        devShells.windows = pkgs.mkShell {
          buildInputs = [
            pkgs.zig
            pkgs.zls
            windows-pkgs.buildPackages.pkg-config
            windows-pkgs.SDL2
            windows-pkgs.SDL2_mixer
            windows-pkgs.libconfig
            pkgs.wineWow64Packages.base
            (pkgs.writeShellScriptBin "pkg-config" ''
              if [ -z "$PKG_CONFIG_FOR_TARGET" ]; then
                echo "Error: PKG_CONFIG_FOR_TARGET undefined"
                exit 1
              fi
              $PKG_CONFIG_FOR_TARGET "$@"
            '')
          ];
        };
      }
    );
}
