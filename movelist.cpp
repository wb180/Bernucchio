#include "movelist.h"

MoveList::MoveList()
{

}

Move *MoveList::next_move()
{
    if(current_move_ == last_move_)
    {
        return nullptr;
    }

    return current_move_++;
}
