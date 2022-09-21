#include <charconv>
#include <iostream>
#include <sstream>

#include "search.h"
#include "timemanager.h"
#include "ucicommand.h"

const std::string_view kUci = "uci";
const std::string_view kQuit = "quit";
const std::string_view kIsReady = "isready";
const std::string_view kPosition = "position";
const std::string_view kStartPos = "startpos";
const std::string_view kFen = "fen";
const std::string_view kStartFen = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";
const std::string_view kMoves = "moves";
const std::string_view kGo = "go";
const std::string_view kWTime = "wtime";
const std::string_view kBTime = "btime";
const std::string_view kWInc = "winc";
const std::string_view kBInc = "binc";
const std::string_view kMovesToGo = "movestogo";
const std::string_view kInfinite = "infinite";
const std::string_view kStop = "stop";
const std::string_view kSpeedTest = "speedtest";
const std::string_view kMoveTime = "movetime";
const std::string_view kSetOption = "setoption";
const std::string_view kCTGBookPath = "CTGBookPath";

bool UCICommand::execute()
{
    return true;
}

std::shared_ptr<UCICommand> CommandFactory::CreateCommand(std::string_view commandString, UCI *uci)
{
    std::shared_ptr<UCICommand> command{nullptr};

    if(commandString.starts_with(kUci))
        command = std::make_shared<CommandUci>(uci);
    else if(commandString.starts_with(kQuit))
        command = std::make_shared<CommandQuit>();
    else if(commandString.starts_with(kIsReady))
        command = std::make_shared<CommandReady>();
    else if(commandString.starts_with(kStop))
        command = std::make_shared<CommandStop>();
    else if(commandString.starts_with(kPosition))
    {
        commandString.remove_prefix(kPosition.size());
        commandString = commandString.substr(std::min(commandString.find_first_not_of(' '), commandString.size()));
        command = std::make_shared<CommandPosition>(commandString);
    }
    else if(commandString.starts_with(kGo))
    {
        commandString.remove_prefix(kGo.size());
        commandString = commandString.substr(std::min(commandString.find_first_not_of(' '), commandString.size()));
        command = std::make_shared<CommandGo>(commandString);
    }
    else if(commandString.starts_with(kSetOption))
    {
        commandString.remove_prefix(kSetOption.size());
        commandString = commandString.substr(std::min(commandString.find_first_not_of(' '), commandString.size()));
        command = std::make_shared<CommandSetOption>(uci, commandString);
    }

    return command;
}

CommandUci::CommandUci(UCI *uci) : OptionCommand{uci}
{}

bool CommandUci::execute()
{
    std::cout << "id name Bernucchio "
              <<  VERSION_MAJOR << "." << VERSION_MINOR << " (" << (GIT_VERSION) << ")" << '\n'
              << "id author " << "wb180" << '\n';

    for(auto &option : mUci->options())
        std::cout << *option << '\n';

     std::cout << "uciok" << std::endl;

    return UCICommand::execute();
}

bool CommandQuit::execute()
{
    return false;
}

bool CommandReady::execute()
{
    std::cout << "readyok" << std::endl;
    return UCICommand::execute();
}

ParamCommand::ParamCommand(const std::string_view param) : mParam{param}
{}

CommandPosition::CommandPosition(const std::string_view position) : ParamCommand{position}
{}

bool CommandPosition::execute()
{
    Searches::GetInstance().GetMainThread()->Wait();

    std::string_view fen = mParam, moves;

    if(fen.starts_with(kFen))
    {
        fen.remove_prefix(kFen.size());
        fen.remove_prefix(std::min(fen.find_first_not_of(' '), fen.size()));

        if(auto pos = fen.find(kMoves); pos != std::string_view::npos)
        {
            moves = fen.substr(pos);
            fen.remove_suffix(moves.size() + 1);
        }

    }
    else if(fen.starts_with(kStartPos))
    {
        moves = fen.substr(std::min(fen.find_first_not_of(' ', std::min(kStartPos.size() + 1, fen.size())), fen.size()));
        fen = kStartFen;
    }

    if(!Searches::GetInstance().GetMainThread()->SetFen(std::string{fen}))
        std::cerr << "Incorrect fen: " << fen << std::endl;

    if(moves.starts_with(kMoves))
    {
        moves.remove_prefix(kMoves.size());
        auto pos = moves.find_first_not_of(' ');
        do
        {
            if(pos != std::string_view::npos)
            {
                auto endPos = moves.find(' ', pos);

                std::string_view move = moves.substr(pos, std::min(endPos - 1, moves.size()));

                if(!Searches::GetInstance().GetMainThread()->MakeMove(std::string{move}))
                {
                    std::cerr << "Illegal move: " << move << std::endl;
                    break;
                }
                moves.remove_prefix(std::min(endPos, moves.size()));
            }
        }
        while((pos = moves.find_first_not_of(' ')) != std::string_view::npos);
    }

    return UCICommand::execute();
}

CommandGo::CommandGo(const std::string_view go) : ParamCommand{go}
{}

int readInt(std::string_view &str)
{
    int value;
    auto pos = std::min(str.find(' '), str.size());
    auto [ptr, res] = std::from_chars(str.data(), str.data() + pos, value);

    if(res == std::errc{})
    {
        if(ptr != str.data() + pos)
            str = str.substr(std::next(ptr) - str.data());
        return value;
    }

    return 0;
}

bool CommandGo::execute()
{
    Searches::GetInstance().GetMainThread()->Wait();
    auto ourSide = Searches::GetInstance().GetMainThread()->GetOurSide();
    bool infinite = false;

    while(!mParam.empty())
    {
        std::string_view opt = mParam.substr(0, std::min(mParam.find(' '), mParam.size()));
        mParam.remove_prefix(std::min(mParam.size(), opt.size() + 1));

        if(opt.starts_with(kWTime))
        {
            if(ourSide == Side::kWhite)
                TimeManager::GetInstance().SetOurTime(readInt(mParam));
            else
                TimeManager::GetInstance().SetOpponentTime(readInt(mParam));
        }
        else if(opt.starts_with(kBTime))
        {
            if(ourSide == Side::kWhite)
                TimeManager::GetInstance().SetOpponentTime(readInt(mParam));
            else
                TimeManager::GetInstance().SetOurTime(readInt(mParam));
        }
        else if(opt.starts_with(kWInc))
        {
            if(ourSide == Side::kWhite)
                TimeManager::GetInstance().SetOurIncrement(readInt(mParam));
            else
                TimeManager::GetInstance().SetOpponentIncrement(readInt(mParam));
        }
        else if(opt.starts_with(kBTime))
        {
            if(ourSide == Side::kWhite)
                TimeManager::GetInstance().SetOpponentIncrement(readInt(mParam));
            else
                TimeManager::GetInstance().SetOurIncrement(readInt(mParam));
        }
        else if(opt.starts_with(kMovesToGo))
        {
            TimeManager::GetInstance().SetMovesToGo(readInt(mParam));
        }
        else if(opt.starts_with(kMoveTime))
        {
            TimeManager::GetInstance().SetOurTime(readInt(mParam));
            TimeManager::GetInstance().SetMovesToGo(1);
        }
        else if(opt.starts_with(kInfinite))
        {
            TimeManager::GetInstance().SetInfinite(true);
            infinite = true;
        }
    }

    if(!infinite)
        TimeManager::GetInstance().CalculateTime();

    Searches::GetInstance().GetMainThread()->Wake();

    return UCICommand::execute();
}

bool CommandStop::execute()
{
    Searches::GetInstance().GetMainThread()->Stop();
    return UCICommand::execute();
}

bool CommandSpeedTest::execute()
{
    GameState s;
    s.SpeedPerft();
    return UCICommand::execute();
}

OptionCommand::OptionCommand(UCI *uci) : mUci{uci}
{}

CommandSetOption::CommandSetOption(UCI *uci, std::string_view option) : OptionCommand{uci}, ParamCommand{option}
{}

bool CommandSetOption::execute()
{
    auto startPos = std::min(mParam.find_first_not_of(' '), mParam.size());
    auto endPos = std::min(mParam.find(' ', startPos), mParam.size());

    auto name = mParam.substr(startPos, endPos - startPos);
    mParam.remove_prefix(endPos);

    startPos = std::min(mParam.find_first_not_of(' '), mParam.size());

    if(mParam.at(startPos) == '\'')
        endPos = std::min(mParam.find_last_of('\'') + 1, mParam.size());
    else
        endPos = std::min(mParam.find(' ', startPos), mParam.size());

    auto value = mParam.substr(startPos, (endPos - startPos));

    auto option = std::find_if(std::begin(mUci->options()), std::end(mUci->options()),
                            [name](const std::unique_ptr<UCIOption> &op){return op->name() == name;});

    if(option == std::end(mUci->options()))
        std::cout << "there is no option with name: " << name;
    else
    {
        if(value.front() == '\'' && value.back() == '\'')
        {
            value.remove_prefix(1);
            value.remove_suffix(1);
        }

        (*option)->setValue(std::string{value});
    }

    return UCICommand::execute();
}
