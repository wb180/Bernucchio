CMAKE_MINIMUM_REQUIRED(VERSION 3.12)

PROJECT(Bernucchio)

SET(CMAKE_CXX_STANDARD 17)
SET(CMAKE_INCLUDE_CURRENT_DIR ON)
SET(GCC_BUILTIN_POPCOUNT_COMPILE_FLAGS "-mpopcnt")
SET(GCC_BUILTIN_CTZLL ON)
SET(BUILD_OPTION_WITH_TESTS OFF)
SET(MINGW_COMPILE_FLAGS "-static-libgcc -static-libstdc++ -static -lpthread")

IF(GCC_BUILTIN_POPCOUNT_COMPILE_FLAGS)
  SET(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} ${GCC_BUILTIN_POPCOUNT_COMPILE_FLAGS}")
  ADD_COMPILE_DEFINITIONS(BUILD_WITH_POPCOUNT)
  MESSAGE(STATUS "build with popcount")
ELSE()
  MESSAGE(STATUS "build without popcount")
ENDIF()

IF(GCC_BUILTIN_CTZLL)
  ADD_COMPILE_DEFINITIONS(BUILD_WITH_CTZLL)
  MESSAGE(STATUS "build with ctzll")
ELSE()
  MESSAGE(STATUS "build without ctzll")
ENDIF()

if(BUILD_OPTION_WITH_TESTS)
  ADD_COMPILE_DEFINITIONS(BUILD_WITH_TESTS)
  SET(SOURCES tests/fen.cpp tests/perft.cpp)
  SET(LIBS gtest)
  MESSAGE(STATUS "build with tests")
ELSE()
  MESSAGE(STATUS "build without tests")
ENDIF()

IF(MINGW)
    SET(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} ${MINGW_COMPILE_FLAGS}")
ENDIF()

SET(SOURCES ${SOURCES} main.cpp board.cpp logger.cpp moves.cpp gamestate.cpp movelist.cpp hashlist.cpp magics.cpp uci.cpp timemanager.cpp search.cpp evaluator.cpp)
SET(HEADERS bits_functions.h board.h logger.h constants.h moves.h gamestate.h movelist.h hashlist.h magics.hpp moveinfo.h uci.h timemanager.h search.h evaluator.h)
SET(OTHER readme.md)

SET(LIBS ${LIBS} pthread)

#ADD_CUSTOM_TARGET(INVALIDATE_FILES ALL
#                  COMMAND ${CMAKE_COMMAND} -E touch uci.cpp)

ADD_EXECUTABLE(${PROJECT_NAME} ${SOURCES} ${HEADERS})
TARGET_LINK_LIBRARIES(${PROJECT_NAME} ${LIBS})
