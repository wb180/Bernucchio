#ifndef MOVELIST_H
#define MOVELIST_H

#include "constants.h"
#include "moveinfo.h"

#include <array>

class MoveList
{
public:
    MoveList();

    std::size_t *GetNextMove();
    void AddMoves(std::size_t from, uint64_t bit_board);
    void AddPawnMoves(bool side, uint64_t bit_board, PawnMoveType move_type);
    void AddPawnPromotions(bool side, uint64_t bit_board, PawnMoveType move_type);
    void addMove(std::size_t from, std::size_t to, MoveFlags flag);
    void Reset();
    bool Empty() const;

private:
    std::array<std::size_t, 256> moves_;
    std::size_t *last_move_;
    std::size_t *current_move_;
};

#endif // MOVELIST_H
