{ mkDerivation, base, containers, filepath, hakyll, pandoc, stdenv
}:
mkDerivation {
  pname = "owenlynch-org";
  version = "0.1.0.0";
  src = ./.;
  isLibrary = false;
  isExecutable = true;
  executableHaskellDepends = [
    base containers filepath hakyll pandoc
  ];
  license = stdenv.lib.licenses.unfree;
  hydraPlatforms = stdenv.lib.platforms.none;
}
