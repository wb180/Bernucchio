#ifndef MOVELIST_H
#define MOVELIST_H

#include "move.h"

#include <array>

class MoveList
{
public:
    MoveList();

    Move *next_move();

private:
    std::array<Move, 256> moves_;
    Move *last_move_;
    Move *current_move_;
};

#endif // MOVELIST_H
