#include <stdio.h>
#include <string.h>
#include "bignum.h"

int main(void)
{
    BIG_DECIMAL A, B, result;

    A = CreateDecimal("51359", strlen("51359"));
    B = CreateDecimal("99962763", strlen("99962763"));

    result = PLUS(&A, &B);
    printDecimal(&result);

    A = CreateDecimal("99995", strlen("99995"));

    result = PlusDigit(&A, 7U);
    printDecimal(&result);

    return 0;
}