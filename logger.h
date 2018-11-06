#ifndef LOGGER_H
#define LOGGER_H

#include "board.h"
#include "movelist.h"

#include <fstream>
#include <memory>

class Logger
{
public:
    enum class LoggerType
    {
        kCout,
        kFile
    };

    Logger();
    explicit Logger(std::string file_name);
    ~Logger();

    Logger& operator<< (const uint64_t& bit_board);
    Logger& operator<< (const Board& board);
    Logger& operator<< (MoveList &move_list);

private:
    std::ostream *output_;
    LoggerType type_;
    std::unique_ptr<std::ofstream> file_stream;
};

#endif // LOGGER_H
