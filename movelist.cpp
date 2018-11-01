#include "bits_functions.h"
#include "movelist.h"

MoveList::MoveList()
{

}

void MoveList::AddMoves(std::size_t from, uint64_t bit_board, std::size_t flag, std::size_t promote)
{
    while(bit_board)
    {
        *current_move_ = from | (GetLSBPos(bit_board) << 6);

        if(flag)
        {
            *current_move_ |= (flag << 14);

            if(promote)
                *current_move_ |= (promote << 12);
        }

        bit_board &= bit_board - 1;
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
