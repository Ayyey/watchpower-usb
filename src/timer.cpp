#include "timer.hpp"
namespace mch
{
    Timer::Timer() { m_time = std::chrono::system_clock::now(); };
    void Timer::start()
    {
        m_time = std::chrono::system_clock::now();
    }
    int64_t Timer::msPassed()
    {
        auto passed = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now() - m_time);
        return passed.count();
    }
    std::string Timer::timeString()
    {
        std::string hours, minutes, seconds, milliseconds;
        using namespace std::chrono;
        auto now = system_clock::now().time_since_epoch();
        hours = std::to_string((duration_cast<std::chrono::hours>(now).count() + 8) % 24);
        minutes = std::to_string(duration_cast<std::chrono::minutes>(now).count() % 60);
        seconds = std::to_string(duration_cast<std::chrono::seconds>(now).count() % 60);
        milliseconds = std::to_string(duration_cast<std::chrono::milliseconds>(now).count() % 1000);
        if (hours.length() == 1)
            hours.insert(0, "0");
        if (minutes.length() == 1)
            minutes.insert(0, "0");
        if (seconds.length() == 1)
            seconds.insert(0, "0");
        while (milliseconds.length() != 3)
            milliseconds.insert(0, "0");
        std::string result = hours + ':' + minutes + ':' + seconds + '.' + milliseconds;
        return result;
    }
} // namespace mch
