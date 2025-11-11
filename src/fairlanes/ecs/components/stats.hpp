#pragma once
#include "fairlanes/concepts/damage.hpp"
#include <entt/entt.hpp>

namespace fairlanes {
class FancyLog;
class AppContext;
} // namespace fairlanes

namespace fairlanes::ecs::components {

using fairlanes::concepts::Damage;
using fairlanes::concepts::Resistances;

struct Stats {
  std::string name_{"Unknown"};
  int hp_ = 10;
  int max_hp_;
  int mp_;
  int max_mp_;
  Resistances resistances_;

  Stats() = default;
  explicit Stats(fairlanes::AppContext &context, std::string name_);
  void take_damage(entt::entity attacker, Damage damage);
};

} // namespace fairlanes::ecs::components
