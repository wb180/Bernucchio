#include <iostream>

#include "bits_functions.h"
#include "gamestate.h"
#include "logger.h"
#include "magics.hpp"
#include "moves.h"

#include <numeric>

#ifdef BUILD_WITH_TESTS
    #include <gtest/gtest.h>
#endif

int main(int argc, char** argv)
{
    InitializeBishopMoves();
    InitializeRookMoves();

#ifdef BUILD_WITH_TESTS
    testing::InitGoogleTest(&argc, argv);
    int result = RUN_ALL_TESTS();
    if(result)
        return result;
    else
    {
#endif

        GameState gs;
        gs.SetFen("r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - 0 1");

        std::cout << gs.GetFen()<< std::endl;
        std::cout << "Total: " << std::endl << gs.SplitPerft(2) << std::endl;

#ifdef BUILD_WITH_TESTS
    }
#endif
    return 0;
}
