#ifndef LOGGER_H
#define LOGGER_H

#include "board.h"
#include "movelist.h"

#include <fstream>
#include <memory>

class Logger
{
private:
    enum class LoggerType
    {
        kCout,
        kFile
    };

    explicit Logger(std::string file_name);

public:
    static Logger& GetInstance(std::string file_name = "")
    {
        static Logger logger{ file_name };
        return logger;
    }

    Logger(const Logger&) = delete;
    Logger(Logger&&) = delete;
    Logger& operator=(const Logger&) = delete;
    Logger& operator=(Logger&&) = delete;

    ~Logger();

    Logger& operator<< (const uint64_t& bit_board);
    Logger& operator<< (const Board& board);

    Logger& operator<< (const std::string& string);

    void PrintMove(std::size_t &move);

private:
    std::ostream *output_;
    LoggerType type_;
    std::unique_ptr<std::ofstream> file_stream;
};

#endif // LOGGER_H
