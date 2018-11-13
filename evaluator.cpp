#include "evaluator.h"

#include <cstdlib>

Evaluator::Evaluator(Board *board) : board_(board)
{

}

int Evaluator::Score()
{
    return rand() % 5000 - 2500;
}
