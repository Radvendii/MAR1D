# TODO: upstream this into nixpkgs
#       maybe as zigBuild, maybe just as regular zig
{ stdenvNoCC, zig }:
stdenvNoCC.mkDerivation {
  pname = "zig-build";
  version = zig.version;
  phases = [ "fixupPhase" ];
  propagatedBuildInputs = [ zig ];
  setupHook = ./setup-hook.sh;
}
