#include "logger.h"
#include "search.h"
#include "timemanager.h"
#include "uci.h"
#include "version.h"

#include <fstream>

#include <sstream>

const std::string kUci = "uci";
const std::string kQuit = "quit";
const std::string kIsReady = "isready";
const std::string kPosition = "position";
const std::string kStartPos = "startpos";
const std::string kFen = "fen";
const std::string kStartFen = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";
const std::string kMoves = "moves";
const std::string kGo = "go";
const std::string kWTime = "wtime";
const std::string kBTime = "btime";
const std::string kWInc = "winc";
const std::string kBInc = "binc";
const std::string kMovesToGo = "movestogo";
const std::string kInfinite = "infinite";
const std::string kStop = "stop";
const std::string kSpeedTest = "speedtest";

UCI::UCI()
{

}

void UCI::Loop()
{
    std::string command;    
    std::string token;
    std::size_t number_value;

    //Logger::GetInstance("log_2.txt") << "New Instance";

//    std::ifstream f("log.txt");

    while(std::getline(std::cin, command))
    {
//        if(command.empty())
//            continue;

//        Logger::GetInstance("log.txt") << command;

        std::istringstream ss{ command };
        ss >> token;

        if(token == kQuit)
          break;
        else if(token == kUci)
        {
            std::ostringstream ss;

            ss << "id name Bernucchio " << GIT_VERSION << std::endl
                      << "id author WirBrauchen180" << std::endl
                      << "uciok" << std::endl;

            std::cout << ss.str();
            std::flush(std::cout);

            //Logger::GetInstance("log.txt") << ss.str();
        }
        else if(token == kIsReady)
        {
            std::ostringstream ss;
            ss << "readyok" << std::endl;
            std::cout << ss.str();
            std::flush(std::cout);

            //Logger::GetInstance("log.txt") << ss.str();
        }
        else if(token == kPosition)
        {
            Searches::GetInstance().GetMainThread()->Wait();

            std::string fen;
            ss >> token;

            if(token == kFen)
            {
                while(ss >> token && token != kMoves)
                {
                    if(!fen.empty())
                        fen.append(" ");

                    fen.append(token);
                }
            }
            else
            {
                fen = kStartFen;
                ss >> token;
            }

            if(!Searches::GetInstance().GetMainThread()->SetFen(fen))
            {
                std::cerr << "Exception: Wrong fen string: " << fen << std::endl;
                //std::exit(1);
            }

            if(token == kMoves)
            {
                while(ss >> token)
                    if(!Searches::GetInstance().GetMainThread()->MakeMove(token))
                    {
                        //Logger::GetInstance() << "Exception: Illegal move: " << token;
                        //std::exit(1);
                    }
            }
        }
        else if(token == kGo)
        {
            Searches::GetInstance().GetMainThread()->Wait();

            while(ss >> token)
            {
                if(token == kWTime || token == kBTime)
                {
                    ss >> number_value;

                    if((Searches::GetInstance().GetMainThread()->GetOurSide() == Side::kWhite && token == kWTime) ||
                            (Searches::GetInstance().GetMainThread()->GetOurSide() == Side::kBlack && token == kBTime))
                        TimeManager::GetInstance().SetOurTime(number_value);
                    else
                        TimeManager::GetInstance().SetOpponentTime(number_value);
                }
                else if(token == kWInc || token == kBInc)
                {
                    ss >> number_value;

                    if((Searches::GetInstance().GetMainThread()->GetOurSide() == Side::kWhite && token == kWInc) ||
                            (Searches::GetInstance().GetMainThread()->GetOurSide() == Side::kBlack && token == kBInc))
                        TimeManager::GetInstance().SetOurIncrement(number_value);
                    else
                        TimeManager::GetInstance().SetOpponentIncrement(number_value);
                }
                else if(token == kMovesToGo)
                {
                    ss >> number_value;
                    TimeManager::GetInstance().SetMovesToGo(number_value);
                }
            }

            if(token != kInfinite)
            {
                TimeManager::GetInstance().CalculateTime();
                Searches::GetInstance().GetMainThread()->Wake();
            }
            else if(token == kInfinite)
            {
                TimeManager::GetInstance().SetInfinite(true);
                Searches::GetInstance().GetMainThread()->Wake();
            }
        }
        else if(token == kStop)
        {
            Searches::GetInstance().GetMainThread()->Stop();
        }
        else if(token == kSpeedTest)
        {
            GameState s;
            s.SpeedPerft();
        }
    }

//    myfile.close();
}
