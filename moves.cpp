#include "bits_functions.h"
#include "constants.h"
#include "moves.h"

#include <stack>

#include <iostream>

static std::array<std::array<uint64_t, 512>, kBitBoardSize> bishop_moves;
static std::array<std::array<uint64_t, 4096>, kBitBoardSize> rook_moves;

Moves::Moves(Board *board, uint64_t *en_passant, std::size_t *castling_rights, Side *side, std::size_t *fifty_moves_counter) : board_(board), en_passant_(en_passant),
    castling_rights_(castling_rights), active_side_(side), fifty_moves_counter_(fifty_moves_counter)
{
}

void Moves::GetWhiteKingAttacks(MoveList *move_list)
{
    std::size_t king_position = GetLSBPos(board_->white_king_);
    uint64_t king_moves = kKingMoves[king_position] & board_->blacks_ & ~kKingMoves[GetLSBPos(board_->black_king_)];
    move_list->AddMoves(king_position, king_moves);
}

void Moves::GetWhiteKingMoves(MoveList *move_list)
{
    std::size_t king_position = GetLSBPos(board_->white_king_);
    uint64_t king_attacks = kKingMoves[king_position] & board_->empty_ & ~kKingMoves[GetLSBPos(board_->black_king_)];
    move_list->AddMoves(king_position, king_attacks);
}

void Moves::GetWhiteAttacksAndPromotions(MoveList *move_list, bool include_checks)
{    
    uint64_t from = 0;
    uint64_t attacks = 0;
    uint64_t blacks = board_->blacks_;
    if(!include_checks)
        blacks ^= board_->black_king_;

    attacks = (board_->white_pawns_ << kMoveRight) & kEmptyLeft & blacks;
    move_list->AddPawnMoves(Side::kWhite, attacks & kEmptyTop, PawnMoveType::kLeftAttack);
    move_list->AddPawnPromotions(Side::kWhite, attacks & kTopRow, PawnMoveType::kLeftAttack);

    attacks = (board_->white_pawns_ << kMoveLeft) & kEmptyRight & blacks;
    move_list->AddPawnMoves(Side::kWhite, attacks & kEmptyTop, PawnMoveType::kRightAttack);
    move_list->AddPawnPromotions(Side::kWhite, attacks & kTopRow, PawnMoveType::kRightAttack);

    attacks = (board_->white_pawns_ << kMoveForward) & board_->empty_;
    move_list->AddPawnPromotions(Side::kWhite, attacks & kTopRow, PawnMoveType::kPush);

    if(*en_passant_)
    {
        move_list->AddPawnMoves(Side::kWhite, (board_->white_pawns_ << kMoveRight) & kEmptyLeft & *en_passant_, PawnMoveType::kEnPassantLeft);
        move_list->AddPawnMoves(Side::kWhite, (board_->white_pawns_ << kMoveLeft) & kEmptyRight & *en_passant_, PawnMoveType::kEnPassantRight);
    }

    uint64_t pieces = board_->white_knights_;

    while(pieces)
    {
        from = GetLSBPos(pieces);
        attacks = kKnightMoves[from] & blacks;
        move_list->AddMoves(from, attacks);
        pieces &= pieces - 1;
    }

    pieces = board_->white_bishops_;

    while(pieces)
    {
        from = GetLSBPos(pieces);
        attacks = bishop_moves[from][(((kBishopMasks[from] & board_->occupied_) * kBishopMagics[from]) >> (kBitBoardSize - GetBitsCount(kBishopMasks[from])))] & blacks;
        move_list->AddMoves(from, attacks);
        pieces &= pieces - 1;
    }

    pieces = board_->white_rooks_;

    while(pieces)
    {
        from = GetLSBPos(pieces);
        attacks = rook_moves[from][((kRookMasks[from] & board_->occupied_) * kRookMagics[from]) >> (kBitBoardSize - GetBitsCount(kRookMasks[from]))] & blacks;
        move_list->AddMoves(from, attacks);
        pieces &= pieces - 1;
    }

    GetWhiteKingAttacks(move_list);
}

void Moves::GetWhiteMoves(MoveList *move_list)
{
    uint64_t pieces = board_->white_knights_;
    uint64_t from = 0;
    uint64_t moves = 0;

    while(pieces)
    {
        from = GetLSBPos(pieces);
        moves = kKnightMoves[from] & board_->empty_;
        move_list->AddMoves(from, moves);
        pieces &= pieces - 1;
    }

    pieces = board_->white_bishops_;

    while(pieces)
    {
        from = GetLSBPos(pieces);
        moves = bishop_moves[from][(((kBishopMasks[from] & board_->occupied_) * kBishopMagics[from]) >> (kBitBoardSize - GetBitsCount(kBishopMasks[from])))] & board_->empty_;
        move_list->AddMoves(from, moves);
        pieces &= pieces - 1;
    }

    pieces = board_->white_rooks_;

    while(pieces)
    {
        from = GetLSBPos(pieces);
        moves = rook_moves[from][(((kRookMasks[from] & board_->occupied_) * kRookMagics[from]) >> (kBitBoardSize - GetBitsCount(kRookMasks[from])))] & board_->empty_;
        move_list->AddMoves(from, moves);
        pieces &= pieces - 1;
    }

    moves = ((((board_->white_pawns_ & kSecondRow ) << kMoveForward) & board_->empty_ ) << kMoveForward) & board_->empty_;
    move_list->AddPawnMoves(Side::kWhite, moves, PawnMoveType::kDoublePush);

    moves = (board_->white_pawns_ << kMoveForward) & board_->empty_;
    move_list->AddPawnMoves(Side::kWhite, moves & kEmptyTop, PawnMoveType::kPush);

    if((*castling_rights_ & Castlings::kWhiteCastling_0_0) && !(kF1G1 & board_->occupied_) && !(kG2H2 & board_->black_king_) )
    {
        move_list->addMove(Squares::E1, Squares::G1, MoveFlags::kCastling );
    }

    if((*castling_rights_ & Castlings::kWhiteCastling_0_0_0) && !(kB1C1D1 & board_->occupied_) && !(kB2C2 & board_->black_king_) )
    {
        move_list->addMove(Squares::E1, Squares::C1, MoveFlags::kCastling );
    }

    GetWhiteKingMoves(move_list);
}

void Moves::GetBlackKingAttacks(MoveList *move_list)
{
    std::size_t king_position = GetLSBPos(board_->black_king_);
    uint64_t king_moves = kKingMoves[king_position] & board_->whites_ & ~kKingMoves[GetLSBPos(board_->white_king_)];
    move_list->AddMoves(king_position, king_moves);
}

void Moves::GetBlackKingMoves(MoveList *move_list)
{
    std::size_t king_position = GetLSBPos(board_->black_king_);
    uint64_t king_attacks = kKingMoves[king_position] & board_->empty_ & ~kKingMoves[GetLSBPos(board_->white_king_)];
    move_list->AddMoves(king_position, king_attacks);
}

void Moves::GetBlackAttacksAndPromotions(MoveList *move_list, bool include_checks)
{    
    uint64_t from = 0;
    uint64_t attacks = 0;
    uint64_t whites = board_->whites_;

    if(!include_checks)
        whites ^= board_->white_king_;

    attacks = (board_->black_pawns_ >> kMoveLeft) & kEmptyLeft & whites;
    move_list->AddPawnMoves(Side::kBlack, attacks & kEmptyBottom, PawnMoveType::kRightAttack);
    move_list->AddPawnPromotions(Side::kBlack, attacks & kBottomRow, PawnMoveType::kRightAttack);

    attacks = (board_->black_pawns_ >> kMoveRight) & kEmptyRight & whites;
    move_list->AddPawnMoves(Side::kBlack, attacks & kEmptyBottom, PawnMoveType::kLeftAttack);
    move_list->AddPawnPromotions(Side::kBlack, attacks & kBottomRow, PawnMoveType::kLeftAttack);

    attacks = (board_->black_pawns_ >> kMoveForward) & board_->empty_;
    move_list->AddPawnPromotions(Side::kBlack, attacks & kBottomRow, PawnMoveType::kPush);

    if(*en_passant_)
    {
        move_list->AddPawnMoves(Side::kBlack, (board_->black_pawns_ >> kMoveRight) & kEmptyRight & *en_passant_, PawnMoveType::kEnPassantLeft);
        move_list->AddPawnMoves(Side::kBlack, (board_->black_pawns_ >> kMoveLeft) & kEmptyLeft & *en_passant_, PawnMoveType::kEnPassantRight);
    }

    uint64_t pieces = board_->black_knights_;

    while(pieces)
    {
        from = GetLSBPos(pieces);
        attacks = kKnightMoves[from] & whites;
        move_list->AddMoves(from, attacks);
        pieces &= pieces - 1;
    }

    pieces = board_->black_bishops_;

    while(pieces)
    {
        from = GetLSBPos(pieces);
        attacks = bishop_moves[from][(((kBishopMasks[from] & board_->occupied_) * kBishopMagics[from]) >> (kBitBoardSize - GetBitsCount(kBishopMasks[from])))] & whites;
        move_list->AddMoves(from, attacks);
        pieces &= pieces - 1;
    }

    pieces = board_->black_rooks_;

    while(pieces)
    {
        from = GetLSBPos(pieces);
        attacks = rook_moves[from][((kRookMasks[from] & board_->occupied_) * kRookMagics[from]) >> (kBitBoardSize - GetBitsCount(kRookMasks[from]))] & whites;
        move_list->AddMoves(from, attacks);
        pieces &= pieces - 1;
    }

    GetBlackKingAttacks(move_list);
}

void Moves::GetBlackMoves(MoveList *move_list)
{
    uint64_t pieces = board_->black_knights_;
    uint64_t from = 0;
    uint64_t moves = 0;

    while(pieces)
    {
        from = GetLSBPos(pieces);
        moves = kKnightMoves[from] & board_->empty_;
        move_list->AddMoves(from, moves);
        pieces &= pieces - 1;
    }

    pieces = board_->black_bishops_;

    while(pieces)
    {
        from = GetLSBPos(pieces);
        moves = bishop_moves[from][(((kBishopMasks[from] & board_->occupied_) * kBishopMagics[from]) >> (kBitBoardSize - GetBitsCount(kBishopMasks[from])))] & board_->empty_;
        move_list->AddMoves(from, moves);
        pieces &= pieces - 1;
    }

    pieces = board_->black_rooks_;

    while(pieces)
    {
        from = GetLSBPos(pieces);
        moves = rook_moves[from][(((kRookMasks[from] & board_->occupied_) * kRookMagics[from]) >> (kBitBoardSize - GetBitsCount(kRookMasks[from])))] & board_->empty_;
        move_list->AddMoves(from, moves);
        pieces &= pieces - 1;
    }

    moves = ((((board_->black_pawns_ & kSeventhRow) >> kMoveForward) & board_->empty_) >> kMoveForward) & board_->empty_;
    move_list->AddPawnMoves(Side::kBlack, moves, PawnMoveType::kDoublePush);

    moves = (board_->black_pawns_ >> kMoveForward) & board_->empty_;
    move_list->AddPawnMoves(Side::kBlack, moves & kEmptyBottom, PawnMoveType::kPush);

    if((*castling_rights_ & Castlings::kBlackCastling_0_0) && !(kF8G8 & board_->occupied_) && !(kG7H7 & board_->white_king_) )
    {
        move_list->addMove(Squares::E8, Squares::G8, MoveFlags::kCastling );
    }

    if((*castling_rights_ & Castlings::kBlackCastling_0_0_0) && !(kB8C8D8 & board_->occupied_) && !(kB7C7 & board_->white_king_) )
    {
        move_list->addMove(Squares::E8, Squares::C8, MoveFlags::kCastling );
    }

    GetBlackKingMoves(move_list);
}

bool Moves::MakeMove(std::size_t move)
{
    uint64_t from = GetBitSet(move & MoveMasks::kFrom);
    uint64_t to = GetBitSet((move & MoveMasks::kTo) >> 6);

    bool is_legal = true;
    PieceType captured = PieceType::KAllPieces;

    if(*active_side_)
    {
        if(board_->white_pawns_ & from)
        {
            board_->white_pawns_ ^= from | to;
            board_->whites_ ^= from | to;
            board_->occupied_ ^= from;

            if((move & MoveMasks::kFlag) == MoveFlags::kEnPassant)
            {
                board_->occupied_ ^= to;
                to >>= kMoveForward;
            }
        }
        else if(board_->white_knights_ & from)
        {
            board_->white_knights_ ^= from | to;
            board_->whites_ ^= from | to;
            board_->occupied_ ^= from;
        }
        else if(board_->white_bishops_ & board_->white_rooks_ & from)
        {
            board_->white_bishops_ ^= from | to;
            board_->white_rooks_ ^= from | to;
            board_->whites_ ^= from | to;
            board_->occupied_ ^= from;
        }
        else if(board_->white_bishops_ & from)
        {
            board_->white_bishops_ ^= from | to;
            board_->whites_ ^= from | to;
            board_->occupied_ ^= from;
        }
        else if(board_->white_rooks_ & from)
        {
            board_->white_rooks_ ^= from | to;
            board_->whites_ ^= from | to;
            board_->occupied_ ^= from;
        }
        else if(board_->white_king_ & from)
        {
            board_->white_king_ ^= from | to;
            board_->whites_ ^= from | to;
            board_->occupied_ ^= from;

            if((move & MoveMasks::kFlag) == MoveFlags::kCastling)
            {
                if(to == FieldBitboard::kC1)
                    is_legal &= !IsSquareAttacked(Squares::C1) && !IsSquareAttacked(Squares::D1) && !IsSquareAttacked(Squares::E1);
                else
                    is_legal &= !IsSquareAttacked(Squares::G1) && !IsSquareAttacked(Squares::F1) && !IsSquareAttacked(Squares::E1);

                if(!is_legal)
                {
                    board_->white_king_ ^= from | to;
                    board_->whites_ ^= from | to;
                    board_->occupied_ ^= from;

                    return is_legal;
                }
            }
        }

        if(board_->blacks_ & to)
        {
            if(board_->black_pawns_ & to)
            {
                board_->black_pawns_ ^= to;
                board_->blacks_ ^= to;

                if((move & MoveMasks::kFlag) == MoveFlags::kEnPassant)
                {
                    board_->occupied_ ^= to;                    
                }

                is_legal &= !IsSquareAttacked(GetLSBPos(board_->white_king_));

                if(!is_legal)
                {
                    board_->black_pawns_ ^= to;
                    board_->blacks_ ^= to;

                    if((move & MoveMasks::kFlag) == MoveFlags::kEnPassant)
                    {
                        board_->occupied_ ^= to;
                        to <<= kMoveForward;
                        board_->occupied_ ^= to;
                    }
                }
                else
                    captured = PieceType::kBlackPawns;
            }
            else if(board_->black_knights_ & to)
            {
                board_->black_knights_ ^= to;
                board_->blacks_ ^= to;

                is_legal &= !IsSquareAttacked(GetLSBPos(board_->white_king_));

                if(!is_legal)
                {
                    board_->black_knights_ ^= to;
                    board_->blacks_ ^= to;
                }
                else
                    captured = PieceType::kBlackKnights;
            }
            else if(board_->black_bishops_ & board_->black_rooks_ & to)
            {
                board_->black_bishops_ ^= to;
                board_->black_rooks_ ^= to;
                board_->blacks_ ^= to;

                is_legal &= !IsSquareAttacked(GetLSBPos(board_->white_king_));

                if(!is_legal)
                {
                    board_->black_bishops_ ^= to;
                    board_->black_rooks_ ^= to;
                    board_->blacks_ ^= to;
                }
                else
                    captured = PieceType::kBlackQueens;
            }
            else if(board_->black_bishops_ & to)
            {
                board_->black_bishops_ ^= to;
                board_->blacks_ ^= to;

                is_legal &= !IsSquareAttacked(GetLSBPos(board_->white_king_));

                if(!is_legal)
                {
                    board_->black_bishops_ ^= to;
                    board_->blacks_ ^= to;
                }
                else
                    captured = PieceType::kBlackBishops;
            }
            else if(board_->black_rooks_ & to)
            {
                board_->black_rooks_ ^= to;
                board_->blacks_ ^= to;

                is_legal &= !IsSquareAttacked(GetLSBPos(board_->white_king_));

                if(!is_legal)
                {
                    board_->black_rooks_ ^= to;
                    board_->blacks_ ^= to;
                }
                else
                    captured = PieceType::kBlackRooks;
            }

            if(!is_legal)
            {
                if(board_->white_pawns_ & to)
                {
                    board_->white_pawns_ ^= from | to;
                    board_->whites_ ^= from | to;
                    board_->occupied_ ^= from;
                }
                else if(board_->white_knights_ & to)
                {
                    board_->white_knights_ ^= from | to;
                    board_->whites_ ^= from | to;
                    board_->occupied_ ^= from;
                }
                else if(board_->white_bishops_ & board_->white_rooks_ & to)
                {
                    board_->white_bishops_ ^= from | to;
                    board_->white_rooks_ ^= from | to;
                    board_->whites_ ^= from | to;
                    board_->occupied_ ^= from;
                }
                else if(board_->white_bishops_ & to)
                {
                    board_->white_bishops_ ^= from | to;
                    board_->whites_ ^= from | to;
                    board_->occupied_ ^= from;
                }
                else if(board_->white_rooks_ & to)
                {
                    board_->white_rooks_ ^= from | to;
                    board_->whites_ ^= from | to;
                    board_->occupied_ ^= from;
                }
                else if(board_->white_king_ & to)
                {
                    board_->white_king_ ^= from | to;
                    board_->whites_ ^= from | to;
                    board_->occupied_ ^= from;
                }

                return is_legal;
            }
        }
        else
        {
            board_->occupied_ ^= to;

            is_legal = !IsSquareAttacked(GetLSBPos(board_->white_king_));

            if(!is_legal)
            {
                if(board_->white_pawns_ & to)
                {
                    board_->white_pawns_ ^= from | to;
                    board_->whites_ ^= from | to;
                    board_->occupied_ ^= from | to;
                }
                else if(board_->white_knights_ & to)
                {
                    board_->white_knights_ ^= from | to;
                    board_->whites_ ^= from | to;
                    board_->occupied_ ^= from | to;
                }
                else if(board_->white_bishops_ & board_->white_rooks_ & to)
                {
                    board_->white_bishops_ ^= from | to;
                    board_->white_rooks_ ^= from | to;
                    board_->whites_ ^= from | to;
                    board_->occupied_ ^= from | to;
                }
                else if(board_->white_bishops_ & to)
                {
                    board_->white_bishops_ ^= from | to;
                    board_->whites_ ^= from | to;
                    board_->occupied_ ^= from | to;
                }
                else if(board_->white_rooks_ & to)
                {
                    board_->white_rooks_ ^= from | to;
                    board_->whites_ ^= from | to;
                    board_->occupied_ ^= from | to;
                }
                else if(board_->white_king_ & to)
                {
                    board_->white_king_ ^= from | to;
                    board_->whites_ ^= from | to;
                    board_->occupied_ ^= from | to;
                }

                return is_legal;
            }
        }
    }
    else
    {
        if(board_->black_pawns_ & from)
        {
            board_->black_pawns_ ^= from | to;
            board_->blacks_ ^= from | to;
            board_->occupied_ ^= from;

            if((move & MoveMasks::kFlag) == MoveFlags::kEnPassant)
            {
                board_->occupied_ ^= to;
                to <<= kMoveForward;
            }
        }
        else if(board_->black_knights_ & from)
        {
            board_->black_knights_ ^= from | to;
            board_->blacks_ ^= from | to;
            board_->occupied_ ^= from;
        }
        else if(board_->black_bishops_ & board_->black_rooks_ & from)
        {
            board_->black_bishops_ ^= from | to;
            board_->black_rooks_ ^= from | to;
            board_->blacks_ ^= from | to;
            board_->occupied_ ^= from;
        }
        else if(board_->black_bishops_ & from)
        {
            board_->black_bishops_ ^= from | to;
            board_->blacks_ ^= from | to;
            board_->occupied_ ^= from;
        }
        else if(board_->black_rooks_ & from)
        {
            board_->black_rooks_ ^= from | to;
            board_->blacks_ ^= from | to;
            board_->occupied_ ^= from;
        }
        else if(board_->black_king_ & from)
        {
            board_->black_king_ ^= from | to;
            board_->blacks_ ^= from | to;
            board_->occupied_ ^= from;

            if((move & MoveMasks::kFlag) == MoveFlags::kCastling)
            {
                if(to == FieldBitboard::kC8)
                    is_legal = !IsSquareAttacked(Squares::C8) && !IsSquareAttacked(Squares::D8) && !IsSquareAttacked(Squares::E8);
                else
                    is_legal = !IsSquareAttacked(Squares::G8) && !IsSquareAttacked(Squares::F8) && !IsSquareAttacked(Squares::E8);

                if(!is_legal)
                {
                    board_->black_king_ ^= from | to;
                    board_->blacks_ ^= from | to;
                    board_->occupied_ ^= from;

                    return is_legal;
                }
            }
        }

        if(board_->whites_ & to)
        {
            if(board_->white_pawns_ & to)
            {
                board_->white_pawns_ ^= to;
                board_->whites_ ^= to;

                if((move & MoveMasks::kFlag) == MoveFlags::kEnPassant)
                {
                    board_->occupied_ ^= to;
                }

                is_legal &= !IsSquareAttacked(GetLSBPos(board_->black_king_));

                if(!is_legal)
                {
                    board_->white_pawns_ ^= to;
                    board_->whites_ ^= to;

                    if((move & MoveMasks::kFlag) == MoveFlags::kEnPassant)
                    {
                        board_->occupied_ ^= to;
                        to >>= kMoveForward;
                        board_->occupied_ ^= to;
                    }
                }
                else
                    captured = PieceType::kWhitePawns;
            }
            else if(board_->white_knights_ & to)
            {
                board_->white_knights_ ^= to;
                board_->whites_ ^= to;

                is_legal &= !IsSquareAttacked(GetLSBPos(board_->black_king_));

                if(!is_legal)
                {
                    board_->white_knights_ ^= to;
                    board_->whites_ ^= to;
                }
                else
                    captured = PieceType::kWhiteKnights;
            }
            else if(board_->white_bishops_ & board_->white_rooks_ & to)
            {
                board_->white_bishops_ ^= to;
                board_->white_rooks_ ^= to;
                board_->whites_ ^= to;

                is_legal &= !IsSquareAttacked(GetLSBPos(board_->black_king_));

                if(!is_legal)
                {
                    board_->white_bishops_ ^= to;
                    board_->white_rooks_ ^= to;
                    board_->whites_ ^= to;
                }
                else
                    captured = PieceType::kWhiteQueens;
            }
            else if(board_->white_bishops_ & to)
            {
                board_->white_bishops_ ^= to;
                board_->whites_ ^= to;

                is_legal &= !IsSquareAttacked(GetLSBPos(board_->black_king_));

                if(!is_legal)
                {
                    board_->white_bishops_ ^= to;
                    board_->whites_ ^= to;
                }
                else
                    captured = PieceType::kWhiteBishops;
            }
            else if(board_->white_rooks_ & to)
            {
                board_->white_rooks_ ^= to;
                board_->whites_ ^= to;

                is_legal &= !IsSquareAttacked(GetLSBPos(board_->black_king_));

                if(!is_legal)
                {
                    board_->white_rooks_ ^= to;
                    board_->whites_ ^= to;
                }
                else
                    captured = PieceType::kWhiteRooks;
            }

            if(!is_legal)
            {
                if(board_->black_pawns_ & to)
                {
                    board_->black_pawns_ ^= from | to;
                    board_->blacks_ ^= from | to;
                    board_->occupied_ ^= from;
                }
                else if(board_->black_knights_ & to)
                {
                    board_->black_knights_ ^= from | to;
                    board_->blacks_ ^= from | to;
                    board_->occupied_ ^= from;
                }
                else if(board_->black_bishops_ & board_->black_rooks_ & to)
                {
                    board_->black_bishops_ ^= from | to;
                    board_->black_rooks_ ^= from | to;
                    board_->blacks_ ^= from | to;
                    board_->occupied_ ^= from;
                }
                else if(board_->black_bishops_ & to)
                {
                    board_->black_bishops_ ^= from | to;
                    board_->blacks_ ^= from | to;
                    board_->occupied_ ^= from;
                }
                else if(board_->black_rooks_ & to)
                {
                    board_->black_rooks_ ^= from | to;
                    board_->blacks_ ^= from | to;
                    board_->occupied_ ^= from;
                }
                else if(board_->black_king_ & to)
                {
                    board_->black_king_ ^= from | to;
                    board_->blacks_ ^= from | to;
                    board_->occupied_ ^= from;
                }

                return is_legal;
            }
        }
        else
        {
            board_->occupied_ ^= to;

            is_legal &= !IsSquareAttacked(GetLSBPos(board_->black_king_));

            if(!is_legal)
            {
                if(board_->black_pawns_ & to)
                {
                    board_->black_pawns_ ^= from | to;
                    board_->blacks_ ^= from | to;
                    board_->occupied_ ^= from | to;
                }
                else if(board_->black_knights_ & to)
                {
                    board_->black_knights_ ^= from | to;
                    board_->blacks_ ^= from | to;
                    board_->occupied_ ^= from | to;
                }
                else if(board_->black_bishops_ & board_->black_rooks_ & to)
                {
                    board_->black_bishops_ ^= from | to;
                    board_->black_rooks_ ^= from | to;
                    board_->blacks_ ^= from | to;
                    board_->occupied_ ^= from | to;
                }
                else if(board_->black_bishops_ & to)
                {
                    board_->black_bishops_ ^= from | to;
                    board_->blacks_ ^= from | to;
                    board_->occupied_ ^= from | to;
                }
                else if(board_->black_rooks_ & to)
                {
                    board_->black_rooks_ ^= from | to;
                    board_->blacks_ ^= from | to;
                    board_->occupied_ ^= from | to;
                }
                else if(board_->black_king_ & to)
                {
                    board_->black_king_ ^= from | to;
                    board_->blacks_ ^= from | to;
                    board_->occupied_ ^= from | to;
                }

                return is_legal;
            }
        }
    }

    last_move_->old_castling_rights_ = *castling_rights_;
    last_move_->old_en_passant_ = *en_passant_;
    last_move_->captured_ = captured;

    if(captured != PieceType::KAllPieces)
        board_->RemovePiece(captured);

    if(captured != PieceType::KAllPieces || (*active_side_ && (board_->white_pawns_ & to) ) || (!*active_side_ && (board_->black_pawns_ & to) ) )
    {
        last_move_->old_fifty_moves_counter_ = *fifty_moves_counter_;
        *fifty_moves_counter_ = 0;
    }
    else
        *fifty_moves_counter_ += 1;

    ++last_move_;

    if(*en_passant_)
        *en_passant_ = 0;

    if(*active_side_)
    {
        if((move & MoveMasks::kFlag) == MoveFlags::kCastling)
        {
            if(to == FieldBitboard::kG1)
            {
                board_->white_rooks_ ^= FieldBitboard::kF1 | FieldBitboard::kH1;
                board_->whites_ ^= FieldBitboard::kF1 | FieldBitboard::kH1;
                board_->occupied_ ^= FieldBitboard::kF1 | FieldBitboard::kH1;
            }
            else
            {
                board_->white_rooks_ ^= FieldBitboard::kD1 | FieldBitboard::kA1;
                board_->whites_ ^= FieldBitboard::kD1 | FieldBitboard::kA1;
                board_->occupied_ ^= FieldBitboard::kD1 | FieldBitboard::kA1;
            }

            *castling_rights_ &= 0XC;
        }
        else if((move & MoveMasks::kFlag) == MoveFlags::kPromotion)
        {
            board_->RemovePiece(PieceType::kWhitePawns);

            switch((move & MoveMasks::kPromote) >> 14)
            {
            case PromotionType::kQueen:
                board_->white_bishops_ ^= to;
                board_->white_rooks_ ^= to;
                board_->AddPiece(PieceType::kWhiteQueens);
                break;

            case PromotionType::kRook:
                board_->white_rooks_ ^= to;
                board_->AddPiece(PieceType::kWhiteRooks);
                break;

            case PromotionType::kBishop:
                board_->white_bishops_ ^= to;
                board_->AddPiece(PieceType::kWhiteBishops);
                break;

            case PromotionType::kKnight:
                board_->white_knights_ ^= to;
                board_->AddPiece(PieceType::kWhiteKnights);
                break;
            }

            board_->white_pawns_ ^= to;
        }
        else if((board_->white_pawns_ & to) && (from << kMoveForwardTwo) == to )
        {
            *en_passant_ = to >> kMoveForward;
        }
        else if(board_->white_rooks_ & to)
        {
            if((*castling_rights_ & Castlings::kWhiteCastling_0_0) && from == FieldBitboard::kH1)
                *castling_rights_ &= 0XE;

            if((*castling_rights_ & Castlings::kWhiteCastling_0_0_0) && from == FieldBitboard::kA1)
                *castling_rights_ &= 0XD;
        }
        else if(board_->white_king_ & to)
        {
            *castling_rights_ &= 0XC;
        }

        if(captured == PieceType::kBlackRooks)
        {
            if((*castling_rights_ & Castlings::kBlackCastling_0_0) && to == FieldBitboard::kH8)
                *castling_rights_ &= 0XB;

            if((*castling_rights_ & Castlings::kBlackCastling_0_0_0) && to == FieldBitboard::kA8)
                *castling_rights_ &= 0X7;
        }

    }
    else
    {
        if((move & MoveMasks::kFlag) == MoveFlags::kCastling)
        {
            if(to == FieldBitboard::kG8)
            {
                board_->black_rooks_ ^= FieldBitboard::kF8 | FieldBitboard::kH8;
                board_->blacks_ ^= FieldBitboard::kF8 | FieldBitboard::kH8;
                board_->occupied_ ^= FieldBitboard::kF8 | FieldBitboard::kH8;
            }
            else
            {
                board_->black_rooks_ ^= FieldBitboard::kD8 | FieldBitboard::kA8;
                board_->blacks_ ^= FieldBitboard::kD8 | FieldBitboard::kA8;
                board_->occupied_ ^= FieldBitboard::kD8 | FieldBitboard::kA8;
            }

            *castling_rights_ &= 0X3;
        }
        else if((move & MoveMasks::kFlag) == MoveFlags::kPromotion)
        {
            board_->RemovePiece(PieceType::kBlackPawns);

            switch((move & MoveMasks::kPromote) >> 14)
            {
            case PromotionType::kQueen:
                board_->black_bishops_ ^= to;
                board_->black_rooks_ ^= to;
                board_->AddPiece(PieceType::kBlackQueens);
                break;

            case PromotionType::kRook:
                board_->black_rooks_ ^= to;
                board_->AddPiece(PieceType::kBlackRooks);
                break;

            case PromotionType::kBishop:
                board_->black_bishops_ ^= to;
                board_->AddPiece(PieceType::kBlackBishops);
                break;

            case PromotionType::kKnight:
                board_->black_knights_ ^= to;
                board_->AddPiece(PieceType::kBlackKnights);
                break;
            }

            board_->black_pawns_ ^= to;
        }
        else if((board_->black_pawns_ & to) && (from >> kMoveForwardTwo) == to )
        {
            *en_passant_ = to << kMoveForward;
        }
        else if(board_->black_rooks_ & to)
        {
            if((*castling_rights_ & Castlings::kBlackCastling_0_0) && from == FieldBitboard::kH8)
                *castling_rights_ &= 0XB;

            if((*castling_rights_ & Castlings::kBlackCastling_0_0_0) && from == FieldBitboard::kA8)
                *castling_rights_ &= 0X7;
        }
        else if(board_->black_king_ & to)
        {
            *castling_rights_ &= 0X3;
        }

        if(captured == PieceType::kWhiteRooks)
        {
            if((*castling_rights_ & Castlings::kWhiteCastling_0_0) && to == FieldBitboard::kH1)
                *castling_rights_ &= 0XE;

            if((*castling_rights_ & Castlings::kWhiteCastling_0_0_0) && to == FieldBitboard::kA1)
                *castling_rights_ &= 0XD;
        }
    }

    board_->empty_ = ~board_->occupied_;

    if(*active_side_)
        *active_side_ = Side::kBlack;
    else
        *active_side_ = Side::kWhite;

    return true;
}

void Moves::UnmakeMove(std::size_t move)
{
    uint64_t from = GetBitSet(move & MoveMasks::kFrom);
    uint64_t to = GetBitSet((move & MoveMasks::kTo) >> 6);

    --last_move_;

    if(!(*active_side_))
    {
        if((move & MoveMasks::kFlag) == MoveFlags::kPromotion)
        {
            switch((move & MoveMasks::kPromote) >> 14)
            {
            case PromotionType::kQueen:
                board_->white_bishops_ ^= to;
                board_->white_rooks_ ^= to;
                board_->RemovePiece(PieceType::kWhiteQueens);
                break;

            case PromotionType::kRook:
                board_->white_rooks_ ^= to;
                board_->RemovePiece(PieceType::kWhiteRooks);
                break;

            case PromotionType::kBishop:
                board_->white_bishops_ ^= to;
                board_->RemovePiece(PieceType::kWhiteBishops);
                break;

            case PromotionType::kKnight:
                board_->white_knights_ ^= to;
                board_->RemovePiece(PieceType::kWhiteKnights);
                break;
            }

            board_->white_pawns_ ^= to;
            board_->AddPiece(PieceType::kWhitePawns);
        }

        if(board_->white_pawns_ & to)
        {
            board_->white_pawns_ ^= from | to;
        }
        else if(board_->white_knights_ & to)
        {
            board_->white_knights_ ^= from | to;
        }
        else if(board_->white_bishops_ & board_->white_rooks_ & to)
        {
            board_->white_bishops_ ^= from | to;
            board_->white_rooks_ ^= from | to;
        }
        else if(board_->white_bishops_ & to)
        {
            board_->white_bishops_ ^= from | to;
        }
        else if(board_->white_rooks_ & to)
        {
            board_->white_rooks_ ^= from | to;
        }
        else if(board_->white_king_ & to)
        {
            board_->white_king_ ^= from | to;

            if((move & MoveMasks::kFlag) == MoveFlags::kCastling)
            {
                if(to == FieldBitboard::kG1)
                {
                    board_->white_rooks_ ^= FieldBitboard::kF1 | FieldBitboard::kH1;
                    board_->whites_ ^= FieldBitboard::kF1 | FieldBitboard::kH1;
                    board_->occupied_ ^= FieldBitboard::kF1 | FieldBitboard::kH1;
                }
                else
                {
                    board_->white_rooks_ ^= FieldBitboard::kD1 | FieldBitboard::kA1;
                    board_->whites_ ^= FieldBitboard::kD1 | FieldBitboard::kA1;
                    board_->occupied_ ^= FieldBitboard::kD1 | FieldBitboard::kA1;
                }
            }
        }

        board_->whites_ ^= from | to;
        board_->occupied_ ^= from;

        switch(last_move_->captured_)
        {
        case PieceType::kBlackPawns:

            if((move & MoveMasks::kFlag) == MoveFlags::kEnPassant)
            {
                board_->occupied_ ^= to;
                to >>= kMoveForward;
                board_->occupied_ ^= to;
            }

            board_->black_pawns_ ^= to;
            board_->blacks_ ^= to;
            break;

        case PieceType::kBlackKnights:
            board_->black_knights_ ^= to;
            board_->blacks_ ^= to;
            break;

        case PieceType::kBlackBishops:
            board_->black_bishops_ ^= to;
            board_->blacks_ ^= to;
            break;

        case PieceType::kBlackQueens:
            board_->black_bishops_ ^= to;
            board_->black_rooks_ ^= to;
            board_->blacks_ ^= to;
            break;

        case PieceType::kBlackRooks:
            board_->black_rooks_ ^= to;
            board_->blacks_ ^= to;
            break;

        default:
            board_->occupied_ ^= to;
            break;
        }

        board_->AddPiece(last_move_->captured_);
    }
    else
    {
        if((move & MoveMasks::kFlag) == MoveFlags::kPromotion)
        {
            switch((move & MoveMasks::kPromote) >> 14)
            {
            case PromotionType::kQueen:
                board_->black_bishops_ ^= to;
                board_->black_rooks_ ^= to;
                board_->RemovePiece(PieceType::kBlackQueens);
                break;

            case PromotionType::kRook:
                board_->black_rooks_ ^= to;
                board_->RemovePiece(PieceType::kBlackRooks);
                break;

            case PromotionType::kBishop:
                board_->black_bishops_ ^= to;
                board_->RemovePiece(PieceType::kBlackBishops);
                break;

            case PromotionType::kKnight:
                board_->black_knights_ ^= to;
                board_->RemovePiece(PieceType::kBlackKnights);
                break;
            }

            board_->black_pawns_ ^= to;
            board_->AddPiece(PieceType::kBlackPawns);
        }

        if(board_->black_pawns_ & to)
        {
            board_->black_pawns_ ^= from | to;
        }
        else if(board_->black_knights_ & to)
        {
            board_->black_knights_ ^= from | to;
        }
        else if(board_->black_bishops_ & board_->black_rooks_ & to)
        {
            board_->black_bishops_ ^= from | to;
            board_->black_rooks_ ^= from | to;
        }
        else if(board_->black_bishops_ & to)
        {
            board_->black_bishops_ ^= from | to;
        }
        else if(board_->black_rooks_ & to)
        {
            board_->black_rooks_ ^= from | to;
        }
        else if(board_->black_king_ & to)
        {
            board_->black_king_ ^= from | to;

            if((move & MoveMasks::kFlag) == MoveFlags::kCastling)
            {
                if(to == FieldBitboard::kG8)
                {
                    board_->black_rooks_ ^= FieldBitboard::kF8 | FieldBitboard::kH8;
                    board_->blacks_ ^= FieldBitboard::kF8 | FieldBitboard::kH8;
                    board_->occupied_ ^= FieldBitboard::kF8 | FieldBitboard::kH8;
                }
                else
                {
                    board_->black_rooks_ ^= FieldBitboard::kD8 | FieldBitboard::kA8;
                    board_->blacks_ ^= FieldBitboard::kD8 | FieldBitboard::kA8;
                    board_->occupied_ ^= FieldBitboard::kD8 | FieldBitboard::kA8;
                }
            }
        }

        board_->blacks_ ^= from | to;
        board_->occupied_ ^= from;

        switch(last_move_->captured_)
        {
        case PieceType::kWhitePawns:

            if((move & MoveMasks::kFlag) == MoveFlags::kEnPassant)
            {
                board_->occupied_ ^= to;
                to <<= kMoveForward;
                board_->occupied_ ^= to;
            }

            board_->white_pawns_ ^= to;
            board_->whites_ ^= to;
            break;

        case PieceType::kWhiteKnights:
            board_->white_knights_ ^= to;
            board_->whites_ ^= to;
            break;

        case PieceType::kWhiteBishops:
            board_->white_bishops_ ^= to;
            board_->whites_ ^= to;
            break;

        case PieceType::kWhiteQueens:
            board_->white_bishops_ ^= to;
            board_->white_rooks_ ^= to;
            board_->whites_ ^= to;
            break;

        case PieceType::kWhiteRooks:
            board_->white_rooks_ ^= to;
            board_->whites_ ^= to;
            break;

        default:
            board_->occupied_ ^= to;
            break;

        }

        board_->AddPiece(last_move_->captured_);
    }

    board_->empty_ = ~board_->occupied_;

    *castling_rights_ = last_move_->old_castling_rights_;
    *en_passant_ = last_move_->old_en_passant_;

    if(last_move_->captured_ != PieceType::KAllPieces || (!*active_side_ && (board_->white_pawns_ & from) ) || (*active_side_ && (board_->black_pawns_ & from) ) )
    {
        *fifty_moves_counter_ = last_move_->old_fifty_moves_counter_;
    }
    else
        *fifty_moves_counter_ -= 1;

    if(*active_side_)
        *active_side_ = Side::kBlack;
    else
        *active_side_ = Side::kWhite;
}

bool Moves::IsKingAttacked(Side side) const
{
    std::size_t square = side == Side::kWhite ? GetLSBPos(board_->white_king_) : GetLSBPos(board_->black_king_);

    if(side == Side::kWhite)
    {
        if(rook_moves[square][(((kRookMasks[square] & board_->occupied_) * kRookMagics[square]) >> (kBitBoardSize - GetBitsCount(kRookMasks[square])))] & (board_->black_rooks_))
            return true;

        if(bishop_moves[square][(((kBishopMasks[square] & board_->occupied_) * kBishopMagics[square]) >> (kBitBoardSize - GetBitsCount(kBishopMasks[square])))] & (board_->black_bishops_))
            return true;

        if (kKnightMoves[square] & board_->black_knights_)
            return true;

        if (kWhitePawnsAttacks[square] & board_->black_pawns_)
            return true;
    }
    else
    {
        if(rook_moves[square][(((kRookMasks[square] & board_->occupied_) * kRookMagics[square]) >> (kBitBoardSize - GetBitsCount(kRookMasks[square])))] & (board_->white_rooks_))
            return true;

        if(bishop_moves[square][(((kBishopMasks[square] & board_->occupied_) * kBishopMagics[square]) >> (kBitBoardSize - GetBitsCount(kBishopMasks[square])))] & (board_->white_bishops_))
            return true;

        if (kKnightMoves[square] & board_->white_knights_)
            return true;

        if (kBlackPawnsAttacks[square] & board_->white_pawns_)
            return true;
    }

    return false;
}

void Moves::Reset()
{
    last_move_ = &move_infos[0];
}

MoveInfo *Moves::GetLastMoveInfo() const
{
    return  (last_move_ - 1);
}

bool Moves::IsSquareAttacked(std::size_t square) const
{
    if(*active_side_)
    {
        if(rook_moves[square][(((kRookMasks[square] & board_->occupied_) * kRookMagics[square]) >> (kBitBoardSize - GetBitsCount(kRookMasks[square])))] & (board_->black_rooks_))
            return true;

        if(bishop_moves[square][(((kBishopMasks[square] & board_->occupied_) * kBishopMagics[square]) >> (kBitBoardSize - GetBitsCount(kBishopMasks[square])))] & (board_->black_bishops_))
            return true;

        if (kKnightMoves[square] & board_->black_knights_)
            return true;

        if (kWhitePawnsAttacks[square] & board_->black_pawns_)
            return true;
    }
    else
    {
        if(rook_moves[square][(((kRookMasks[square] & board_->occupied_) * kRookMagics[square]) >> (kBitBoardSize - GetBitsCount(kRookMasks[square])))] & (board_->white_rooks_))
            return true;

        if(bishop_moves[square][(((kBishopMasks[square] & board_->occupied_) * kBishopMagics[square]) >> (kBitBoardSize - GetBitsCount(kBishopMasks[square])))] & (board_->white_bishops_))
            return true;

        if (kKnightMoves[square] & board_->white_knights_)
            return true;

        if (kBlackPawnsAttacks[square] & board_->white_pawns_)
            return true;
    }

    return false;
}

std::size_t Moves::GetMove(const std::string &move_string)
{
    std::size_t move = 0;

    std::size_t from = static_cast<std::size_t>(move_string[1] - '1') * kBoardSize + static_cast<std::size_t>(move_string[0] - 'a');
    std::size_t to = static_cast<std::size_t>(move_string[3] - '1') * kBoardSize + static_cast<std::size_t>(move_string[2] - 'a');
    uint64_t from_bitboard = GetBitSet(from);
    uint64_t to_bitboard = GetBitSet(to);

    if(*active_side_)
    {
        if((from_bitboard & FieldBitboard::kE1) && (to_bitboard & FieldBitboard::kG1) && (from_bitboard & board_->white_king_) )
        {
            if((*castling_rights_ & Castlings::kWhiteCastling_0_0) && (board_->white_rooks_ & FieldBitboard::kH1) )
                move = from | (to << 6) | MoveFlags::kCastling;
        }
        else if((from_bitboard & FieldBitboard::kE1) && (to_bitboard & FieldBitboard::kC1) && (from_bitboard & board_->white_king_) )
        {
            if((*castling_rights_ & Castlings::kWhiteCastling_0_0_0) && (board_->white_rooks_ & FieldBitboard::kA1) )
                move = from | (to << 6) | MoveFlags::kCastling;
        }
        else if(*en_passant_ && (from_bitboard & board_->white_pawns_) && (to_bitboard & *en_passant_) )
        {
            if((to_bitboard & board_->empty_) && ((to_bitboard >>= kMoveForward) & board_->black_pawns_) )
                move = from | (to << 6) | MoveFlags::kEnPassant;
        }
        else if(move_string.size() == 5)
        {
            if((to_bitboard & kTopRow) && (from_bitboard & board_->white_pawns_))
                switch(move_string[4])
                {
                case 'q':
                    move |= from | (to << 6) | MoveFlags::kPromotion | PromotionType::kQueen << 14;
                    break;
                case 'r':
                    move |= from | (to << 6) | MoveFlags::kPromotion | PromotionType::kRook << 14;
                    break;
                case 'b':
                    move |= from | (to << 6) | MoveFlags::kPromotion | PromotionType::kBishop << 14;
                    break;
                case 'n':
                    move |= from | (to << 6) | MoveFlags::kPromotion | PromotionType::kKnight << 14;
                    break;
                }
        }
        else if(from_bitboard & board_->whites_)
        {
            move = from | (to << 6);
        }
    }
    else
    {
        if((from_bitboard & FieldBitboard::kE8) && (to_bitboard & FieldBitboard::kG8) && (from_bitboard & board_->black_king_) )
        {
            if((*castling_rights_ & Castlings::kBlackCastling_0_0) && (board_->black_rooks_ & FieldBitboard::kH8) )
                move = from | (to << 6) | MoveFlags::kCastling;
        }
        else if((from_bitboard & FieldBitboard::kE8) && (to_bitboard & FieldBitboard::kC8) && (from_bitboard & board_->black_king_) )
        {
            if((*castling_rights_ & Castlings::kBlackCastling_0_0_0) && (board_->black_rooks_ & FieldBitboard::kA8) )
                move = from | (to << 6) | MoveFlags::kCastling;
        }
        else if(*en_passant_ && (from_bitboard & board_->black_pawns_) && (to_bitboard & *en_passant_))
        {
            if((to_bitboard & board_->empty_) && ((to_bitboard <<= kMoveForward) & board_->white_pawns_) )
                move = from | (to << 6) | MoveFlags::kEnPassant;
        }
        else if(move_string.size() == 5)
        {
            if((to_bitboard & kBottomRow) && (from_bitboard & board_->black_pawns_))
                switch(move_string[4])
                {
                case 'q':
                    move |= from | (to << 6) | MoveFlags::kPromotion | PromotionType::kQueen << 14;
                    break;
                case 'r':
                    move |= from | (to << 6) | MoveFlags::kPromotion | PromotionType::kRook << 14;
                    break;
                case 'b':
                    move |= from | (to << 6) | MoveFlags::kPromotion | PromotionType::kBishop << 14;
                    break;
                case 'n':
                    move |= from | (to << 6) | MoveFlags::kPromotion | PromotionType::kKnight << 14;
                    break;
                }
        }
        else if(from_bitboard & board_->blacks_)
        {
            move = from | (to << 6);
        }
    }

    return move;
}

void InitializeBishopMoves()
{
    std::array<uint64_t, 512> blocked;
    uint64_t mask;
    std::size_t count;

    for(std::size_t i = 0; i < kBitBoardSize; ++i)
    {
        blocked[0] = 0;

        mask = kBishopMasks[i];
        count = GetBitsCount(mask);

        std::size_t j = 0;

        for(; j < (static_cast<uint64_t>(1) << count) - 1; ++j)
        {
            blocked[j+1] = GetBitMaskNextPermutation(blocked[j], mask);
        }

        for(std::size_t l  = 0; l <= j; ++l)
        {
            uint64_t a = blocked[l];
            uint64_t s, from = GetBitSet(i);
            uint64_t moves = 0;

            s = from;

            do
            {
                s <<= 7;
                s &= kEmptyRight;

                moves |= s;

                if(!(s & ~a))
                    break;
            }

            while(true);

            s = from;

            do
            {
                s <<= 9;
                s &= kEmptyLeft;

                moves |= s;

                if(!(s & ~a))
                    break;
            }

            while(true);

            s = from;

            do
            {
                s >>= 9;
                s &= kEmptyRight;

                moves |= s;

                if(!(s & ~a))
                    break;
            }

            while(true);

            s = from;

            do
            {
                s >>= 7;
                s &= kEmptyLeft;

                moves |= s;

                if(!(s & ~a))
                    break;
            }

            while(true);

            bishop_moves[i][(a * kBishopMagics[i]) >> (64 - count)] = moves;
        }
    }
}

void InitializeRookMoves()
{
    std::array<uint64_t, 4096> blocked;
    uint64_t mask;
    std::size_t count;

    for(std::size_t i = 0; i < kBitBoardSize; ++i)
    {
        blocked[0] = 0;

        mask = kRookMasks[i];
        count = GetBitsCount(mask);

        std::size_t j = 0;

        for(; j < (static_cast<uint64_t>(1) << count) - 1; ++j)
        {
            blocked[j + 1] = GetBitMaskNextPermutation(blocked[j], mask);
        }

        for(std::size_t l  = 0; l <= j; ++l)
        {
            uint64_t a = blocked[l];
            uint64_t s, from = GetBitSet(i);
            uint64_t moves = 0;

            s = from;

            do
            {
                s >>= 1;
                s &= kEmptyRight;

                moves |= s;

                if(!(s & ~a))
                    break;
            }

            while(true);

            s = from;

            do
            {

                s <<= 1;
                s &= kEmptyLeft;

                moves |= s;

                if(!(s & ~a))
                    break;
            }

            while(true);

            s = from;

            do
            {
                s <<= 8;

                moves |= s;

                if(!(s & ~a))
                    break;
            }

            while(true);

            s = from;

            do
            {
                s >>= 8;

                moves |= s;

                if(!(s & ~a))
                    break;
            }

            while(true);

            rook_moves[i][(a * kRookMagics[i]) >> (64 - count)] = moves;
        }
    }
}
