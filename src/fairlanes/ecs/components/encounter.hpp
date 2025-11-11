#pragma once
#include <entt/entt.hpp>
#include <vector>

#include "app/app_context.hpp"
#include "fairlanes/fsm/party_loop_ctx.hpp"

namespace fairlanes::ecs::components {
using fairlanes::fsm::PartyLoopCtx;
struct Encounter {

  entt::entity party_{entt::null};    // the single party in this encounter
  std::vector<entt::entity> enemies_; // enemy entities participating
  std::vector<entt::entity> players(PartyLoopCtx &ctx_);
  entt::entity random_alive_enemy(PartyLoopCtx &ctx_);
  entt::entity random_alive_player(PartyLoopCtx &ctx_);
  Encounter(PartyLoopCtx &context);
};

struct InEncounter {                   // attach to the party
  entt::entity encounter_{entt::null}; // backlink to the encounter entity
};

} // namespace fairlanes::ecs::components
