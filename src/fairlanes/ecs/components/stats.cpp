#include <fmt/core.h>

#include "app/app_context.hpp"
#include "fairlanes/concepts/damage.hpp"
#include "fairlanes/ecs/components/stats.hpp"
#include "stats.hpp"
#include "widgets/fancy_log.hpp"

using fairlanes::ecs::components::Stats;

Stats::Stats(AppContext &context, std::string name) : name_(name) {
  (void)context;
}

void Stats::take_damage(entt::entity attacker, Damage damage) {
  auto adjusted = fairlanes::concepts::apply_resistance(damage, resistances_);
  int total = adjusted.physical + adjusted.magical + adjusted.fire +
              adjusted.ice + adjusted.lightning;

  hp_ = std::max(0, hp_ - total);
  (void)attacker;
  // spdlog::info("{} hits {} for {} damage ({} HP left)",
  //            attacker.name(), name_, total, hp_);
}
