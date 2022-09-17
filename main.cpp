#include "moves.h"
#include "uci.h"

int main([[maybe_unused]] int argc, [[maybe_unused]] char** argv)
{
    InitializeBishopMoves();
    InitializeRookMoves();

    UCI uci;
    uci.Loop();

    return 0;
}
