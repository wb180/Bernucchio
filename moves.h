#ifndef MOVES_H
#define MOVES_H

#include "board.h"
#include "movelist.h"

class Moves
{
public:
    explicit Moves(Board *board);

    void GetWhiteSingleCheckEvasions();
    void GetWhiteDoubleCheckEvasions();
    void GetWhiteCaptures();
    void GetWhiteQuiet();
    void GetWhiteChecks();

    void GetWhiteKingAttacks();
    void GetWhiteKingMoves();
    void GetWhiteCastlings(const std::size_t &castling_rights);
    void GetWhiteAttacks();
    void GetWhiteMoves();

private:
    Board *board_;
    MoveList move_list_;
};

#endif // MOVES_H
