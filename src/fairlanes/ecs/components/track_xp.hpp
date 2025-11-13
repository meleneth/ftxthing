#pragma once
#include <entt/entt.hpp>
namespace fairlanes::widgets {
class FancyLog;
}
namespace fairlanes {
class AppContext;
} // namespace fairlanes
namespace fairlanes::ecs::components {

struct TrackXP {
  static constexpr int BASE_XP_VALUE = 256;

  int level_ = 1;
  int xp_ = 0;
  int next_level_at = BASE_XP_VALUE * ((level_ + 1) * (level_ + 2)) / 2;
  std::shared_ptr<fairlanes::widgets::FancyLog> log_;

  /// Closed-form XP curve: sum_{i=1}^{n} i * BASE_XP_VALUE
  int xp_for_level(int level_calc);
  TrackXP() = default;
  explicit TrackXP(fairlanes::AppContext &context, int starting_xp);

  void add_xp(entt::handle self, int amount);
};

} // namespace fairlanes::ecs::components
