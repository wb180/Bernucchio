#ifndef BITS_FUNCTIONS_H
#define BITS_FUNCTIONS_H

#include <cstdint>

inline uint64_t GetBitSet(std::size_t i)
{
    return uint64_t(1) << i;
}

const std::size_t index64[64] =
{
    0, 47,  1, 56, 48, 27,  2, 60,
   57, 49, 41, 37, 28, 16,  3, 61,
   54, 58, 35, 52, 50, 42, 21, 44,
   38, 32, 29, 23, 17, 11,  4, 62,
   46, 55, 26, 59, 40, 36, 15, 53,
   34, 51, 20, 43, 31, 22, 10, 45,
   25, 39, 14, 33, 19, 30,  9, 24,
   13, 18,  8, 12,  7,  6,  5, 63
};

inline std::size_t GetLSBPos(uint64_t bb)
{
   return index64[((bb ^ (bb-1)) * 0x03f79d71b4cb0a89) >> 58];
}

inline uint64_t GetLSB(uint64_t bb)
{
   return bb & ~(bb-1);
}

inline std::size_t GetMSBPos(uint64_t bb)
{
   bb |= bb >> 1;
   bb |= bb >> 2;
   bb |= bb >> 4;
   bb |= bb >> 8;
   bb |= bb >> 16;
   bb |= bb >> 32;
   return index64[(bb * 0x03f79d71b4cb0a89) >> 58];
}

inline std::size_t GetBitsCount(uint64_t bb)
{
    bb = bb - ((bb >> 1) & 0X5555555555555555);
    bb = (bb & 0X3333333333333333) + ((bb >> 2) & 0X3333333333333333);
    bb = (bb + (bb >> 4)) & 0XF0F0F0F0F0F0F0F;
    return (bb * 0X101010101010101) >> 56;
}

#endif // BITS_FUNCTIONS_H
