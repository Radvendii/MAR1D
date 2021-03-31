{
  description = "MAR1D Website";
  inputs.hakyll-flake.url = "github:Radvendii/hakyll-flake-gen";
  inputs.flake-utils.url = "github:numtide/flake-utils";
  inputs.nixpkgs.url = "github:nixos/nixpkgs";

  outputs = { self, hakyll-flake, flake-utils, nixpkgs }:
    flake-utils.lib.eachDefaultSystem (
      system:
      hakyll-flake.gen (
        {
          inherit system;
          name = "mar1d-site";
          src = ./.;
        }
      )
    );
}
