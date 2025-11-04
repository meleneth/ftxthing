#pragma once
#include "fairlanes/ecs/fwd.hpp"

namespace fairlanes::ecs::components {

// Marks an entity as an Encounter
struct IsEncounter {
  fairlanes::ecs::Entity party_a{};
  fairlanes::ecs::Entity party_b{};
};

} // namespace fairlanes::ecs::components
