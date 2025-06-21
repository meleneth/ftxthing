#pragma once

#include <ftxui/component/component.hpp>
#include <ftxui/component/screen_interactive.hpp>

class BattleScreen {
public:
  BattleScreen();
  void Run();

private:
  ftxui::ScreenInteractive screen;
  ftxui::Component component;
  std::deque<std::string> log;
  const int log_height = 10;
};
