#include "timemanager.h"

#include <iostream>

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
        move_time = static_cast<uint64_t>(our_time_ / 40 + 0.75 * our_increment_);
    }

    finish_time =  std::chrono::steady_clock::now() + std::chrono::milliseconds {move_time};
}

bool TimeManager::CheckTime() const
{
    /*auto t1 = std::chrono::steady_clock::now();
    auto t2 = finish_time;

    std::cout << std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count() << std::endl;*/

    return  std::chrono::steady_clock::now() < finish_time;
}
