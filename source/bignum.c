#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include "util.h"
#include "bignum.h"

#define OPERAND_A       (0U)
#define OPERAND_B       (1U)
#define OPERAND_S       (2U)
#define MAX_OF_OPERAND  (3U)

static uint8_t HexString2Nibble(char strHex)
{
    uint8_t ret = 0U;

    if((strHex >= 'A') && (strHex <= 'F'))
    {
        ret = (uint8_t)strHex - (uint8_t)'A' + 0x0AU;
    }
    else if((strHex >= 'a') && (strHex <= 'f'))
    {
        ret = (uint8_t)strHex - (uint8_t)'a' + 0x0AU;
    }
    else if((strHex >= '0') && (strHex <= '9'))
    {
        ret = (uint8_t)strHex - (uint8_t)'0';
    }
    else
    {   
        ret = 0xFF;
    }

    return ret;
}

static eBigIntegerSts_t OperatorAdd128(LPBigInteger128_t A, LPBigInteger128_t B, LPBigInteger128_t S)
{
    eBigIntegerSts_t Ret = BIG_INT_STS_SUCCESS;
    volatile uint32_t carry = 0U;
    volatile uint32_t i;

    for(i = 0U; i < 4U; i += 1U)
    {
        S->memory[i] = A->memory[i] + B->memory[i] + carry;
        if(S->memory[i] <  A->memory[i])
        {
            carry = 1U;
        }
        else
        {
            carry = 0U;
        }
    }

    if(carry != 0U)
    {
        Ret = BIG_INT_STS_OVERFLOW;
    }

    return Ret;
}

static eBigIntegerSts_t OperatorSub128(LPBigInteger128_t A, LPBigInteger128_t B, LPBigInteger128_t S)
{
    eBigIntegerSts_t Ret = BIG_INT_STS_SUCCESS;
    eBigIntegerSts_t result;
    volatile uint32_t carry = 0U;
    volatile uint32_t i;

    result = CMP128(A, B);
    if(result == BIG_INT_STS_A_IS_BIGGER)
    {
        for(i = 0U; i < 4U; i += 1U)
        {
            S->memory[i] = A->memory[i] - B->memory[i] - carry;
            if(S->memory[i] >  A->memory[i])
            {
                carry = 1U;
            }
            else
            {
                carry = 0U;
            }
        }
        S->bSign = false;
    }
    else if(result == BIG_INT_STS_B_IS_BIGGER)
    {
        for(i = 0U; i < 4U; i += 1U)
        {
            S->memory[i] = B->memory[i] - A->memory[i] - carry;
            if(S->memory[i] >  B->memory[i])
            {
                carry = 1U;
            }
            else
            {
                carry = 0U;
            }
        }     
        S->bSign = true;   
    }
    else if(result == BIG_INT_STS_EQUAL_A_B)
    {
        S->bSign = false;
    }
    else
    {
        /* code */
    }

    return Ret;
}

eBigIntegerSts_t HexStr2BigInt(bool bSign, char HexString[], LPBigInteger128_t S)
{
    volatile int32_t i, j, k;
    int32_t length;
    uint8_t value;
    uint8_t* pos = (uint8_t*)S->memory;
    char* strPos;
    bool bIsHighByte = false;

    if((HexString[0] == '0') && (HexString[1] == 'x'))
    {
        strPos = &HexString[2];
        for(i = 0; strPos[i] != '\0'; i += 1);
        length = i;

        Memset32U(S->memory, 0U, CNT_OF_WORDS_128BIT);

        k = 0;
        j = CNT_OF_BYTES_128BIT - 4;
        for(i = (length - 1); i > -1; i -= 1)
        {
            value = HexString2Nibble(strPos[i]);
            if(bIsHighByte == true)
            {
                pos[j + k] |= (value << 4);
                bIsHighByte = false;
                k += 1;
                if(k == 4)
                {
                    k = 0;
                    j -= 4;
                }
            }
            else
            {
                pos[j + k] = value;
                bIsHighByte = true;
            }
        }
    }
    else
    {
        /* code */
    }
    S->bSign = bSign;

    return BIG_INT_STS_SUCCESS;
}

eBigIntegerSts_t ADD128(LPBigInteger128_t A, LPBigInteger128_t B, LPBigInteger128_t S)
{
    eBigIntegerSts_t Ret = BIG_INT_STS_SUCCESS;
    BigInteger128_t Operand[3U];

    Memcpy32U(true, Operand[OPERAND_A].memory, A->memory, CNT_OF_WORDS_128BIT);
    Memcpy32U(true, Operand[OPERAND_B].memory, B->memory, CNT_OF_WORDS_128BIT);

    Operand[OPERAND_A].bSign = A->bSign;
    Operand[OPERAND_B].bSign = B->bSign;

    Memset32U(Operand[OPERAND_S].memory, 0U, CNT_OF_WORDS_128BIT);

    /* 01. S(+) = A(+) + B(+) */
    if((A->bSign == false) && (B->bSign == false))
    {
        Ret = OperatorAdd128(&Operand[OPERAND_A], &Operand[OPERAND_B], &Operand[OPERAND_S]);
    }
    /* 02. S = A(+) + B(-) */
    else if((A->bSign == false) && (B->bSign == true))
    {
        Ret = OperatorSub128(&Operand[OPERAND_A], &Operand[OPERAND_B], &Operand[OPERAND_S]);
    }
    /* 03. S = A(-) + B(+) */
    else if((A->bSign == true) && (B->bSign == true))
    {
        Ret = OperatorSub128(&Operand[OPERAND_B], &Operand[OPERAND_A], &Operand[OPERAND_S]);
    }
    /* 04. S(-) = A(-) + B(-) */
    else if((A->bSign == true) && (B->bSign == true))
    {
        Ret = OperatorAdd128(&Operand[OPERAND_A], &Operand[OPERAND_B], &Operand[OPERAND_S]);
        Operand[OPERAND_S].bSign = true;
    }

    Memcpy32U(true, S->memory , Operand[OPERAND_S].memory, 4U);
    S->bSign = Operand[OPERAND_S].bSign;

    return Ret;
}

eBigIntegerSts_t SUB128(LPBigInteger128_t A, LPBigInteger128_t B, LPBigInteger128_t S)
{
    eBigIntegerSts_t Ret = BIG_INT_STS_SUCCESS;
    BigInteger128_t Operand[3U];

    Memcpy32U(true, Operand[OPERAND_A].memory, A->memory, 4U);
    Memcpy32U(true, Operand[OPERAND_B].memory, B->memory, 4U);

    Operand[OPERAND_A].bSign = A->bSign;
    Operand[OPERAND_B].bSign = B->bSign;

    Memset32U(Operand[OPERAND_S].memory, 0U, CNT_OF_WORDS_128BIT);

    /* 01. S = A(+) - B(+) */
    if((A->bSign == false) && (B->bSign == false))
    {
        Ret = OperatorSub128(&Operand[OPERAND_A], &Operand[OPERAND_B], &Operand[OPERAND_S]);
    }
    /* 02. S(+) = A(+) - B(-) */
    else if((A->bSign == false) && (B->bSign == true))
    {
        Ret = OperatorAdd128(&Operand[OPERAND_A], &Operand[OPERAND_B], &Operand[OPERAND_S]);
    }
    /* 03. S(-) = A(-) - B(+) */
    else if((A->bSign == true) && (B->bSign == true))
    {
        Ret = OperatorAdd128(&Operand[OPERAND_B], &Operand[OPERAND_A], &Operand[OPERAND_S]);
        Operand[OPERAND_S].bSign = true;
    }
    /* 04. S = A(-) - B(-) */
    else if((A->bSign == true) && (B->bSign == true))
    {
        Ret = OperatorSub128(&Operand[OPERAND_B], &Operand[OPERAND_A], &Operand[OPERAND_S]);
    }

    Memcpy32U(true, S->memory , Operand[OPERAND_S].memory, 4U);
    S->bSign = Operand[OPERAND_S].bSign;

    return Ret;
}

eBigIntegerSts_t MUL128(LPBigInteger128_t A, LPBigInteger128_t B, LPBigInteger128_t S)
{
    eBigIntegerSts_t Ret = BIG_INT_STS_SUCCESS;

    return Ret;
}

eBigIntegerSts_t DIV128(LPBigInteger128_t A, LPBigInteger128_t B, LPBigInteger128_t S)
{
    eBigIntegerSts_t Ret = BIG_INT_STS_SUCCESS;

    return Ret;
}

eBigIntegerSts_t MOD128(LPBigInteger128_t A, LPBigInteger128_t B, LPBigInteger128_t S)
{
    eBigIntegerSts_t Ret = BIG_INT_STS_SUCCESS;

    return Ret;
}

eBigIntegerSts_t AND128(LPBigInteger128_t A, LPBigInteger128_t B, LPBigInteger128_t S)
{
    eBigIntegerSts_t Ret = BIG_INT_STS_SUCCESS;

    return Ret;
}

eBigIntegerSts_t OR128(LPBigInteger128_t A, LPBigInteger128_t B, LPBigInteger128_t S)
{
    eBigIntegerSts_t Ret = BIG_INT_STS_SUCCESS;

    return Ret;
}

eBigIntegerSts_t NOT128(LPBigInteger128_t A, LPBigInteger128_t B, LPBigInteger128_t S)
{
    eBigIntegerSts_t Ret = BIG_INT_STS_SUCCESS;

    return Ret;
}

eBigIntegerSts_t XOR128(LPBigInteger128_t A, LPBigInteger128_t B, LPBigInteger128_t S)
{
    eBigIntegerSts_t Ret = BIG_INT_STS_SUCCESS;

    return Ret;
}

eBigIntegerSts_t CMP128(LPBigInteger128_t A, LPBigInteger128_t B)
{
    eBigIntegerSts_t Ret = BIG_INT_STS_EQUAL_A_B;
    volatile uint32_t i = CNT_OF_WORDS_128BIT;

    do {
        i -= 1U;
        if(A->memory[i] > B->memory[i])
        {
            Ret = BIG_INT_STS_A_IS_BIGGER;
            break;
        }
        else if(A->memory[i] < B->memory[i])
        {
            Ret = BIG_INT_STS_B_IS_BIGGER;
            break;
        }
        else if(A->memory[i] == B->memory[i])
        {
            Ret = BIG_INT_STS_EQUAL_A_B;
        }
        else
        {
            /* code */
        }
    } while(i != 0U);

    return Ret;
}

eBigIntegerSts_t ASSIGN128(LPBigInteger128_t A, LPBigInteger128_t B)
{
    eBigIntegerSts_t Ret = BIG_INT_STS_SUCCESS;

    return Ret;
}

void printBigInt128(char Message[], LPBigInteger128_t BigInt)
{
    volatile uint32_t i;

    printf("[%s]\n", Message);

    printf(">> sign: %s\n", BigInt->bSign ? "true" : "false");
    fputs(">> value: 0x", stdout);
    for(i = 0U; i < 4U; i += 1U)
    {
        printf("%08X", BigInt->memory[i]);
    }
    putc('\n', stdout);
}
