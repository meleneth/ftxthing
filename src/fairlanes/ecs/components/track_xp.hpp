#pragma once
#include "fairlanes/ecs/fwd.hpp"
#include "fairlanes/fsm/party_loop.hpp"
#include "fairlanes/fsm/party_loop_ctx.hpp"
#include "systems/log.hpp"

#include <spdlog/spdlog.h>

namespace fairlanes::ecs::components {

struct TrackXP {
  static constexpr int BASE_XP_VALUE = 256;

  int level = 1;
  int xp = 0;
  int next_level_at = BASE_XP_VALUE * ((level + 1) * (level + 2)) / 2;

  /// Closed-form XP curve: sum_{i=1}^{n} i * BASE_XP_VALUE
  static constexpr int xp_for_level(int level_calc) noexcept {
    if (level_calc <= 0)
      return 0;
    if (level_calc > 100)
      return 0;
    return BASE_XP_VALUE * (level_calc * (level_calc + 1)) / 2;
  }

  TrackXP() = default;
  explicit TrackXP(int starting_xp)
      : xp(starting_xp), next_level_at(xp_for_level(level + 1)) {}

  void add_xp(int amount) {
    xp += amount;
    while (xp >= next_level_at) {
      ++level;
      next_level_at = xp_for_level(level + 1);
      spdlog::debug("Level up! level={} next={}", level, next_level_at);
    }
  }
};

} // namespace fairlanes::ecs::components
