#pragma once

#include "transform.hpp"

namespace PKEngine {
    class RelativeTransform : public Transform {
    protected:
        struct {
            Vector3 position, rotation, scale;
        } absolute_cache;

        const Transform & parent_transform;

    public:
        inline RelativeTransform(EngineNode & _parent, const Transform & _parent_transform):
          Transform(_parent),
          parent_transform(_parent_transform) { }

        Vector3 & absolute_position = absolute_cache.position;
        Vector3 & absolute_rotation = absolute_cache.rotation;
        Vector3 & absolute_scale = absolute_cache.scale;

        inline void sync_update() override {
            absolute_cache.position = parent_transform.absolute_position + position;
            absolute_cache.rotation = parent_transform.absolute_rotation + rotation;
            absolute_cache.scale = parent_transform.absolute_scale + scale;
        }
    };
}