#ifndef GAMESTATE_H
#define GAMESTATE_H

#include "board.h"
#include "moves.h"

#include <cstdint>

class GameState
{
public:
    GameState();

    bool SetFen(const std::string &fen_string);
    std::string GetFen() const;

private:
    Board board_;
    Moves moves_;

    bool active_side_;

    bool castling_O_O_;
    bool castling_O_O_O_;
    bool castling_o_o_;
    bool castling_o_o_o_;

    uint64_t en_passant_;
    std::size_t fifty_moves_counter_;
    std::size_t full_moves_counter_;
};

#endif // GAMESTATE_H
