#!/bin/sh

rm -rf build/
mkdir -p build
cd build
cmake -DCMAKE_SH="CMAKE_SH-NOTFOUND" -G "MinGW Makefiles" ..
make
