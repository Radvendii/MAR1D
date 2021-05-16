# darwin-app.nix
# compile using `nix-build darwin-app.nix` to compile a .app bundle for Mac OS X
#
# NOTE: this must be compiled *on* darwin as well
# TODO: would be lovely if this could cross-compile

{ nixpkgs ? <nixpkgs>
, system ? builtins.currentSystem
}:

with import nixpkgs { inherit system; };

let
  bindir = "MAR1D.app/Contents/MacOS";
  datadir = "MAR1D.app/Contents/Resources";
in
(callPackage ./package.nix {}).overrideAttrs (old: {
  nativeBuildInputs = old.nativeBuildInputs
                      ++ [ buildPackages.dylibbundler
                           buildPackages.makeWrapper
                         ];

  mesonFlags = [
    "--bindir=${bindir}"
    "--datadir=${datadir}"
    "-Dportable=true"
  ];

  postInstall = ''
    # transfer accoutrements
    cp $src/accoutrements/MAR1D.icns $out/${datadir}
    cp $src/accoutrements/Info.plist $out/MAR1D.app/Contents

    # find dynamic libraries
    mkdir $out/MAR1D.app/Contents/Libs
    dylibbundler -b -x $out/MAR1D.app/Contents/MacOS/MAR1D -d $out/MAR1D.app/Contents/Libs

    # wrap the program manually (no wrapProgram because we want to break nix purity)
    mv $out/${bindir}/{MAR1D,.MAR1D-wrapped}
    cat <<'EOF' > $out/${bindir}/MAR1D
#!/usr/bin/env bash -e
cd $(dirname $0)/../../..
export MAR1D_RECORDING_DIR="$HOME/Desktop"
exec -a "$0" "$(dirname $0)/.MAR1D-wrapped"  "$@"
EOF
    chmod +x $out/${bindir}/MAR1D
  '';

  # this should be portable to non-nix systems
  dontPatchShebangs = true;
})
