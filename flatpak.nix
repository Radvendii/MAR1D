# flatpak.nix
# compile using `nix-build flatpak.nix` to compile a MAR1D.flatpak file for transferring to other distros
#
# TODO: clean up the main mechanics of this and move them to it's own flake

{ nixpkgs ? <nixpkgs>
, system ? builtins.currentSystem
}:

with import nixpkgs { inherit system; };

let
  inherit (lib) concatStringsSep mapAttrsToList attrValues;
  inherit (lib.strings) sanitizeDerivationName;

  myFlatpak = flatpak.override {
    bubblewrap = bubblewrap.overrideAttrs (old: {
      patches = [
        # nix doesn't give access to things like /sys/block.
        ./always-try.patch
        # nix doesn't give access to /run/user/, which is where the lock is stored
        # TODO: probably better to modify where flatpak puts the lock, but this
        #       hasn't caused me problems yet.
        ./dont-lock.patch
      ];
    });
  };
  # fix a TLS error (upstream PR #119661)
  myOSTree = ostree.overrideAttrs (old: {
    nativeBuildInputs = old.nativeBuildInputs ++ [ wrapGAppsHook ];
    buildInputs = old.buildInputs ++ [ glib-networking ];
  });

  # TODO: should probably have a fetchOSTree that actually checks out the commit
  #       and call this fetchOSTreeRaw or something
  fetchOSTree =
    { ref
    , commit
    , sha256
    , remote
      # Shell code executed before fetching. this can be used to modify
      # variables given information that is only available at runtime
    , preFetch ? ""
      # Shell code executed after the file has been fetched successfully. This
      # can do things like check or transform the files.
    , postFetch ? ""
    }:
    stdenv.mkDerivation {
      inherit ref commit remote preFetch postFetch;
      name = sanitizeDerivationName ref;
      nativeBuildInputs = [ myOSTree libfaketime ];
      buildCommand = ''
        runHook preFetch

        mkdir $out
        ostree --repo=$out init --mode bare-user-only
        ostree --repo=$out remote add remote $remote --set=tls-permissive=true --set=gpg-verify=false
        ostree --repo=$out pull remote $ref@$commit

        # re-export the ref
        ostree --repo=$out refs --create=$ref $commit

        # use fake timestamp (the unix epoch) for determinism
        FAKETIME_FMT="%s" faketime -f 0 \
        ostree --repo=$out summary -u

        # eradicate the non-determinism
        rm -rf $out/tmp/*

        runHook postFetch
      '';

      # Doing the download on a remote machine just duplicates network
      # traffic, so don't do that by default
      preferLocalBuild = true;
      outputHashMode = "recursive";
      outputHashAlgo = "sha256";
      outputHash = sha256;
    };

  fetchFlatpakRuntime =
    { id
    , branch
    , arch
    , commit
    , sha256
    , remote ? "https://flathub.org/repo/flathub.flatpakrepo"
    }:
    fetchOSTree {
      inherit remote commit sha256;
      ref = "runtime/${id}/${arch}/${branch}";
      preFetch = ''
        # taken from fetchurl
        curl=(
            ${curl}/bin/curl
            --location
            --max-redirs 20
            --retry 3
            --disable-epsv
            --cookie-jar cookies
            --insecure
        )

        # if we were given a flatpak remote, switch to the ostree remote
        if [[ $remote =~ \.flatpakrepo$ ]]; then
          remote=$("''${curl[@]}" $remote | awk -F"=" '/^Url/ { print $2 }')
        fi
      '';
    };

  mkFlatpakDerivation =
    { flatpakRuntimes ? {}
    , flatpakSources ? {}
    , ...
    }@args:
    stdenv.mkDerivation (removeAttrs args [ "flatpakRuntimes" "flatpakSources" ] // rec {
      nativeBuildInputs = (args.nativeBuildInputs or []) ++ [ myFlatpak myOSTree ];

      # TODO: should probably be a more unique name and/or not be in the build directory
      #       because what if the project has a flatpak-cruft folder?
      FLATPAK_DIR = "/build/flatpak-cruft";

      # use as `flatpak-builder --state-dir=$FLATPAK_STATE_DIR`
      FLATPAK_STATE_DIR = "${FLATPAK_DIR}/state";

      # These are used by flatpak. Together with --user, they keep the state
      # contained. Not all are necessary, but better safe than sorry.
      # Unfortunately, not all flatpak commands accept --user, so we can't make
      # it default.
      FLATPAK_USER_DIR = "${FLATPAK_DIR}/user";
      FLATPAK_SYSTEM_DIR = "${FLATPAK_DIR}/system";
      FLATPAK_CONFIG_DIR = "${FLATPAK_DIR}/config";
      FLATPAK_RUN_DIR = "${FLATPAK_DIR}/run";
      prePhases = (args.prePhases or []) ++ [ "unpackFlatpakDeps" ];

      unpackFlatpakDeps = ''
        # grab and install the pre-fetched flatpak runtimes
        ${concatStringsSep "\n" (map (dep: ''
          flatpak --user remote-add --if-not-exists ${dep.name} ${dep} --no-gpg-verify
          flatpak --user install --noninteractive ${dep.name} ${dep.ref}
        '') (attrValues flatpakRuntimes))}

        # grab the pre-fetched external sources
        ${concatStringsSep "\n" (map (src: ''
          mkdir -p $FLATPAK_STATE_DIR/downloads/${src.outputHash}
          cp ${src} $FLATPAK_STATE_DIR/downloads/${src.outputHash}/${src.name}
        '') (attrValues flatpakSources))}
      '';
    });

in

mkFlatpakDerivation {
  pname = "MAR1D-flatpak";
  version = "0.2.0";
  src = ./.;

  nativeBuildInputs = [ flatpak-builder ];

  flatpakRuntimes = {
    sdk = fetchFlatpakRuntime {
      id = "org.freedesktop.Sdk";
      branch = "20.08";
      arch = "x86_64";
      commit = "97971f6234f1a4b8474a0e31d509a913507061da6454992a5e72cd210d39e031";
      sha256 = "sha256-Y6R7Dp4P8m/BnpZEa52WVCjg36AFFET7IOQjGc77BuM=";
    };

    platform = fetchFlatpakRuntime {
      id = "org.freedesktop.Platform";
      branch = "20.08";
      arch = "x86_64";
      commit = "a4f37bb933cf4792d472db2055886427d55074cdaf5bd8e4397dc3e2bc0305e1";
      sha256 = "sha256-z1vNPC5FNX4nrk7USPtArj/bm5NujjJ5eq0/OPN0eRI=";
    };

    gl = fetchFlatpakRuntime {
      id = "org.freedesktop.Platform.GL.default";
      branch = "20.08";
      arch = "x86_64";
      commit = "a35dffb561996166d74b33de9768d6dcf34c62e8c728cbb1aa42f9f80cad1f5a";
      sha256 = "sha256-NGWV121heHYk4daJgRPQxsGo1g8Zk2pI52Fd5p25TGI=";
    };
  };

  flatpakSources = {
    libconfig = fetchurl {
      url = "https://hyperrealm.github.io/libconfig/dist/libconfig-1.7.2.tar.gz";
      sha256 = "7c3c7a9c73ff3302084386e96f903eb62ce06953bb1666235fac74363a16fad9";
    };

    glu = fetchurl {
      url = "https://mesa.freedesktop.org/archive/glu/glu-9.0.1.tar.gz";
      sha256 = "f6f484cfcd51e489afe88031afdea1e173aa652697e4c19ddbcb8260579a10f7";
    };
  };

  buildPhase = ''
    flatpak-builder --user                          \
                    --disable-download              \
                    --assumeyes                     \
                    --disable-rofiles-fuse          \
                    --repo=$FLATPAK_DIR/repo        \
                    --state-dir=$FLATPAK_STATE_DIR  \
                    --force-clean                   \
                    $FLATPAK_DIR/build              \
                    com.mar1d.MAR1D.yml
    flatpak build-bundle $FLATPAK_DIR/repo          \
                         MAR1D.flatpak              \
                         com.mar1d.MAR1D            \
                         --runtime-repo=https://flathub.org/repo/flathub.flatpakrepo
  '';

  installPhase = ''
    mkdir $out
    cp MAR1D.flatpak $out
  '';
}
