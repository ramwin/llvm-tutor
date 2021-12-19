#!/bin/bash
# Xiang Wang(ramwin@qq.com)

if [ ! -d "build_hello_world" ];
then
    mkdir build_hello_world
fi
cd build_hello_world
cmake ../HelloWorld/
cmake --build .
clang -O1 -S -emit-llvm ../inputs/input_for_hello.c -o input_for_hello.ll
opt -load-pass-plugin ./libHelloWorld.so -passes=hello-world input_for_hello.ll -o input_for_hello2.ll
lli input_for_hello2.ll
