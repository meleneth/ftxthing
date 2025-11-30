#include "party_loop.hpp"

#include "fairlanes/concepts/encounter_builder.hpp"
#include "fairlanes/context/attack_ctx.hpp"
#include "fairlanes/context/entity_ctx.hpp"
#include "fairlanes/ecs/components/encounter.hpp"
#include "fairlanes/ecs/components/is_party.hpp"
#include "fairlanes/ecs/components/stats.hpp"
#include "fairlanes/skills/thump.hpp"
#include "fairlanes/systems/grant_xp_to_party.hpp"
#include "fairlanes/systems/replenish_party.hpp"
#include "fairlanes/widgets/fancy_log.hpp"
#include <spdlog/spdlog.h>

namespace fairlanes::fsm {
void PartyLoop::enter_idle(fairlanes::context::EntityCtx &ctx) {
  // Mark the party attached to this FSM as idle.
  (void)ctx;
};

void PartyLoop::enter_farming(fairlanes::context::EntityCtx &ctx) {
  // Also set the label for the party tied to this FSM (nice for local UI)

  using fairlanes::concepts::EncounterBuilder;
  using fairlanes::ecs::components::Encounter;
  EncounterBuilder{ctx}.thump_it_out();
};

void PartyLoop::exit_farming(fairlanes::context::EntityCtx &ctx) {
  using fairlanes::ecs::components::Encounter;
  ctx.reg_.remove<Encounter>(ctx.self_);
  ctx.log_.append_plain("Returned to town.");
  entt::handle h{ctx.reg_, ctx.self_};
  using fairlanes::systems::ReplenishParty;
  ReplenishParty::commit(h);
};

void PartyLoop::enter_fixing(fairlanes::context::EntityCtx &ctx) { (void)ctx; };

void PartyLoop::combat_tick(fairlanes::context::EntityCtx &ctx) {
  using fairlanes::skills::Thump;
  Thump in_the_night;

  using fairlanes::ecs::components::Encounter;
  using fairlanes::ecs::components::Stats;
  auto &encounter = ctx.reg_.get<Encounter>(ctx.self_);
  encounter.attackers_->for_each_alive_member([&](entt::entity attacker) {
    auto defender = encounter.defenders_->random_alive_member();
    if (defender) {
      in_the_night.thump(
          fairlanes::context::AttackCtx::make_attack(ctx, attacker, *defender));
    }
  });

  encounter.defenders_->for_each_alive_member([&](entt::entity attacker) {
    auto defender = encounter.attackers_->random_alive_member();
    if (defender) {
      in_the_night.thump(
          fairlanes::context::AttackCtx::make_attack(ctx, attacker, *defender));
    }
  });
};
bool PartyLoop::in_combat(fairlanes::context::EntityCtx &ctx) {
  auto &is_party = ctx.reg_.get<fairlanes::ecs::components::IsParty>(ctx.self_);
  return is_party.in_combat();
}

bool PartyLoop::needs_town(fairlanes::context::EntityCtx &ctx) {
  auto &is_party = ctx.reg_.get<fairlanes::ecs::components::IsParty>(ctx.self_);
  return is_party.needs_town();
}
} // namespace fairlanes::fsm
