#include "bits_functions.h"
#include "board.h"
#include "constants.h"

#include <algorithm>
#include <bitset>
#include <iostream>

Board::Board()
{

}

uint64_t Board::get_bitboard(Board::BitboardType type) const
{
    uint64_t bit_board = 0;

    switch(type)
    {
    case BitboardType::kWhites:
        bit_board = whites_;
        break;
    case BitboardType::kWhitePawns:
        bit_board = white_pawns_;
        break;
    case BitboardType::kWhiteKnights:
        bit_board = white_knights_;
        break;
    case BitboardType::kWhiteBishops:
        bit_board = white_bishops_;
        break;
    case BitboardType::kWhiteRooks:
        bit_board = white_rooks_;
        break;
    case BitboardType::kWhiteQueens:
        bit_board = white_queens_;
        break;
    case BitboardType::kWhiteKing:
        bit_board = white_king_;
        break;
    case BitboardType::kBlacks:
        bit_board = blacks_;
        break;
    case BitboardType::kBlackPawns:
        bit_board = black_pawns_;
        break;
    case BitboardType::kBlackKnights:
        bit_board = black_knights_;
        break;
    case BitboardType::kBlackBishops:
        bit_board = black_bishops_;
        break;
    case BitboardType::kBlackRooks:
        bit_board = black_rooks_;
        break;
    case BitboardType::kBlackQueens:
        bit_board = black_queens_;
        break;
    case BitboardType::kBlackKing:
        bit_board = black_king_;
        break;
    case BitboardType::kOccupied:
        bit_board = occupied_;
        break;
    case BitboardType::kEmpty:
        bit_board = empty_;
        break;
    }

    return bit_board;
}

char Board::GetPieceSymbolAtPosition(std::size_t position) const
{
    return (white_pawns_ & get_bit_set(position) ? 'P' :
           (white_bishops_ & get_bit_set(position) ? 'B' :
           (white_knights_ & get_bit_set(position) ? 'N' :
           (white_rooks_ & get_bit_set(position) ? 'R' :
           (white_queens_ & get_bit_set(position) ? 'Q' :
           (white_king_ & get_bit_set(position) ? 'K' :
           (black_pawns_ & get_bit_set(position) ? 'p' :
           (black_bishops_ & get_bit_set(position) ? 'b' :
           (black_knights_ & get_bit_set(position) ? 'n' :
           (black_rooks_ & get_bit_set(position) ? 'r' :
           (black_queens_ & get_bit_set(position) ? 'q' :
           (black_king_ & get_bit_set(position) ? 'k' : 0))))))))))));
}

bool Board::SetFen(const std::string &fen_string)
{
    std::size_t row = 0;
    std::size_t column = 0;

    bool result = true;

    black_pawns_ = black_knights_  = black_bishops_ =
            black_rooks_  = black_queens_ = black_king_ = 0;
    white_pawns_ = white_knights_  = white_bishops_  =
            white_rooks_ = white_queens_ = white_king_ = 0;

    for(auto&& symbol : fen_string)
    {
        if(symbol >= '1' && symbol <= '8')
            column += static_cast<std::size_t>(symbol) - '1' + 1;
        else if(symbol == '/')
        {
            if(column == kBoardSize)
            {
                column = 0;
                ++row;
            }
            else
            {
                result = false;
                break;
            }

            if(row >= kBoardSize)
            {
                result = false;
                break;
            }
        }
        else if(symbol == ' ')
            break;
        else
        {
            std::size_t bit_number = (kBoardSize - 1 - row) * kBoardSize + column;

            switch(symbol)
            {
                case 'p': black_pawns_ |= get_bit_set(bit_number); break;
                case 'n': black_knights_ |= get_bit_set(bit_number); break;
                case 'b': black_bishops_ |= get_bit_set(bit_number); break;
                case 'r': black_rooks_ |= get_bit_set(bit_number); break;
                case 'q': black_queens_ |= get_bit_set(bit_number); break;
                case 'k': black_king_ |= get_bit_set(bit_number); break;
                case 'P': white_pawns_ |= get_bit_set(bit_number); break;
                case 'N': white_knights_ |= get_bit_set(bit_number); break;
                case 'B': white_bishops_ |= get_bit_set(bit_number); break;
                case 'R': white_rooks_ |= get_bit_set(bit_number); break;
                case 'Q': white_queens_ |= get_bit_set(bit_number); break;
                case 'K': white_king_ |= get_bit_set(bit_number); break;
                default : result = false; break;
            }

            if(result)
                ++column;
            else
                break;
        }
    }

    if(result)
    {
        whites_ = white_pawns_ | white_knights_ | white_bishops_ | white_rooks_ | white_queens_ | white_king_;
        blacks_ = black_pawns_ | black_knights_ | black_bishops_ | black_rooks_ | black_queens_ | black_king_;
        occupied_ = whites_ | blacks_;
        empty_ = ~occupied_;
    }

    return result;
}

std::string Board::GetFen() const
{
    std::string fen_string;

    std::bitset<64> binary_number(occupied_);
    std::size_t empty_in_row = 0;

    std::size_t row = 0;
    std::size_t column = 0;
    std::size_t bit_number = 0;

    while(row < kBoardSize)
    {
        bit_number = (kBoardSize - 1 - row) * kBoardSize + column;

        if(get_bit_set(bit_number) & occupied_)
        {
            if(empty_in_row)
                fen_string.append(std::to_string(empty_in_row));

            fen_string.append(1, GetPieceSymbolAtPosition(bit_number));
            empty_in_row = 0;
        }
        else
            ++empty_in_row;

        if(!((bit_number + 1) % kBoardSize))
        {
            if(empty_in_row)
            {
                fen_string.append(std::to_string(empty_in_row));
                empty_in_row = 0;
            }

            if(bit_number + 1 != kBoardSize)
                fen_string.append(1, '/');
        }

        if(++column == kBoardSize)
        {
            ++row;
            column = 0;
        }
    }

    return fen_string;
}

std::string Board::GetPositionFromBitBoard(const uint64_t &bb) const
{
    std::string position;
    std::size_t bit_number = get_lsb(bb);

    position.append(1, char('a' + bit_number % 8));
    position.append(1, char('1' + bit_number / 8));

    return position;
}

//std::string position_to_square(int pos)
//{
//    std::string square;

//    square.append(1, char('a' + pos % 8));
//    square.append(1, char('1' + pos / 8));

//    return square;
//}

//std::string Board::from_bitboard_to_square(uint64_t bb)
//{
//    int pos = get_lsb(bb);

//    return position_to_square(pos);
//}

//std::string Board::from_move_to_uci(const uint16_t *bb)
//{
//    std::string move;

//    move.append(position_to_square((*bb) >> 10));
//    move.append(position_to_square(((*bb) >> 4) & 0x3f));

//    if((*bb) & 8)
//    {
//        switch((*bb) & 3)
//        {
//            case 0: move.append(1, 'n'); break;
//            case 1: move.append(1, 'b'); break;
//            case 2: move.append(1, 'r'); break;
//            case 3: move.append(1, 'q'); break;
//        }
//    }

//    return move;
//}
