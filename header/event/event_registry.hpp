#pragma once

#include "event_group.hpp"

namespace PKEngine {

    class EventRegistry {
    public:
        const EventGroup<"Start Handler", void(int)> start;
        const EventGroup<"Update Handler", void(int)> update;
    };
}