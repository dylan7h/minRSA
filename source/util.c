#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include "util.h"

void printDump(uint32_t Mode, void* buf, uint32_t length)
{
    volatile uint32_t i;

    uint8_t*    pU8     = (uint8_t*)buf;
    uint16_t*   pU16    = (uint16_t*)buf;
    uint32_t*   pU32    = (uint32_t*)buf;
    uint64_t*   pU64    = (uint64_t*)buf;

    for(i = 0U; i < length; i += 1U)
    {
        switch (Mode)
        {
            case PRINT_MOD_U8:
                if((i % 32U) == 0U)
                {
                    printf("\n[0x%08X] ", i);
                }
                printf("0x%02X ", pU8[i]);
            break;

            case PRINT_MOD_U16:
                if((i % 16U) == 0U)
                {
                    printf("\n[0x%08X] ", i);
                }
                printf("0x%04X ", pU16[i]);
            break;

            case PRINT_MOD_U32:
                if((i % 8U) == 0U)
                {
                    printf("\n[0x%08X] ", i);
                }
                printf("0x%08X ", pU32[i]);
            break;

            case PRINT_MOD_U64:
                if((i % 4U) == 0U)
                {
                    printf("\n[0x%08X] ", i);
                }
                printf("0x%016lX ", pU64[i]);
            break;
        
            default:
            break;
        }
    }
    printf("\n");
}

void printHex(uint8_t* buf, uint32_t length)
{
    volatile uint32_t i;

    for(i = 0U; i < length; i += 1U)
    {
        printf("%02X", buf[i]);
    }
    printf("\n");
}

void printString(char buf[], uint32_t length)
{
    volatile uint32_t i;

    for(i = 0U; i < length; i += 1U)
    {
        printf("%c", buf[i]);
    }
    printf("\n");
}

void Memcpy8U(bool bReverse, uint8_t* pDst, uint8_t* pSrc, uint32_t cntOfByte)
{
    volatile uint32_t i;

    if(bReverse == true)
    {
        i = cntOfByte;
        do {
            pDst[i] = pSrc[i];
            i -= 1U;
        } while(i != 0U);
    }
    else
    {
        for(i = 0U; i < cntOfByte; i += 1U)
        {
            pDst[i] = pSrc[i];
        }
    }
}

void Memcpy32U(bool bReverse, uint32_t* pDst, uint32_t* pSrc, uint32_t cntOfWord)
{
    volatile uint32_t i;

    if(bReverse == true)
    {
        i = cntOfWord;
        do {
            pDst[i] = pSrc[i];
            i -= 1U;
        } while(i != 0U);
    }
    else
    {
        for(i = 0U; i < cntOfWord; i += 1U)
        {
            pDst[i] = pSrc[i];
        }
    }
}

void Memset8U(uint8_t* pDst, uint8_t value, uint32_t cntOfByte)
{
    volatile uint32_t i;

    for(i = 0U; i < cntOfByte; i += 1U)
    {
        pDst[i] = value;
    }
}

void Memset32U(uint32_t* pDst, uint32_t value, uint32_t cntOfWord)
{
    volatile uint32_t i;

    for(i = 0U; i < cntOfWord; i += 1U)
    {
        pDst[i] = value;
    }
}
