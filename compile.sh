#!/usr/bin/env bash
rm -rf ./build
mkdir build
mkdir install
cd build
cmake ../src -DCMAKE_INSTALL_PREFIX=../install
make
make install
