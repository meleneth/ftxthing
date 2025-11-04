#pragma once
#include "fairlanes/ecs/fwd.hpp"

namespace fairlanes::ecs::components {

// Tag a player/member that is participating in a specific encounter
struct PlayerEncounter {
  fairlanes::ecs::Entity encounter{};
};

// Tag a monster that is participating in a specific encounter
struct MonsterEncounter {
  fairlanes::ecs::Entity encounter{};
};

} // namespace fairlanes::ecs::components
