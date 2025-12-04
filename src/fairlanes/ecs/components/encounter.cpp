#include "encounter.hpp"
#include "fairlanes/combat/combat_events.hpp"
#include "fairlanes/concepts/team.hpp"
#include "fairlanes/ecs/components/is_party.hpp"
#include "fairlanes/ecs/components/party_member.hpp"
#include "fairlanes/ecs/components/stats.hpp"
#include "fairlanes/widgets/fancy_log.hpp"
#include "systems/random_hub.hpp"

namespace fairlanes::ecs::components {

Encounter::Encounter(EntityCtx context) : ctx_(std::move(context)) {
  setup_combat_bus_listeners();
  setup_ready_listeners();
};

void on_encounter_destroy(entt::registry &reg, entt::entity e) {
  auto &enc = reg.get<Encounter>(e); // valid: signal fires before removal
  enc.finalize();
}

void install_encounter_hooks(entt::registry &reg) {
  reg.on_destroy<Encounter>().connect<&on_encounter_destroy>();
}

void Encounter::finalize() {
  for (auto e_cleanup : e_to_cleanup_) {
    ctx_.reg_.destroy(e_cleanup);
  }

  // ctx_.log_.append_markup(fmt::format("Encounter {} finalized and cleaned
  // up",
  //                                     int(entt::to_integral(ctx_.self_))));
}

bool Encounter::has_alive_enemies() {
  using fairlanes::ecs::components::Stats;

  for (auto e : e_to_cleanup_) {
    if (!ctx_.reg_.valid(e) || !ctx_.reg_.all_of<Stats>(e)) {
      continue; // stale or already destroyed, ignore
    }

    auto &enemy = ctx_.reg_.get<Stats>(e);
    if (enemy.is_alive()) {
      return true;
    }
  }

  return false;
}

bool Encounter::is_over() {
  // For now, "over" simply means there are no alive enemies.
  return !has_alive_enemies();
}

void Encounter::setup_combat_bus_listeners() {
  using namespace fairlanes::combat;
  using Id = CombatEventId;

  // Example: centralize ApplyDamage handling
  combat_bus_.appendListener(Id::ApplyDamage, [this](const CombatEvent &e) {
    using fairlanes::ecs::components::Stats;
    auto &stats = reg_.get<Stats>(e.target);
    stats.take_damage(e.amount);

    if (stats.hp_ <= 0) {
      CombatEvent death{
          .type = CombatEventId::Death,
          .source = e.source,
          .target = e.target,
          .amount = 0,
      };
      combat_bus_.dispatch(CombatEventId::Death, death);
    }
  });

  // Example: Death handler (can later emit XP, cleanup, etc.)
  combat_bus_.appendListener(Id::Death, [this](const CombatEvent &e) {
    // mark entity dead, remove from teams, etc.
    // (keep it minimal for now)
  });
}

void Encounter::setup_ready_listeners() {
  using Id = ReadyEventId;

  // When a combatant leaves the ready queue, start their attack flow.
  // This is the "one at a time" lane.
  ready_combatants_.appendListener(Id::CombatantReady,
                                   [this](const ReadyEvent &e) {
                                     // Mark the lane busy until the attack
                                     // completes.
                                     lane_busy_ = true;

                                     // Here you would typically:
                                     //   - pick target
                                     //   - notify that actor's FSM (BeginAttack
                                     //   event)
                                     //   - drive animation / timing
                                     //
                                     // For now, this can just be a stub:
                                     // start_attack_for(e.actor);

                                     // When the attack *actually* finishes
                                     // (e.g. FSM or animation system), you must
                                     // eventually set lane_busy_ = false so the
                                     // next ready actor can be processed.
                                   });
}

void Encounter::update(double dt) {
  // later: tick FSMs, regen, buffs etc.

  // Process the ready_combatants queue *very slowly*:
  // at most one actor at a time, and only when the lane is not busy.
  process_ready_lane();

  // combat_bus_ doesn't queue — nothing to "update" here;
  // its events are dispatched immediately when dispatch() is called.
}

void Encounter::process_ready_lane() {
  if (lane_busy_) {
    // current attacker is still "doing their thing"
    return;
  }

  // Process at most one "ready" event.
  // If the queue is empty, processOne() returns false.
  ready_combatants_.processOne();
}

void Encounter::emit_apply_damage(entt::entity source, entt::entity target,
                                  int amount) {
  CombatEvent evt{
      .type = CombatEventId::ApplyDamage,
      .source = source,
      .target = target,
      .amount = amount,
  };

  combat_bus_.dispatch(CombatEventId::ApplyDamage, evt);
}

void Encounter::mark_combatant_ready(entt::entity actor) {
  ReadyEvent evt{
      .type = ReadyEventId::CombatantReady,
      .actor = actor,
  };

  ready_combatants_.enqueue(ReadyEventId::CombatantReady, evt);
}

} // namespace fairlanes::ecs::components
