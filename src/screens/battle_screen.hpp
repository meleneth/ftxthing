#pragma once

#include <memory>

#include <ftxui/component/screen_interactive.hpp>
#include <ftxui/component/component.hpp>
#include <entt/entt.hpp>

#include <widget_types.hpp>

class BattleScreen {
public:
  BattleScreen(entt::registry &combatants_);
  void Run();

private:
  ftxui::ScreenInteractive screen;
  entt::registry &combatants;
  CombatLogPtr log;
  ftxui::Component combatant_list = ftxui::Container::Vertical({});
  ftxui::Component root;
  std::vector<ftxui::Component> combatant_components;

  int log_height = 10;
};
