#ifndef MOVELIST_H
#define MOVELIST_H

#include "board.h"
#include "constants.h"
#include "move.h"
#include "moveinfo.h"

#include <array>

class MoveList
{
public:
    MoveList();

    Move *GetNextMove();
    void AddMoves(std::size_t from, uint64_t bit_board);
    void AddPawnMoves(bool side, uint64_t bit_board, PawnMoveType move_type);
    void AddPawnPromotions(bool side, uint64_t bit_board, PawnMoveType move_type);
    void addMove(std::size_t from, std::size_t to, MoveFlags flag);
    void Reset();
    bool Empty() const;
    void Sort();
    void UpdateSortValues(const Board* board);

private:
    std::array<Move, 256> moves_;
    Move *last_move_;
    Move *current_move_;
};

#endif // MOVELIST_H
