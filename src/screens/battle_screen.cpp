#include "battle_screen.hpp"

#include <ftxui/component/component.hpp>
#include <ftxui/dom/elements.hpp>

using namespace ftxui;

BattleScreen::BattleScreen()
    : screen(ScreenInteractive::Fullscreen()) {
  component = Renderer([this] {
    auto log_view = vbox(
        [&] {
          Elements lines;
          const size_t total_lines = log.size();
          const size_t lines_to_show = std::min<size_t>(total_lines, log_height);
          for (size_t i = total_lines - lines_to_show; i < total_lines; ++i) {
            lines.push_back(text(log[i]));
          }
          return lines;
        }()
    ) | vscroll_indicator | frame | border;

    auto main_view = filler() | center | hcenter | border;

    return vbox({
      main_view | flex_grow,
      log_view | size(HEIGHT, EQUAL, log_height),
    });
  });

  for (int i = 0; i < 30; ++i) {
    log.push_back("Log entry #" + std::to_string(i + 1));
  }
}

void BattleScreen::Run() {
  screen.Loop(component);
}
