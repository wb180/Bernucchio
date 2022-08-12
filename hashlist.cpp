#include "bits_functions.h"
#include "hashlist.h"

#include <algorithm>
#include <random>
#include <iostream>
#include <stack>
#include "logger.h"

HashList::HashList(const Board *board) : board_(board)
{
    std::mt19937_64 rng;
    rng.seed(std::random_device{}());

    for(std::size_t i = 0; i < pieces_hashes_.size(); ++i)
        for(std::size_t bit_number = 0; bit_number < kBitBoardSize; ++bit_number )
            pieces_hashes_[i][bit_number] = rng();

    for(std::size_t bit_number = Squares::A3; bit_number < Squares::H3; ++bit_number)
        en_passant_hashes_[(GetBitSet(bit_number) * kHashEnPassantMagic) >> kBitShift16] = rng();

    for(std::size_t bit_number = Squares::A6; bit_number < Squares::H6; ++bit_number)
        en_passant_hashes_[(GetBitSet(bit_number) * kHashEnPassantMagic) >> kBitShift16] = rng();

    auto max_castling = kWhiteCastling_0_0 | kWhiteCastling_0_0_0 | kBlackCastling_0_0 | kBlackCastling_0_0_0;

    for(std::size_t castling_rights = 0; castling_rights <= max_castling; ++castling_rights )
        castling_hashes_[castling_rights] = rng();

    side_hash_ = rng();
}

void HashList::ComputeInitialHash(const uint64_t &en_passant, const std::size_t &castlings, Side side)
{
    uint64_t occupied = board_->occupied_;
    uint64_t hash = 0;

    hashes_ = {};

    std::size_t lsb;
    PieceType type;

    do
    {
        type = GetHashType(GetLSBPos(occupied));
        lsb = GetLSBPos(occupied);

        hash ^= pieces_hashes_[type][lsb];
        occupied &= occupied - 1;
    }
    while(occupied);

    if(en_passant)
        hash ^= en_passant_hashes_[(en_passant * kHashEnPassantMagic) >> kBitShift16];

    hash ^= castling_hashes_[castlings];

    if(side == Side::kWhite)
        hash ^= side_hash_;

    current_hash_ = &hashes_[0];
    (*current_hash_).first = hash;
    (*current_hash_).second = 0;
}

void HashList::UpdateHash(const uint64_t &move, MoveInfo* move_info, const uint64_t &en_passant, const std::size_t &castlings)
{
    uint64_t hash = (*current_hash_).first;
    std::size_t index = (*current_hash_).second;
    ++current_hash_;

    std::size_t from = move & MoveMasks::kFrom;
    std::size_t to = (move & MoveMasks::kTo) >> 6;

    std::size_t hash_type = GetHashType(to);

    hash ^= pieces_hashes_[hash_type][from];
    hash ^= pieces_hashes_[hash_type][to];

    if(move_info->captured_ != PieceType::KAllPieces && (move & MoveMasks::kFlag) != MoveFlags::kEnPassant)
        hash ^= pieces_hashes_[move_info->captured_][to];

    if((move & MoveMasks::kFlag) == MoveFlags::kEnPassant)
    {
        hash ^= en_passant_hashes_[(move_info->old_en_passant_ * kHashEnPassantMagic) >> kBitShift16];
        std::size_t en_passant_target = GetLSBPos(move_info->old_en_passant_);
        hash ^= pieces_hashes_[move_info->captured_][hash_type == PieceType::kWhitePawns ? en_passant_target - kMoveForward : en_passant_target + kMoveForward];
    }
    else if(move_info->old_en_passant_)
        hash ^= en_passant_hashes_[(move_info->old_en_passant_ * kHashEnPassantMagic) >> kBitShift16];

    if((move & MoveMasks::kFlag) == MoveFlags::kPromotion)
    {
        hash ^= pieces_hashes_[hash_type][from];
        hash ^= pieces_hashes_[hash_type <= PieceType::kWhiteQueens ? PieceType::kWhitePawns : PieceType::kBlackPawns][from];
    }

    if((move & MoveMasks::kFlag) == MoveFlags::kCastling)
    {
        hash ^= castling_hashes_[move_info->old_castling_rights_];

        switch(to)
        {
        case Squares::G8:
            hash ^= pieces_hashes_[PieceType::kBlackRooks][Squares::H8];
            hash ^= pieces_hashes_[PieceType::kBlackRooks][Squares::F8];
            break;
        case Squares::C8:
            hash ^= pieces_hashes_[PieceType::kBlackRooks][Squares::A8];
            hash ^= pieces_hashes_[PieceType::kBlackRooks][Squares::D8];
            break;
        case Squares::G1:
            hash ^= pieces_hashes_[PieceType::kWhiteRooks][Squares::H1];
            hash ^= pieces_hashes_[PieceType::kWhiteRooks][Squares::F1];
            break;
        case Squares::C1:
            hash ^= pieces_hashes_[PieceType::kWhiteRooks][Squares::A1];
            hash ^= pieces_hashes_[PieceType::kWhiteRooks][Squares::D1];
            break;
        }

        hash ^= castling_hashes_[castlings];
    }

    if(en_passant)
        hash ^= en_passant_hashes_[(en_passant * kHashEnPassantMagic) >> kBitShift16];

    hash ^= side_hash_;

    (*current_hash_).first = hash;

    if(move_info->captured_ != PieceType::KAllPieces || (move & MoveMasks::kFlag) || hash_type == PieceType::kWhitePawns || hash_type == PieceType::kBlackPawns)
    {
        (*current_hash_).second = ++index;
    }
    else
    {
        (*current_hash_).second = index;
    }
}

void HashList::UpdateHash()
{
    --current_hash_;
}

uint64_t HashList::GetHash() const
{
    return (*current_hash_).first;
}

bool HashList::Is3FoldRepetition() const
{
    auto start_search = current_hash_ - 2;
    auto end_search = &hashes_[current_hash_->second];
    std::size_t repetitions = 0;

    while(start_search >= end_search && repetitions != 2)
    {
        if(start_search->first == current_hash_->first)
            ++repetitions;

        start_search -= 2;
    }

    return repetitions == 2;
}

PieceType HashList::GetHashType(std::size_t lsb) const
{
    uint64_t bit_board = GetBitSet(lsb);

    if(bit_board & board_->black_pawns_)
        return PieceType::kBlackPawns;

    if(bit_board & board_->black_bishops_)
    {
        if(bit_board & board_->black_rooks_)
            return PieceType::kBlackQueens;
        else
            return PieceType::kBlackBishops;
    }

    if(bit_board & board_->black_rooks_)
        return PieceType::kBlackRooks;

    if(bit_board & board_->black_knights_)
        return PieceType::kBlackKnights;

    if(bit_board & board_->black_king_)
        return PieceType::kBlackKing;

    if(bit_board & board_->white_pawns_)
        return PieceType::kWhitePawns;

    if(bit_board & board_->white_bishops_)
    {
        if(bit_board & board_->white_rooks_)
            return PieceType::kWhiteQueens;
        else
            return PieceType::kWhiteBishops;
    }

    if(bit_board & board_->white_rooks_)
        return PieceType::kWhiteRooks;

    if(bit_board & board_->white_knights_)
        return PieceType::kWhiteKnights;

    if(bit_board & board_->white_king_)
        return PieceType::kWhiteKing;

    return PieceType::KAllPieces;
}
