#include <chrono>
#include <iostream>
#include <optional>

#include "entities/entities.hpp"
#include "grand_central.hpp"
#include "screens/battle_screen.hpp"
#include "systems/log.hpp"
#include "widgets/root_component.hpp"

using namespace fairways;

GrandCentral::GrandCentral()
    : root_component(std::make_shared<RootComponent>()) {}

void GrandCentral::main_loop() {

  using namespace ftxui;
  ScreenInteractive screen = ScreenInteractive::Fullscreen();
  screen.SetCursor(Screen::Cursor{.shape = Screen::Cursor::Hidden});
  auto root = Make<RootComponent>();

  auto ui = Renderer(root, [&] {
    using clock = std::chrono::steady_clock;
    static auto last = clock::now();
    auto now = clock::now();
    double dt = std::chrono::duration<double>(now - last).count();
    last = now;
    (void)dt;
    // advance sim here (on UI thread)
    // TODO
    // sim.tick(dt);

    return root->Render();
  });

  // 🔑 Add this: catch keyboard/mouse events.
  ui = CatchEvent(ui, [&](Event e) {
    // quit
    if (e == Event::Character('q') || e == Event::Escape) {
      screen.Exit();
      return true; // we handled it
    }
    if (e == Event::Character("`")) {
      root->toggle_console();
      return true;
    }
    if (e == Event::Character("~")) {
      root->set_full_open();
      root->toggle_console();
      return true;
    }
    // arrows, etc.
    if (e == Event::ArrowLeft) { /* do something */
      return true;
    }
    if (e == Event::ArrowRight) { /* do something */
      return true;
    }

    // characters
    // if (e.is_character()) {
    // e.character() gives the char
    // ... handle text input or hotkeys ...
    //  return true;
    // }

    return false; // not handled → let others see it
  });

  // wake UI at ~60Hz
  std::atomic<bool> running = true;
  std::thread ticker([&] {
    using namespace std::chrono_literals;
    while (running) {
      screen.PostEvent(ftxui::Event::Custom); // causes a rerender -> tick
                                              // happens in Renderer
      std::this_thread::sleep_for(16ms);
    }
  });

  screen.Loop(ui);
  running = false;
  ticker.join();
}