#include "timemanager.h"

TimeManager::TimeManager()
{

}

void TimeManager::SetOurTime(std::size_t time)
{
    our_time_ = time;
}

void TimeManager::SetOpponentTime(std::size_t time)
{
    opponent_time_ = time;
}

void TimeManager::SetOurIncrement(std::size_t increment)
{
    our_increment_ = increment;
}

void TimeManager::SetOpponentIncrement(std::size_t increment)
{
    opponent_increment_ = increment;
}

void TimeManager::SetMovesToGo(std::size_t moves)
{
    moves_to_go_ = moves;
}

void TimeManager::CalculateTime()
{
    uint64_t move_time;

    if(moves_to_go_)
    {
        move_time = our_time_ / moves_to_go_ + our_increment_;
    }
    else
    {
        move_time = our_time_ / 50 + our_increment_;
    }

    calculated_move_time_ = std::chrono::milliseconds {move_time};
}

std::chrono::milliseconds TimeManager::GetMoveTime() const
{
    return calculated_move_time_;
}
