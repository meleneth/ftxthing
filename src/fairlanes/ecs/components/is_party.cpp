#include "is_party.hpp"
#include "app/app_context.hpp"
#include "fairlanes/context/entity_ctx.hpp"
#include "fairlanes/ecs/components/party_member.hpp"
#include "fairlanes/ecs/components/stats.hpp"
#include "fairlanes/fsm/party_loop.hpp"
#include "systems/log.hpp"

namespace fairlanes::ecs::components {
namespace sml = boost::sml;
using fairlanes::fsm::NextEvent;
using fairlanes::fsm::PartyLoop;

IsParty::IsParty(AppContext &context, entt::entity party, std::string name,
                 entt::entity account, fairlanes::context::EntityCtx ctx2)
    : ctx_{&context.registry(), party, context.log(), context.rng()},
      sm_{PartyLoop{}, ctx_}, account_{account}, name_{std::move(name)},
      ctx2_(std::move(ctx2)) {}

void IsParty::next() { sm_.process_event(NextEvent{}); }

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
