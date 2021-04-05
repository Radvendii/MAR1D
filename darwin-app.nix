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
  nativeBuildInputs = old.nativeBuildInput
                      ++ [ buildPackages.dylibbundler
                           buildPackages.makeWrapper
                         ];

  mesonFlags = [
    "--bindir=${bindir}"
    "--datadir=${datadir}"
    "-Dportable=true"
  ];

  postInstall = ''
    cp darwin/MAR1D.icns $out/${datadir}
    cp darwin/Info.plist $out/MAR1D.app/Contents
    # cp darwin/MAR1D-MacOS-wrap $out/${bindir}
    # cd into MAR1D.app and save recordings to the desktop
    wrapProgram $out/MAR1D.app/Contents/MacOS/MAR1D \
      --run 'cd $(dirname $0)/../../..' \
      --set MAR1D_RECORDING_DIR '$HOME/Desktop'
    # need the wrapper to refer to relative path of the executable
    substituteInPlace $out/${bindir}/MAR1D \
      --replace "$out/MAR1D.app/" './'
    mkdir $out/MAR1D.app/Contents/Libs
    dylibbundler -b -x $outMAR1D.app/Contents/MacOS/MAR1D -d $out/MAR1D.app/Contents/Libs
  '';
})
