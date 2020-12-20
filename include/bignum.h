#ifndef __BIGNUM__H__
#define __BIGNUM__H__

#define CNT_OF_BYTES_128BIT     (16)
#define CNT_OF_WORDS_128BIT     (4)

/* Little Endian(Intel X86_64) - LSB First
    val: 0x12345678
                    LSB                        MSB
    ┌────────────┬────────┬────────┬────────┬────────┐
    │addr offset │  0x00  │  0x01  │  0x02  │  0x03  │  
    ├────────────┼────────┼────────┼────────┼────────┤
    │    val     │  0x78  │  0x56  │  0x34  │  0x12  │   
    └────────────┴────────┴────────┴────────┴────────┘
 */

/* Big Endian(RISC) - MSB First
    val: 0x12345678
                     MSB                       LSB 
    ┌────────────┬────────┬────────┬────────┬────────┐
    │addr offset │  0x00  │  0x01  │  0x02  │  0x03  │  
    ├────────────┼────────┼────────┼────────┼────────┤
    │    val     │  0x12  │  0x34  │  0x56  │  0x78  │   
    └────────────┴────────┴────────┴────────┴────────┘
 */

typedef enum 
{
    BIG_INT_STS_A_IS_BIGGER = -1,
    BIG_INT_STS_EQUAL_A_B   =  0,
    BIG_INT_STS_B_IS_BIGGER =  1,
    BIG_INT_STS_SUCCESS     =  2, 
    BIG_INT_STS_OVERFLOW    =  3, 
    BIG_INT_STS_DIV_AS_ZERO =  4
} eBigIntegerSts_t;

typedef struct 
{
    uint32_t    memory[CNT_OF_WORDS_128BIT];
    bool        bSign;
} BigInteger128_t, *LPBigInteger128_t;

eBigIntegerSts_t HexStr2BigInt(bool bSign, char HexString[], LPBigInteger128_t S);

eBigIntegerSts_t ADD128(LPBigInteger128_t A, LPBigInteger128_t B, LPBigInteger128_t S);
eBigIntegerSts_t SUB128(LPBigInteger128_t A, LPBigInteger128_t B, LPBigInteger128_t S);
eBigIntegerSts_t MUL128(LPBigInteger128_t A, LPBigInteger128_t B, LPBigInteger128_t S);
eBigIntegerSts_t DIV128(LPBigInteger128_t A, LPBigInteger128_t B, LPBigInteger128_t S);
eBigIntegerSts_t MOD128(LPBigInteger128_t A, LPBigInteger128_t B, LPBigInteger128_t S);
eBigIntegerSts_t AND128(LPBigInteger128_t A, LPBigInteger128_t B, LPBigInteger128_t S);
eBigIntegerSts_t OR128(LPBigInteger128_t A, LPBigInteger128_t B, LPBigInteger128_t S);
eBigIntegerSts_t NOT128(LPBigInteger128_t A, LPBigInteger128_t B, LPBigInteger128_t S);
eBigIntegerSts_t XOR128(LPBigInteger128_t A, LPBigInteger128_t B, LPBigInteger128_t S);
eBigIntegerSts_t CMP128(LPBigInteger128_t A, LPBigInteger128_t B);
eBigIntegerSts_t ASSIGN128(LPBigInteger128_t A, LPBigInteger128_t B);

void printBigInt128(char Message[], LPBigInteger128_t BigInt);

#endif  //!__BIGNUM__H__
