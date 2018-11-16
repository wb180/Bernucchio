#include "search.h"
#include "logger.h"

#include <iostream>

Search::Search() : thread_(&Search::Loop, this)
{
    //Logger::GetInstance("log.txt") << "Wait for search finished in Constructor";
    Wait();
    //Logger::GetInstance("log.txt") << "Search finished in Constructor";
}

bool Search::SetFen(const std::string &fen_string)
{
    //Logger::GetInstance("log.txt") << "Wait for search finished in SetFen";
    Wait();
    //Logger::GetInstance("log.txt") << "Search finished in SetFen";
    return state_.SetFen(fen_string);
}

bool Search::MakeMove(const std::string &move)
{
    //Logger::GetInstance("log.txt") << "Wait for search finished in MakeMove";
    Wait();
    //Logger::GetInstance("log.txt") << "Search finished in MakeMove";
    return state_.MakeMove(move);
}

Side Search::GetOurSide()
{
    //Logger::GetInstance("log.txt") << "Wait for search finished in GetOurSide";
    Wait();
    //Logger::GetInstance("log.txt") << "Search finished in GetOurSide";
    return state_.GetSide();
}

Search::~Search()
{
    exit_ = true;
    Start();
    thread_.join();
}

void Search::StartSearch()
{
    Wait();
    Start();
}

void Search::Start()
{
    //Logger::GetInstance("log.txt") << "Start search in Start";
    //Logger::GetInstance("log.txt") << "Wait for search finished in Start";

    //Wait();
    //Logger::GetInstance("log.txt") << "Search finished in Start";

    std::scoped_lock<std::mutex> lock(mutex_);
    start_search_ = true;
    condition_variable_.notify_one();

    //Logger::GetInstance("log.txt") << "Exit start search in Start";
}

void Search::Stop()
{
    stop_ = true;
}

void Search::Loop()
{
    while(true)
    {
        //Logger::GetInstance("log.txt") << "Loop in Loop";

        std::unique_lock<std::mutex> lock(mutex_);
        start_search_ = false;
        condition_variable_.notify_one();

        //Logger::GetInstance("log.txt") << "Waiting in Loop in Loop";

        condition_variable_.wait(lock, [&]{ return start_search_; });

        //Logger::GetInstance("log.txt") << "Waiting finished in Loop";

        if(exit_)
            return;

        lock.unlock();

        //Logger::GetInstance("log.txt") << "Run search in Loop";
        RunSearch();
        //Logger::GetInstance("log.txt") << "Loop search finished in Loop";
    }
}

void Search::RunSearch()
{
    stop_ = false;
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
