#include "bits_functions.h"
#include "constants.h"
#include "evaluator.h"

Evaluator::Evaluator(Board *board) : board_(board)
{

}

int Evaluator::Score()
{
    std::size_t total_pieces = GetBitsCount(board_->occupied_);

    if( total_pieces == 2)
        return 0;
    else if(total_pieces == 3)
    {
        if(GetBitsCount(board_->white_knights_) || GetBitsCount(board_->white_bishops_) ||
                GetBitsCount(board_->black_knights_) || GetBitsCount(board_->black_bishops_))
            return 0;
    }
    else if(total_pieces == 4)
    {
        if(GetBitsCount(board_->white_knights_) == 2 || GetBitsCount(board_->black_knights_ == 2))
         return 0;
    }

    int score = static_cast<int>(GetBitsCount(board_->white_pawns_)) * 100 + static_cast<int>(GetBitsCount(board_->white_knights_)) * 300 +
            static_cast<int>(GetBitsCount(board_->white_bishops_)) * 340 + static_cast<int>(GetBitsCount(board_->white_rooks_)) * 500 -
            static_cast<int>(GetBitsCount(board_->black_pawns_)) * 100 - static_cast<int>(GetBitsCount(board_->black_knights_)) * 300 -
            static_cast<int>(GetBitsCount(board_->black_bishops_)) * 340 - static_cast<int>(GetBitsCount(board_->black_rooks_)) * 500;

    uint64_t pieces = board_->white_pawns_;

    while(pieces)
    {
        score += kPstWhitePawns[GetLSBPos(pieces)];
        pieces &= pieces - 1;
    }

    pieces = board_->black_pawns_;

    while(pieces)
    {
        score -= kPstBlackPawns[GetLSBPos(pieces)];
        pieces &= pieces - 1;
    }

    pieces = board_->white_knights_;

    while(pieces)
    {
        score += kPstWhiteKnights[GetLSBPos(pieces)];
        pieces &= pieces - 1;
    }

    pieces = board_->black_knights_;

    while(pieces)
    {
        score -= kPstBlackKnights[GetLSBPos(pieces)];
        pieces &= pieces - 1;
    }

    pieces = board_->white_bishops_;

    while(pieces)
    {
        score += kPstWhiteBishops[GetLSBPos(pieces)];
        pieces &= pieces - 1;
    }

    pieces = board_->black_bishops_;

    while(pieces)
    {
        score -= kPstBlackBishops[GetLSBPos(pieces)];
        pieces &= pieces - 1;
    }

    pieces = board_->white_rooks_;

    while(pieces)
    {
        score += kPstWhiteRooks[GetLSBPos(pieces)];
        pieces &= pieces - 1;
    }

    pieces = board_->black_rooks_;

    while(pieces)
    {
        score -= kPstBlackRooks[GetLSBPos(pieces)];
        pieces &= pieces - 1;
    }

    pieces = board_->white_king_;

    while(pieces)
    {
        score += kPstWhiteKing[GetLSBPos(pieces)];
        pieces &= pieces - 1;
    }

    pieces = board_->black_king_;

    while(pieces)
    {
        score -= kPstBlackKing[GetLSBPos(pieces)];
        pieces &= pieces - 1;
    }

   return score;
}
