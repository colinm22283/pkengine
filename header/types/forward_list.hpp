#pragma once

#include <algorithm>

#include <types/optional_pointer.hpp>

namespace PKEngine {
    template<typename T>
    class ForwardList {
    protected:
        struct Node {
            OptionalPtr<Node> next;
            T data;

            template<typename... Args>
            inline explicit Node(OptionalPtr<Node> _next, Args&&... args):
              next(_next), data(std::forward<Args>(args)...) { }

            inline ~Node() { delete next; }
        };

    public:
        OptionalPtr<Node> head;

        inline ForwardList(): head() { }
        inline ~ForwardList() { delete head; }

        inline ForwardList(ForwardList &) = delete;
        inline ForwardList(ForwardList && _other) noexcept {
            ForwardList & other = std::move(_other);

            this->head = other.head;
            other.head = nullptr;
        }

        template<typename... Args>
        inline T & emplace_front(Args&&... args) {
            Node * temp = new Node(head, std::forward<Args>(args)...);
            return (head = temp)->data;
        }
    };
}