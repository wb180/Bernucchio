#include "bits_functions.h"
#include "constants.h"
#include "logger.h"

#include <algorithm>
#include <bitset>
#include <iostream>

Logger::Logger() : output_(&std::cout), type_(LoggerType::kCout)
{

}

Logger::Logger(std::string file_name) : output_(nullptr), type_(LoggerType::kFile)
{
    file_stream = std::make_unique<std::ofstream>();

    if(file_stream)
        file_stream->open(file_name);

    if( !file_stream || !file_stream->is_open() )
    {
        std::cerr << "Could not open the file: " << file_name << std::endl;
        quick_exit (EXIT_FAILURE);
    }
    else
        output_ = file_stream.get();
}

Logger::~Logger()
{
    if(type_ == LoggerType::kFile)
    {
        file_stream->close();

        if(file_stream->is_open())
        {
            std::cerr << "Could not close the file" << std::endl;
            quick_exit (EXIT_FAILURE);
        }
    }
}

Logger &Logger::operator<<(const uint64_t &bit_board)
{
    std::bitset<64> binary_number(bit_board);

    for(std::size_t row = 0; row < kBoardSize; ++row)
    {
        *output_ << kBoardSize - row << " | ";

        for(std::size_t column = 0; column < kBoardSize; ++column)
        {
            *output_ << ' ' << binary_number[(kBoardSize - 1 - row) * kBoardSize + column];
        }

        *output_ << std::endl;
    }

    *output_ << "- - - - - - - - - - " << std::endl;
    *output_ << "     a b c d e f g h" << std::endl << std::endl;

    return *this;
}

Logger &Logger::operator<<(const Board &board)
{
    std::array<char, kBoardSize*kBoardSize> board_pieces;

    std::transform(std::begin(board_pieces), std::end(board_pieces), std::begin(board_pieces), [board, idx = static_cast<std::size_t>(0)](const auto &) mutable
    {
        ++idx;

        return board.GetPieceSymbolAtPosition(idx);
    } );

    for(std::size_t row = 0; row < kBoardSize; ++row)
    {
        *output_ << kBoardSize - row << " | ";

        for(std::size_t column = 0; column < kBoardSize; ++column)
        {
            *output_ << ' ' << board_pieces[(kBoardSize - 1 - row) * kBoardSize + column];
        }

        *output_ << std::endl;
    }

    *output_ << "- - - - - - - - - - " << std::endl;
    *output_ << "     a b c d e f g h" << std::endl << std::endl;

    return *this;
}

Logger &Logger::operator<<(MoveList &move_list)
{
    std::string move_string;
    uint64_t from, to;
    uint64_t *move;

    std::size_t idx = 1;

    while((move = move_list.GetNextMove()))
    {
        move_string.clear();

        from = *move & MoveMasks::kFrom;
        to = (*move & MoveMasks::kTo) >> 6;

        move_string.append(1, char('a' + from % kBoardSize));
        move_string.append(1, char('1' + from / kBoardSize));

        move_string.append(1, char('a' + to % kBoardSize));
        move_string.append(1, char('1' + to / kBoardSize));

        if((*move & MoveMasks::kFlag) == MoveFlags::kPromotion)
        {
            switch(*move & MoveMasks::kPromote)
            {
            case PromotionType::kQueen:
                move_string.append(1, char('q'));
                break;
            case PromotionType::kRook:
                move_string.append(1, char('r'));
                break;
            case PromotionType::kBishop:
                move_string.append(1, char('b'));
                break;
            case PromotionType::kKnight:
                move_string.append(1, char('n'));
                break;
            }
        }

        *output_ << idx++ << ": " << move_string << std::endl;
    }

    return *this;
}
