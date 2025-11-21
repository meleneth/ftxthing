#pragma once
#include <entt/entt.hpp>
#include <vector>

#include "fairlanes/context/encounter_ctx.hpp"
#include "fairlanes/fsm/party_loop_ctx.hpp"
#include "fairlanes/widgets/fancy_log.hpp"

namespace fairlanes::ecs::components {
using fairlanes::fsm::PartyLoopCtx;
struct Encounter {
  fairlanes::context::EncounterCtx ctx_;
  entt::entity party_{entt::null};    // the single party in this encounter
  std::vector<entt::entity> enemies_; // enemy entities participating
  std::vector<entt::entity> players();

  entt::entity random_alive_enemy();
  entt::entity random_alive_player();

  Encounter(fairlanes::context::EncounterCtx context);
  void finalize(entt::registry &reg, entt::entity self) const;
};

struct InEncounter {                   // attach to the party
  entt::entity encounter_{entt::null}; // backlink to the encounter entity
};

void on_encounter_destroy(entt::registry &reg, entt::entity e);
void install_encounter_hooks(entt::registry &reg);
} // namespace fairlanes::ecs::components
