#!/bin/bash
# Xiang Wang(ramwin@qq.com)

set -e

if [ ! -d "build_delete_instruction" ];
then
    mkdir build_delete_instruction
fi
cd build_delete_instruction
cmake ../HelloWorld/
cmake --build .
clang -O0 -Xclang -disable-O0-optnone -S -emit-llvm ../inputs/input_for_delete_instruction.c -o input_for_delete_instruction.ll
opt -load-pass-plugin ./libDeleteInstruction.so \
    -S \
    -passes=delete-instruction \
    -o input_for_delete_instruction2.ll \
    input_for_delete_instruction.ll
lli input_for_delete_instruction2.ll
