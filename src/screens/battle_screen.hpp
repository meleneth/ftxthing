#pragma once

#include <memory>

#include <entt/entt.hpp>
#include <ftxui/component/component.hpp>
#include <ftxui/component/screen_interactive.hpp>

#include "animation/animator.hpp"
#include "fairlanes/widgets/fancy_log.hpp"
#include "fairlanes/widgets/widget_types.hpp"

namespace fairlanes::widgets {

class BattleScreen {
public:
  BattleScreen(entt::registry &combatants_);
  void StartDropInAnimations();
  void Run();

private:
  ftxui::ScreenInteractive screen;
  entt::registry &combatants;
  std::shared_ptr<FancyLog> log;
  // ftxui::Component combatant_list = ftxui::Container::Vertical({});
  ftxui::Component root;
  std::vector<ftxui::Component> combatant_components;
  Animator animator;

  int log_height = 10;
};

} // namespace fairlanes::widgets