#include "take_damage.hpp"
#include "fairlanes/concepts/damage.hpp"
#include "fairlanes/context/attack_ctx.hpp"
#include "fairlanes/ecs/components/party_member.hpp"
#include "fairlanes/ecs/components/stats.hpp"
#include "fairlanes/ecs/components/track_xp.hpp"
#include "fairlanes/widgets/fancy_log.hpp"
#include "grant_xp_to_party.hpp"

namespace fairlanes::systems {
using fairlanes::context::EntityCtx;

void TakeDamage::commit(fairlanes::context::AttackCtx &ctx) {
  using fairlanes::ecs::components::Stats;
  auto &defender_stats = ctx.reg_.get<Stats>(ctx.defender_);

  auto adjusted = fairlanes::concepts::apply_resistance(
      ctx.damage_, defender_stats.resistances_);
  int total = adjusted.physical + adjusted.magical + adjusted.fire +
              adjusted.ice + adjusted.lightning;
  auto &attacker_stats = ctx.reg_.get<Stats>(ctx.attacker_);

  auto was_alive = defender_stats.is_alive();
  defender_stats.hp_ = std::max(0, defender_stats.hp_ - total);
  if (!defender_stats.is_alive() && was_alive) {
    auto party = ctx.reg_.try_get<fairlanes::ecs::components::PartyMember>(
        ctx.attacker_);

    ctx.log_.append_markup(fmt::format("[name]({}) [error](killed) [name]({})!",
                                       attacker_stats.name_,
                                       defender_stats.name_));
    if (party) {
      fairlanes::systems::GrantXPToParty::commit(
          ctx.entity_context(ctx.attacker_), 256);
    }
  }
  // spdlog::info("{} hits {} for {} damage ({} HP left)",
  //            attacker.name(), name_, total, hp_);
}

} // namespace fairlanes::systems
