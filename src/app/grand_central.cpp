#include <atomic>
#include <chrono>
#include <iostream>
#include <optional>
#include <thread>

#include "app/app_context.hpp"
#include "entities/entities.hpp"
#include "grand_central.hpp"
#include "screens/battle_screen.hpp"
#include "systems/log.hpp"
#include "systems/random_hub.hpp"
#include "widgets/fancy_log.hpp"
#include "widgets/root_component.hpp"

using namespace fairlanes;

GrandCentral::GrandCentral(const AppConfig &cfg) {
  using namespace ftxui;

  // UI bits
  console_ = std::make_shared<FancyLog>();
  root_component_ = Make<RootComponent>(console_);
  console_->append_markup("[name](Snail) uses [error](Slime Blast) 🔥");

  // RNG setup (safe default if seed is absent)
  const uint64_t seed =
      cfg.seed.value_or(static_cast<uint64_t>(std::random_device{}()));
  spdlog::debug("GrandCentral ctor: seed={}, stream={}", seed, cfg.stream);

  random_ = std::make_shared<RandomHub>(seed, cfg.stream);

  // Build a temporary context to wire new subsystems
  AppContext ctx(*console_, registry_, *random_);

  // Parties reference registry/log/rng extracted from ctx; ctx can die after
  // this
  auto &party = parties_.emplace_back(ctx);
  auto character = party.create_member("APlayer");
  (void)character;
}

void GrandCentral::main_loop() {
  using namespace ftxui;

  ScreenInteractive screen = ScreenInteractive::Fullscreen();
  screen.SetCursor(Screen::Cursor{.shape = Screen::Cursor::Hidden});
  auto root = Make<RootComponent>(console_);

  auto ui = Renderer(root, [&] {
    using clock = std::chrono::steady_clock;
    static auto last = clock::now();
    const auto now = clock::now();
    const double dt = std::chrono::duration<double>(now - last).count();
    (void)dt;
    last = now;

    // TODO: sim.tick(dt);

    return root->Render();
  });

  ui = CatchEvent(ui, [&](Event e) {
    if (e == Event::Character('q') || e == Event::Escape) {
      screen.Exit();
      return true;
    }
    if (e == Event::Character('`')) {
      root->toggle_console();
      return true;
    }
    if (e == Event::Character('~')) {
      root->set_full_open();
      root->toggle_console();
      return true;
    }
    if (e == Event::ArrowLeft) {
      // ...
      return true;
    }
    if (e == Event::ArrowRight) {
      // ...
      return true;
    }
    return false;
  });

  // wake UI at ~60Hz
  std::atomic<bool> running = true;
  std::thread ticker([&] {
    using namespace std::chrono_literals;
    while (running) {
      screen.PostEvent(Event::Custom); // kick a rerender (~60 Hz)
      std::this_thread::sleep_for(16ms);
    }
  });

  screen.Loop(ui);
  running = false;
  ticker.join();
}
