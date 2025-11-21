#include <fmt/core.h>

#include "fairlanes/context/app_ctx.hpp"
#include "fairlanes/context/entity_ctx.hpp"
#include "fairlanes/ecs/components/stats.hpp"
#include "fairlanes/widgets/fancy_log.hpp"
#include "track_xp.hpp"

using fairlanes::ecs::components::TrackXP;

/// Closed-form XP curve: sum_{i=1}^{n} i * BASE_XP_VALUE
int TrackXP::xp_for_level(int level_calc) {
  if (level_calc <= 0)
    return 0;
  if (level_calc > 100)
    return 0;
  return BASE_XP_VALUE * (level_calc * (level_calc + 1)) / 2;
}

TrackXP::TrackXP(fairlanes::context::EntityCtx ctx, int starting_xp)
    : ctx_(ctx), xp_(starting_xp), next_level_at(xp_for_level(level_ + 1)) {}

void TrackXP::add_xp(int amount) {
  xp_ += amount;
  using fairlanes::ecs::components::Stats;
  while (next_level_at && xp_ >= next_level_at) {
    ++level_;
    next_level_at = xp_for_level(level_ + 1);
    if (auto info = ctx_.reg_->try_get<Stats>(ctx_.self_)) {
      ctx_.log_->append_markup(fmt::format(
          "[name]({}) Level up! level=[level]({}) in next=[xp]({}) XP",
          info->name_, level_, next_level_at));
    } // spdlog::debug("Level up! level={} next={}", level, next_level_at);
  }
}
