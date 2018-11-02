#include "bits_functions.h"
#include "constants.h"
#include "moves.h"

Moves::Moves(Board *board) : board_(board)
{

}

void Moves::GetWhiteDoubleCheckEvasions()
{
    GetWhiteKingAttacks();
    GetWhiteKingMoves();
}

void Moves::GetWhiteKingAttacks()
{
    std::size_t king_position = GetLSBPos(board_->white_king_);
    uint64_t king_moves = kKingMoves[king_position] & board_->empty_ & ~kKingMoves[GetLSBPos(board_->black_king_)];
    move_list_.AddMoves(king_position, king_moves);
}

void Moves::GetWhiteKingMoves()
{
    std::size_t king_position = GetLSBPos(board_->white_king_);
    uint64_t king_attacks = kKingMoves[king_position] & board_->blacks_ & ~kKingMoves[GetLSBPos(board_->black_king_)];
    move_list_.AddMoves(king_position, king_attacks);
}

void Moves::GetWhiteCastlings(const std::size_t &castling_rights)
{
    if(castling_rights & Castlings::kWhiteCastling_0_0)
        move_list_.AddMoves(Squares::E1, GetBitSet(Squares::G1));

    if(castling_rights & Castlings::kWhiteCastling_0_0_0)
        move_list_.AddMoves(Squares::E1, GetBitSet(Squares::C1));
}

void Moves::GetWhiteAttacks()
{
    uint64_t pieces = board_->white_knights_;
    uint64_t from = 0;
    uint64_t attacks = 0;

    while(pieces)
    {
        from = GetLSBPos(pieces);
        attacks = kKnightMoves[from] & board_->blacks_;
        move_list_.AddMoves(from, attacks);
        pieces &= pieces - 1;
    }

    pieces = board_->white_bishops_;

    while(pieces)
    {
        from = GetLSBPos(pieces);
        attacks = kKnightMoves[from] & board_->blacks_;
        move_list_.AddMoves(from, attacks);
        pieces &= pieces - 1;
    }

    pieces = board_->white_rooks_;

    while(pieces)
    {
        from = GetLSBPos(pieces);
        attacks = kKnightMoves[from] & board_->blacks_;
        move_list_.AddMoves(from, attacks);
        pieces &= pieces - 1;
    }

    pieces = board_->white_pawns_;

    while(pieces)
    {
        attacks = (board_->white_pawns_ << kMoveLeft) & kEmptyRight & board_->blacks_;
        move_list_.AddPawnMoves(true, attacks & kEmptyBottom, PawnMoveType::kLeftAttack);
        move_list_.AddPawnPromotions(true, attacks & kBottomRow, PawnMoveType::kLeftAttack);

        attacks = (board_->white_pawns_ << kMoveRight) & kEmptyLeft & board_->blacks_;
        move_list_.AddPawnMoves(true, attacks & kEmptyBottom, PawnMoveType::kRightAttack);
        move_list_.AddPawnPromotions(true, attacks & kBottomRow, PawnMoveType::kRightAttack);

        pieces &= pieces - 1;
    }
}

/*void Moves::GetForWhite(const std::size_t castling_rights, const uint64_t &en_passant)
{
    /*std::size_t king_position = get_lsb(board_->white_king_);

        uint64_t attack = king_attack_from(from);

        bool side = !active_side;

        int count = pop_count(attack);

        switch(count)
        {
        case 0:
            generate_king_attacks(ml);
            generate_pawn_attacks(ml, board[PIECES | side]);
            generate_knight_attacks(ml, board[PIECES | side]);
            generate_bishop_attacks(ml, board[PIECES | side]);
            generate_rook_attacks(ml, board[PIECES | side]);

            generate_castlings(ml);
            generate_rook_moves(ml, board[EMPTY]);
            generate_bishop_moves(ml, board[EMPTY]);
            generate_knight_moves(ml, board[EMPTY]);
            generate_pawn_moves(ml, board[EMPTY]);
            generate_king_moves(ml);

            sort_captures(ml);
            break;
        case 1:
            if(attack & board[PAWN | side] || attack & board[KNIGHT | side])
            {
                generate_king_attacks(ml);
                generate_pawn_attacks(ml, attack);
                generate_knight_attacks(ml, attack);
                generate_bishop_attacks(ml, attack);
                generate_rook_attacks(ml, attack);

                generate_king_moves(ml);

                sort_captures(ml);
            }
            else if(attack & board[BISHOP | side])
            {
                int bishop_square = bit_scan_forward(attack);

                uint64_t beetween = bishop_moves[from][(((bishop_masks[from] & board[OCCUPIED]) * bishop_magics[from])
                                    >> (64 - pop_count(bishop_masks[from])))] & bishop_moves[bishop_square]
                        [(((bishop_masks[bishop_square] & board[OCCUPIED]) * bishop_magics[bishop_square])
                          >> (64 - pop_count(bishop_masks[bishop_square])))];

                generate_king_attacks(ml);
                generate_king_moves(ml);
                generate_pawn_attacks(ml, attack);
                generate_knight_attacks(ml, attack);
                generate_bishop_attacks(ml, attack);
                generate_rook_attacks(ml, attack);

                if(beetween)
                {
                    generate_rook_moves(ml, beetween);
                    generate_bishop_moves(ml, beetween);
                    generate_knight_moves(ml, beetween);
                    generate_pawn_moves(ml, beetween);
                }

                sort_captures(ml);
            }
            else if(attack & board[ROOK | side])
            {
                int rook_square = bit_scan_forward(attack);

                uint64_t beetween = rook_moves[from][(((rook_masks[from] & board[OCCUPIED]) * rook_magics[from])
                                    >> (64 - pop_count(rook_masks[from])))] & rook_moves[rook_square]
                        [(((rook_masks[rook_square] & board[OCCUPIED]) * rook_magics[rook_square])
                          >> (64 - pop_count(rook_masks[rook_square])))];

                generate_king_attacks(ml);
                generate_pawn_attacks(ml, attack);
                generate_knight_attacks(ml, attack);
                generate_bishop_attacks(ml, attack);
                generate_rook_attacks(ml, attack);

                generate_king_moves(ml);

                if(beetween)
                {
                    generate_rook_moves(ml, beetween);
                    generate_bishop_moves(ml, beetween);
                    generate_knight_moves(ml, beetween);
                    generate_pawn_moves(ml, beetween);
                }

                sort_captures(ml);
            }
            else
            {
                generate_king_attacks(ml);
                generate_pawn_attacks(ml, attack);
                generate_knight_attacks(ml, attack);
                generate_bishop_attacks(ml, attack);
                generate_rook_attacks(ml, attack);

                generate_king_moves(ml);

                int queen_square = bit_scan_forward(attack);

                uint64_t queen_attack = bishop_moves[from][(((bishop_masks[from] & board[OCCUPIED]) * bishop_magics[from])
                                                             >> (64 - pop_count(bishop_masks[from])))];
                uint64_t beetween;

                if(queen_attack & board[QUEEN | side])
                {

                    beetween = queen_attack & bishop_moves[queen_square]
                            [(((bishop_masks[queen_square] & board[OCCUPIED]) * bishop_magics[queen_square])
                            >> (64 - pop_count(bishop_masks[queen_square])))];

                    if(beetween)
                    {
                        generate_rook_moves(ml, beetween);
                        generate_bishop_moves(ml, beetween);
                        generate_knight_moves(ml, beetween);
                        generate_pawn_moves(ml, beetween);
                    }
                }

                queen_attack = rook_moves[from][(((rook_masks[from] & board[OCCUPIED]) * rook_magics[from])
                                                         >> (64 - pop_count(rook_masks[from])))];
                if(queen_attack & board[QUEEN | !active_side])
                {

                    beetween = queen_attack & rook_moves[queen_square]
                                  [(((rook_masks[queen_square] & board[OCCUPIED]) * rook_magics[queen_square])
                                    >> (64 - pop_count(rook_masks[queen_square])))];

                    if(beetween)
                    {
                        generate_rook_moves(ml, beetween);
                        generate_bishop_moves(ml, beetween);
                        generate_knight_moves(ml, beetween);
                        generate_pawn_moves(ml, beetween);
                    }
                }

                sort_captures(ml);
            }
            break;
        default:
            generate_king_attacks(ml);
            generate_king_moves(ml);

            sort_captures(ml);
        }*/
//}
