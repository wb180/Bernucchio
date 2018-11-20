#include "search.h"
#include "logger.h"

#include <iostream>

Search::Search() : state_(States::kWait)
{
    thread_ = std::thread(&Search::Loop, this);
}

bool Search::SetFen(const std::string &fen_string)
{
    return gamestate_.SetFen(fen_string);
}

bool Search::MakeMove(const std::string &move)
{
    return gamestate_.MakeMove(move);
}

Side Search::GetOurSide()
{
    return gamestate_.GetSide();
}

Search::~Search()
{
    {
        std::lock_guard<std::mutex> lock(mutex_);
        state_ = States::kQuit;
        condition_variable_.notify_one();
    }

    try { thread_.join(); }
    catch ( const std::system_error& ){ };
}

void Search::Wake()
{
    std::lock_guard<std::mutex> lock(mutex_);
    state_ = States::kWork;
    condition_variable_.notify_one();
}

void Search::Stop()
{
    stop_ = true;
}

void Search::Loop()
{
    while(true)
    {
      {
        std::unique_lock<std::mutex> lock(mutex_);

        while(state_ == States::kWait)
        {
          condition_variable_.wait(lock);
        }

        if(state_ == States::kQuit)
            break;

        state_ = States::kWait;
      }

      RunSearch();
    }
}

void Search::RunSearch()
{
    stop_ = false;
    gamestate_.Search(100, &stop_);
}

void Search::Wait()
{
    std::unique_lock<std::mutex> lock(mutex_);

    while(state_ != States::kWait)
    {
      condition_variable_.wait(lock);
    }
}

Searches::Searches(std::size_t threads_number) : search_vector_{threads_number}
{

}

Search *Searches::GetMainThread()
{
    return &search_vector_.front();
}
