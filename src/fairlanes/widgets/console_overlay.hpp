// console_overlay.hpp
#pragma once
#include <deque>
#include <ftxui/component/component.hpp>
#include <ftxui/component/event.hpp>
#include <ftxui/dom/elements.hpp>
#include <string>

#include "fancy_log.hpp"

namespace fairlanes::widgets {

class ConsoleOverlay : public ftxui::ComponentBase {
public:
  ConsoleOverlay(std::shared_ptr<FancyLog> console);

  bool open() const;
  void toggle();
  void set_open(bool v);
  void set_full_open();

  // Call this from your frame tick to animate height
  void tick();
  void change_console(std::shared_ptr<FancyLog> console);

  // Simple fake command hook
  std::function<void(std::string_view)> on_command = [](std::string_view) {};

  bool OnEvent(ftxui::Event e) override;
  void FocusInput();

  ftxui::Element Render() override;
  bool should_show();

private:
  bool open_ = false;
  bool should_open_full_ = false;
  int rows_ = 0;
  int target_rows_ = 0;
  FancyLog *console_;
  std::string line_;
  ftxui::Component input_;
};

} // namespace fairlanes::widgets