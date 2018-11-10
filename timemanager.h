#ifndef TIMEMANAGER_H
#define TIMEMANAGER_H

#include <chrono>

class TimeManager
{
public:
    static TimeManager& GetInstance()
    {
        static TimeManager manager;
        return manager;
    }

    TimeManager(const TimeManager&) = delete;
    TimeManager(TimeManager&&) = delete;
    TimeManager& operator=(const TimeManager&) = delete;
    TimeManager& operator=(TimeManager&&) = delete;

    void SetOurTime(std::size_t time);
    void SetOpponentTime(std::size_t time);
    void SetOurIncrement(std::size_t increment);
    void SetOpponentIncrement(std::size_t increment);
    void SetMovesToGo(std::size_t moves);

    void CalculateTime();
    std::chrono::milliseconds GetMoveTime() const;

private:
    TimeManager();

    std::size_t our_time_;
    std::size_t opponent_time_;
    std::size_t our_increment_;
    std::size_t opponent_increment_;
    std::size_t moves_to_go_;

    std::chrono::milliseconds calculated_move_time_;
};

#endif // TIMEMANAGER_H
