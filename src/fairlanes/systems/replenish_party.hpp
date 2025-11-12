#pragma once
#include <entt/entt.hpp>

#include "fairlanes/ecs/components/party_member.hpp"
#include "fairlanes/ecs/components/track_xp.hpp"

namespace fairlanes::systems {

class ReplenishParty {
public:
  static void commit(entt::handle party);
};

} // namespace fairlanes::systems
