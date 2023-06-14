#pragma once

#include <thread>
#include <mutex>
#include <condition_variable>

namespace PKEngine {
    namespace _DispatchableThreadPrivate {
        template<typename... Arguments> class _DispatchableThread;

        template<typename... Arguments>
        class _DispatchableThread<void(Arguments...)> {
        protected:
            using function_t = void (*)(Arguments...);

            volatile bool awaiting_join = true;

            function_t function_to_call = nullptr;
            std::tuple<Arguments...> function_args;

            std::mutex primary_mutex;

            std::mutex thread_mutex;
            std::condition_variable cv;

            static void thread_loop(
                volatile bool & awaiting_join,
                function_t & function_to_call,
                std::tuple<Arguments...> & function_args,
                std::mutex & primary_mutex,
                std::mutex & thread_mutex,
                std::condition_variable & cv
            ) {
                std::unique_lock<std::mutex> lock(thread_mutex);

                awaiting_join = false;

                while (!awaiting_join) {
                    if (cv.wait_for(lock, std::chrono::seconds(1)) == std::cv_status::no_timeout) {
                        std::apply(function_to_call, function_args);

                        primary_mutex.unlock();
                    }
                }
            }

            std::thread thread = std::thread(
                thread_loop,
                std::ref(awaiting_join),
                std::ref(function_to_call),
                std::ref(function_args),
                std::ref(primary_mutex),
                std::ref(thread_mutex),
                std::ref(cv)
            );

        public:
            inline _DispatchableThread() {
                while (awaiting_join) { }

                std::lock_guard<std::mutex> lock(thread_mutex);
            }

            inline ~_DispatchableThread() {
                primary_mutex.lock();

                awaiting_join = true;
                thread.join();
            }

            inline void call(function_t function, Arguments... arguments) noexcept {
                primary_mutex.lock();

                function_to_call = function;
                function_args = { arguments... };

                std::lock_guard<std::mutex> lock(thread_mutex);

                cv.notify_all();
            }
        };
    }

    template<typename FunctionType>
    using DispatchableThread = _DispatchableThreadPrivate::_DispatchableThread<FunctionType>;
}