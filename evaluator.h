#ifndef EVALUATOR_H
#define EVALUATOR_H

#include "board.h"

#include <vector>

class Evaluator
{
public:
    explicit Evaluator(Board *board);

    int Score();
    int Score(std::vector<int>&);
    int ScoreMate();

private:
    Board *board_;
};

#endif // EVALUATOR_H
