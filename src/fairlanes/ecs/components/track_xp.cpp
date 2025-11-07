#include <fmt/core.h>

#include "app/app_context.hpp"
#include "fairlanes/ecs/components/stats.hpp"
#include "track_xp.hpp"
#include "widgets/fancy_log.hpp"

using fairlanes::ecs::components::TrackXP;

/// Closed-form XP curve: sum_{i=1}^{n} i * BASE_XP_VALUE
int TrackXP::xp_for_level(int level_calc) {
  if (level_calc <= 0)
    return 0;
  if (level_calc > 100)
    return 0;
  return BASE_XP_VALUE * (level_calc * (level_calc + 1)) / 2;
}

TrackXP::TrackXP(AppContext &context, int starting_xp)
    : xp(starting_xp), next_level_at(xp_for_level(level + 1)),
      log_(context.log()) {}

void TrackXP::add_xp(entt::handle self, int amount) {
  xp += amount;
  using fairlanes::ecs::components::Stats;
  while (next_level_at && xp >= next_level_at) {
    ++level;
    next_level_at = xp_for_level(level + 1);
    if (auto info = self.try_get<Stats>()) {
      log_.append_markup(fmt::format(
          "[name]({}) Level up! level=[level]({}) in next=[xp]({}) XP",
          info->name_, level, next_level_at));
    } // spdlog::debug("Level up! level={} next={}", level, next_level_at);
  }
}
