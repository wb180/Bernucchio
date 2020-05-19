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
        score += kPieceValue[0] + kPstWhitePawns[GetLSBPos(pieces)];
        pieces &= pieces - 1;
    }

    pieces = board_->black_pawns_;

    while(pieces)
    {
        score -= kPieceValue[0] + kPstBlackPawns[GetLSBPos(pieces)];
        pieces &= pieces - 1;
    }

    pieces = board_->white_knights_;

    while(pieces)
    {
        score += kPieceValue[1] + kPstWhiteKnights[GetLSBPos(pieces)];
        pieces &= pieces - 1;
    }

    pieces = board_->black_knights_;

    while(pieces)
    {
        score -= kPieceValue[1] + kPstBlackKnights[GetLSBPos(pieces)];
        pieces &= pieces - 1;
    }

    pieces = board_->white_bishops_ & ~board_->white_rooks_;

    while(pieces)
    {
        score += kPieceValue[2] + kPstWhiteBishops[GetLSBPos(pieces)];
        pieces &= pieces - 1;
    }

    pieces = board_->black_bishops_  & ~board_->black_rooks_;

    while(pieces)
    {
        score -= kPieceValue[2] + kPstBlackBishops[GetLSBPos(pieces)];
        pieces &= pieces - 1;
    }

    pieces = board_->white_rooks_ & ~board_->white_bishops_;

    while(pieces)
    {
        score += kPieceValue[3] + kPstWhiteRooks[GetLSBPos(pieces)];
        pieces &= pieces - 1;
    }

    pieces = board_->black_rooks_ & ~board_->black_bishops_;

    while(pieces)
    {
        score -= kPieceValue[3] + kPstBlackRooks[GetLSBPos(pieces)];
        pieces &= pieces - 1;
    }

    pieces = board_->white_rooks_ & board_->white_bishops_;

    while(pieces)
    {
        score += kPieceValue[4] + kPstWhiteQueens[GetLSBPos(pieces)];
        pieces &= pieces - 1;
    }

    pieces = board_->black_rooks_ & board_->black_bishops_;

    while(pieces)
    {
        score -= kPieceValue[4] + kPstBlackQueens[GetLSBPos(pieces)];
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

std::vector<int> flipVector(std::vector<int> vec)
{
    std::vector<int> result;
    result.resize(64);

    for(int i = 0; i < 8; ++i)
        for(int j = 0; j < 8; ++j)
        {
            result[i * 8 + j] = vec[(7 - i) * 8 + j];
        }

    return result;
}

int Evaluator::Score(std::vector<int> &weights)
{
    int score = 0;
    uint64_t pieces = board_->white_pawns_;

    std::vector<int> whitePawnsPST(weights.begin() + 5, weights.begin() + 68);
    std::vector<int> blackPawnsPST = flipVector(whitePawnsPST);

    while(pieces)
    {
        score += weights[0] + whitePawnsPST[GetLSBPos(pieces)];
        pieces &= pieces - 1;
    }

    pieces = board_->black_pawns_;

    while(pieces)
    {
        score -= weights[0] + blackPawnsPST[GetLSBPos(pieces)];
        pieces &= pieces - 1;
    }

    std::vector<int> whiteKnightPST(weights.begin() + 69, weights.begin() + 132);
    std::vector<int> blackKnightPST = flipVector(whiteKnightPST);

    pieces = board_->white_knights_;

    while(pieces)
    {
        score += weights[1] + whiteKnightPST[GetLSBPos(pieces)];
        pieces &= pieces - 1;
    }

    pieces = board_->black_knights_;

    while(pieces)
    {
        score -= weights[1] + blackKnightPST[GetLSBPos(pieces)];
        pieces &= pieces - 1;
    }

    std::vector<int> whiteBishopPST(weights.begin() + 133, weights.begin() + 196);
    std::vector<int> blackBishopPST = flipVector(whiteBishopPST);

    pieces = board_->white_bishops_ & ~board_->white_rooks_;

    while(pieces)
    {
        score += weights[2] + whiteBishopPST[GetLSBPos(pieces)];
        pieces &= pieces - 1;
    }

    pieces = board_->black_bishops_  & ~board_->black_rooks_;

    while(pieces)
    {
        score -= weights[2] + blackBishopPST[GetLSBPos(pieces)];
        pieces &= pieces - 1;
    }

    std::vector<int> whiteRookPST(weights.begin() + 197, weights.begin() + 260);
    std::vector<int> blackRookPST = flipVector(whiteRookPST);

    pieces = board_->white_rooks_ & ~board_->white_bishops_;

    while(pieces)
    {
        score += weights[3] + whiteRookPST[GetLSBPos(pieces)];
        pieces &= pieces - 1;
    }

    pieces = board_->black_rooks_ & ~board_->black_bishops_;

    while(pieces)
    {
        score -= weights[3] + blackRookPST[GetLSBPos(pieces)];
        pieces &= pieces - 1;
    }

    std::vector<int> whiteQueenPST(weights.begin() + 261, weights.begin() + 324);
    std::vector<int> blackQueenPST = flipVector(whiteQueenPST);

    pieces = board_->white_rooks_ & board_->white_bishops_;

    while(pieces)
    {
        score += weights[4] + whiteQueenPST[GetLSBPos(pieces)];
        pieces &= pieces - 1;
    }

    pieces = board_->black_rooks_ & board_->black_bishops_;

    while(pieces)
    {
        score -= weights[4] + blackQueenPST[GetLSBPos(pieces)];
        pieces &= pieces - 1;
    }

    std::vector<int> whiteKingPST(weights.begin() + 325, weights.end());
    std::vector<int> blackKingPST = flipVector(whiteKingPST);

    pieces = board_->white_king_;

    while(pieces)
    {
        score += whiteKingPST[GetLSBPos(pieces)];
        pieces &= pieces - 1;
    }

    pieces = board_->black_king_;

    while(pieces)
    {
        score -= blackKingPST[GetLSBPos(pieces)];
        pieces &= pieces - 1;
    }

    return score;
}
