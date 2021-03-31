# darwin-app.nix
# compile using `nix-build darwin-app.nix` to compile a .app bundle for Mac OS X
#
# NOTE: this must be compiled *on* darwin as well
# TODO: would be lovely if this could cross-compile

{ nixpkgs ? <nixpkgs>
, system ? builtins.currentSystem
}:

with import nixpkgs { inherit system; };

(callPackage ./package.nix {}).overrideAttrs (old: {
  mesonFlags = [
    "--bindir=MAR1D.app/Contents/MacOS"
    "--datadir=MAR1D.app/Contents/Resources"
    "-Dportable=true"
  ];
  postInstall = ''
    cd $out
    cp $src/darwin/MAR1D.icns MAR1D.app/Contents/Resources
    cp $src/darwin/Info.plist MAR1D.app/Contents
    cp $src/darwin/MAR1D-MacOS-wrap MAR1D.app/Contents/MacOS
    mkdir MAR1D.app/Contents/Libs
    ${buildPackages.dylibbundler}/bin/dylibbundler -b -x MAR1D.app/Contents/MacOS/MAR1D -d MAR1D.app/Contents/Libs
  '';
})
