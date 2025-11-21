#pragma once
#include <entt/entt.hpp>

namespace fairlanes::context {
struct AppCtx;
}

namespace fairlanes::ecs {
using Registry = entt::registry;
using Entity = entt::entity;

// Small helpers
inline bool alive(const Registry &reg, Entity e) { return reg.valid(e); }
} // namespace fairlanes::ecs
