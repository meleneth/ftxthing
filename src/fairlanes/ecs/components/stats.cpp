#include <fmt/core.h>

#include "fairlanes/concepts/damage.hpp"
#include "fairlanes/context/app_ctx.hpp"
#include "fairlanes/ecs/components/party_member.hpp"
#include "fairlanes/ecs/components/stats.hpp"
#include "fairlanes/fsm/party_loop_ctx.hpp"
#include "fairlanes/systems/grant_xp_to_party.hpp"
#include "fairlanes/widgets/fancy_log.hpp"
#include "stats.hpp"

using fairlanes::ecs::components::Stats;

Stats::Stats(fairlanes::context::AppCtx &context, std::string name)
    : name_(name) {
  (void)context;
}
using fairlanes::fsm::PartyLoopCtx;
void Stats::take_damage(PartyLoopCtx &ctx, entt::entity attacker,
                        entt::entity target, Damage damage) {
  auto adjusted = fairlanes::concepts::apply_resistance(damage, resistances_);
  int total = adjusted.physical + adjusted.magical + adjusted.fire +
              adjusted.ice + adjusted.lightning;
  auto was_alive = is_alive(ctx);
  hp_ = std::max(0, hp_ - total);
  if (!is_alive(ctx) && was_alive) {
    auto &attacker_stats = ctx.reg_->get<Stats>(attacker);
    auto &target_stats = ctx.reg_->get<Stats>(target);
    auto party = ctx.reg_->try_get<PartyMember>(target);

    ctx.log_->append_markup(
        fmt::format("[name]({}) [error](killed) [name]({})!",
                    attacker_stats.name_, target_stats.name_));
    if (party) {
      using fairlanes::systems::GrantXPToParty;
      entt::handle h{*ctx.reg_, ctx.party_};
      GrantXPToParty::commit(ctx, h, 256);
    }
  }
  (void)attacker;
  // spdlog::info("{} hits {} for {} damage ({} HP left)",
  //            attacker.name(), name_, total, hp_);
}

bool Stats::is_alive(PartyLoopCtx &ctx) {
  (void)ctx;
  return hp_ > 0;
}
