#ifndef HASHLIST_H
#define HASHLIST_H

#include "board.h"
#include "constants.h"

class Board;
class MoveInfo;

class HashList
{
public:
    explicit HashList(const Board *board);

    void ComputeInitialHash(const uint64_t &en_passant, const std::size_t &castlings, Side side);
    void UpdateHash(const uint64_t &move, MoveInfo* move_info, const uint64_t &en_passant, const std::size_t &castlings);
    void UpdateHash();

    uint64_t GetHash() const;

    bool Is3FoldRepetition() const;

private:
    PieceType GetHashType(std::size_t lsb) const;

    const Board* board_;
    std::array<std::array<uint64_t, kBitBoardSize>, PieceType::KAllPieces> pieces_hashes_;
    std::array<uint64_t, kHashEnPassantSize> en_passant_hashes_;
    std::array<uint64_t, kHashCastlingSize> castling_hashes_;
    uint64_t side_hash_;

    std::array<std::pair<uint64_t, std::size_t>, 256> hashes_;
    std::pair<uint64_t, std::size_t> *current_hash_;
};

#endif // HASHLIST_H
