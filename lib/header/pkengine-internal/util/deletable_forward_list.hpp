#pragma once

#include <algorithm>

namespace PKEngine::Util {
    template<typename T>
    class DeletableForwardList {
    protected:
        struct node_t {
            T value;
            node_t * next;
        };

        node_t * head = nullptr;

        class iterator_t {
        protected:
            node_t * prev;
            node_t * node;

        public:
            explicit inline iterator_t(node_t * _node) noexcept: prev(_node), node(_node) { }

            inline T & operator*() const noexcept { return node->value; }
            inline T * operator->() const noexcept { return &node->value; }
            inline bool operator==(iterator_t & n) const noexcept { return node == n.node; }
            inline bool operator!=(iterator_t & n) const noexcept { return node != n.node; }
            inline bool operator==(iterator_t && n) const noexcept { return node == n.node; }
            inline bool operator!=(iterator_t && n) const noexcept { return node != n.node; }
            inline void operator++(int) noexcept { node = node->next; }
            inline void operator++() noexcept { ++*this; }

            /// @brief remove the previously iterated node
            inline void remove() const noexcept {
                
            }
        };

    public:
        inline ~DeletableForwardList() {
            node_t next = head->next;
            for (node_t * node = head; node != nullptr; node = next) {
                next = node->next;
                delete node;
            }
        }

        inline T & push_front(T & value) { head = new node_t { .value = value, .next = head, }; return head->value; }
        inline T & push_front(T && value) { head = new node_t { .value = std::move(value), .next = head, }; return head->value; }
        template<typename... Args> inline T & emplace_front(Args &&... args) { head = new node_t { .value = T(std::forward<Args>(args)...), .next = head, }; return head->value; }

        [[nodiscard]] inline bool empty() const noexcept { return head == nullptr; }

        [[nodiscard]] inline iterator_t begin() const noexcept { return iterator_t(head); }
        [[nodiscard]] inline iterator_t end() const noexcept { return iterator_t(nullptr); }
    };
}