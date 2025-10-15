#include <entt/entt.hpp>
#include <CLI/CLI.hpp>

#include <optional>
#include <iostream>

#include "entities/entities.hpp"
#include "screens/battle_screen.hpp"
#include "systems/log.hpp"

#include "app/my_ftx_app.hpp"

int main(int argc, char *argv[]) {
  CLI::App app{"ftxthing — FTXUI/EnTT autobattler"};
 
  bool debug = false;
    std::string filename;

    app.add_flag("-d,--debug", debug, "Enable debug logging");
    app.add_option("input", filename, "Path to MVE file")
      ->required()
      ->check(CLI::ExistingFile);


  std::optional<uint64_t> seed_opt;
  uint64_t stream = 0;
  bool fullscreen = true;

  app.add_option("--seed", seed_opt, "Seed for RNG (omit for time-based seed)");
  app.add_option("--stream", stream, "PCG stream number (for independent RNG streams)");
  app.add_flag("--windowed,!--no-windowed", fullscreen, "Run in windowed mode (default fullscreen)");

  CLI11_PARSE(app, argc, argv);

    spdlog::set_level(debug ? spdlog::level::debug : spdlog::level::info);
    spdlog::set_pattern("[%H:%M:%S] [%^%l%$] %v");
    spdlog::info("Starting up...");

  //cfg.seed = seed_opt;
  //cfg.stream = stream;
  //cfg.fullscreen = fullscreen;

  MyFTXApp myapp = MyFTXApp();
  return myapp.run();

  //entt::registry reg = build_demo_registry();

  //BattleScreen(reg).Run();
  return 0;
}
