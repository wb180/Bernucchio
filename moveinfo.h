#ifndef MOVEINFO_H
#define MOVEINFO_H

#include "constants.h"

struct MoveInfo
{
public:
    MoveInfo() : old_castling_rights_(0), old_en_passant_(0), captured_(PieceType::KAllPieces)
    {

    }

    std::size_t old_castling_rights_;
    std::size_t old_en_passant_;
    PieceType captured_;
};

#endif // MOVEINFO_H
