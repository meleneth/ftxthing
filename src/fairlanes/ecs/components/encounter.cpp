#include "encounter.hpp"
#include "fairlanes/concepts/team.hpp"
#include "fairlanes/ecs/components/is_party.hpp"
#include "fairlanes/ecs/components/party_member.hpp"
#include "fairlanes/ecs/components/stats.hpp"
#include "fairlanes/systems/take_damage.hpp"
#include "fairlanes/widgets/fancy_log.hpp"
#include "systems/random_hub.hpp"

namespace fairlanes::ecs::components {

Encounter::Encounter(EncounterCtx context) : ctx_(std::move(context)) {
  setup_combat_listeners();
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

void Encounter::setup_combat_listeners() {
  using Id = fairlanes::combat::CombatEventId;
  using CombatEvent = fairlanes::combat::CombatEvent;
  using fairlanes::ecs::components::Stats;
  using fairlanes::systems::TakeDamage;

  combat_bus_.appendListener(Id::ApplyDamage, [this](const CombatEvent &event) {
    using fairlanes::ecs::components::Stats;
    auto &stats = ctx_.reg_.get<Stats>(event.target);
    if (stats.is_alive()) {
      stats.hp_ = stats.hp_ - event.damage_.sum();
    }

    if (stats.hp_ <= 0) {
      CombatEvent death{.type = Id::Death,
                        .source = event.source,
                        .target = event.target,
                        .damage_ = event.damage_};
      combat_bus_.dispatch(Id::Death, death);
    }
  });

  combat_bus_.appendListener(Id::Death, [this](const CombatEvent &event) {
    (void)event;
    // central death handling: flags, team removal, etc.
    // for now you can stub or preserve existing logic
  });
}

} // namespace fairlanes::ecs::components
