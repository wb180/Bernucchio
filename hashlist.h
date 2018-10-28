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
    void UpdateHash(const Move& move);

private:
    std::size_t GetHashType(std::size_t lsb) const;

    const Board* board_;
    std::array<std::array<uint64_t, kBitBoardSize>, static_cast<std::underlying_type_t<Board::BitboardType> >(Board::BitboardType::KAllPieces)> pieces_hashes_;
    std::array<uint64_t, kHashEnPassantSize> en_passant_hashes_;
    std::array<uint64_t, kHashCastlingSize> castling_hashes_;
};

#endif // HASHLIST_H
