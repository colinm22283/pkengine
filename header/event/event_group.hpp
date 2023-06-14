#pragma once

#include <thread>

#include <util/const_string.hpp>
#include <util/thread_dispatcher.hpp>

#include <logger/logger.hpp>

#include <thread/dispatchable_thread.hpp>

namespace PKEngine {
    template<Util::ConstString name, typename... Args>
    class _EventGroup {
    protected:
        static constexpr auto logger = FileLogger<Util::ANSI::CyanFg, "EVENT \"" + name + "\"", "pkengine.log">();
        static constexpr auto prefix = "Exception occurred during call to \"" + name + "\"";

        class InternalControl;

        using handler_t = void(* const)(InternalControl &, Args...);

        std::mutex mutex;

        struct EventHandler {
            handler_t handler;
            bool enabled = true;

            EventHandler * next;
            EventHandler * prev;

            inline EventHandler(EventHandler * _next, EventHandler * _prev): next(_next), prev(_prev), handler(nullptr) { }
            inline EventHandler(EventHandler * _next, EventHandler * _prev, handler_t _handler): next(_next), prev(_prev), handler(_handler) { }
        };

        class InternalControl {
        protected:
            EventHandler * event;

            bool should_remove = false;

        public:
            explicit inline InternalControl(EventHandler * _event): event(_event) { }
            inline ~InternalControl() {
                if (should_remove) {
                    event->prev->next = event->next;
                    event->next->prev = event->prev;
                    delete event;
                }
            }

            InternalControl(InternalControl &) = delete;
            InternalControl(InternalControl &&) = delete;

            inline void remove() noexcept { should_remove = true; }
            inline void enable() noexcept { event->enabled = true; }
            inline void disable() noexcept { event->enabled = false; }
        };
        class ExternalControl {
        protected:
            std::mutex & mutex;
            EventHandler * event;

        public:
            inline ExternalControl(std::mutex & _mutex, EventHandler * _event): mutex(_mutex), event(_event) { }

            inline void remove() noexcept {
                std::lock_guard<std::mutex> lock(mutex);

                event->prev->next = event->next;
                event->next->prev = event->prev;
                delete event;
            }

            inline void enable() noexcept { event->enabled = true; }
            inline void disable() noexcept { event->enabled = false; }
        };

        EventHandler head = EventHandler(&tail, nullptr);
        EventHandler tail = EventHandler(nullptr, &head);

        DispatchableThread<void(EventHandler *, EventHandler *, Args...)> thread;

    public:
        inline ExternalControl add(handler_t handler) noexcept {
            std::lock_guard<std::mutex> lock(mutex);

            EventHandler * temp = new EventHandler(head.next, &head, handler);

            head.next->prev = temp;
            head.next = temp;

            return ExternalControl(mutex, temp);
        }

        inline void call(Args... args) {
            std::lock_guard<std::mutex> lock(mutex);

            EventHandler * event = head.next;

            thread.call(
                +[](EventHandler * event, EventHandler * tail, Args... args) {
                    while (event != tail) {
                        if (event->enabled) {
                            InternalControl control(event);

                            try {
                                event->handler(control, args...);
                            }
                            catch (std::exception & e) {
                                logger.error().template log<prefix>();
                                logger.error().template log<"what(): ">() << e.what();
                            }
                        }

                        event = event->next;
                    }
                },
                event, &tail, args...
            );
        }
        inline void operator()(Args... args) { call(std::forward<Args>(args)...); }
    };

    namespace _EventGroupPrivate {
        template<Util::ConstString name, typename... Args>
        struct _EventGroupBuilder;

        template<Util::ConstString name, typename... Args>
        struct _EventGroupBuilder<name, void(Args...)> {
            using EventClass = _EventGroup<name, Args...>;
        };
    }

    template<Util::ConstString name, typename Handler>
    using EventGroup = _EventGroupPrivate::_EventGroupBuilder<name, Handler>::EventClass;
}