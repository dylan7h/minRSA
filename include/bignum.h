#ifndef __BIGNUM__H__
#define __BIGNUM__H__

#include <stdint.h>
#include <stdbool.h>

/* 
    int32_t num = 7251326;
    uint8_t arr = { 0x06, 0x02, 0x03, 0x01, 0x05, 0x02, 0x07 };
    | 6 | 2 | 3 | 1 | 5 | 2 | 7 |
 */

typedef struct 
{
    uint8_t*    ptrSpace;       /* buffer address   */
    int32_t     size;           /* buffer size      */
    bool        sign;           /* signed flag      */
} BIG_NUMBER;

typedef struct 
{
    uint8_t*    ptrSpace;       /* buffer address   */
    int32_t     size;           /* buffer size      */
    int32_t     deciamlPoint;   /* decimal point    */
    bool        sign;           /* signed flag      */
} BIG_FLOAT;

typedef struct
{
    uint8_t*    digit;          /* buffer address   */
    int32_t     size;           /* buffer size      */
    bool        sign;           /* signed flag      */
} BIG_DECIMAL;

typedef struct
{
    uint8_t*    byte;           /* buffer address   */
    int32_t     size;           /* buffer size      */
} BIG_BINARY;

BIG_DECIMAL CreateDecimal(char strVal[], int32_t size);
void        printDecimal(BIG_DECIMAL* pDeciaml);
void        fprintDecimal(FILE* fp, BIG_DECIMAL* pDeciaml);

BIG_BINARY  CreateBinary(uint8_t bytes[], int32_t length);
void        printBinary(BIG_BINARY* pBinary);
void        fprintBinary(FILE* fp, BIG_BINARY* pBinary);

bool        IsEqual(BIG_DECIMAL* A, BIG_DECIMAL* B);
bool        IsBigger(BIG_DECIMAL* A, BIG_DECIMAL* B);

BIG_DECIMAL PLUS(BIG_DECIMAL* A, BIG_DECIMAL* B);
BIG_DECIMAL PlusDigit(BIG_DECIMAL* A, uint8_t digit);

#endif  //!__BIGNUM__H__