#pragma once

#include <atomic>
#include <thread>

namespace PKEngine::Util {
    class SpinLock {
    protected:
        volatile std::atomic<bool> _is_locked;

    public:
        inline SpinLock() : _is_locked(false) { }

        inline void lock() noexcept {
            while (true) {
                if (!_is_locked.exchange(true, std::memory_order_acquire)) {
                    return;
                }
                while (_is_locked.load(std::memory_order_relaxed)) {
                    __builtin_ia32_pause();
                }
            }
        }

        inline void unlock() noexcept {
            _is_locked.store(false, std::memory_order_release);
        }

        inline bool try_lock() noexcept {
            return
                !_is_locked.load(std::memory_order_relaxed) &&
                !_is_locked.exchange(true, std::memory_order_acquire);
        }

        [[nodiscard]] inline bool is_locked() const noexcept { return _is_locked.load(std::memory_order_relaxed); }
    };
}