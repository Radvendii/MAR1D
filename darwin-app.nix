# darwin-app.nix
# compile using `nix-build darwin-app.nix` to compile a .app bundle for Mac OS X
#
# Note that this must be compiled *on* darwin as well

with import <nixpkgs> { };

(callPackage ./package.nix {}).overrideAttrs (old: {
  mesonFlags = [
    "--bindir=MAR1D.app/Contents/MacOS"
    "--datadir=MAR1D.app/Contents/Resources"
    "-Dportable=true"
  ];
  postInstall = ''
    cd $out
    cp $src/MAR1D.icns MAR1D.app/Contents/Resources
    cp $src/Info.plist MAR1D.app/Contents
    cp $src/MAR1D-MacOS-wrap MAR1D.app/Contents/MacOS
    mkdir MAR1D.app/Contents/Libs
    ${buildPackages.dylibbundler}/bin/dylibbundler -b -x MAR1D.app/Contents/MacOS/MAR1D -d MAR1D.app/Contents/Libs
  '';
})
