#!/bin/bash
# Xiang Wang(ramwin@qq.com)

set -e

if [ ! -d "build_count_bb" ];
then
    mkdir build_count_bb
fi
cd build_count_bb
cmake ../HelloWorld/
cmake --build .
clang -O0 -Xclang -disable-O0-optnone -S -emit-llvm ../inputs/input_for_count_bb.c -o input_for_count_bb.ll
opt -load-pass-plugin ./libCountBB.so -passes=count-bb -disable-output input_for_count_bb.ll
