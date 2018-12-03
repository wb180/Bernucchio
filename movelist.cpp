#include "bits_functions.h"
#include "constants.h"
#include "movelist.h"

#include "logger.h"

#include <algorithm>

MoveList::MoveList()
{
    current_move_ = last_move_ = &moves_[0];
}

void MoveList::AddMoves(std::size_t from, uint64_t bit_board)
{
    while(bit_board)
    {
        last_move_->move_ = from | (GetLSBPos(bit_board) << 6);
        ++last_move_;
        bit_board &= bit_board - 1;
    }
}

void MoveList::AddPawnMoves(bool side, uint64_t bit_board, PawnMoveType move_type)
{
    uint64_t to, from = 0;

    if(side)
    {
        while(bit_board)
        {
            to = GetLSBPos(bit_board);

            switch(move_type)
            {
            case PawnMoveType::kPush:
                from = to - kBoardSize;
                break;

            case PawnMoveType::kRightAttack:
            case PawnMoveType::kEnPassantRight:
                from = to - kMoveLeft;
                break;

            case PawnMoveType::kLeftAttack:
            case PawnMoveType::kEnPassantLeft:
                from = to - kMoveRight;
                break;

            case PawnMoveType::kDoublePush:
                from = to - 2 * kBoardSize;
                break;
            }

            last_move_->move_ = from | (to << 6);

            if(move_type == PawnMoveType::kEnPassantLeft || move_type == PawnMoveType::kEnPassantRight)
                last_move_->move_ |= MoveFlags::kEnPassant;

            ++last_move_;
            bit_board &= bit_board - 1;
        }
    }
    else
    {
        while(bit_board)
        {
            to = GetLSBPos(bit_board);

            switch(move_type)
            {
            case PawnMoveType::kPush:
                from = to + kBoardSize;
                break;

            case PawnMoveType::kRightAttack:
            case PawnMoveType::kEnPassantRight:
                from = to + kMoveLeft;
                break;

            case PawnMoveType::kLeftAttack:
            case PawnMoveType::kEnPassantLeft:
                from = to + kMoveRight;
                break;

            case PawnMoveType::kDoublePush:
                from = to + 2 * kBoardSize;
                break;
            }

            last_move_->move_ = from | (to << 6);

            if(move_type == PawnMoveType::kEnPassantLeft || move_type == PawnMoveType::kEnPassantRight)
                last_move_->move_ |= MoveFlags::kEnPassant;

            ++last_move_;
            bit_board &= bit_board - 1;
        }
    }
}

void MoveList::AddPawnPromotions(bool side, uint64_t bit_board, PawnMoveType move_type)
{
    uint64_t to, from = 0;

    if(side)
    {
        while(bit_board)
        {
            to = GetLSBPos(bit_board);

            switch(move_type)
            {
            case PawnMoveType::kPush:
                from = to - kBoardSize;
                break;

            case PawnMoveType::kRightAttack:
                from = to - kMoveLeft;
                break;

            case PawnMoveType::kLeftAttack:
                from = to - kMoveRight;
                break;

            default:
                break;
            }

            for(std::size_t promote_to = PromotionType::kQueen; promote_to <= PromotionType::kKnight; ++promote_to)
            {
                last_move_->move_ = from | (to << 6) | MoveFlags::kPromotion | (promote_to << 14);
                ++last_move_;
            }

            bit_board &= bit_board - 1;
        }
    }
    else
    {
        while(bit_board)
        {
            to = GetLSBPos(bit_board);

            switch(move_type)
            {
            case PawnMoveType::kPush:
                from = to + kBoardSize;
                break;

            case PawnMoveType::kRightAttack:
                from = to + kMoveLeft;
                break;

            case PawnMoveType::kLeftAttack:
                from = to + kMoveRight;
                break;

            default:
                break;
            }

            for(std::size_t promote_to = PromotionType::kQueen; promote_to <= PromotionType::kKnight; ++promote_to)
            {
                last_move_->move_ = from | (to << 6) | MoveFlags::kPromotion | (promote_to << 14);
                ++last_move_;
            }

            bit_board &= bit_board - 1;
        }
    }
}

void MoveList::addMove(std::size_t from, std::size_t to, MoveFlags flag)
{
    last_move_->move_ = from | (to << 6) | flag;
    ++last_move_;
}

void MoveList::Reset()
{
    current_move_ = &moves_[0];
}

bool MoveList::Empty() const
{
    return current_move_ == last_move_;
}

void MoveList::Sort()
{
    std::stable_sort(current_move_, last_move_, [](const auto &a, const auto &b){return a.sort_ > b.sort_;});
}

void MoveList::UpdateSortValues(const Board *board)
{
    std::for_each(current_move_, last_move_, [&](auto &m){
        uint64_t to = (m.move_ & MoveMasks::kTo) >> 6;
        PieceType type = board->GetPieceOnSquare(to);
        std::size_t sort = 0;

        switch(type)
        {
        case PieceType::kWhiteQueens:
        case PieceType::kBlackQueens:
            sort += 900;
            break;

        case PieceType::kWhiteRooks:
        case PieceType::kBlackRooks:
            sort += 500;
            break;

        case PieceType::kWhiteBishops:
        case PieceType::kBlackBishops:
            sort += 340;
            break;

        case PieceType::kWhiteKnights:
        case PieceType::kBlackKnights:
            sort += 300;
            break;

        case PieceType::kWhitePawns:
        case PieceType::kBlackPawns:
            sort += 100;
            break;

        default:
            break;
        }

        if((m.move_ & MoveMasks::kFlag) == MoveFlags::kEnPassant)
            sort = 101;

        if((m.move_ & MoveMasks::kFlag) == MoveFlags::kPromotion)
            switch((m.move_ & MoveMasks::kPromote) >> 14)
            {
            case PromotionType::kQueen:
                sort += 800;
                break;

            case PromotionType::kRook:
                sort += 400;
                break;

            case PromotionType::kBishop:
                sort += 240;
                break;

            case PromotionType::kKnight:
                sort += 200;
                break;
            }

        m.sort_ = sort;
    });
}

Move *MoveList::GetNextMove()
{
    if(current_move_ == last_move_)
    {
        return nullptr;
    }

    return current_move_++;
}
