#ifndef BITS_FUNCTIONS_H
#define BITS_FUNCTIONS_H

#include <cstddef>
#include <cstdint>

#ifdef MSVC_COMPILER
    #include <intrin.h>
    #if defined(MSVC_SSE_4_2)
        #include <nmmintrin.h>
    #endif            
#endif

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
    #if defined(BUILTIN_CTZLL)
        #if defined(GCC_COMPILER)
            return static_cast<std::size_t>(__builtin_ctzll(bb));
        #elif defined(MSVC_COMPILER)
            unsigned long r = 0;
            _BitScanForward64(&r, bb);
            return static_cast<std::size_t>(r);
        #endif
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
    #if defined(GCC_COMPILER)
        return  static_cast<std::size_t>(__builtin_popcountll(bb));
    #elif defined(MSVC_COMPILER)
        #if defined(MSVC_SSE_4_2)
            return static_cast<std::size_t>(_mm_popcnt_u64(bb));
        #elif defined(MSVC_POPCOUNT)
            return static_cast<std::size_t>(__popcnt64(bb));
        #endif
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
    #if defined(GCC_COMPILER)
        return __builtin_bswap64(bb);
    #elif defined(MSVC_COMPILER)
        return _byteswap_uint64(bb);
    #else
        bb = (bb & 0x00000000FFFFFFFF) << 32 | (bb & 0xFFFFFFFF00000000) >> 32;
        bb = (bb & 0x0000FFFF0000FFFF) << 16 | (bb & 0xFFFF0000FFFF0000) >> 16;
        bb = (bb & 0x00FF00FF00FF00FF) << 8  | (bb & 0xFF00FF00FF00FF00) >> 8;

        return bb;
    #endif
}

inline uint64_t Flip32bitsVertically(uint32_t bb)
{
    #if defined(GCC_COMPILER)
        return __builtin_bswap32(bb);
    #elif defined(MSVC_COMPILER)
        return _byteswap_uint32(bb);
    #else
        uint32_t y = (x >> 24) & 0xff;
        y |= ((x >> 16) & 0xff) << 8;
        y |= ((x >> 8) & 0xff) << 16;
        y |= (x & 0xff) << 24;

        return y;
    #endif
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
