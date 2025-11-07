// console_overlay.cpp

#include <deque>
#include <ftxui/component/component.hpp>
#include <ftxui/component/event.hpp>
#include <string>

#include "console_overlay.hpp"
#include "fancy_log.hpp"
#include "log.hpp"

using namespace fairlanes;

ConsoleOverlay::ConsoleOverlay(std::shared_ptr<FancyLog> console)
    : console_(console) {
  input_ = ftxui::Input(&line_);
  Add(input_);
}

bool ConsoleOverlay::open() const { return open_; }
void ConsoleOverlay::toggle() { set_open(!open_); }

void ConsoleOverlay::set_open(bool v) {
  open_ = v;
  if (open_) {
    if (should_open_full_) {
      target_rows_ = ftxui::HEIGHT;
    } else {
      target_rows_ = 22;
    }
  } else {
    target_rows_ = 0;
  }
}

void ConsoleOverlay::FocusInput() { input_->TakeFocus(); }

void ConsoleOverlay::set_full_open() {
  should_open_full_ = !should_open_full_;
  set_open(true);
}

// Call this from your frame tick to animate height
void ConsoleOverlay::tick() {
  if (rows_ == target_rows_)
    return;
  rows_ += (rows_ < target_rows_) ? 1 : -1;
}

// Simple fake command hook
std::function<void(std::string_view)> on_command = [](std::string_view) {};

bool ConsoleOverlay::OnEvent(ftxui::Event e) {
  if (!open_) {
    return false;
  }

  if (e == ftxui::Event::Return) {
    if (!line_.empty()) {
      console_->append_plain("> " + line_);
      on_command(line_);
      line_.clear();
    }
    input_->TakeFocus(); // keep typing after submit
    return true;
  }

  if (input_->OnEvent(e)) {
    return true;
  }

  if (e == ftxui::Event::Escape) {
    set_open(false);
    return true;
  }

  // 3) Swallow everything else while console is open
  return true;
}
ftxui::Element ConsoleOverlay::Render() {
  using namespace ftxui;

  // 1) Let the log view expand vertically so the input gets pushed to the
  // bottom.
  auto log_view = vbox(console_->Render()) | vscroll_indicator | yframe |
                  yflex_grow; // <- key: consume spare vertical space

  auto input_bar = hbox({text("> "), input_->Render()});
  auto content = vbox({log_view, separator(), input_bar});

  auto panel = window(text("Console"), content);

  // 2) Clear underneath and clamp the overlay height EXACTLY to rows_.
  return panel | xflex_grow | clear_under |
         size(HEIGHT, EQUAL, std::max(0, rows_));
}

bool ConsoleOverlay::should_show() { return open_; }
