#pragma once

#include <chrono>

namespace PKEngine {
    class Time {
        friend class engine_instance;
    public:
        using clock = std::chrono::system_clock;
        using duration = std::chrono::system_clock::duration;
        using time_point = std::chrono::system_clock::time_point;

    protected:
        static time_point start_time, last_update_time;
        static duration _delta_time;

        static inline void start() {
            start_time = last_update_time = clock::now();
        }

        static inline void update() {
            time_point old_update_time = last_update_time;
            last_update_time = clock::now();
            _delta_time = last_update_time - old_update_time;
        }

    public:
        static inline duration delta_time() noexcept { return _delta_time; }
        static inline duration uptime() noexcept { return last_update_time - start_time; }
    };
}