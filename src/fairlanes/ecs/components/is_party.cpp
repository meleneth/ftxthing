#include "is_party.hpp"
#include "fairlanes/context/app_ctx.hpp"
#include "fairlanes/ecs/components/party_member.hpp"
#include "fairlanes/ecs/components/stats.hpp"
#include "fairlanes/fsm/party_loop.hpp"
#include "systems/log.hpp"

namespace fairlanes::ecs::components {
namespace sml = boost::sml;
using fairlanes::fsm::NextEvent;
using fairlanes::fsm::PartyLoop;

IsParty::IsParty(fairlanes::context::AppCtx &context, entt::entity party,
                 std::string name, entt::entity account)
    : ctx_{&context.reg_, party, context.log_, context.rng_},
      sm_{PartyLoop{}, ctx_}, account_{account}, name_{std::move(name)} {}

void IsParty::next() { sm_.process_event(NextEvent{}); }

bool IsParty::needs_town() {
  auto &reg = *ctx_.reg_; // convenience alias

  auto view = reg.view<PartyMember, Stats>();
  for (auto &&[entity, member, stats] : view.each()) {
    if (member.party_ == ctx_.self_ && stats.hp_ < 1) {
      return true; // any downed member means “needs town”
    }
  }
  return false;
}
} // namespace fairlanes::ecs::components
