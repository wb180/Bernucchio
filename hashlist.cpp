#include "bits_functions.h"
#include "hashlist.h"

#include <random>
#include <iostream>

HashList::HashList(const Board *board) : board_(board)
{
    std::mt19937_64 rng;
    rng.seed(std::random_device{}());

    for(std::size_t i = 0; i < pieces_hashes_.size(); ++i)
        for(std::size_t bit_number = 0; bit_number < kBitBoardSize; ++bit_number )
            pieces_hashes_[i][bit_number] = rng();

    for(std::size_t bit_number = kThirdRowBitsStartBit; bit_number < kThirdRowBitsStartBit + kBoardSize; ++bit_number)
        en_passant_hashes_[(GetBitSet(bit_number) * kHashEnPassantMagic) >> kBitShift16] = rng();

    for(std::size_t bit_number = kSixthRowBitsStartBit; bit_number < kSixthRowBitsStartBit + kBoardSize; ++bit_number)
        en_passant_hashes_[(GetBitSet(bit_number) * kHashEnPassantMagic) >> kBitShift16] = rng();

    auto max_castling = kWhiteCastling_0_0 | kWhiteCastling_0_0_0 | kBlackCastling_0_0 | kBlackCastling_0_0_0;

    for(std::size_t castling_rights = 0; castling_rights <= max_castling; ++castling_rights )
        castling_hashes_[castling_rights] = rng();
}

void HashList::ComputeInitialHash(const uint64_t &en_passant, const std::size_t &castlings)
{
    uint64_t occupied = board_->occupied_;
    uint64_t hash = 0;
    std::size_t lsb = 0;

    do
    {
        lsb = GetLSBPos(occupied);
        hash ^= pieces_hashes_[GetHashType(lsb)][lsb];
        occupied &= occupied - 1;
    }
    while(occupied);

    hash ^= en_passant_hashes_[(en_passant * kHashEnPassantMagic) >> kBitShift16];
    hash ^= castling_hashes_[castlings];
}

void HashList::UpdateHash(const Move &move)
{

}

std::size_t HashList::GetHashType(std::size_t lsb) const
{
    uint64_t bit_board = GetBitSet(lsb);

    if(bit_board & board_->black_pawns_)
        return static_cast<std::underlying_type_t<Board::BitboardType> >(Board::BitboardType::kBlackPawns);

    if(bit_board & board_->black_bishops_)
    {
        if(bit_board & board_->black_rooks_)
            return static_cast<std::underlying_type_t<Board::BitboardType> >(Board::BitboardType::kBlackQueens);
        else
            return static_cast<std::underlying_type_t<Board::BitboardType> >(Board::BitboardType::kBlackBishops);
    }

    if(bit_board & board_->black_rooks_)
        return static_cast<std::underlying_type_t<Board::BitboardType> >(Board::BitboardType::kBlackRooks);

    if(bit_board & board_->black_knights_)
        return static_cast<std::underlying_type_t<Board::BitboardType> >(Board::BitboardType::kBlackKnights);

    if(bit_board & board_->black_king_)
        return static_cast<std::underlying_type_t<Board::BitboardType> >(Board::BitboardType::kBlackKing);

    if(bit_board & board_->white_pawns_)
        return static_cast<std::underlying_type_t<Board::BitboardType> >(Board::BitboardType::kWhitePawns);

    if(bit_board & board_->white_bishops_)
    {
        if(bit_board & board_->white_rooks_)
            return static_cast<std::underlying_type_t<Board::BitboardType> >(Board::BitboardType::kWhiteQueens);
        else
            return static_cast<std::underlying_type_t<Board::BitboardType> >(Board::BitboardType::kWhiteBishops);
    }

    if(bit_board & board_->white_rooks_)
        return static_cast<std::underlying_type_t<Board::BitboardType> >(Board::BitboardType::kWhiteRooks);

    if(bit_board & board_->white_knights_)
        return static_cast<std::underlying_type_t<Board::BitboardType> >(Board::BitboardType::kWhiteKnights);

    if(bit_board & board_->white_king_)
        return static_cast<std::underlying_type_t<Board::BitboardType> >(Board::BitboardType::kWhiteKing);

    return static_cast<std::underlying_type_t<Board::BitboardType> >(Board::BitboardType::KAllPieces);
}
