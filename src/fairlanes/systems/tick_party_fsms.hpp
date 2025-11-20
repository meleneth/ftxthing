#pragma once
#include <entt/entt.hpp>

#include "fairlanes/ecs/components/party_member.hpp"
#include "fairlanes/ecs/components/track_xp.hpp"

namespace fairlanes::systems {
class TickPartyFSMs {
public:
  static void commit(entt::registry *reg);
};

} // namespace fairlanes::systems
