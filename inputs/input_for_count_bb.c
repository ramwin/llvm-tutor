//=============================================================================
// FILE:
//      input_for_hello.c
//
// DESCRIPTION:
//      Sample input file for HelloWorld and InjectFuncCall
//
// License: MIT
//=============================================================================
//
#include <stdio.h>


int main() {
    int a = 3;
    int b = 4;
    b += a;
    b += 0;
    // b = b + 0 + 2;
    // if (a > 2) {
    //     b+=0;
    // } else {
    //     b+=1;
    // }
    printf("b是: %d\n", b);
    return b;
}
