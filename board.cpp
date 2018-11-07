#include "bits_functions.h"
#include "board.h"
#include "constants.h"
#include "logger.h"

#include <algorithm>
#include <bitset>
#include <iostream>

Board::Board()
{

}

char Board::GetPieceSymbolAtPosition(std::size_t position) const
{
    return (white_pawns_ & GetBitSet(position) ? 'P' :
           (white_rooks_ & white_bishops_ & GetBitSet(position) ? 'Q' :
           (white_bishops_ & GetBitSet(position) ? 'B' :
           (white_knights_ & GetBitSet(position) ? 'N' :
           (white_rooks_ & GetBitSet(position) ? 'R' :
           (white_king_ & GetBitSet(position) ? 'K' :
           (black_pawns_ & GetBitSet(position) ? 'p' :
           (black_rooks_ & black_bishops_ & GetBitSet(position) ? 'q' :
           (black_bishops_ & GetBitSet(position) ? 'b' :
           (black_knights_ & GetBitSet(position) ? 'n' :
           (black_rooks_ & GetBitSet(position) ? 'r' :
           (black_king_ & GetBitSet(position) ? 'k' : 0))))))))))));
}

bool Board::SetFen(const std::string &fen_string)
{
    std::size_t row = 0;
    std::size_t column = 0;

    bool result = true;

    black_pawns_ = black_knights_  = black_bishops_ =
            black_rooks_  = black_king_ = 0;
    white_pawns_ = white_knights_  = white_bishops_  =
            white_rooks_ = white_king_ = 0;

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
                case 'p': black_pawns_ |= GetBitSet(bit_number); break;
                case 'n': black_knights_ |= GetBitSet(bit_number); break;
                case 'b': black_bishops_ |= GetBitSet(bit_number); break;
                case 'r': black_rooks_ |= GetBitSet(bit_number); break;
                case 'q': black_rooks_ |= GetBitSet(bit_number);
                          black_bishops_ |= GetBitSet(bit_number); break;
                case 'k': black_king_ |= GetBitSet(bit_number); break;
                case 'P': white_pawns_ |= GetBitSet(bit_number); break;
                case 'N': white_knights_ |= GetBitSet(bit_number); break;
                case 'B': white_bishops_ |= GetBitSet(bit_number); break;
                case 'R': white_rooks_ |= GetBitSet(bit_number); break;
                case 'Q': white_rooks_ |= GetBitSet(bit_number);
                          white_bishops_ |= GetBitSet(bit_number);break;
                case 'K': white_king_ |= GetBitSet(bit_number); break;
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
        whites_ = white_pawns_ | white_knights_ | white_bishops_ | white_rooks_ | white_king_;
        blacks_ = black_pawns_ | black_knights_ | black_bishops_ | black_rooks_ | black_king_;
        occupied_ = whites_ | blacks_;
        empty_ = ~occupied_;
    }

    std::cout << black_pawns_ << std::endl;

    Logger lg;
    lg << black_pawns_;

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

        if(GetBitSet(bit_number) & occupied_)
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
    std::size_t bit_number = GetLSBPos(bb);

    position.append(1, char('a' + bit_number % 8));
    position.append(1, char('1' + bit_number / 8));

    return position;
}
