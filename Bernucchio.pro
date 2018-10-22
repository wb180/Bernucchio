TEMPLATE = app
CONFIG += console c++14
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
        main.cpp \
    board.cpp \
    logger.cpp \
    moves.cpp \
    move.cpp \
    gamestate.cpp \
    movelist.cpp

HEADERS += \
    bits_functions.h \
    board.h \
    logger.h \
    constants.h \
    moves.h \
    move.h \
    gamestate.h \
    movelist.h

DISTFILES += \
    readme.rd
