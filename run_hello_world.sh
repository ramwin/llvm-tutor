#!/bin/bash
# Xiang Wang(ramwin@qq.com)

set -e
BUILD=build_hello_world
export LLVM_DIR=/home/wangx/github/llvm-project/build

if [ ! -d $BUILD ];
then
    mkdir $BUILD
    cd $BUILD
    cmake -G Ninja -DLT_LLVM_INSTALL_DIR=$LLVM_DIR ../HelloWorld/
    cmake --build . -j
    cd ..
fi

cd $BUILD
cmake --build . -j
cd ..
$LLVM_DIR/bin/clang -O1 -S -emit-llvm ./inputs/input_for_hello.c -o input_for_hello.ll
$LLVM_DIR/bin/opt -load-pass-plugin $BUILD/libHelloWorld.so -passes=hello-world input_for_hello.ll -o input_for_hello2.ll
lli input_for_hello2.ll
