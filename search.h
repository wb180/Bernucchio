#ifndef SEARCH_H
#define SEARCH_H

#include "gamestate.h"

#include <atomic>
#include <chrono>
#include <condition_variable>
#include <mutex>
#include <thread>
#include <vector>

class Search
{
public:
    Search();

    enum class States
    {
        kQuit,
        kWait,
        kWork
    };

    bool SetFen(const std::string &fen_string);
    bool MakeMove(const std::string &move);
    Side GetOurSide();

    ~Search();
    void Wake();
    void Wait();
    void Stop();

private:
    void Loop();
    void RunSearch();    
    void Start();


    bool exit_ = false;
    States state_;
    std::atomic<bool> stop_;

    std::condition_variable condition_variable_;
    std::thread thread_;
    std::mutex mutex_;    

    GameState gamestate_;
};

class Searches
{
private:
    explicit Searches(std::size_t threads_number);

public:
    static Searches& GetInstance(std::size_t threads_number = 1)
    {
        static Searches searches{ threads_number };
        return searches;
    }

    Searches(const Searches&) = delete;
    Searches(Searches&&) = delete;
    Searches& operator=(const Searches&) = delete;
    Searches& operator=(Searches&&) = delete;

    Search *GetMainThread();

private:
    std::vector<Search> search_vector_;
};

#endif // SEARCH_H
