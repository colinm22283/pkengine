#pragma once

#include <memory>
#include <forward_list>

#include "pkengine/node/base_node.hpp"

namespace PKEngine {
    class _ObjectTree {
    protected:
        class Node {
            friend class _ObjectTree;
        protected:
            std::unique_ptr<BaseNode> data;
            std::forward_list<Node> branches;

            inline void start() const noexcept {
                data->start();
                for (auto & ele : branches) ele.start();
            }
            inline void update() const noexcept {
                data->update();
                for (auto & ele : branches) ele.update();
            }

        public:
            inline Node(auto && uni_ptr): data(std::move(uni_ptr)) { }
        };

        template<typename T>
        class NodeAccessor : public Node {
        public:
            inline operator T &() noexcept { return (T &) *(data.get()); }
            inline T * operator->() noexcept { return (T *) data.get(); }

            template<typename _T, typename... Args>
            inline NodeAccessor<_T> & add(Args &&... args) {
                return (NodeAccessor<_T> &) branches.emplace_front(std::make_unique<_T>(std::forward<Args>(args)...));
            }
        };

        std::forward_list<Node> branches;

    public:
        template<typename T, typename... Args>
        inline NodeAccessor<T> & add(Args &&... args) {
            return (NodeAccessor<T> &) branches.emplace_front(std::make_unique<T>(std::forward<Args>(args)...));
        }

        inline void start() const noexcept {
            for (auto & ele : branches) ele.start();
        }
        inline void update() const noexcept {
            for (auto & ele : branches) ele.update();
        }
    };

    extern _ObjectTree object_tree;
}