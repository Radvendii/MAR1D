# nix/default.nix
{ sources ? import ./sources.nix }:
with
  { overlay = _: _:
      {
        niv = (import sources.niv {}).niv; # this is how you import niv into the overlay as well
        snack = (import sources.snack).snack-exe;
      };
  };
import sources.nixpkgs
  { overlays = [ overlay ] ; config = {}; }
