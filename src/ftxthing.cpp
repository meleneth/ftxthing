#include <iostream>
#include <optional>

#include <CLI/CLI.hpp>
#include <entt/entt.hpp>
#include <spdlog/sinks/basic_file_sink.h>

#include "app/grand_central.hpp"
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
  auto grand_central = fairlanes::GrandCentral();
  grand_central.main_loop();

  return 0;
}
