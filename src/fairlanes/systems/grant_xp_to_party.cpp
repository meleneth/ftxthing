#include "grant_xp_to_party.hpp"
#include "fairlanes/context/entity_ctx.hpp"
#include "fairlanes/ecs/components/party_member.hpp"
#include "fairlanes/ecs/components/track_xp.hpp"
#include "fairlanes/fsm/party_loop_ctx.hpp"
#include "fairlanes/widgets/fancy_log.hpp"

namespace fairlanes::systems {
using fairlanes::context::EntityCtx;

void GrantXPToParty::commit(EntityCtx ctx, int amount) {
  using namespace fairlanes::ecs::components;
  auto view = ctx.reg_->view<PartyMember, TrackXP>();
  ctx.log_->append_markup(fmt::format("Party received [xp]({}) XP.", amount));
  for (auto &&[entity, member, track] : view.each()) {
    if (member.party_ == ctx.self_) {
      track.add_xp(amount);
    }
  }
}

} // namespace fairlanes::systems
