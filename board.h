#ifndef BOARD_H
#define BOARD_H

#include "constants.h"

#include <string>

class Board
{
    friend class Moves;
    friend class HashList;
    friend class Evaluator;

public:
    Board();

    char GetPieceSymbolAtPosition(std::size_t position) const;
    std::string GetPositionFromBitBoard(const uint64_t &bb) const;

    bool SetFen(const std::string &fen_string);
    std::string GetFen() const;

    bool IsSufficientMaterial() const;
    void AddPiece(PieceType piece);
    void RemovePiece(PieceType piece);

    bool operator!=(const Board &b) const;

private:

    uint64_t whites_;
    uint64_t white_pawns_;
    uint64_t white_knights_;
    uint64_t white_bishops_;
    uint64_t white_rooks_;
    uint64_t white_king_;

    uint64_t blacks_;
    uint64_t black_pawns_;
    uint64_t black_knights_;
    uint64_t black_bishops_;
    uint64_t black_rooks_;
    uint64_t black_king_;

    std::size_t black_pawns_count;
    std::size_t white_pawns_count;
    std::size_t black_knights_count;
    std::size_t white_knights_count;
    std::size_t black_bishops_count;
    std::size_t white_bishops_count;
    std::size_t black_rooks_count;
    std::size_t white_rooks_count;
    std::size_t black_queens_count;
    std::size_t white_queens_count;

    uint64_t occupied_;
    uint64_t empty_;
};

#endif // BOARD_H
