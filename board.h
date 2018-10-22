#ifndef BOARD_H
#define BOARD_H

#include <string>

class Board
{
public:
    Board();

    enum class BitboardType
    {
        kWhites,
        kWhitePawns,
        kWhiteKnights,
        kWhiteBishops,
        kWhiteRooks,
        kWhiteQueens,
        kWhiteKing,

        kBlacks,
        kBlackPawns,
        kBlackKnights,
        kBlackBishops,
        kBlackRooks,
        kBlackQueens,
        kBlackKing,

        kOccupied,
        kEmpty
    };

    uint64_t get_bitboard(BitboardType type) const;

    char GetPieceSymbolAtPosition(std::size_t position) const;
    std::string GetPositionFromBitBoard(const uint64_t &bb) const;

    bool SetFen(const std::string &fen_string);
    std::string GetFen() const;

private:

    //std::string from_move_to_uci(const uint16_t *bb);

    uint64_t whites_;
    uint64_t white_pawns_;
    uint64_t white_knights_;
    uint64_t white_bishops_;
    uint64_t white_rooks_;
    uint64_t white_queens_;
    uint64_t white_king_;

    uint64_t blacks_;
    uint64_t black_pawns_;
    uint64_t black_knights_;
    uint64_t black_bishops_;
    uint64_t black_rooks_;
    uint64_t black_queens_;
    uint64_t black_king_;

    uint64_t occupied_;
    uint64_t empty_;
};

#endif // BOARD_H
