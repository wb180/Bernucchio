#include "bits_functions.h"
#include "constants.h"
#include "gamestate.h"

GameState::GameState() : moves_(&board_)
{

}

bool GameState::SetFen(const std::string &fen_string)
{
    castling_O_O_ = false;
    castling_O_O_O_ = false;
    castling_o_o_ = false;
    castling_o_o_o_ = false;

    en_passant_ = 0;

    fifty_moves_counter_ = 0;
    full_moves_counter_ = 0;

    bool result = board_.SetFen(fen_string);

    if(result)
    {
        std::size_t position = fen_string.find(' ');
        if(position != std::string::npos && ++position != fen_string.size())
        {
            if(fen_string.at(position) == 'w')
                active_side_ = true;
            else if(fen_string.at(position) == 'b')
                active_side_ = false;
            else
                result = false;

             if(++position == fen_string.size() || fen_string.at(position) != ' ')
                 result = false;

            while(result && ++position != fen_string.size() && fen_string.at(position) != ' ')
            {
                switch(fen_string.at(position))
                {
                    case 'K': castling_O_O_ = true; break;
                    case 'Q': castling_O_O_O_ = true; break;
                    case 'k': castling_o_o_ = true; break;
                    case 'q': castling_o_o_o_ = true; break;

                    case '-': break;
                    default: result = false; break;
                }
            }

            if(result && fen_string.at(position) == ' ' && ++position != fen_string.size())
            {
                if(fen_string.at(position) >= 'a' && fen_string.at(position) <= 'h')
                {
                    std::size_t row = static_cast<std::size_t>(fen_string.at(position)) - 'a';

                    if( ++position != fen_string.size() && (fen_string.at(position) == '3' || fen_string.at(position) == '6'))
                    {
                        std::size_t column = static_cast<std::size_t>(fen_string.at(position)) - '1';

                        en_passant_ = get_bit_set(kBoardSize * column + row);
                    }
                    else
                        result = false;
                }
                else if(position == fen_string.size() || fen_string.at(position) != '-')
                    result = false;
            }

            if(result && ++position != fen_string.size() && fen_string.at(position) != ' ')
                result = false;
            else if(result && position >= fen_string.size())
                return true;

            if(result)
            {
                while(result && ++position != fen_string.size() && fen_string.at(position) != ' ')
                {
                    if(fen_string.at(position) >= '0' && fen_string.at(position) <= '9')
                    {
                        fifty_moves_counter_ *= 10;
                        fifty_moves_counter_ += static_cast<std::size_t>(fen_string.at(position)) - '0';
                    }
                    else
                        result = false;
                }
            }

            if(result && position != fen_string.size())
            {
                while(result && ++position != fen_string.size())
                {
                    if(fen_string.at(position) >= '0' && fen_string.at(position) <= '9')
                    {
                        full_moves_counter_ *= 10;
                        full_moves_counter_ += static_cast<std::size_t>(fen_string.at(position)) - '0';
                    }
                    else
                        result = false;
                }
            }
            else
                result = false;
        }
        else
            result = false;
    }

    return result;
}

std::string GameState::GetFen() const
{
    std::string fen_string = board_.GetFen();
    fen_string.append(1, ' ').append(1, active_side_ ? 'w' : 'b').append(1, ' ');

    std::string castling;
    if(castling_O_O_)
        castling.append(1, 'K');
    if(castling_O_O_O_)
        castling.append(1, 'Q');
    if(castling_o_o_)
        castling.append(1, 'k');
    if(castling_o_o_o_)
        castling.append(1, 'q');

    if(castling.empty())
        fen_string.append("- ");
    else
        fen_string.append(castling).append(1, ' ');

    if(en_passant_)
        fen_string.append(board_.GetPositionFromBitBoard(en_passant_)).append(1, ' ');
    else
        fen_string.append("- ");

    fen_string.append(std::to_string(fifty_moves_counter_)).append(1, ' ');
    fen_string.append(std::to_string(full_moves_counter_));

    return fen_string;
}
