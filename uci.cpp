#include "search.h"
#include "timemanager.h"
#include "uci.h"

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

UCI::UCI()
{

}

void UCI::Loop()
{
    std::string command;    
    std::string token;
    std::size_t number_value;

//    std::ofstream myfile;
//    myfile.open ("log_in.txt", std::ios::app);

    while(std::getline(std::cin, command))
    {
//        myfile << command << std::endl;

        std::istringstream ss{ command };
        ss >> token;

        if(token == kQuit)
          break;
        else if(token == kUci)
        {
            std::cout << "id name Bernucchio v_11122018" << std::endl
                      << "id author WirBrauchen180" << std::endl
                      << "uciok" << std::endl;

            std::flush(std::cout);
        }
        else if(token == kIsReady)
        {
            std::cout << "readyok" << std::endl;
            std::flush(std::cout);
        }
        else if(token == kPosition)
        {
            std::string fen;
            ss >> token;

            if(token == kFen)
            {
                while(ss >> token && token != kMoves)
                    fen.append(token);
            }
            else
            {
                fen = kStartFen;
                ss >> token;
            }

            if(!Searches::GetInstance().GetMainThread()->SetFen(fen))
            {
                std::cerr << "Exception: Wrong fen string: " << fen << std::endl;
                std::exit(1);
            }

            if(token == kMoves)
            {
                while(ss >> token)
                    if(!Searches::GetInstance().GetMainThread()->MakeMove(token))
                    {
                        std::cerr << "Exception: Illegal move: " << token << std::endl;
                        std::exit(1);
                    }
            }
        }
        else if(token == kGo)
        {
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
                TimeManager::GetInstance().CalculateTime();

            Searches::GetInstance().GetMainThread()->Start();
        }
        else if(token == kStop)
        {
            Searches::GetInstance().GetMainThread()->Stop();
        }
    }

//    myfile.close();
}
