{
  description = "MAR1D Website";
  inputs.hakyll-flakes.url = "github:Radvendii/hakyll-flakes";
  inputs.flake-utils.url = "github:numtide/flake-utils";
  inputs.nixpkgs.url = "github:nixos/nixpkgs";

  outputs = { self, hakyll-flakes, flake-utils, nixpkgs }:
    flake-utils.lib.eachDefaultSystem (
      system:
      hakyll-flakes.lib.mkAllOutputs {
        inherit system;
        name = "mar1d-site";
        src = ./.;
      }
    );
}
