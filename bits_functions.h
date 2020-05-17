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
#ifdef BUILD_WITH_CTZLL
    return static_cast<std::size_t>(__builtin_ctzll(bb));
#else
   return index64[((bb ^ (bb-1)) * 0x03f79d71b4cb0a89) >> 58];
#endif
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
#ifdef BUILD_WITH_POPCOUNT
    return  static_cast<std::size_t>(__builtin_popcountll(bb));
#else
    bb = bb - ((bb >> 1) & 0X5555555555555555);
    bb = (bb & 0X3333333333333333) + ((bb >> 2) & 0X3333333333333333);
    bb = (bb + (bb >> 4)) & 0XF0F0F0F0F0F0F0F;
    return (bb * 0X101010101010101) >> 56;
#endif
}

inline uint64_t GetBitMaskNextPermutation(uint64_t permutation, uint64_t mask)
{
    uint64_t m = 1;

    while(permutation & m || (~permutation & m & ~mask))
    {
        if(permutation & m & mask)
            permutation ^= m;

        m <<= 1;
    }

    permutation ^= m;

    return permutation;
}

inline uint64_t FlipBitboardVertically(uint64_t bb)
{
    return __builtin_bswap64(bb);
}

inline uint64_t FlipBitboardHorizontally(uint64_t bb)
{
    bb = ((bb >> 1) & 0x5555555555555555) +  2*(bb & 0x5555555555555555);
    bb = ((bb >> 2) & 0x3333333333333333) +  4*(bb & 0x3333333333333333);
    bb = ((bb >> 4) & 0x0f0f0f0f0f0f0f0f) + 16*(bb & 0x0f0f0f0f0f0f0f0f);

    return bb;
}

void append_bits_reverse(uint8_t *buf, uint8_t bits, int bit_position, int num_bits);

#endif // BITS_FUNCTIONS_H
