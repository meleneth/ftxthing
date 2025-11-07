#include "grant_xp_to_party.hpp"

#include <entt/entt.hpp>

#include "fairlanes/ecs/components/party_member.hpp"
#include "fairlanes/ecs/components/track_xp.hpp"

namespace fairlanes::systems {

void GrantXPToParty::commit(entt::handle party, int amount) {
  using namespace fairlanes::ecs::components;
  auto &reg = *party.registry();
  auto view = reg.view<PartyMember, TrackXP>();
  for (auto &&[entity, member, track] : view.each()) {
    if (member.party_ == party.entity()) {
      entt::handle h{reg, entity};
      track.add_xp(h, amount);
    }
  }
}

} // namespace fairlanes::systems
