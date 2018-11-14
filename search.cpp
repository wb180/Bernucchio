#include "search.h"

#include <iostream>

Search::Search() : thread_(&Search::Loop, this)
{
    Wait();
}

bool Search::SetFen(const std::string &fen_string)
{
    Wait();
    return state_.SetFen(fen_string);
}

bool Search::MakeMove(const std::string &move)
{
    Wait();
    return state_.MakeMove(move);
}

Side Search::GetOurSide()
{
    Wait();
    return state_.GetSide();
}

Search::~Search()
{
    exit_ = true;
    Start();
    thread_.join();
}

void Search::Start()
{
    Wait();

    std::unique_lock<std::mutex> lock(mutex_);
    start_search_ = true;
    stop_ = false;
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
        std::unique_lock<std::mutex> lock(mutex_);
        start_search_ = false;
        condition_variable_.notify_one();
        condition_variable_.wait(lock, [&]{ return start_search_; });

        if(exit_)
            return;

        lock.unlock();
        RunSearch();
    }
}

void Search::RunSearch()
{
    state_.Search(100, &stop_);
}

void Search::Wait()
{
    std::unique_lock<std::mutex> lock(mutex_);
    condition_variable_.wait(lock, [&]{ return !start_search_; });
}

Searches::Searches(std::size_t threads_number) : search_vector_{threads_number}
{

}

Search *Searches::GetMainThread()
{
    return &search_vector_.front();
}
