#pragma once

#include <chrono>

namespace PKEngine {
    class Time {
        friend class engine_instance;
    public:
        using clock = std::chrono::system_clock;
        using duration = std::chrono::microseconds;
        using time_point = std::chrono::time_point<clock, duration>;

    protected:
        static clock::time_point start_time, last_update_time;
        static duration _delta_time;

        static inline void start() {
            start_time = last_update_time = clock::now();
        }

        static inline void update() {
            clock::time_point current_time = clock::now();
            _delta_time = std::chrono::duration_cast<duration>(current_time - last_update_time);
            last_update_time = current_time;
        }

    public:
        static inline float float_delta() noexcept { return (float) delta_time().count() / 1000000.0f; }

        template<typename DurationType = duration>
        static inline duration delta_time() noexcept { return std::chrono::duration_cast<DurationType>(_delta_time); }

        template<typename DurationType = duration>
        static inline duration uptime() noexcept { return std::chrono::duration_cast<DurationType>(last_update_time - start_time); }

        static inline float frames_per_second() noexcept { return 1000000.0f / (float) delta_time<std::chrono::microseconds>().count(); }
    };
}