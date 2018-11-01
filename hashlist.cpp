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

    for(std::size_t bit_number = Squares::A3; bit_number < Squares::H3; ++bit_number)
        en_passant_hashes_[(GetBitSet(bit_number) * kHashEnPassantMagic) >> kBitShift16] = rng();

    for(std::size_t bit_number = Squares::A6; bit_number < Squares::H6; ++bit_number)
        en_passant_hashes_[(GetBitSet(bit_number) * kHashEnPassantMagic) >> kBitShift16] = rng();

    auto max_castling = kWhiteCastling_0_0 | kWhiteCastling_0_0_0 | kBlackCastling_0_0 | kBlackCastling_0_0_0;

    for(std::size_t castling_rights = 0; castling_rights <= max_castling; ++castling_rights )
        castling_hashes_[castling_rights] = rng();

    current_hash_ = &hashes_[0];
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

    if(en_passant)
        hash ^= en_passant_hashes_[(en_passant * kHashEnPassantMagic) >> kBitShift16];

    if(castlings)
        hash ^= castling_hashes_[castlings];

    *current_hash_ = hash;
    ++current_hash_;

    en_passant_old_ = en_passant;
    castlings_old_ = castlings;
}

void HashList::UpdateHash(const uint64_t &move, PieceType captured)
{
    uint64_t hash = *current_hash_;

    std::size_t from = GetBitSet(move & MoveMasks::kFrom);
    std::size_t to = GetBitSet(move & MoveMasks::kTo);

    std::size_t hash_type = GetHashType(from);

    hash ^= pieces_hashes_[hash_type][from];
    hash ^= pieces_hashes_[hash_type][to];

    if(captured != PieceType::KAllPieces)
        hash ^= pieces_hashes_[captured][to];

    if((move & MoveMasks::kFlag) == MoveFlags::kEnPassant)
    {
        hash ^= en_passant_hashes_[(en_passant_old_ * kHashEnPassantMagic) >> kBitShift16];
        std::size_t en_passant_target = GetLSBPos(en_passant_old_);
        hash ^= pieces_hashes_[GetHashType(en_passant_target)][en_passant_target];
        en_passant_old_ = 0;
    }

    if((move & MoveMasks::kFlag) == MoveFlags::kPromotion)
    {
        switch(move & MoveMasks::kPromote)
        {
        case PromotionType::kQueen:
            hash ^= pieces_hashes_[hash_type == PieceType::kWhitePawns ? PieceType::kWhiteQueens : PieceType::kBlackQueens][to];
        break;

        case PromotionType::kRook:
            hash ^= pieces_hashes_[hash_type == PieceType::kWhitePawns ? PieceType::kWhiteRooks : PieceType::kBlackRooks][to];
        break;

        case PromotionType::kBishop:
            hash ^= pieces_hashes_[hash_type == PieceType::kWhitePawns ? PieceType::kWhiteBishops : PieceType::kBlackBishops][to];
        break;

        case PromotionType::kKnight:
            hash ^= pieces_hashes_[hash_type == PieceType::kWhitePawns ? PieceType::kWhiteKnights : PieceType::kBlackKnights][to];
        break;
        }
    }

    if((move & MoveMasks::kFlag) == MoveFlags::kCastling)
    {
        hash ^= castling_hashes_[castlings_old_];

        switch(to)
        {
        case Squares::G1:
            castlings_old_ ^= kBlackCastling_0_0;
            break;
        case Squares::C1:
            castlings_old_ ^= kBlackCastling_0_0_0;
            break;
        case Squares::G8:
            castlings_old_ ^= kWhiteCastling_0_0;
            break;
        case Squares::C8:
            castlings_old_ ^= kWhiteCastling_0_0_0;
            break;
        }

        hash ^= castling_hashes_[castlings_old_];
    }

    if(hash_type == PieceType::kWhitePawns && from >= Squares::A2 && from <= Squares::H2 && to >= Squares::A4 && to <= Squares::H4)
    {
        hash ^= en_passant_hashes_[((en_passant_old_ = GetBitSet(from + kBoardSize) ) * kHashEnPassantMagic) >> kBitShift16];
    }

    if(hash_type == PieceType::kBlackPawns && from >= Squares::A7 && from <= Squares::H7 && to >= Squares::A5 && to <= Squares::H5)
    {
        hash ^= en_passant_hashes_[((en_passant_old_ = GetBitSet(to + kBoardSize) ) * kHashEnPassantMagic) >> kBitShift16];
    }

    *current_hash_ = hash;
    ++current_hash_;
}

void HashList::UpdateHash()
{
    --current_hash_;
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
