#!/usr/bin/env bash
outdir=$HOME/taeer.bar-yam/misc/mar1d
rm -rf ./build $outdir
mkdir build
mkdir $outdir
mkdir $outdir/resources
cd build
emconfigure cmake ../src -DCMAKE_INSTALL_PREFIX=$outdir
emmake make
emmake make install
emcc -std=gnu99 -g -s LEGACY_GL_EMULATION=1 -s GL_UNSAFE_OPTS=0 -s USE_GLFW=3 -s USE_PTHREADS=1 -s TOTAL_MEMORY=536870912 --preload-file $outdir/resources@/ -o output.html @CMakeFiles/MAR1D.dir/objects1.rsp
cp output.* $outdir
