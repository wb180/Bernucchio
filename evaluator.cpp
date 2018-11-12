#include "evaluator.h"

#include <random>

Evaluator::Evaluator(Board *board) : board_(board)
{

}

int Evaluator::Score()
{    
    static std::mt19937 rng{};
    rng.seed(std::random_device{}());
    static std::uniform_int_distribution<> d(-200, 200);

    return d(rng);
}
