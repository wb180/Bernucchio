#include "bits_functions.h"
#include "evaluator.h"

Evaluator::Evaluator(Board *board) : board_(board)
{

}

int Evaluator::Score()
{
   return GetBitsCount(board_->white_pawns_) * 100 + GetBitsCount(board_->white_knights_) * 300 + GetBitsCount(board_->white_bishops_) * 340 +
           GetBitsCount(board_->white_rooks_) * 500 - GetBitsCount(board_->black_pawns_) * 100 - GetBitsCount(board_->black_knights_) * 300 -
            GetBitsCount(board_->black_bishops_) * 340 - GetBitsCount(board_->black_rooks_) * 500;
}
