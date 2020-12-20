#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include "util.h"
#include "bignum.h"

int main(void)
{
    BigInteger128_t A, B, S;

    HexStr2BigInt(false, "0x123456789ABCDEF12", &A);
    HexStr2BigInt(false, "0x12121212121212121", &B);
    
    printBigInt128("A", &A);
    printBigInt128("B", &B);

    ADD128(&A, &B, &S);
    printf("[ S = A + B ]\n");
    printBigInt128("S", &S);
    printf("\n\n");

    SUB128(&A, &B, &S);
    printf("[ S = A - B ]\n");
    printBigInt128("S", &S);
    printf("\n\n");

    return 0;
}