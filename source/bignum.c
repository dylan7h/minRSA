#include <stdio.h>
#include <stdlib.h>
#include "bignum.h"

BIG_DECIMAL CreateDecimal(char strVal[], int32_t size)
{
    BIG_DECIMAL decimal;
    int32_t i;

    decimal.digit = (uint8_t*)malloc(size);
    for(i = 0; i < size; i++)
    {
        decimal.digit[i] = strVal[size - (i + 1)] - '0';
    }

    decimal.sign = false;
    decimal.size = size;

    return decimal;
}

void printDecimal(BIG_DECIMAL* pDeciaml)
{
    int32_t i;

    if(pDeciaml->sign == true)
    {
        putc('-', stdout);
    }

    for(i = pDeciaml->size - 1; i >= 0; i--)
    {
        putc(pDeciaml->digit[i] + '0', stdout);
    }
    putc('\r', stdout);
    putc('\n', stdout);
}

void fprintDecimal(FILE* fp, BIG_DECIMAL* pDeciaml)
{
    int32_t i;

    if(pDeciaml->sign == true)
    {
        putc('-', fp);
    }

    for(i = pDeciaml->size - 1; i >= 0; i--)
    {
        putc(pDeciaml->digit[i] + '0', fp);
    }
    putc('\r', fp);
    putc('\n', fp);
}

BIG_BINARY CreateBinary(uint8_t bytes[], int32_t length)
{
    BIG_BINARY binary;
    int32_t i;

    binary.byte = (uint8_t*)malloc(length);
    for(i = 0; i < length; i++)
    {
        binary.byte[i] = bytes[length - (i + 1)];
    }
    binary.size = length;

    return binary;
}

void printBinary(BIG_BINARY* pBinary)
{
    int i, j;
    uint8_t flag;

    for(i = pBinary->size - 1; i >= 0; i--)
    {
        flag = 0x80U;

        for(j = 0; j < 8; j++)
        {
            if((pBinary->byte[i] & flag) != 0U)
            {
                putc('1', stdout);
            }
            else
            {
                putc('0', stdout);
            }
            
            flag >>= 1U;
        }
    }
    putc('\r', stdout);
    putc('\n', stdout);
}

void fprintBinary(FILE* fp, BIG_BINARY* pBinary)
{
    int i, j;
    uint8_t flag;

    for(i = pBinary->size - 1; i >= 0; i--)
    {
        flag = 0x80U;

        for(j = 0; j < 8; j++)
        {
            if((pBinary->byte[i] & flag) != 0U)
            {
                putc('1', fp);
            }
            else
            {
                putc('0', fp);
            }
            
            flag >>= 1U;
        }
    }
    putc('\r', fp);
    putc('\n', fp);
}

bool IsEqual(BIG_DECIMAL* A, BIG_DECIMAL* B)
{
    int32_t i;

    if(A->size != B->size)
    {
        return false;
    }

    for(i = 0; i < A->size; i++)
    {
        if(A->digit[i] != B->digit[i])
        {
            return false;
        }
    }

    return true;
}

bool IsBigger(BIG_DECIMAL* A, BIG_DECIMAL* B)
{
    int32_t i;

    if(A->size > B->size)
    {
        return true;
    }
    else if (A->size < B->size)
    {
        return false;
    }
    else
    {
        for(i = A->size - 1; i >= 0; i--)
        {
            if(A->digit[i] > B->digit[i])
            {
                return true;
            }
            else if(A->digit[i] < B->digit[i])
            {
                return false;
            }
        }
    }
    
    return true;
}

BIG_DECIMAL PLUS(BIG_DECIMAL* A, BIG_DECIMAL* B)
{
    BIG_DECIMAL result;
    uint32_t    min, max;
    uint32_t    size, i;
    uint8_t     temp;

    BIG_DECIMAL* biggerNum = (A->size > B->size) ? A : B;

    min = (A->size > B->size) ? B->size : A->size;
    max = (A->size > B->size) ? A->size : B->size;

    size = max + 1;
    result.digit = (uint8_t*)malloc(size);
    for(i = 0, temp = 0; i < min; i++)
    {
        result.digit[i] = A->digit[i] + B->digit[i] + temp;
        if(result.digit[i] > 0x09U)
        {
            temp = 0x01U;
        }
        else
        {
            temp = 0x00U;
        }

        result.digit[i] %= 0x0AU;
    }

    for(; i < max; i++)
    {
        result.digit[i] = biggerNum->digit[i] + temp;
        if(result.digit[i] > 0x09U)
        {
            temp = 0x01U;
        }
        else
        {
            temp = 0x00U;
        }
        
        result.digit[i] %= 0x0A;
    }

    if(temp != 0U)
    {
        result.digit[i] = temp;
        result.size = size;
    }
    else
    {
        result.size = size - 1;
    }
    
    result.sign = false;

    return result;
}

BIG_DECIMAL PlusDigit(BIG_DECIMAL* A, uint8_t digit)
{
    BIG_DECIMAL result;

    int32_t i;
    uint32_t size;
    uint8_t temp;

    size = A->size + 1U;
    result.digit = (uint8_t*)malloc(size);

    result.digit[0]  = A->digit[0]       +   digit;
    temp             = result.digit[0]   /   0x0AU;
    result.digit[0] %= 0x0AU;

    for(i = 1; i < A->size; i++)
    {
        result.digit[i]  = A->digit[i]       +   temp;
        temp             = result.digit[i]   /   0x0AU;
        result.digit[i] %= 0x0AU;
    }

    if(temp != 0U)
    {
        result.digit[i] = temp;
        result.size = size;
    }
    else
    {
        result.size = size;
    }
    
    result.sign = A->sign;

    return result;
}
