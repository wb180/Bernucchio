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
        gs.SetFen("r3k2r/p1ppNpb1/bn2pn2/3P4/1p2P3/2N2Q2/PPPBBPpP/R3K2R b KQkq - 0 2");

        std::cout << gs.GetFen()<< std::endl;
        std::cout << "Total: " << std::endl << gs.SplitPerft(0) << std::endl;

#ifdef BUILD_WITH_TESTS
    }
#endif
    return 0;
}
