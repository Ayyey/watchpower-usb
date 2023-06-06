#pragma once

#include <chrono>
#include <string>

namespace mch
{

    class Timer
    {
    public:
        Timer();
        void start();
        int64_t msPassed();
        std::string timeString();

    private:
        std::chrono::time_point<std::chrono::system_clock> m_time;
    };
} // namespace mch
