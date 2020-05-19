#include <iostream>
#include <ctime>

#include "moves.h"
#include "uci.h"
#include "gamestate.h"
#include "optimization.h"

#ifdef BUILD_WITH_TESTS
    #include <gtest/gtest.h>
#endif

int main([[maybe_unused]] int argc, [[maybe_unused]] char** argv)
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

        //optimize();

        UCI uci;
        uci.Loop();

#ifdef BUILD_WITH_TESTS
    }
#endif
    return 0;
}
