#include <iostream>

#include "moves.h"
#include "uci.h"
#include "gamestate.h"

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
        /*UCI uci;
        uci.Loop();*/

        GameState gs;
        gs.SetFen("r2qkb1r/pp2nppp/3p4/2pNN1B1/2BnP3/3P4/PPP2PPP/R2bK2R w KQkq - 1 0");
        gs.Search(4);

#ifdef BUILD_WITH_TESTS
    }
#endif
    return 0;
}
