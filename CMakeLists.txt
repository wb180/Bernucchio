cmake_minimum_required(VERSION 3.12)

project(Bernucchio)

add_compile_definitions(BUILD_WITH_TESTS)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(SOURCES main.cpp board.cpp logger.cpp moves.cpp gamestate.cpp movelist.cpp tests/fen.cpp hashlist.cpp magics.cpp tests/perft.cpp uci.cpp timemanager.cpp)
set(HEADERS bits_functions.h board.h logger.h constants.h moves.h gamestate.h movelist.h hashlist.h magics.hpp moveinfo.h uci.h timemanager.h)
set(OTHER readme.md)
set(LIBS gtest)

add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})
target_link_libraries(${PROJECT_NAME} ${LIBS})
