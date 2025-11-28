#include "combatant.hpp"

#include "fairlanes/ecs/components/stats.hpp"
#include "fairlanes/ecs/components/track_xp.hpp"

namespace fairlanes::widgets {

Combatant::Combatant(entt::registry &reg_, entt::entity entity_)
    : reg(reg_), entity(entity_) {}

ftxui::Element Combatant::Render() {
  using namespace fairlanes::ecs::components;
  auto &stats = reg.get<Stats>(entity);
  auto &level = reg.get<TrackXP>(entity);

  constexpr int bar_width = 20;

  // Avoid div-by-zero and clamp [0, 1].
  float percent = 0.0f;
  if (stats.max_hp_ > 0) {
    percent = static_cast<float>(stats.hp_) / static_cast<float>(stats.max_hp_);
    if (percent < 0.0f)
      percent = 0.0f;
    if (percent > 1.0f)
      percent = 1.0f;
  }

  int filled = static_cast<int>(bar_width * percent);
  // Double-clamp for safety.
  if (filled < 0)
    filled = 0;
  if (filled > bar_width)
    filled = bar_width;

  int empty = bar_width - filled;
  // Shouldn’t be needed, but makes it bulletproof:
  if (empty < 0)
    empty = 0;

  std::string bar;
  bar.reserve(bar_width);
  bar.append(static_cast<std::size_t>(filled), '#');
  bar.append(static_cast<std::size_t>(empty), '-');

  // Build the HP line element
  ftxui::Element hp_line = ftxui::hbox({
      ftxui::text("HP: "),
      ftxui::text("[" + bar + "] " + std::to_string(stats.hp_) + "/" +
                  std::to_string(stats.max_hp_)),
  });

  // Add top border labels
  // clang-format off
    ftxui::Element border = ftxui::window(
      ftxui::hbox({
        ftxui::text(stats.name_) | ftxui::color(ftxui::Color::BlueLight),
        ftxui::filler(),
        ftxui::text("[" + std::to_string(level.level_) + "]") | ftxui::bold,
      }),
      hp_line
    );
  // clang-format on

  return border;
}

} // namespace fairlanes::widgets
