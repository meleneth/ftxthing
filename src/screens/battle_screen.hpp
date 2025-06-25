#pragma once

#include <ftxui/component/screen_interactive.hpp>
#include <ftxui/component/component.hpp>
#include <memory>

#include <widget_types.hpp>

class BattleScreen {
public:
  BattleScreen();
  void Run();

private:
  ftxui::ScreenInteractive screen;
  ftxui::Component root;
  CombatLogPtr log;
  int log_height = 10;
};
