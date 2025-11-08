#include <entt/entt.hpp>

#include "fairlanes/ecs/components/is_party.hpp"
#include "tick_party_fsms.hpp"

namespace fairlanes::systems {
void TickPartyFSMs::commit(entt::registry &reg) {
  auto parties = reg.view<fairlanes::ecs::components::IsParty>();

  // Step 1: tick all party FSMs and update “what the party is doing” for UI
  for (auto [party_e, party] : parties.each()) {
    party.next();
  }
}

} // namespace fairlanes::systems
