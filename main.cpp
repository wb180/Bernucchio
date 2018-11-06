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
#ifdef BUILD_WITH_TESTS
    testing::InitGoogleTest(&argc, argv);
    int result = RUN_ALL_TESTS();
    if(result)
        return result;
    else
    {
#endif

        InitializeBishopMoves();
        InitializeRookMoves();

        GameState gs;
        gs.SetFen("8/8/1k6/8/2pP4/8/5BK1/8 b - d3 0 1");


#ifdef BUILD_WITH_TESTS
    }
#endif
    return 0;
}
