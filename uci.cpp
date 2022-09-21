#include "ctg.h"
#include "logger.h"
#include "search.h"
#include "timemanager.h"
#include "uci.h"
#include "ucicommand.h"
#include "version.h"

#include <fstream>
#include <sstream>
#include <string_view>

const std::string_view kSetOption = "setoption";
const std::string_view kCTGBookPath = "CTGBookPath";

UCI::UCI()
{
    mOptions.emplace_back(std::make_unique<CTGBookOption>(kCTGBookPath, std::string{}));
}

void UCI::Loop()
{
    std::string commandString;
    std::string token;

    while(std::getline(std::cin, commandString))
    {
        std::istringstream ss{commandString};
        ss >> token;

        auto command = CommandFactory::CreateCommand(commandString, this);

        if(command && !command->execute())
            break;
    }
}

const std::vector<std::unique_ptr<UCIOption>> &UCI::options() const
{
    return mOptions;
}
