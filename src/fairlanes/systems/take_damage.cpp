#include "take_damage.hpp"
#include "fairlanes/concepts/damage.hpp"
#include "fairlanes/context/entity_ctx.hpp"
#include "fairlanes/ecs/components/party_member.hpp"
#include "fairlanes/ecs/components/stats.hpp"
#include "fairlanes/ecs/components/track_xp.hpp"
#include "fairlanes/fsm/party_loop_ctx.hpp"
#include "fairlanes/widgets/fancy_log.hpp"
#include "grant_xp_to_party.hpp"

namespace fairlanes::systems {
using fairlanes::context::EntityCtx;

void TakeDamage::commit(entt::entity attacker, EntityCtx ctx,
                        fairlanes::concepts::Damage damage) {
  using fairlanes::ecs::components::Stats;
  auto &target_stats = ctx.reg_->get<Stats>(ctx.self_);

  auto adjusted =
      fairlanes::concepts::apply_resistance(damage, target_stats.resistances_);
  int total = adjusted.physical + adjusted.magical + adjusted.fire +
              adjusted.ice + adjusted.lightning;
  auto &attacker_stats = ctx.reg_->get<Stats>(attacker);

  auto was_alive = target_stats.is_alive();
  target_stats.hp_ = std::max(0, target_stats.hp_ - total);
  if (!target_stats.is_alive() && was_alive) {
    auto party =
        ctx.reg_->try_get<fairlanes::ecs::components::PartyMember>(ctx.self_);

    ctx.log_->append_markup(
        fmt::format("[name]({}) [error](killed) [name]({})!",
                    attacker_stats.name_, target_stats.name_));
    if (party) {
      fairlanes::systems::GrantXPToParty::commit(ctx.entity_context(attacker),
                                                 256);
    }
  }
  (void)attacker;
  // spdlog::info("{} hits {} for {} damage ({} HP left)",
  //            attacker.name(), name_, total, hp_);
}

} // namespace fairlanes::systems
