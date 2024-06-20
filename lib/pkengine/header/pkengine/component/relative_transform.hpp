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
          Transform(_parent, absolute_cache.position, absolute_cache.rotation, absolute_cache.scale),
          parent_transform(_parent_transform) { }

        inline void pre_update() override {
            absolute_cache.position = parent_transform.absolute_position + position;
            absolute_cache.rotation = parent_transform.absolute_rotation + rotation;
            absolute_cache.scale = parent_transform.absolute_scale + scale;
        }
    };
}