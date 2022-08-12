#include "bits_functions.h"
#include "constants.h"
#include "evaluator.h"
#include "logger.h"
#include "pst.h"

#include <iostream>
#include <sstream>

Evaluator::Evaluator(Board *board) : board_(board)
{

}

int Evaluator::Score()
{
    if(board_->IsKingSolo(Side::kBlack) || board_->IsKingSolo(Side::kWhite))
        return ScoreMate();

    int score = 0;

    int game_stage = 100 * board_->totalMaterial() / kEndGame;
    uint64_t pieces = board_->white_pawns_;

    while(pieces)
    {
        score += kPieceValue[0] + (((100 - game_stage) * kPstWhitePawnsEnd[GetLSBPos(pieces)] + game_stage * kPstWhitePawns[GetLSBPos(pieces)]))/100;
        pieces &= pieces - 1;
    }

    pieces = board_->black_pawns_;

    while(pieces)
    {
        score -= kPieceValue[0] + (((100 - game_stage) * kPstBlackPawnsEnd[GetLSBPos(pieces)] + game_stage * kPstBlackPawns[GetLSBPos(pieces)]))/100;
        pieces &= pieces - 1;
    }

    pieces = board_->white_knights_;

    while(pieces)
    {
        score += kPieceValue[1] + (((100 - game_stage) * kPstWhiteKnightsEnd[GetLSBPos(pieces)] + game_stage * kPstWhiteKnights[GetLSBPos(pieces)]))/100;
        pieces &= pieces - 1;
    }

    pieces = board_->black_knights_;

    while(pieces)
    {
        score -= kPieceValue[1] + (((100 - game_stage) * kPstBlackKnightsEnd[GetLSBPos(pieces)] + game_stage * kPstBlackKnights[GetLSBPos(pieces)]))/100;
        pieces &= pieces - 1;
    }

    pieces = board_->white_bishops_ & ~board_->white_rooks_;

    while(pieces)
    {
        score += kPieceValue[2] + (((100 - game_stage) * kPstWhiteBishopsEnd[GetLSBPos(pieces)] + game_stage * kPstWhiteBishops[GetLSBPos(pieces)]))/100;
        pieces &= pieces - 1;
    }

    pieces = board_->black_bishops_  & ~board_->black_rooks_;

    while(pieces)
    {
        score -= kPieceValue[2] + (((100 - game_stage) * kPstBlackBishopsEnd[GetLSBPos(pieces)] + game_stage * kPstBlackBishops[GetLSBPos(pieces)]))/100;
        pieces &= pieces - 1;
    }

    pieces = board_->white_rooks_ & ~board_->white_bishops_;

    while(pieces)
    {
        score += kPieceValue[3] + (((100 - game_stage) * kPstWhiteRooksEnd[GetLSBPos(pieces)] + game_stage * kPstWhiteRooks[GetLSBPos(pieces)]))/100;
        pieces &= pieces - 1;
    }

    pieces = board_->black_rooks_ & ~board_->black_bishops_;

    while(pieces)
    {
        score -= kPieceValue[3] + (((100 - game_stage) * kPstBlackRooksEnd[GetLSBPos(pieces)] + game_stage * kPstBlackRooks[GetLSBPos(pieces)]))/100;
        pieces &= pieces - 1;
    }

    pieces = board_->white_rooks_ & board_->white_bishops_;

    while(pieces)
    {
        score += kPieceValue[4] + (((100 - game_stage) * kPstWhiteQueensEnd[GetLSBPos(pieces)] + game_stage * kPstWhiteQueens[GetLSBPos(pieces)]))/100;
        pieces &= pieces - 1;
    }

    pieces = board_->black_rooks_ & board_->black_bishops_;

    while(pieces)
    {
        score -= kPieceValue[4] + (((100 - game_stage) * kPstBlackQueensEnd[GetLSBPos(pieces)] + game_stage * kPstBlackQueens[GetLSBPos(pieces)]))/100;
        pieces &= pieces - 1;
    }

    score += (((100 - game_stage) * kPstWhiteKingEnd[GetLSBPos(board_->white_king_)] + game_stage * kPstWhiteKing[GetLSBPos(board_->white_king_)]))/100;
    score -= (((100 - game_stage) * kPstBlackKingEnd[GetLSBPos(board_->black_king_)] + game_stage * kPstBlackKing[GetLSBPos(board_->black_king_)]))/100;

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

    int threshold = weights[389];
    int game_stage = 100 * board_->totalMaterial(weights) / threshold;

    uint64_t pieces = board_->white_pawns_;

    std::vector<int> whitePawnsPST(weights.begin() + 5, weights.begin() + 68);
    std::vector<int> blackPawnsPST = flipVector(whitePawnsPST);

    std::vector<int> whitePawnsPSTEnd(weights.begin() + 390, weights.begin() + 453);
    std::vector<int> blackPawnsPSTEnd = flipVector(whitePawnsPST);

    while(pieces)
    {
        score += weights[0] + (((100 - game_stage) * whitePawnsPSTEnd[GetLSBPos(pieces)] + game_stage * whitePawnsPST[GetLSBPos(pieces)]))/100;
        pieces &= pieces - 1;
    }

    pieces = board_->black_pawns_;

    while(pieces)
    {
        score -= weights[0] + (((100 - game_stage) * blackPawnsPSTEnd[GetLSBPos(pieces)] + game_stage * blackPawnsPST[GetLSBPos(pieces)]))/100;
        pieces &= pieces - 1;
    }

    std::vector<int> whiteKnightPST(weights.begin() + 69, weights.begin() + 132);
    std::vector<int> blackKnightPST = flipVector(whiteKnightPST);

    std::vector<int> whiteKnightPSTEnd(weights.begin() + 454, weights.begin() + 517);
    std::vector<int> blackKnightPSTEnd = flipVector(whiteKnightPST);

    pieces = board_->white_knights_;

    while(pieces)
    {
        score += weights[1] + (((100 - game_stage) * whiteKnightPSTEnd[GetLSBPos(pieces)] + game_stage * whiteKnightPST[GetLSBPos(pieces)]))/100;
        pieces &= pieces - 1;
    }

    pieces = board_->black_knights_;

    while(pieces)
    {
        score -= weights[1] + (((100 - game_stage) * blackKnightPSTEnd[GetLSBPos(pieces)] + game_stage * blackKnightPST[GetLSBPos(pieces)]))/100;
        pieces &= pieces - 1;
    }

    std::vector<int> whiteBishopPST(weights.begin() + 133, weights.begin() + 196);
    std::vector<int> blackBishopPST = flipVector(whiteBishopPST);

    std::vector<int> whiteBishopPSTEnd(weights.begin() + 518, weights.begin() + 581);
    std::vector<int> blackBishopPSTEnd = flipVector(whiteBishopPST);

    pieces = board_->white_bishops_ & ~board_->white_rooks_;

    while(pieces)
    {
        score += weights[2] + (((100 - game_stage) * whiteBishopPSTEnd[GetLSBPos(pieces)] + game_stage * whiteBishopPST[GetLSBPos(pieces)]))/100;
        pieces &= pieces - 1;
    }

    pieces = board_->black_bishops_  & ~board_->black_rooks_;

    while(pieces)
    {
        score -= weights[2] + (((100 - game_stage) * blackBishopPSTEnd[GetLSBPos(pieces)] + game_stage * blackBishopPST[GetLSBPos(pieces)]))/100;
        pieces &= pieces - 1;
    }

    std::vector<int> whiteRookPST(weights.begin() + 197, weights.begin() + 260);
    std::vector<int> blackRookPST = flipVector(whiteRookPST);

    std::vector<int> whiteRookPSTEnd(weights.begin() + 582, weights.begin() + 645);
    std::vector<int> blackRookPSTEnd = flipVector(whiteRookPST);

    pieces = board_->white_rooks_ & ~board_->white_bishops_;

    while(pieces)
    {
        score += weights[3] + (((100 - game_stage) * whiteRookPSTEnd[GetLSBPos(pieces)] + game_stage * whiteRookPST[GetLSBPos(pieces)]))/100;
        pieces &= pieces - 1;
    }

    pieces = board_->black_rooks_ & ~board_->black_bishops_;

    while(pieces)
    {
        score -= weights[3] + (((100 - game_stage) * blackRookPSTEnd[GetLSBPos(pieces)] + game_stage * blackRookPST[GetLSBPos(pieces)]))/100;
        pieces &= pieces - 1;
    }

    std::vector<int> whiteQueenPST(weights.begin() + 261, weights.begin() + 324);
    std::vector<int> blackQueenPST = flipVector(whiteQueenPST);

    std::vector<int> whiteQueenPSTEnd(weights.begin() + 646, weights.begin() + 709);
    std::vector<int> blackQueenPSTEnd = flipVector(whiteQueenPST);

    pieces = board_->white_rooks_ & board_->white_bishops_;

    while(pieces)
    {
        score += weights[4] + (((100 - game_stage) * whiteQueenPSTEnd[GetLSBPos(pieces)] + game_stage * whiteQueenPST[GetLSBPos(pieces)]))/100;
        pieces &= pieces - 1;
    }

    pieces = board_->black_rooks_ & board_->black_bishops_;

    while(pieces)
    {
        score -= weights[4] + (((100 - game_stage) * blackQueenPSTEnd[GetLSBPos(pieces)] + game_stage * blackQueenPST[GetLSBPos(pieces)]))/100;
        pieces &= pieces - 1;
    }

    std::vector<int> whiteKingPST(weights.begin() + 325, weights.begin() + 388);
    std::vector<int> blackKingPST = flipVector(whiteKingPST);

    std::vector<int> whiteKingPSTEnd(weights.begin() + 710, weights.begin() + 773);
    std::vector<int> blackKingPSTEnd = flipVector(whiteKingPST);

    pieces = board_->white_king_;

    while(pieces)
    {
        score += (((100 - game_stage) * whiteKingPSTEnd[GetLSBPos(pieces)] + game_stage * whiteKingPST[GetLSBPos(pieces)]))/100;
        pieces &= pieces - 1;
    }

    pieces = board_->black_king_;

    while(pieces)
    {
        score -= (((100 - game_stage) * blackKingPSTEnd[GetLSBPos(pieces)] + game_stage * blackKingPST[GetLSBPos(pieces)]))/100;
        pieces &= pieces - 1;
    }

    return score;
}

int Evaluator::ScoreMate()
{
    int score = 0;
    uint64_t pieces = board_->white_pawns_;

    while(pieces)
    {
        score += kPieceValue[0];
        pieces &= pieces - 1;
    }

    pieces = board_->black_pawns_;

    while(pieces)
    {
        score -= kPieceValue[0];
        pieces &= pieces - 1;
    }

    pieces = board_->white_knights_;

    while(pieces)
    {
        score += kPieceValue[1];
        pieces &= pieces - 1;
    }

    pieces = board_->black_knights_;

    while(pieces)
    {
        score -= kPieceValue[1];
        pieces &= pieces - 1;
    }

    pieces = board_->white_bishops_ & ~board_->white_rooks_;

    while(pieces)
    {
        score += kPieceValue[2];
        pieces &= pieces - 1;
    }

    pieces = board_->black_bishops_  & ~board_->black_rooks_;

    while(pieces)
    {
        score -= kPieceValue[2];
        pieces &= pieces - 1;
    }

    pieces = board_->white_rooks_ & ~board_->white_bishops_;

    while(pieces)
    {
        score += kPieceValue[3];
        pieces &= pieces - 1;
    }

    pieces = board_->black_rooks_ & ~board_->black_bishops_;

    while(pieces)
    {
        score -= kPieceValue[3];
        pieces &= pieces - 1;
    }

    pieces = board_->white_rooks_ & board_->white_bishops_;

    while(pieces)
    {
        score += kPieceValue[4];
        pieces &= pieces - 1;
    }

    pieces = board_->black_rooks_ & board_->black_bishops_;

    while(pieces)
    {
        score -= kPieceValue[4];
        pieces &= pieces - 1;
    }

    if(board_->IsKingSolo(Side::kBlack))
    {
        //int square = GetLSBPos(board_->black_king_);
        score += kKingMate[GetLSBPos(board_->white_king_)][GetLSBPos(board_->black_king_)];
        score -= kKingMated[GetLSBPos(board_->black_king_)];

//        pieces = board_->white_knights_ & board_->white_bishops_ & board_->white_rooks_;

//        while(pieces)
//        {
//            score += kKingMate[GetLSBPos(pieces)][GetLSBPos(board_->black_king_)];
//            pieces &= pieces - 1;
//        }
    }
    else
    {
        score -= kKingMate[GetLSBPos(board_->black_king_)][GetLSBPos(board_->white_king_)];
        score += kKingMated[GetLSBPos(board_->white_king_)];

//        pieces = board_->black_knights_ & board_->black_bishops_ & board_->black_rooks_;

//        while(pieces)
//        {
//            score -= kKingMate[GetLSBPos(pieces)][GetLSBPos(board_->white_king_)];
//            pieces &= pieces - 1;
//        }
    }

//    Logger::GetInstance() << *board_;
//    std::stringstream ss;
//    ss << score;
//    Logger::GetInstance() << ss.str();

    return score;
}
