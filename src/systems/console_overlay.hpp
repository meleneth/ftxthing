// console_overlay.hpp
#pragma once
#include <ftxui/component/component.hpp>
#include <ftxui/component/component_base.hpp>
#include <ftxui/component/captured_mouse.hpp>
#include <ftxui/dom/elements.hpp>
#include "log.hpp"

class ConsoleOverlay : public ftxui::ComponentBase {
public:
  using ExecFn = std::function<void(const std::string&)>;

  ConsoleOverlay(std::shared_ptr<LogBuffer> buf, ExecFn exec)
    : buf_(std::move(buf)), exec_(std::move(exec)) {
    input_ = ftxui::Input(&line_);
    Add(input_);
  }

  void SetOpen(bool v) { open_ = v; }
  bool IsOpen() const { return open_; }

  ftxui::Element Render() override {
    //if (last_seq_ != buf_->seq()) {
    //  cache_ = buf_->snapshot();
    //  last_seq_ = buf_->seq();
    //}

    using namespace ftxui;

    // Log pane (last N lines for speed)
    //const size_t take = std::min<size_t>(cache_.size(), 500);
    //std::vector<Element> rows;
    //rows.reserve(take);
    //auto it = cache_.end();
    //for (size_t i=0; i<take; ++i) {
    //  --it;
     // rows.push_back(text(it->text)); // coloring optional
    //}
    //std::reverse(rows.begin(), rows.end());

    auto log_window =
//      window(text(" Console "), vbox(std::move(rows)) | yflex) | size(HEIGHT, GREATER_THAN, 12);
      window(text(" Console "), vbox("foo") | yflex) | size(HEIGHT, GREATER_THAN, 12);

    auto bar = hbox({
      text("> "),
      input_->Render() | ftxui::yframe | ftxui::flex,
    });

    auto overlay = vbox({log_window, bar}) |
                   bgcolor(ftxui::Color::Black) |
                   ftxui::inverted; // “pulldown” vibe

    // When closed, render nothing; caller decides if overlay is composed.
    return open_ ? overlay : ftxui::emptyElement();
  }

  bool OnEvent(ftxui::Event e) override {
    if (!open_) return false;

    if (e == ftxui::Event::Return) {
      auto cmd = line_;
      if (!cmd.empty()) {
        history_.push_back(cmd);
        hist_pos_ = history_.size();
      }
      if (exec_) exec_(cmd);
      line_.clear();
      return true;
    }

    if (e == ftxui::Event::ArrowUp) {
      if (hist_pos_ > 0) { --hist_pos_; line_ = history_[hist_pos_]; }
      return true;
    }
    if (e == ftxui::Event::ArrowDown) {
      if (hist_pos_ + 1 < history_.size()) { ++hist_pos_; line_ = history_[hist_pos_]; }
      else { hist_pos_ = history_.size(); line_.clear(); }
      return true;
    }

    return ComponentBase::OnEvent(e);
  }

private:
  std::shared_ptr<LogBuffer> buf_;
  //std::deque<LogRecord> cache_;
  uint64_t last_seq_{0};

  ftxui::Component input_;
  std::string line_;
  std::vector<std::string> history_;
  size_t hist_pos_{0};

  ExecFn exec_;
  bool open_{false};
};
