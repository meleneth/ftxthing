#pragma once
#include <entt/entt.hpp>

namespace fairlanes {
class FancyLog;
class AppContext;
} // namespace fairlanes
namespace fairlanes::ecs::components {

struct Stats {
  std::string name_{"Unknown"};
  int hp_;
  int max_hp_;

  Stats() = default;
  explicit Stats(fairlanes::AppContext &context, std::string name_);
};

} // namespace fairlanes::ecs::components
