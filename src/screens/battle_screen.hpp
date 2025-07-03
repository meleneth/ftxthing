#pragma once

#include <memory>

#include <ftxui/component/screen_interactive.hpp>
#include <ftxui/component/component.hpp>
#include <entt/entt.hpp>

#include <widget_types.hpp>
#include <animator.hpp>

class BattleScreen {
public:
  BattleScreen(entt::registry &combatants_);
  void StartDropInAnimations();
  void Run();

private:
  ftxui::ScreenInteractive screen;
  entt::registry &combatants;
  CombatLogPtr log;
  ftxui::Component combatant_list = ftxui::Container::Vertical({});
  ftxui::Component root;
  std::vector<ftxui::Component> combatant_components;
  Animator animator;

  int log_height = 10;
};
