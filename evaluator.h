#ifndef EVALUATOR_H
#define EVALUATOR_H

#include "board.h"

class Evaluator
{
public:
    explicit Evaluator(Board *board);

    int Score();

private:
    Board *board_;
};

#endif // EVALUATOR_H
