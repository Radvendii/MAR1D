This website is generated with [hakyll](https://jaspervdj.be/hakyll/), using [hakyll-flake-gen](https://github.com/Radvendii/hakyll-flake-gen).

On a system with the Nix package manager with nix-flakes enabled, there are a number of things you can do:

`nix build`: generate the website (found at ./result)
`nix build .#builder`: generate the hakyll site builder (found at ./result/bin/mar1d-site)
`nix run . -- watch`: generate the site and host it at localhost:8000
```

It will take while the first time.
