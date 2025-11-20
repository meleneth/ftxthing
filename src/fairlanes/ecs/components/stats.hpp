#pragma once
#include <entt/entt.hpp>

#include "fairlanes/concepts/damage.hpp"

namespace fairlanes::ecs::components {

using fairlanes::concepts::Damage;
using fairlanes::concepts::Resistances;

struct Stats {
  int hp_ = 10;
  int max_hp_ = 10;
  int mp_ = 0;
  int max_mp_ = 0;
  Resistances resistances_;
  std::string name_{"Unknown"};

  explicit Stats(std::string name_);

  bool is_alive();
};

} // namespace fairlanes::ecs::components
