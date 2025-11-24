#include "fairlanes/ecs/components/party_member.hpp"
#include "fairlanes/ecs/components/track_xp.hpp"
#include "fairlanes/widgets/fancy_log.hpp"

#include "grant_xp_to_party.hpp"

namespace fairlanes::systems {
using fairlanes::context::EntityCtx;

void GrantXPToParty::commit(const EntityCtx &ctx, int amount) {
  using namespace fairlanes::ecs::components;
  auto view = ctx.reg_.view<PartyMember, TrackXP>();
  ctx.log_.append_markup(fmt::format("Party received [xp]({}) XP.", amount));
  for (auto &&[entity, member, track] : view.each()) {
    if (member.party_ == ctx.self_) {
      entt::handle h{ctx.reg_, entity};
      track.add_xp(h, amount);
    }
  }
}

} // namespace fairlanes::systems
