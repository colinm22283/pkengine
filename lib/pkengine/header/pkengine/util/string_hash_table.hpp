#pragma once

#include <array>
#include <forward_list>
#include <cstring>
#include <string>

namespace PKEngine::Util {
    template<typename T, std::size_t bucket_count>
    class StringHashTable {
    protected:
        static inline std::size_t hash(const char * str) noexcept {
            std::size_t mult = 1, sum = 0;
            for (; *str != '\0'; str++) {
                sum += mult * *str;
                mult *= 2;
            }
            return sum;
        }

        struct entry_t {
            std::string tag;
            T value;
        };

        std::array<std::forward_list<entry_t>, bucket_count> buckets;

    public:
        inline T & search(const char * tag) {
            std::size_t idx = hash(tag) % bucket_count;
            for (auto & ele : buckets[idx]) if (ele.tag == tag) return ele.value;
        }

        inline T & push(const char * tag, T & value) {
            std::size_t idx = hash(tag) % bucket_count;
            return buckets[idx].push_front(entry_t { .tag = tag, .value = value }).value;
        }
        inline T & push(const char * tag, T && value) {
            std::size_t idx = hash(tag) % bucket_count;
            return buckets[idx].push_front(entry_t { .tag = tag, .value = std::forward(value) }).value;
        }
        template<typename... Args>
        inline T & emplace(const char * tag, Args &&... args) {
            std::size_t idx = hash(tag) % bucket_count;
            return buckets[idx].push_front(entry_t { .tag = tag, .value = T(std::forward<Args>(args)...) }).value;
        }
    };
}