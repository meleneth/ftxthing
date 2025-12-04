#pragma once
#include <entt/entt.hpp>
#include <vector>

#include "fairlanes/combat/combat_buses.hpp"
#include "fairlanes/concepts/team.hpp"
#include "fairlanes/context/entity_ctx.hpp"
#include "fairlanes/ecs/components/is_party.hpp"
#include "fairlanes/ecs/components/stats.hpp"


namespace fairlanes::ecs::components {

struct Encounter {
  fairlanes::context::EntityCtx ctx_;
  std::unique_ptr<fairlanes::concepts::Team> attackers_ = nullptr;
  std::unique_ptr<fairlanes::concepts::Team> defenders_ = nullptr;
  std::vector<entt::entity> e_to_cleanup_;
  fairlanes::combat::CombatBus combat_bus_; // fire-now, no queue
  fairlanes::combat::ReadyCombatantsQueue
      ready_combatants_; // long-lived "ready" lane

  bool has_alive_enemies();
  bool is_over();
  void setup_combat_bus_listeners();
  void setup_ready_listeners();
  void update(double);
  void process_ready_lane();
  void emit_apply_damage(entt::entity source, entt::entity target, int amount);

  Encounter(EntityCtx context);
  void finalize();
};

struct InEncounter {                   // attach to the party
  entt::entity encounter_{entt::null}; // backlink to the encounter entity
};

void on_encounter_destroy(entt::registry &reg, entt::entity e);
void install_encounter_hooks(entt::registry &reg);

} // namespace fairlanes::ecs::components
