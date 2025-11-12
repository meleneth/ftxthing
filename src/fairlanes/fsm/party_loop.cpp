#include "party_loop.hpp"

#include "fairlanes/concepts/encounter_builder.hpp"
#include "fairlanes/ecs/components/encounter.hpp"
#include "fairlanes/ecs/components/is_party.hpp"
#include "fairlanes/ecs/components/stats.hpp"
#include "fairlanes/fsm/party_loop_ctx.hpp"
#include "fairlanes/skills/thump.hpp"
#include "fairlanes/systems/grant_xp_to_party.hpp"
#include "fairlanes/systems/replenish_party.hpp"
#include <spdlog/spdlog.h>

namespace fairlanes::fsm {
void PartyLoop::enter_idle(PartyLoopCtx &ctx) {
  // Mark the party attached to this FSM as idle.
  using fairlanes::systems::GrantXPToParty;
  (void)ctx;
  // No more free XP for slackers!
  /*entt::handle h{*ctx.reg_, ctx.party_};
  GrantXPToParty::commit(h, 256);*/
};

void PartyLoop::enter_farming(PartyLoopCtx &ctx) {
  // Also set the label for the party tied to this FSM (nice for local UI)

  using fairlanes::concepts::EncounterBuilder;
  using fairlanes::ecs::components::Encounter;
  EncounterBuilder::thump_it_out(ctx);
};

void PartyLoop::exit_farming(PartyLoopCtx &ctx) {
  using fairlanes::ecs::components::Encounter;
  ctx.reg_->remove<Encounter>(ctx.party_);
  ctx.log_->append_plain("Returned to town.");
  entt::handle h{*ctx.reg_, ctx.party_};
  using fairlanes::systems::ReplenishParty;
  ReplenishParty::commit(h);
};

void PartyLoop::enter_fixing(PartyLoopCtx &ctx) { (void)ctx; };

void PartyLoop::combat_tick(PartyLoopCtx &ctx) {
  using fairlanes::skills::Thump;
  Thump in_the_night;

  using fairlanes::ecs::components::Encounter;
  using fairlanes::ecs::components::Stats;
  auto &encounter = ctx.reg_->get<Encounter>(ctx.party_);
  for (auto player : encounter.players(ctx)) {
    auto defender = encounter.random_alive_enemy(ctx);
    in_the_night.thump(ctx, player, defender);
  }
  for (auto enemy : encounter.enemies_) {
    auto defender = encounter.random_alive_player(ctx);
    in_the_night.thump(ctx, enemy, defender);
  }
};

bool PartyLoop::needs_town(PartyLoopCtx &ctx) {
  using fairlanes::ecs::components::IsParty;
  auto &party = ctx.reg_->get<IsParty>(ctx.party_);
  return party.needs_town();
};

bool PartyLoop::in_combat(PartyLoopCtx &ctx) {
  using fairlanes::ecs::components::Encounter;
  using fairlanes::ecs::components::Stats;
  auto &encounter = ctx.reg_->get<Encounter>(ctx.party_);
  for (auto e : encounter.enemies_) {
    auto &enemy = ctx.reg_->get<Stats>(e);
    if (enemy.hp_ >= 1) {
      return true;
    }
  }
  return false;
};
} // namespace fairlanes::fsm
