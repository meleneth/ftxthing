#pragma once
#include "fairlanes/ecs/fwd.hpp"

namespace fairlanes::concepts {
class Encounter;
}
namespace fairlanes::ecs::components {
using fairlanes::concepts::Encounter;
// Marks an entity as an Encounter
struct IsEncounter {
  std::shared_ptr<Encounter> encounter_;
};

} // namespace fairlanes::ecs::components
