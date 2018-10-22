#ifndef MOVE_H
#define MOVE_H

#include <stdint.h>

struct Move
{
public:
    Move();

    enum class MoveType
    {
        kQuiet,
        kCastling_0_0,
        kCastling_0_0_0,
        kCapture,
        kEnPassant,
        kPromotion
    };

    uint64_t from_;
    uint64_t to_;

    int promotion_;
    int captured_;

    MoveType type_;
};

#endif // MOVE_H
