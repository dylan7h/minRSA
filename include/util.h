#ifndef __UTIL__H__
#define __UTIL__H__



#define PRINT_MOD_U8  (0U)
#define PRINT_MOD_U16 (1U)
#define PRINT_MOD_U32 (2U)
#define PRINT_MOD_U64 (3U)

void printDump(uint32_t Mode, void* buf, uint32_t length);
void printHex(uint8_t* buf, uint32_t length);

void printString(char buf[], uint32_t length);

void Memcpy8U(bool bReverse, uint8_t* pDst, uint8_t* pSrc, uint32_t cntOfByte);
void Memcpy32U(bool bReverse, uint32_t* pDst, uint32_t* pSrc, uint32_t cntOfWord);

void Memset8U(uint8_t* pDst, uint8_t value, uint32_t cntOfByte);
void Memset32U(uint32_t* pDst, uint32_t value, uint32_t cntOfWord);

#endif  //!__UTIL__H__