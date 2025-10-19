// console_overlay.cpp

#include <deque>
#include <ftxui/component/component.hpp>
#include <ftxui/component/event.hpp>
#include <string>

#include "console_overlay.hpp"

ConsoleOverlay::ConsoleOverlay() {
  input_ = ftxui::Input(&line_);
  Add(input_);
}

bool ConsoleOverlay::open() const { return open_; }
void ConsoleOverlay::toggle() { set_open(!open_); }

void ConsoleOverlay::set_open(bool v) {
  open_ = v;
  target_rows_ = open_ ? 12 : 0; // desired visible rows
  if (open_)
    TakeFocus(); // steal focus when opened
}

void ConsoleOverlay::set_full_open() {
  should_open_full_ = true;
  set_open(true);
}

// Call this from your frame tick to animate height
void ConsoleOverlay::tick() {
  if (rows_ == target_rows_)
    return;
  rows_ += (rows_ < target_rows_) ? 1 : -1;
}

void ConsoleOverlay::push_line(std::string s) {
  log_.push_back(std::move(s));
  if (log_.size() > 5000)
    log_.pop_front();
}

// Simple fake command hook
std::function<void(std::string_view)> on_command = [](std::string_view) {};

bool ConsoleOverlay::OnEvent(ftxui::Event e) {
  if (!open_)
    return false; // ignore when closed
  if (e == ftxui::Event::Return) {
    if (!line_.empty()) {
      push_line("> " + line_);
      on_command(line_);
      line_.clear();
    }
    return true;
  }
  return ComponentBase::OnEvent(e);
}

ftxui::Element ConsoleOverlay::Render() {
  using namespace ftxui;

  if (should_open_full_) {
    target_rows_ = HEIGHT;
  }

  auto log_view = vbox(TransformLog()) | vscroll_indicator | yframe;
  auto input_bar = hbox({text("> "), input_->Render()});
  auto panel =
      window(text("Console"), vbox({log_view, separator(), input_bar}));
  // Clamp height by rows_
  return panel | size(HEIGHT, LESS_THAN, std::max(0, rows_));
}

std::vector<ftxui::Element> ConsoleOverlay::TransformLog() const {
  std::vector<ftxui::Element> out;
  out.reserve(log_.size());
  for (auto &s : log_)
    out.push_back(ftxui::text(s));
  return out;
}

bool ConsoleOverlay::should_show() { return false; }