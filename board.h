#ifndef BOARD_H
#define BOARD_H

#include "constants.h"

#include <string>

class Board
{
    friend class Moves;
    friend class HashList;
    friend class Evaluator;
    friend class CTGReader;

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

    PieceType GetPieceOnSquare(std::size_t square) const;

    void InvertPieces();
    void FlipVertically();
    void FlipHorizontally();

    bool IsKingOnTheLeftHalf(Side side) const;

    bool IsEnPassantPossible(Side side, uint64_t bb) const;

    int totalMaterial() const;

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

    int black_pawns_count;
    int white_pawns_count;
    int black_knights_count;
    int white_knights_count;
    int black_bishops_count;
    int white_bishops_count;
    int black_rooks_count;
    int white_rooks_count;
    int black_queens_count;
    int white_queens_count;

    uint64_t occupied_;
    uint64_t empty_;
};

#endif // BOARD_H
