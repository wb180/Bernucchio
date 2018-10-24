#include <iostream>

#include "gamestate.h"
#include "logger.h"

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
#ifdef BUILD_WITH_TESTS
    }
#endif
    return 0;
}
