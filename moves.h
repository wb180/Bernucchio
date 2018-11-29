#ifndef MOVES_H
#define MOVES_H

#include "board.h"
#include "movelist.h"

#include <array>

class Moves
{
public:
    explicit Moves(Board *board, uint64_t *en_passant, std::size_t *castling_rights, Side *side, std::size_t *fifty_moves_counter);

    void GetWhiteKingAttacks(MoveList *move_list);
    void GetWhiteKingMoves(MoveList *move_list);
    void GetWhiteAttacksAndPromotions(MoveList *move_list, bool include_checks = true);
    void GetWhiteMoves(MoveList *move_list);

    void GetBlackKingAttacks(MoveList *move_list);
    void GetBlackKingMoves(MoveList *move_list);
    void GetBlackAttacksAndPromotions(MoveList *move_list, bool include_checks = true);
    void GetBlackMoves(MoveList *move_list);

    std::size_t GetMove(const std::string& move_string);

    bool MakeMove(std::size_t move);
    void UnmakeMove(std::size_t move);

    bool IsKingAttacked(Side side) const;

    void Reset();
    MoveInfo *GetLastMoveInfo() const;

private:
    Board *board_;
    uint64_t *en_passant_;
    std::size_t *castling_rights_;
    Side *active_side_;
    std::array<MoveInfo, 512> move_infos;
    MoveInfo *last_move_;
    std::size_t *fifty_moves_counter_;

    bool IsSquareAttacked(std::size_t square) const;
};

void InitializeBishopMoves();
void InitializeRookMoves();

#endif // MOVES_H
