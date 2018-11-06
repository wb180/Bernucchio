#ifndef GAMESTATE_H
#define GAMESTATE_H

#include "board.h"
#include "hashlist.h"
#include "moves.h"

#include <cstdint>

class GameState
{
public:
    GameState();

    bool SetFen(const std::string &fen_string);
    std::string GetFen() const;

    uint64_t Perft(std::size_t depth);
    uint64_t SplitPerft(std::size_t depth);

private:
    Board board_;
    Moves moves_;
    HashList hashes_;

    std::array<MoveInfo, 512> move_infos;
    MoveInfo *last_move_;

    Side active_side_;

    std::size_t castlings_;

    uint64_t en_passant_;
    std::size_t fifty_moves_counter_;
    std::size_t full_moves_counter_;
};

#endif // GAMESTATE_H
