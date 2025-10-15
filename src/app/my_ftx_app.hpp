#pragma once
#include <memory>
#include <vector>
#include <functional>
#include <string>

#include <ftxui/component/component.hpp>
#include <ftxui/component/screen_interactive.hpp>

// forward decls of your stuff:
struct LogBuffer;
std::shared_ptr<LogBuffer> make_ftx_logger(std::shared_ptr<LogBuffer>, ftxui::ScreenInteractive*);

struct ConsoleOverlay;
struct CommandEngine;

// FSM bits:
#include "ui_fsm.hpp"  // provides UiCtx, UiMachine, events

class MyFTXApp {
public:
  using clock = std::chrono::steady_clock;

  MyFTXApp();
  int run();

private:
  // FTXUI
  ftxui::ScreenInteractive screen_{ftxui::ScreenInteractive::Fullscreen()};
  ftxui::Component root_current_;                // current base screen
  std::vector<ftxui::Component> screens_;        // base screens
  std::vector<ftxui::Component> override_stack_; // modal/fullscreen overlays

  // console + commands + logs
  std::shared_ptr<LogBuffer> logs_;
  //CommandEngine commands_;
  std::shared_ptr<ConsoleOverlay> console_;

  // FSM
  UiCtx ctx_{};
  sml::sm<UiMachine> sm_; // constructed with ctx_

  // builders
  ftxui::Component BuildAccountMonitor();
  ftxui::Component BuildPartyPlaceholder();
  ftxui::Component BuildOverride(int kind);
};
