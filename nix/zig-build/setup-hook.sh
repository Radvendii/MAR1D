zigBuildPhase() {
    runHook preBuild

    if [ -z "${dontAddPrefix-}" ]; then
        zigFlags="--prefix $prefix $zigFlags"
    fi

    zigFlags="-Doptimize=${zigOptimize:-ReleaseFast} $zigFlags"

    # See multiple-outputs.sh
    zigFlags="\
        --prefix-lib-dir ${!outputLib}/lib \
        --prefix-exe-dir ${!outputBin}/bin \
        --prefix-include-dir ${!outputInclude}/include \
        $zigFlags"

    zigFlags="${crossZigFlags+$crossZigFlags } $zigFlags"

    echo "zig flags: $zigFlags ${zigFlagsArray[@]}"

    export HOME=$TMPDIR
    zig build $zigFlags "${zigFlagsArray[@]}"
    runHook postBuild
}

if [ -z "${dontUseZigBuild-}" -a -z "${buildPhase-}" ]; then
    setOutputFlags=
    buildPhase=zigBuildPhase
fi
