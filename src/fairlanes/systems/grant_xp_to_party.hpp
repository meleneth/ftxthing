#pragma once
#include <entt/entt.hpp>

#include "fairlanes/context/entity_ctx.hpp"

namespace fairlanes::ecs::components {
struct PartyMember;
struct TrackXp;
} // namespace fairlanes::ecs::components

namespace fairlanes::context {
struct EntityCtx;
}

namespace fairlanes::systems {
using fairlanes::context::EntityCtx;

class GrantXPToParty {
public:
  static void commit(const fairlanes::context::EntityCtx &ctx, int amount);
};

} // namespace fairlanes::systems
