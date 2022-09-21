PROJECT(BernucchioTest)

set(HEADERS ../bits_functions.h ../gamestate.h)
set(SOURCES fen.cpp
            hash.cpp
            perft.cpp
            main.cpp
            ../board.cpp
            ../moves.cpp
            ../gamestate.cpp
            ../movelist.cpp
            ../hashlist.cpp
            ../magics.cpp
            ../uci.cpp
            ../timemanager.cpp
            ../search.cpp
            ../evaluator.cpp
            ../ctg.cpp
            ../optimization.cpp
            ../ucicommand.cpp
            ../ucioption.cpp)

set(gtest_force_shared_crt on)
add_subdirectory(googletest)

add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})

target_include_directories(${PROJECT_NAME} PRIVATE googletest/googletest/include)
target_link_libraries(${PROJECT_NAME} PRIVATE gtest)
