#include "bits_functions.h"
#include "constants.h"
#include "movelist.h"

#include "logger.h"

MoveList::MoveList()
{
    current_move_ = last_move_ = &moves_[0];
}

void MoveList::AddMoves(std::size_t from, uint64_t bit_board)
{
    while(bit_board)
    {
        *last_move_ = from | (GetLSBPos(bit_board) << 6);

        if(*last_move_ == 1502)
        {
            Logger lg;
            lg.PrintMove(*last_move_);
        }

        ++last_move_;
        bit_board &= bit_board - 1;
    }
}

void MoveList::AddPawnMoves(bool side, uint64_t bit_board, PawnMoveType move_type)
{
    uint64_t to, from;

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
                from = to - kMoveRight;
                break;

            case PawnMoveType::kLeftAttack:
            case PawnMoveType::kEnPassantLeft:
                from = to - kMoveLeft;
                break;

            case PawnMoveType::kDoublePush:
                from = to - 2 * kBoardSize;
                break;
            }

            *last_move_ = from | (to << 6);

            if(move_type == PawnMoveType::kEnPassantLeft || move_type == PawnMoveType::kEnPassantRight)
                *last_move_ |= MoveFlags::kEnPassant;

            if(*last_move_ == 1502)
            {
                Logger lg;
                lg.PrintMove(*last_move_);
            }

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

            *last_move_ = from | (to << 6);

            if(move_type == PawnMoveType::kEnPassantLeft || move_type == PawnMoveType::kEnPassantRight)
                *last_move_ |= MoveFlags::kEnPassant;

            if(*last_move_ == 1502)
            {
                Logger lg;
                lg.PrintMove(*last_move_);
            }

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

            case PawnMoveType::kDoublePush:
                from = to - 2 * kBoardSize;
                break;

            default:
                break;
            }

            for(std::size_t promote_to = PromotionType::kQueen; promote_to <= PromotionType::kBishop; ++promote_to)
            {
                *last_move_ = from | (to << 6) | (MoveFlags::kPromotion << 12) | (promote_to << 14);

                if(*last_move_ == 1502)
                {
                    Logger lg;
                    lg.PrintMove(*last_move_);
                }
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

            case PawnMoveType::kDoublePush:
                from = to + 2 * kBoardSize;
                break;

            default:
                break;
            }

            for(std::size_t promote_to = PromotionType::kQueen; promote_to <= PromotionType::kBishop; ++promote_to)
            {
                *last_move_ = from | (to << 6) | (MoveFlags::kPromotion << 12) | (promote_to << 14);

                if(*last_move_ == 1502)
                {
                    Logger lg;
                    lg.PrintMove(*last_move_);
                }
                ++last_move_;
            }

            bit_board &= bit_board - 1;
        }
    }
}

void MoveList::addMove(std::size_t from, std::size_t to, MoveFlags flag)
{
    *last_move_ = from | (to << 6) | (flag << 12);

    if(*last_move_ == 1502)
    {
        Logger lg;
        lg.PrintMove(*last_move_);
    }

    ++last_move_;
}

void MoveList::Reset()
{
    current_move_ = &moves_[0];
}

std::size_t *MoveList::GetNextMove()
{
    if(current_move_ == last_move_)
    {
        return nullptr;
    }

    return current_move_++;
}
