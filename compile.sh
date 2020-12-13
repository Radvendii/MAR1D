#!/usr/bin/env bash
rm -rf ./build ./install
mkdir build
mkdir install
cd build
cmake .. -DCMAKE_INSTALL_PREFIX=../install
make
make install
