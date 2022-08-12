#ifndef PST_H
#define PST_H

#include "constants.h"

// Opening pst
const std::array kPstBlackPawns   { -13, -10,  20,   8,  16,   2,  10,  -3,
                                    109,  94, 109,  54, -40,  91, 156,  49,
                                     91,  33,  57,  45,  36,  44,  38,  11,
                                    -17,   4,   4,  -5,   6, -18,   9,   0,
                                    -18, -11, -21,   6, -10, -15,  11,  -8,
                                    -11, -22, -26, -24, -13, -24,  -1, -22,
                                     -8, -17, -28, -68, -37,  -4,  11, -23,
                                     15,  -4,  11,  -4, -11,   4,  -2,   8 };

const std::array kPstWhitePawns   {  15,  -4,  11,  -4, -11,   4,  -2,   8,
                                     -8, -17, -28, -68, -37,  -4,  11, -23,
                                    -11, -22, -26, -24, -13, -24,  -1, -22,
                                    -18, -11, -21,   6, -10, -15,  11,  -8,
                                    -17,   4,   4,  -5,   6, -18,   9,   0,
                                     91,  33,  57,  45,  36,  44,  38,  11,
                                    109,  94, 109,  54, -40,  91, 156,  49,
                                    -13, -10,  20,   8,  16,   2,  10,  -3 };

const std::array kPstBlackKnights { -12, -12,   7,   3,  -2,   1, -29,  -8,
                                    -15, -13, -16,  21,  35,  66, -25,  38,
                                      0,  17,  79,  22,  32,  34,  59,   9,
                                     -9,  39,   7,  73,  65,  54,  18, -10,
                                    -24,  12,  37,  31,  29,  29,  49, -14,
                                    -16,  -7,  16,  36,  30,  10,  -4, -33,
                                    -36, -48, -11,  -1,  15,   8, -22, -33,
                                    -31, -52,   2, -48, -65, -21, -30, -19 };

const std::array kPstWhiteKnights { -31, -52,   2, -48, -65, -21, -30, -19,
                                    -36, -48, -11,  -1,  15,   8, -22, -33,
                                    -16,  -7,  16,  36,  30,  10,  -4, -33,
                                    -24,  12,  37,  31,  29,  29,  49, -14,
                                     -9,  39,   7,  73,  65,  54,  18, -10,
                                      0,  17,  79,  22,  32,  34,  59,   9,
                                    -15, -13, -16,  21,  35,  66, -25,  38,
                                    -12, -12,   7,   3,  -2,   1, -29,  -8 };

const std::array kPstBlackBishops { -15,  25, -18, -13,  -9, -14, -14,   0,
                                    -33,  10,   6,  15,   6,   0,  -9,   5,
                                      3,   3,  40,  30, -24,  -3, -13, -24,
                                      3,  21,  13,  13,  10,  20,  14,   7,
                                      3,  13,  11,  33,  21,  12, -21,  -4,
                                     11,  -3,  21,   5,   0,  15,  19,  -3,
                                    -40,  11,   7,  -6,  -2,  -3,   6, -19,
                                    -23,  -3, -34, -37, -28, -29,  -9,  -2 };

const std::array kPstWhiteBishops { -23,  -3, -34, -37, -28, -29,  -9,  -2,
                                    -40,  11,   7,  -6,  -2,  -3,   6, -19,
                                     11,  -3,  21,   5,   0,  15,  19,  -3,
                                      3,  13,  11,  33,  21,  12, -21,  -4,
                                      3,  21,  13,  13,  10,  20,  14,   7,
                                      3,   3,  40,  30, -24,  -3, -13, -24,
                                    -33,  10,   6,  15,   6,   0,  -9,   5,
                                    -15,  25, -18, -13,  -9, -14, -14,   0 };

const std::array kPstBlackRooks   {  43,  16,  35,  58,  47,  44, -12,  -1,
                                     40,  43,  29,  56,  35,  63,   4,  -9,
                                     19,   5, -17,   4,   2,  -3,  20, -21,
                                     19,  18,  12,  13,   0,   2,   1,  27,
                                     -3, -19, -36, -12,   7,  15, -24,  37,
                                    -20, -14,  -1, -15,  30,  12,  38,  -8,
                                    -21, -13,  -3, -22, -25, -19,   1,  -5,
                                    -21,  -7, -11,   0,  -2, -11, -18, -46 };

const std::array kPstWhiteRooks   { -21,  -7, -11,   0,  -2, -11, -18, -46,
                                    -21, -13,  -3, -22, -25, -19,   1,  -5,
                                    -20, -14,  -1, -15,  30,  12,  38,  -8,
                                     -3, -19, -36, -12,   7,  15, -24,  37,
                                     19,  18,  12,  13,   0,   2,   1,  27,
                                     19,   5, -17,   4,   2,  -3,  20, -21,
                                     40,  43,  29,  56,  35,  63,   4,  -9,
                                     43,  16,  35,  58,  47,  44, -12,  -1 };

const std::array kPstBlackQueens  {  -5, -36,  -3,  -3,  -3,  -6,  -3,  17,
                                    -17, -49,   1,  32,  12,  56,  23,  20,
                                    -21,  -5,  23,  21,  29,  33,  59,  54,
                                      3, -44,  -1,  33,  10,  47,  16,  18,
                                     -8, -36,  -9,   6,  19,  -4,  42,   9,
                                    -16, -12,  12, -10,  13,   4,  15,   4,
                                    -10, -29, -20, -12,  -1,  12, -12, -25,
                                    -12, -24, -14,  -5,  -4, -38, -15, -13 };

const std::array kPstWhiteQueens  { -12, -24, -14,  -5,  -4, -38, -15, -13,
                                    -10, -29, -20, -12,  -1,  12, -12, -25,
                                    -16, -12,  12, -10,  13,   4,  15,   4,
                                     -8, -36,  -9,   6,  19,  -4,  42,   9,
                                      3, -44,  -1,  33,  10,  47,  16,  18,
                                    -21,  -5,  23,  21,  29,  33,  59,  54,
                                    -17, -49,   1,  32,  12,  56,  23,  20,
                                     -5, -36,  -3,  -3,  -3,  -6,  -3,  17 };

const std::array kPstBlackKing    { -12,   2,  13,  42,   9,  29,  31,   6,
                                    -27,  18,  16,   4,   6,  -2, -34, -29,
                                    -43,  -2, -13, -26,  -9, -21,  16, -26,
                                     18,  13,  -6, -10, -43,   6,  -8, -22,
                                     13,  19,  18, -37,  -1,  -3,   9, -19,
                                     23,  24,  -9, -15,  -4,   4,   4,   0,
                                     18,  11,   3,   2, -24,   3,   5, -15,
                                    -10,  13, -12, -60, -32, -13,   5, -41 };

const std::array kPstWhiteKing    { -10,  13, -12, -60, -32, -13,   5, -41,
                                     18,  11,   3,   2, -24,   3,   5, -15,
                                     23,  24,  -9, -15,  -4,   4,   4,   0,
                                     13,  19,  18, -37,  -1,  -3,   9, -19,
                                     18,  13,  -6, -10, -43,   6,  -8, -22,
                                    -43,  -2, -13, -26,  -9, -21,  16, -26,
                                    -27,  18,  16,   4,   6,  -2, -34, -29,
                                    -12,   2,  13,  42,   9,  29,  31,   6 };

// Endgame pst
const std::array<int, kBitBoardSize> kPstBlackPawnsEnd =  {{  10,    4,   4,   9,  10,  -5,  13,  -3,
                                                              74,  149,  37,  13,  -2,  43,  138, 17,
                                                              59,   -1,  31,  74,  23,  54,  21,  60,
                                                              12,    7,  23,  -2, -29,  12,  20,   9,
                                                              -4,   -8,  -1, -21, -24, -29,   6,   7,
                                                               0,  -27, -23,  -4,  14, -21,  -5, -15,
                                                              -2,  -14, -22, -50,  17, -38,  11,  14,
                                                              -2,  -10,  -2,  11,   0,  12, -10,  10 }};

const std::array<int, kBitBoardSize> kPstWhitePawnsEnd =  {{  -2, -10,  -2,  11,   0,  12, -10,  10,
                                                              -2, -14, -22, -50,  17, -38,  11,  14,
                                                               0, -27, -23,  -4,  14, -21,  -5, -15,
                                                              -4,  -8,  -1, -21, -24, -29,   6,   7,
                                                              12,   7,  23,  -2, -29,  12,  20,   9,
                                                              59,  -1,  31,  74,  23,  54,  21,  60,
                                                              74, 149,  37,  13,  -2,  43, 138,  17,
                                                              10,   4,   4,   9,  10,  -5,  13,  -3 }};

const std::array<int, kBitBoardSize> kPstBlackKnightsEnd = {{ -3,  -9,  -8,   4,   2,  -2,  10,   1,
                                                              -3, -11,   0,   4,  15,  -4,   6,  -3,
                                                             -21,  -4,  15,  -3, -18,  -3,  15,  -2,
                                                              -9,   0,   7, -40,  15,  66,  24,  12,
                                                              16,  -3, -13,  -2,  -1,  -4, -23, -18,
                                                               1, -26,  -2,  28,  -1,  17, -21, -10,
                                                               2, -28,  -5,   1, -14,  21,   7,  -5,
                                                               1,   0,  17, -17,  -4,  -4, -10,  -8 }};

const std::array<int, kBitBoardSize> kPstWhiteKnightsEnd = {{  1,   0,  17, -17,  -4,  -4, -10,  -8,
                                                               2, -28,  -5,   1, -14,  21,   7,  -5,
                                                               1, -26,  -2,  28,  -1,  17, -21, -10,
                                                              16,  -3, -13,  -2,  -1,  -4, -23, -18,
                                                              -9,   0,   7, -40,  15,  66,  24,  12,
                                                             -21,  -4,  15,  -3, -18,  -3,  15,  -2,
                                                              -3, -11,   0,   4,  15,  -4,   6,  -3,
                                                              -3,  -9,  -8,   4,   2,  -2,  10,   1 }};

const std::array<int, kBitBoardSize> kPstBlackBishopsEnd = {{
                                                                6, -11,  -5,  -4,  -2, -25,  -7,   4,
                                                                  -2,  -6,  -6,   2,  -2,  32,  30,   5,
                                                                  -19,  11,  -5, -16, -23,  41,   4,  -8,
                                                                  25,   3,  12, -17,  36, -16,  10,  -1,
                                                                  9,   3, -20,  10,  -3,  11,   8, -10,
                                                                  -13,   4,  23,   0,  19, -11,  18,   2,
                                                                  -24,   3,  10,  22, -18,  34,   1, -11,
                                                                   -1, -21,   7, -23,  11, -17,   2, -25,    }};

const std::array<int, kBitBoardSize> kPstWhiteBishopsEnd = {{
                                                                -1, -21,   7, -23,  11, -17,   2, -25,
                                                               -24,   3,  10,  22, -18,  34,   1, -11,
                                                               -13,   4,  23,   0,  19, -11,  18,   2,
                                                                 9,   3, -20,  10,  -3,  11,   8, -10,
                                                                25,   3,  12, -17,  36, -16,  10,  -1,
                                                               -19,  11,  -5, -16, -23,  41,   4,  -8,
                                                                -2,  -6,  -6,   2,  -2,  32,  30,   5,
                                                                 6, -11,  -5,  -4,  -2, -25,  -7,   4,   }};

const std::array<int, kBitBoardSize> kPstBlackRooksEnd = {{
                                                              22,  -7,  44,  16,   5,   2,  24,  -2,
                                                               15,  10,  18,  29,  33,  22,  21, -15,
                                                               -3,  46,   0,  25,  18,   4,  28,  -6,
                                                               -14, -28,  -7, -20, -20, -33,   5, -18,
                                                               -1, -12,  11, -18,  -6, -10,  -5,  12,
                                                               -23,   0, -30, -13, -66, -12, -46, -25,
                                                               4, -10,  20,   3, -27,  -5, -17, -15,
                                                               3, -22,  -9,  -3,   3,  -4,   5,   0, }};

const std::array<int, kBitBoardSize> kPstWhiteRooksEnd = {{
                                                              3, -22,  -9,  -3,   3,  -4,   5,   0,
                                                              4, -10,  20,   3, -27,  -5, -17, -15,
                                                            -23,   0, -30, -13, -66, -12, -46, -25,
                                                             -1, -12,  11, -18,  -6, -10,  -5,  12,
                                                            -14, -28,  -7, -20, -20, -33,   5, -18,
                                                             -3,  46,   0,  25,  18,   4,  28,  -6,
                                                             15,  10,  18,  29,  33,  22,  21, -15,
                                                             22,  -7,  44,  16,   5,   2,  24,  -2,    }};

const std::array<int, kBitBoardSize> kPstBlackQueensEnd = {{
                                                               -7, -32,  -7,  -1,   9,   4,  11,   1,
                                                                -32,   9, -21,  14,  -9,  41,  13,   8,
                                                                2,   4,  18,  29,  16,  33,  18,   7,
                                                                14,  14,   3,  16,   5,   9,  11,  -9,
                                                                11,  -5,   7,  -9,  11, -16,  14, -20,
                                                                -2,   1,  -5,  -5,   6,  28, -11,   4,
                                                                -5,   3,  26,   3, -11, -19,  -5, -21,
                                                                -2, -13,  -4,   3, -12,  -6,  -2,  10,  }};

const std::array<int, kBitBoardSize> kPstWhiteQueensEnd = {{
                                                               -2, -13,  -4,   3, -12,  -6,  -2,  10,
                                                               -5,   3,  26,   3, -11, -19,  -5, -21,
                                                               -2,   1,  -5,  -5,   6,  28, -11,   4,
                                                               11,  -5,   7,  -9,  11, -16,  14, -20,
                                                               14,  14,   3,  16,   5,   9,  11,  -9,
                                                                2,   4,  18,  29,  16,  33,  18,   7,
                                                              -32,   9, -21,  14,  -9,  41,  13,   8,
                                                               -7, -32,  -7,  -1,   9,   4,  11,   1,    }};

const std::array<int, kBitBoardSize> kPstBlackKingEnd = {{
                                                             17,  -4,  30,   8,   5,  35,  46,   8,
                                                              25,  14,  31,  15,   9,  10,  23,  26,
                                                              38,  40,   7,  -6,  17,  -8,  23,  -5,
                                                              2,  49, -18, -13, -35, -14,   9, -15,
                                                              -6,   0,  -5, -42,  -6, -15,  13,   7,
                                                              3,  -7, -14,  22,  15,  14,  12,  -2,
                                                              -6,  -3, -32,  15,  37,  21,  11,  28,
                                                              11, -21, -12,  19, -13,  -4,  -3,   5,       }};

const std::array<int, kBitBoardSize> kPstWhiteKingEnd = {{
                                                             11, -21, -12,  19, -13,  -4,  -3,   5,
                                                             -6,  -3, -32,  15,  37,  21,  11,  28,
                                                              3,  -7, -14,  22,  15,  14,  12,  -2,
                                                             -6,   0,  -5, -42,  -6, -15,  13,   7,
                                                              2,  49, -18, -13, -35, -14,   9, -15,
                                                             38,  40,   7,  -6,  17,  -8,  23,  -5,
                                                             25,  14,  31,  15,   9,  10,  23,  26,
                                                             17,  -4,  30,   8,   5,  35,  46,   8,     }};

#endif // PST_H