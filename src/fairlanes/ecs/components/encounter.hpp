#pragma once
#include <entt/entt.hpp>
#include <vector>

namespace fairlanes::ecs::components {

struct Encounter {
  entt::entity party_{entt::null};    // the single party in this encounter
  std::vector<entt::entity> enemies_; // enemy entities participating
};

struct InEncounter {                   // attach to the party
  entt::entity encounter_{entt::null}; // backlink to the encounter entity
};

} // namespace fairlanes::ecs::components
