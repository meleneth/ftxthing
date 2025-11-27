#include "is_party.hpp"
#include "fairlanes/context/entity_ctx.hpp"
#include "fairlanes/ecs/components/encounter.hpp"
#include "fairlanes/ecs/components/party_member.hpp"
#include "fairlanes/ecs/components/stats.hpp"
#include "fairlanes/fsm/party_loop.hpp"
#include "systems/log.hpp"

namespace fairlanes::ecs::components {
namespace sml = boost::sml;
using fairlanes::fsm::NextEvent;
using fairlanes::fsm::PartyLoop;

IsParty::IsParty(fairlanes::context::EntityCtx ctx, std::string name,
                 entt::entity account)
    : ctx_{std::move(ctx)}, sm_{PartyLoop{}, ctx_}, account_{account},
      name_{std::move(name)} {}

void IsParty::next() {
  spdlog::info("IsParty :: NextEvent{}");
  sm_.process_event(NextEvent{});
}

bool IsParty::needs_town() {

  bool does_need_town = false;
  for_each_member([&](entt::entity member) {
    // auto &party_member = ctx_.reg_.get<PartyMember>(member);
    auto &stats = ctx_.reg_.get<fairlanes::ecs::components::Stats>(member);
    if (!stats.is_alive()) {
      does_need_town = true;
    }
  });
  return does_need_town;
}

bool IsParty::in_combat() {
  using fairlanes::ecs::components::Encounter;

  auto encounter = ctx_.reg_.try_get<Encounter>(ctx_.self_);
  if (!encounter) {
    return false;
  }
  // "in combat" is just "the encounter is not over yet"
  return !encounter->is_over();
}

} // namespace fairlanes::ecs::components
