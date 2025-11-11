#pragma once
#include <entt/entt.hpp>

namespace fairlanes {
class FancyLog;
class AppContext;
} // namespace fairlanes

namespace fairlanes::concepts {
struct Damage;
}

namespace fairlanes::ecs::components {

using fairlanes::concepts::Damage;

struct Stats {
  std::string name_{"Unknown"};
  int hp_;
  int max_hp_;

  Stats() = default;
  explicit Stats(fairlanes::AppContext &context, std::string name_);
  void take_damage(entt::entity attacker, Damage damage);
};

} // namespace fairlanes::ecs::components
