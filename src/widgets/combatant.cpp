#include "combatant.hpp"

#include "fairlanes/ecs/components/stats.hpp"
#include "fairlanes/ecs/components/track_xp.hpp"

namespace fairlanes {

Combatant::Combatant(entt::registry &reg_, entt::entity entity_)
    : reg(reg_), entity(entity_) {}

ftxui::Element Combatant::Render() {
  using namespace fairlanes::ecs::components;

  auto &name = reg.get<Stats>(entity);
  auto &hp = reg.get<Stats>(entity);
  auto &level = reg.get<TrackXP>(entity);

  float percent = static_cast<float>(hp.hp_) / hp.max_hp_;
  int bar_width = 20;
  int filled = static_cast<int>(bar_width * percent);

  std::string bar =
      std::string(filled, '#') + std::string(bar_width - filled, '-');

  // Build the HP line element
  ftxui::Element hp_line = ftxui::hbox({
      ftxui::text("HP: "),
      ftxui::text("[" + bar + "] " + std::to_string(hp.hp_) + "/" +
                  std::to_string(hp.max_hp_)),
  });

  // Add top border labels
  // clang-format off
    ftxui::Element border = ftxui::window(
      ftxui::hbox({
        ftxui::text(name.name_) | ftxui::color(ftxui::Color::BlueLight),
        ftxui::filler(),
        ftxui::text("[" + std::to_string(level.level_) + "]") | ftxui::bold,
      }),
      hp_line
    );
  // clang-format on

  return border;
}

} // namespace fairlanes
