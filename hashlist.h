#ifndef HASHLIST_H
#define HASHLIST_H

#include "board.h"
#include "constants.h"

class Board;
class Move;

class HashList
{
public:
    explicit HashList(const Board *board);

    void ComputeInitialHash(const uint64_t &en_passant, const std::size_t &castlings);
    void UpdateHash(const uint64_t &move, PieceType captured);
    void UpdateHash();

private:
    PieceType GetHashType(std::size_t lsb) const;

    const Board* board_;
    std::array<std::array<uint64_t, kBitBoardSize>, PieceType::KAllPieces> pieces_hashes_;
    std::array<uint64_t, kHashEnPassantSize> en_passant_hashes_;
    std::array<uint64_t, kHashCastlingSize> castling_hashes_;

    std::array<uint64_t, 512> hashes_;
    uint64_t *current_hash_;

    uint64_t en_passant_old_;
    std::size_t castlings_old_;
};

#endif // HASHLIST_H
