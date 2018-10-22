#include <iostream>

#include "gamestate.h"
#include "logger.h"

using namespace std;

int main()
{

    GameState gs;
    Logger lg;

    std::string f1 = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1",
                f2 = "rnbqkbnr/pppppppp/8/8/4P3/8/PPPP1PPP/RNBQKBNR b KQkq e3 0 1",
                f3 = "rnbqkbnr/pp1ppppp/8/2p5/4P3/8/PPPP1PPP/RNBQKBNR w KQkq c6 0 2",
                f4 = "rnbqkbnr/pp1ppppp/8/2p5/4P3/5N2/PPPP1PPP/RNBQKB1R b KQkq - 1 2",
                f5 = "r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq -",
                f6 = "5k2/3K2p1/6Pp/7P/2B5/8/8/8 w - - 0 1";

    gs.SetFen(f6);
    std::cout << f6 << std::endl
              << gs.GetFen() << std::endl;

    return 0;
}
