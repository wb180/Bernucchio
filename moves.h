#ifndef MOVES_H
#define MOVES_H

#include "board.h"
#include "movelist.h"

class Moves
{
public:
    explicit Moves(Board *board);

    void GetForWhite(const bool &castling_O_O, const bool &castling_O_O_O, const uint64_t &en_passant);
    void GetForBlack(const bool &castling_o_o, const bool &castling_o_o_o, const uint64_t &en_passant);

private:
    Board *board_;
    MoveList move_list_;
};

#endif // MOVES_H
