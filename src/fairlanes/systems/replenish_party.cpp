#include "replenish_party.hpp"

#include <entt/entt.hpp>

#include "fairlanes/ecs/components/party_member.hpp"
#include "fairlanes/ecs/components/stats.hpp"
#include "fairlanes/ecs/components/track_xp.hpp"

namespace fairlanes::systems {

void ReplenishParty::commit(entt::handle party) {
  using namespace fairlanes::ecs::components;
  auto &reg = *party.registry();
  auto view = reg.view<PartyMember, Stats>();
  for (auto &&[entity, member, stats] : view.each()) {
    if (member.party_ == party.entity()) {
      stats.hp_ = stats.max_hp_;
      stats.mp_ = stats.max_mp_;
    }
  }
}

} // namespace fairlanes::systems
