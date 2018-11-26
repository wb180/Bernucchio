#include "bits_functions.h"
#include "constants.h"
#include "evaluator.h"

Evaluator::Evaluator(Board *board) : board_(board)
{

}

int Evaluator::Score()
{
    int score = 0;
    uint64_t pieces = board_->white_pawns_;

    while(pieces)
    {
        score += 100 + kPstWhitePawns[GetLSBPos(pieces)];
        pieces &= pieces - 1;
    }

    pieces = board_->black_pawns_;

    while(pieces)
    {
        score -= 100 + kPstBlackPawns[GetLSBPos(pieces)];
        pieces &= pieces - 1;
    }

    pieces = board_->white_knights_;

    while(pieces)
    {
        score += 300 + kPstWhiteKnights[GetLSBPos(pieces)];
        pieces &= pieces - 1;
    }

    pieces = board_->black_knights_;

    while(pieces)
    {
        score -= 300 + kPstBlackKnights[GetLSBPos(pieces)];
        pieces &= pieces - 1;
    }

    pieces = board_->white_bishops_;

    while(pieces)
    {
        score += 340 + kPstWhiteBishops[GetLSBPos(pieces)];
        pieces &= pieces - 1;
    }

    pieces = board_->black_bishops_;

    while(pieces)
    {
        score -= 340 + kPstBlackBishops[GetLSBPos(pieces)];
        pieces &= pieces - 1;
    }

    pieces = board_->white_rooks_;

    while(pieces)
    {
        score += 500 + kPstWhiteRooks[GetLSBPos(pieces)];
        pieces &= pieces - 1;
    }

    pieces = board_->black_rooks_;

    while(pieces)
    {
        score -= 500 + kPstBlackRooks[GetLSBPos(pieces)];
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
