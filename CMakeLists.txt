cmake_minimum_required(VERSION 3.12)

project(Bernucchio VERSION 1.0)

set(CMAKE_CXX_STANDARD 20)

set(BUILTIN_CTZLL ON)
set(BUILTIN_SSE_4_2 ON)
set(BUILTIN_POPCOUNT ON)
set(BUILD_WITH_TESTS ON)
set(INSTALL_GTEST OFF)

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release)
endif()

add_compile_definitions(BUILTIN_CTZLL)

if (CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    # using Clang
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    add_compile_definitions(GCC_COMPILER)
    if(BUILTIN_SSE_4_2)
        add_compile_options(-msse4.2)
    elseif(BUILTIN_POPCOUNT)
        add_compile_options(-mpopcnt)
    endif()
    set(CMAKE_CXX_FLAGS_RELEASE "-O3")
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "Intel")
  # using Intel C++
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
  add_compile_definitions(MSVC_COMPILER)  
  if(BUILTIN_SSE_4_2)
      add_compile_definitions(MSVC_SSE_4_2)
  elseif(BUILTIN_POPCOUNT)
      add_compile_definitions(MSVC_POPCOUNT)      
  endif()
  set(CMAKE_CXX_FLAGS_RELEASE "/O2")
endif()

set(HEADERS
    bits_functions.h
    board.h
    logger.h
    constants.h
    moves.h
    gamestate.h
    movelist.h
    hashlist.h
    magics.hpp
    moveinfo.h
    uci.h
    timemanager.h
    search.h
    evaluator.h
    move.h
    ctg.h
    openga.hpp
    optimization.h
    pst.h)

set(SOURCES main.cpp
    board.cpp
    logger.cpp
    moves.cpp
    gamestate.cpp
    movelist.cpp
    hashlist.cpp
    magics.cpp
    uci.cpp
    timemanager.cpp
    search.cpp
    evaluator.cpp
    ctg.cpp
    optimization.cpp)

set(OTHER readme.md)

execute_process(
  COMMAND git log "-1" "--pretty=format:%ad %h" "--date=format:%Y-%m-%d"
  WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
  OUTPUT_VARIABLE GIT_VERSION
  OUTPUT_STRIP_TRAILING_WHITESPACE
)

add_definitions(-DGIT_VERSION="${GIT_VERSION}")
add_definitions(-DVERSION_MAJOR="${PROJECT_VERSION_MAJOR}")
add_definitions(-DVERSION_MINOR="${PROJECT_VERSION_MINOR}")

configure_file(
  ${CMAKE_SOURCE_DIR}/version.h.in
  ${CMAKE_BINARY_DIR}/generated/version.h
)

include_directories(${CMAKE_BINARY_DIR}/generated)

add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})
target_link_libraries(${PROJECT_NAME} ${LIBS})

if(BUILD_WITH_TESTS)
    add_subdirectory(test)
endif()
