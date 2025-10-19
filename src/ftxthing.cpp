#include <iostream>
#include <optional>

#include <CLI/CLI.hpp>
#include <entt/entt.hpp>
#include <spdlog/sinks/basic_file_sink.h>

#include "entities/entities.hpp"
#include "screens/battle_screen.hpp"
#include "systems/log.hpp"
#include "widgets/root_component.hpp"

#include "app/my_ftx_app.hpp"

int main(int argc, char *argv[]) {
  CLI::App app{"ftxthing — FTXUI/EnTT autobattler"};

  bool debug = false;
  std::optional<uint64_t> seed_opt;
  uint64_t stream = 0;
  bool fullscreen = true;

  app.add_flag("-d,--debug", debug, "Enable debug logging");
  app.add_option("--seed", seed_opt, "Seed for RNG (omit for time-based seed)");
  app.add_option("--stream", stream,
                 "PCG stream number (for independent RNG streams)");
  app.add_flag("--windowed,!--no-windowed", fullscreen,
               "Run in windowed mode (default fullscreen)");

  CLI11_PARSE(app, argc, argv);
  /*try {
    auto file_logger =
        spdlog::basic_logger_mt("file_logger", "ftxthing.log", true);
    spdlog::set_default_logger(file_logger);
    spdlog::flush_on(spdlog::level::info);
    spdlog::set_level(debug ? spdlog::level::debug : spdlog::level::info);
    spdlog::info("Logging initialized");
  } catch (const spdlog::spdlog_ex &ex) {
    fprintf(stderr, "Log init failed: %s\n", ex.what());
    exit(1);
  }*/
  spdlog::set_level(debug ? spdlog::level::debug : spdlog::level::info);
  spdlog::set_pattern("[%H:%M:%S] [%^%l%$] %v");
  spdlog::info("Starting up...");

  // cfg.seed = seed_opt;
  // cfg.stream = stream;
  // cfg.fullscreen = fullscreen;
  using clock = std::chrono::steady_clock;

  using namespace ftxui;
  ScreenInteractive screen = ScreenInteractive::Fullscreen();
  screen.SetCursor(Screen::Cursor{.shape = Screen::Cursor::Hidden});
  auto root = Make<RootComponent>();

  auto ui = Renderer(root, [&] {
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

  // MyFTXApp myapp = MyFTXApp();
  // return myapp.run();

  // entt::registry reg = build_demo_registry();

  // BattleScreen(reg).Run();
  return 0;
}
