#pragma once
#include <entt/entt.hpp>
#include <vector>

#include "fairlanes/context/entity_ctx.hpp"

namespace fairlanes::ecs::components {
using fairlanes::context::EntityCtx;
struct Encounter {

  entt::entity party_{entt::null};    // the single party in this encounter
  std::vector<entt::entity> enemies_; // enemy entities participating
  fairlanes::widgets::FancyLog &log_;

  std::vector<entt::entity> players(EntityCtx &ctx_);
  entt::entity random_alive_enemy(EntityCtx &ctx_);
  entt::entity random_alive_player(EntityCtx &ctx_);
  Encounter(EntityCtx &context);
  void finalize(entt::registry &reg, entt::entity self) const;
};

struct InEncounter {                   // attach to the party
  entt::entity encounter_{entt::null}; // backlink to the encounter entity
};

void on_encounter_destroy(entt::registry &reg, entt::entity e);
void install_encounter_hooks(entt::registry &reg);
} // namespace fairlanes::ecs::components
