#include "bits_functions.h"
#include "moves.h"

Moves::Moves(Board *board) : board_(board)
{

}

void Moves::GetForWhite(const bool &castling_O_O, const bool &castling_O_O_O, const uint64_t &en_passant)
{
    std::size_t king_position = get_lsb(board_->white_king_);

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
        }
}
