TEMPLATE = app
CONFIG += console c++14
CONFIG -= app_bundle
CONFIG -= qt

#DEFINES += BUILD_WITH_TESTS

SOURCES += \
        main.cpp \
    board.cpp \
    logger.cpp \
    moves.cpp \
    gamestate.cpp \
    movelist.cpp \
    tests/fen.cpp \
    hashlist.cpp \
    magics.cpp \
    tests/perft.cpp

HEADERS += \
    bits_functions.h \
    board.h \
    logger.h \
    constants.h \
    moves.h \
    gamestate.h \
    movelist.h \
    hashlist.h \
    magics.hpp \
    moveinfo.h

DISTFILES += \
    readme.md

unix|win32: LIBS += -lgtest
