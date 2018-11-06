#ifndef MOVES_H
#define MOVES_H

#include "board.h"
#include "movelist.h"

#include <array>

class Moves
{
public:
    explicit Moves(Board *board, uint64_t *en_passant, std::size_t *castling_rights, Side *side);

    void GetWhiteKingAttacks();
    void GetWhiteKingMoves();
    void GetWhiteAttacksAndPromotions();
    void GetWhiteMoves();

    void GetBlackKingAttacks();
    void GetBlackKingMoves();
    void GetBlackAttacksAndPromotions();
    void GetBlackMoves();

    bool MakeMove(std::size_t move);
    bool UnmakeMove(std::size_t move = 0);

    MoveList move_list_;

private:
    Board *board_;
    uint64_t *en_passant_;
    std::size_t *castling_rights_;
    Side *active_side_;
    std::array<MoveInfo, 512> move_infos;
    MoveInfo *last_move_;

    bool IsSquareAttacked(std::size_t square);
};

void InitializeBishopMoves();
void InitializeRookMoves();

#endif // MOVES_H
