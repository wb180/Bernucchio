#include "gamestate.h"
#include "uci.h"

const std::string kUci = "uci";
const std::string kQuit = "quit";
const std::string KIsReady = "isready";
const std::string kPosition = "position";
const std::string kSpeedTest = "speed test";

UCI::UCI()
{

}

void UCI::Loop()
{
    std::string command;

    while(std::getline(std::cin, command))
    {
        if(command == kQuit)
          break;
        else if(command == kUci)
        {
            std::cout << "id name Shredder" << std::endl
                      << "id author Stefan MK" << std::endl
                      << "uciok" << std::endl;

            std::flush(std::cout);
        }
        else if(command == KIsReady)
        {
            std::cout << "readyok" << std::endl;
            std::flush(std::cout);
        }
        else if(command == kPosition)
        {

        }
        else if(command == kSpeedTest)
        {
            GameState gs;
            gs.SpeedPerft();
        }
    }
}
