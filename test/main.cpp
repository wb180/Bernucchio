#include "gtest/gtest.h"

#include "../moves.h"

int main(int argc, char** argv)
{
    InitializeBishopMoves();
    InitializeRookMoves();

    testing::InitGoogleTest(&argc, argv);
    int result = RUN_ALL_TESTS();
    
	return result;	
}
