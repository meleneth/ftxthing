#pragma once
#include "fairlanes/concepts/damage.hpp"
#include <entt/entt.hpp>
namespace fairlanes::widgets {
class FancyLog;
}
namespace fairlanes::fsm {
class PartyLoopCtx;
}

namespace fairlanes::ecs::components {

using fairlanes::concepts::Damage;
using fairlanes::concepts::Resistances;

struct Stats {
  std::string name_{"Unknown"};
  int hp_ = 10;
  int max_hp_ = 10;
  int mp_ = 0;
  int max_mp_ = 0;
  Resistances resistances_;

  Stats() = default;
  explicit Stats(std::string name_);
  bool is_alive();
};

} // namespace fairlanes::ecs::components
