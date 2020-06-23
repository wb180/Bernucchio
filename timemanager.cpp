#include "timemanager.h"

#include <iostream>
#include <cmath>

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

void TimeManager::SetInfinite(bool is_infinite)
{
    is_infinite_ = is_infinite;
}

bool TimeManager::IsInfinite() const
{
    return is_infinite_;
}

void TimeManager::CalculateTime()
{
    uint64_t move_time;
    is_infinite_ = false;

    if(moves_to_go_)
    {
        move_time = static_cast<uint64_t>(our_time_ / moves_to_go_ + 0.6 * our_increment_);
    }
    else
    {
        if(our_time_ > 926)
            move_time = std::pow(our_time_, 2)/(5*our_time_ + 26000);
        else
            move_time = std::pow(our_time_, 2)/(32*our_time_ + 1000);

        move_time = move_time ? move_time : 1;
    }

    start_time_ = std::chrono::steady_clock::now();
    finish_time =  start_time_ + std::chrono::milliseconds {move_time};
}

bool TimeManager::CheckTime() const
{
    if(is_infinite_)
        return true;

//    std::cout << (std::chrono::steady_clock::now() < finish_time) << std::endl;

    return  std::chrono::steady_clock::now() < finish_time;
}

double TimeManager::TimeElapsed() const
{
    auto time_now = std::chrono::steady_clock::now();

    return (time_now - start_time_)/(finish_time - start_time_);
}
