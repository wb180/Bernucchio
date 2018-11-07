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
        gs.SetFen("8/5bk1/8/2Pp4/8/1K6/8/8 w - d6 0 1");

        std::cout << gs.GetFen()<< std::endl;
        std::cout << "Total: " << gs.SplitPerft(0) << std::endl;

#ifdef BUILD_WITH_TESTS
    }
#endif
    return 0;
}
