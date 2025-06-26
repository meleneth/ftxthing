#include <ftxui/component/component.hpp>
#include <ftxui/dom/elements.hpp>
#include <ftxui/screen/screen.hpp>

#include <battle_screen.hpp>
#include <combat_log.hpp>  // the new widget

using namespace ftxui;

BattleScreen::BattleScreen()
    : screen(ScreenInteractive::Fullscreen()),
      log(std::make_shared<CombatLog>(log_height))
{
  // Append test data to the log
  log->Append("[info](Battle begins)");
  log->Append("[warn](Enemies approaching)");
  log->Append("[name](Snail) uses [error](Slime Blast)");

  screen.SetCursor(ftxui::ScreenInteractive::CursorShape::Hidden);

  // Optional top content
  auto main_view = Renderer([] {
    return text("Battle UI") | bold | center | border;
  });

  // Combine into root layout
  root = Renderer([=, this] {
    return vbox({
      main_view->Render() | flex_grow,
      log->Render() | size(HEIGHT, EQUAL, log_height),
    });
  });
}

void BattleScreen::Run() {
  screen.Loop(root);
}
