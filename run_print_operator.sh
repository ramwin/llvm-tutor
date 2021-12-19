#!/bin/bash
# Xiang Wang(ramwin@qq.com)

set -e

if [ ! -d "build_print_operator" ];
then
    mkdir build_print_operator
fi
cd build_print_operator
cmake ../HelloWorld/
cmake --build .
clang -O0 -Xclang -disable-O0-optnone -S -emit-llvm ../inputs/input_for_print_operator.c -o input_for_print_operator.ll
opt -load-pass-plugin ./libPrintOperator.so -passes=print-operator input_for_print_operator.ll -o input_for_print_operator2.ll
lli input_for_print_operator2.ll
