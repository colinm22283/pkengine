#pragma once

#include <cstring>
#include <memory>

namespace PKEngine::Util {
/**
 * An erasable list
 *
 * Every item created will recieve a Deleter object which
 * will automatically delete the node upon leaving scope
 *
 * This class is move-only.
 */

    template<typename Type>
    class ErasableList {
    protected:
        /**
         * @brief Represents a node with no data, only used for root node
         */
        class EmptyNode {
            friend class ErasableList;

        protected:
            EmptyNode * next;
            EmptyNode * prev;

        public:
            EmptyNode(EmptyNode * _next, EmptyNode * _prev) :
                next(_next),
                prev(_prev) { }
        };

        /**
         * @brief Represents a node with data, this is the main node type used in the erasable list
         */
        class DataNode : public EmptyNode {
            friend class ErasableList;

        protected:
            Type value;

        public:
            DataNode(EmptyNode * _next, EmptyNode * _prev, Type && _value) :
                EmptyNode(_next, _prev),
                value(std::move(_value)) { }
        };

    public:
        /**
         * @brief Erasable list iterator
         */
        class Iterator {
            friend class ErasableList;

        protected:
            EmptyNode *
            node;

            Iterator(EmptyNode * _node) :
                node(_node) { }

        public:
            inline Iterator & operator++() {
                node = node->next;

                return *this;
            }

            inline Iterator operator++(int) {
                node = node->next;

                return Iterator(node->prev);
            }

            inline Iterator & operator--() {
                node = node->prev;

                return *this;
            }

            inline Iterator operator--(int) {
                node = node->prev;

                return Iterator(node->next);
            }

            inline Type & operator*() {
                return (((DataNode *) node)->value);
            }

            inline bool operator==(Iterator itr) {
                return node == itr.node;
            }

            inline bool operator!=(Iterator itr) {
                return node != itr.node;
            }
        };

        /**
         * @brief Erasable list eraser, upon leaving scope will delete the node it is assigned to
         */
        class Eraser {
        protected:
            DataNode *
            node;

        public:
            /**
             * @brief Removes the node and joins the list around it
             */
            ~Eraser() {
                node->prev->next = node->next;
                node->next->prev = node->prev;

                delete node;
            }

            Eraser(DataNode * _node) :
                node(_node) { }

            /**
             * @brief Get the value the eraser represents
             */
            inline Type & get() {
                return node->value;
            }
        };

    protected:
        EmptyNode root;

    public:
        ErasableList() :
            root(&root, &root) { }

        ~ErasableList() {
            root.next->prev = root.prev;
            root.prev->next = root.next;
        }

        ErasableList(ErasableList && old) {
            *this = std::move(old);
        }

        ErasableList & operator=(ErasableList && old) {
            root.next = old.root.next;
            root.prev = old.root.prev;
            return *this;
        }

        // Delete copy constructor
        ErasableList(ErasableList & er) = delete;

        /**
         * @brief Construct a new object and add it to the front of the list
         */
        template<typename... Args>
        [[nodiscard]] Eraser emplace_front(Args && ... args) {
            DataNode * node = new DataNode(root.next, &root, Type(std::forward<Args>(args)...));
            root.next->prev = node;
            root.next = node;

            return Eraser(node);
        }

        /**
         * @brief Returns an iterator at the front of the list
         */
        inline Iterator begin() {
            return Iterator(root.next);
        }

        /**
         * @brief Returns an iterator at the end of the list
         */
        inline Iterator end() {
            return Iterator(&root);
        }
    };
}