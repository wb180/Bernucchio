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
        gs.SpeedPerft();

#ifdef BUILD_WITH_TESTS
    }
#endif
    return 0;
}
