#include "gtest/gtest.h"

#include "../moves.h"

int main([[maybe_unused]] int argc, [[maybe_unused]] char** argv)
{
    InitializeBishopMoves();
    InitializeRookMoves();

    testing::InitGoogleTest(&argc, argv);
    int result = RUN_ALL_TESTS();
    
	return result;	
}
