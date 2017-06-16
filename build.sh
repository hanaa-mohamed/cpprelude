#!/usr/bin/env bash
premake5 gmake --cc=clang
cd build
make -j4 config=$1shared_x86_64 $2
cd ../
