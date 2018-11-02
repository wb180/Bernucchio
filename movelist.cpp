#include "bits_functions.h"
#include "constants.h"
#include "movelist.h"

MoveList::MoveList()
{

}

void MoveList::AddMoves(std::size_t from, uint64_t bit_board)
{
    while(bit_board)
    {
        *current_move_ = from | (GetLSBPos(bit_board) << 6);
        ++current_move_;
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
                from = to - kMoveLeft;
                break;

            case PawnMoveType::kLeftAttack:
                from = to - kMoveRight;
                break;

            case PawnMoveType::kDoublePush:
                from = to - 2 * kBoardSize;
                break;
            }

            *current_move_ = from | (to << 6);
            ++current_move_;
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
            }

            *current_move_ = from | (to << 6);
            ++current_move_;
            bit_board &= bit_board - 1;
        }
    }
}

void MoveList::AddPawnPromotions(bool side, uint64_t bit_board, PawnMoveType move_type)
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
                from = to - kMoveLeft;
                break;

            case PawnMoveType::kLeftAttack:
                from = to - kMoveRight;
                break;

            case PawnMoveType::kDoublePush:
                from = to - 2 * kBoardSize;
                break;
            }

            for(std::size_t promote_to = PromotionType::kQueen; promote_to <= PromotionType::kBishop; ++promote_to)
            {
                *current_move_ = from | (to << 6) | (MoveFlags::kPromotion << 12) | (promote_to << 14);
                ++current_move_;
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
            }

            for(std::size_t promote_to = PromotionType::kQueen; promote_to <= PromotionType::kBishop; ++promote_to)
            {
                *current_move_ = from | (to << 6) | (MoveFlags::kPromotion << 12) | (promote_to << 14);
                ++current_move_;
            }

            bit_board &= bit_board - 1;
        }
    }
}

std::size_t *MoveList::GetNextMove()
{
    if(current_move_ == last_move_)
    {
        return nullptr;
    }

    return current_move_++;
}
