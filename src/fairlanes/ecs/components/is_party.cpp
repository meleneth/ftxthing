#include "is_party.hpp"
#include "fairlanes/context/app_ctx.hpp"
#include "fairlanes/ecs/components/party_member.hpp"
#include "fairlanes/ecs/components/stats.hpp"
#include "fairlanes/ecs/components/track_xp.hpp"
#include "fairlanes/fsm/party_loop.hpp"
#include "systems/log.hpp"

namespace fairlanes::ecs::components {
namespace sml = boost::sml;
using fairlanes::fsm::NextEvent;
using fairlanes::fsm::PartyLoop;

IsParty::IsParty(fairlanes::fsm::PartyLoopCtx context, entt::entity party,
                 std::string name, entt::entity account)
    : ctx_{context}, sm_{PartyLoop{}, ctx_}, account_{account}, self_(party),
      name_{std::move(name)} {}

void IsParty::next() { sm_.process_event(NextEvent{}); }

entt::entity IsParty::create_member(std::string name) {
  auto e = ctx_.reg_->create();
  ctx_.reg_->emplace<fairlanes::ecs::components::PartyMember>(e, ctx_.party_);
  ctx_.reg_->emplace<fairlanes::ecs::components::TrackXP>(
      e, ctx_.entity_context(e), 0);
  ctx_.reg_->emplace<fairlanes::ecs::components::Stats>(e, name);
  return e;
}

bool IsParty::needs_town() {
  auto &reg = *ctx_.reg_; // convenience alias

  auto view = reg.view<PartyMember, Stats>();
  for (auto &&[entity, member, stats] : view.each()) {
    if (member.party_ == ctx_.party_ && stats.hp_ < 1) {
      return true; // any downed member means “needs town”
    }
  }
  return false;
}
} // namespace fairlanes::ecs::components
