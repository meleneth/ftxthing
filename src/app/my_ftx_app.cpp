#include "my_ftx_app.hpp"

#include <chrono>
#include <spdlog/spdlog.h>

#include <ftxui/component/component.hpp>
#include <ftxui/component/captured_mouse.hpp>
#include <ftxui/component/event.hpp>
#include <ftxui/dom/elements.hpp>

#include <console_overlay.hpp>
using namespace ftxui;

MyFTXApp::MyFTXApp()
  : logs_(std::make_shared<LogBuffer>(4000)),
    sm_{UiMachine{ctx_}} // pass context into FSM
{
//  auto logger = make_ftx_logger(logs_, &screen_);

  // Build base screens
  auto account_monitor = BuildAccountMonitor();
  auto party_placeholder = BuildPartyPlaceholder();
  screens_.push_back(account_monitor);
  screens_.push_back(party_placeholder);

  // Console overlay
  console_ = std::make_shared<ConsoleOverlay>(logs_, [this](const std::string& line) {
    spdlog::info(line);
    //commands_.execute_line(line);
    screen_.PostEvent(Event::Custom); // wake UI
  });

  // Wire UiCtx actions / state
  ctx_.screen_count = (int)screens_.size();
  ctx_.current_index = 0;

  ctx_.show_screen = [this](int idx) {
    if (idx < 0 || idx >= (int)screens_.size()) return;
    root_current_ = screens_[idx];
    screen_.PostEvent(Event::Custom);
  };
  ctx_.open_party_screen = [this](int party_id) {
    spdlog::info("open party {}", party_id);
    ctx_.current_index = 1;
    ctx_.show_screen(ctx_.current_index);
  };
  ctx_.push_override = [this](int kind) {
    override_stack_.push_back(BuildOverride(kind));
    screen_.PostEvent(Event::Custom);
  };
  ctx_.pop_override = [this]() {
    if (!override_stack_.empty()) override_stack_.pop_back();
    screen_.PostEvent(Event::Custom);
  };

  static bool console_open = false; // lives for app lifetime
  ctx_.set_console_open = [this](bool v) {
    console_open = v;
    console_->SetOpen(v);
    screen_.PostEvent(Event::Custom);
  };
  ctx_.is_console_open = [] { return console_open; };
  ctx_.has_override = [this] { return !override_stack_.empty(); };

  // Start on screen 0
  ctx_.show_screen(0);
}

int MyFTXApp::run() {
  auto now = clock::now();
  double dt = 0.0;

  auto root = Renderer([&] {
    // frame timing
    auto new_now = clock::now();
    dt = std::chrono::duration<double>(new_now - now).count();
    now = new_now;

    // base
    Element base = root_current_ ? root_current_->Render() : text("no screen") | center;

    // overlays (topmost last)
    std::vector<Element> layers;
    layers.push_back(base);

    for (auto& o : override_stack_) {
      layers.push_back(o->Render());
    }
    if (console_->IsOpen()) {
      layers.push_back(console_->Render());
    }

    // draw overlays on top of base
    return dbox(std::move(layers));
  });

  // keybindings at the edge of the app
  auto keymap = CatchEvent(root, [&](Event e) {
    if (e == Event::Character('`'))   { sm_.process_event(ev_toggle_console{}); return true; }
    if (e == Event::Tab)              { if (!ctx_.is_console_open()) sm_.process_event(ev_tab_next{}); return true; }
    if (e == Event::TabReverse)       { if (!ctx_.is_console_open()) sm_.process_event(ev_tab_prev{}); return true; }
    if (e == Event::Escape)           { sm_.process_event(ev_esc{}); return true; }
    return false;
  });

  screen_.Loop(keymap);
  return 0;
}

ftxui::Component MyFTXApp::BuildAccountMonitor() {
  using namespace ftxui;
  static int cursor = 0;
  static std::vector<std::string> parties = {
    "Party 0 — Adventuring (Forest)",
    "Party 1 — In Town (Healer)",
    "Party 2 — Idle"
  };

  auto c = Renderer([&] {
    std::vector<Element> rows;
    rows.reserve(parties.size());
    for (int i = 0; i < (int)parties.size(); ++i) {
      auto line = text(parties[i]) | (i == cursor ? inverted : nothing);
      rows.push_back(line);
    }
    return window(text(" Account Monitor "), vbox(std::move(rows))) | flex;
  });

  c = CatchEvent(c, [&](Event e) {
    if (e == Event::ArrowUp)   { cursor = (cursor + (int)parties.size() - 1) % (int)parties.size(); return true; }
    if (e == Event::ArrowDown) { cursor = (cursor + 1) % (int)parties.size(); return true; }
    if (e == Event::Return)    { sm_.process_event(ev_open_party{cursor}); return true; }
    return false;
  });

  return c;
}

ftxui::Component MyFTXApp::BuildPartyPlaceholder() {
  using namespace ftxui;
  return Renderer([] {
    return window(text(" Party "), vbox({ text("Party Details Here") })) | flex;
  });
}

ftxui::Component MyFTXApp::BuildOverride(int kind) {
  using namespace ftxui;
  return Renderer([kind] {
    // Make it look modal by dimming the background underneath.
    return window(text(" Override "),
                  vbox({ text("Override kind " + std::to_string(kind)),
                         separator(),
                         text("Press ESC to close") }))
           | bgcolor(Color::DarkBlue)
           | border;
  });
}
