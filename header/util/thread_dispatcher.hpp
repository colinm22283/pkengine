#pragma once

#include <cstdint>
#include <array>
#include <thread>
#include <condition_variable>
#include <chrono>

namespace PKEngine::Util {
    template<std::size_t _thread_count>
    class ThreadDispatcher {
    protected:
        template<typename... Args>
        struct queued_function_t {
            std::tuple<Args&...> arguments;
            void (* function)(Args...);

        };

        struct Thread {
            bool not_awaiting_join = true;

            std::mutex function_mutex;
            void (* function_to_run)() = nullptr;

            std::mutex thread_mutex;
            std::unique_lock<std::mutex> thread_lock = std::unique_lock<std::mutex>(thread_mutex);
            std::condition_variable cv;

            static void thread_loop(std::condition_variable & cv, bool & not_awaiting_join, void (*& function_to_run)(), std::mutex & function_mutex, std::unique_lock<std::mutex> & thread_lock) {
                while (not_awaiting_join) {
                    if (cv.wait_for(thread_lock, std::chrono::milliseconds(10)) == std::cv_status::no_timeout) {
                        function_to_run();
                        thread_lock.unlock();
                        function_mutex.unlock();
                    }
                }
            }

            std::thread thread = std::thread(
                thread_loop,
                std::ref(cv),
                std::ref(not_awaiting_join),
                std::ref(function_to_run),
                std::ref(function_mutex),
                std::ref(thread_lock)
            );

            template<typename... Args>
            inline bool try_call(queued_function_t<Args...> qf) noexcept {
                if (function_mutex.try_lock()) {
                    function_to_run = qf.function;
                    {
                        std::lock_guard<std::mutex> lock(thread_mutex);
                        cv.notify_all();
                    }

                    return true;
                }
                return false;
            }
        };

        std::array<Thread, _thread_count> threads;

    public:
        inline ~ThreadDispatcher() noexcept {
            for (std::size_t i = 0; i < threads.size(); i++) threads[i].not_awaiting_join = false;
            for (std::size_t i = 0; i < threads.size(); i++) threads[i].thread.join();
        }

        template<typename... Args>
        inline void call(void (* function)(Args...), Args... args) noexcept {
            queued_function_t<Args...> qf = {
                .arguments = { args... },
                .function = function,
            };

            for (std::size_t i = 0; true; i++) {
                if (i == threads.size()) i = 0;

                if (threads[i].try_call(qf)) return;
            }
        }

        [[nodiscard]] consteval std::size_t thread_count() const noexcept { return _thread_count; }
    };
}