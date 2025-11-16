#include <chrono>
#include <iostream>
#include <optional>
#include <vector>

#include <CLI/CLI.hpp>
#include <entt/entt.hpp>
#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/spdlog.h>

#include "app/app_config.hpp"
#include "app/grand_central.hpp"

int main(int argc, char *argv[]) {
  CLI::App app{"ftxthing — FTXUI/EnTT autobattler"};

  // CLI targets
  bool debug = false;
  std::optional<std::uint64_t> seed_opt;
  std::uint64_t stream = 0;
  bool fullscreen = true;
  std::optional<std::string> log_path;

  app.add_flag("-d,--debug", debug, "Enable debug logging");
  app.add_option("--seed", seed_opt, "Seed for RNG (omit for time-based seed)");
  app.add_option("--stream", stream,
                 "PCG stream number (for independent RNG streams)");
  app.add_flag("--windowed,!--no-windowed", fullscreen,
               "Run in windowed mode (default fullscreen)");

  // -l/--log: optional argument. If present w/o value -> "ftxthing.log".
  std::string log_path_tmp; // holds value if provided
  auto *log_opt = app.add_option("-l,--log", log_path_tmp,
                                 "Write logs to file (optionally pass a path)")
                      ->expected(0, 1);

  CLI11_PARSE(app, argc, argv);

  // Determine logging path from presence/value
  if (log_opt->count() > 0) {
    log_path = log_path_tmp.empty() ? std::optional<std::string>{"ftxthing.log"}
                                    : std::optional<std::string>{log_path_tmp};
  }

  // ---- logging setup (console + optional file) ----
  std::vector<spdlog::sink_ptr> sinks;
  auto console_sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
  sinks.push_back(console_sink);

  if (log_path) {
    auto file_sink = std::make_shared<spdlog::sinks::basic_file_sink_mt>(
        *log_path, /*truncate=*/true);
    sinks.push_back(file_sink);
  }

  auto logger =
      std::make_shared<spdlog::logger>("ftxthing", sinks.begin(), sinks.end());
  spdlog::set_default_logger(logger);
  spdlog::set_level(debug ? spdlog::level::debug : spdlog::level::info);
  spdlog::set_pattern("[%H:%M:%S] [%^%l%$] %v");
  spdlog::flush_on(spdlog::level::info);

  if (log_path) {
    spdlog::info("File logging enabled: {}", *log_path);
  }
  spdlog::info("Starting up...");

  // ---- build config for GrandCentral ----
  fairlanes::AppConfig cfg;
  cfg.debug = debug;
  cfg.fullscreen = fullscreen;
  cfg.log_path = log_path;
  cfg.stream = stream;

  // seed: if user didn't pass one, derive from time
  if (seed_opt) {
    cfg.seed = *seed_opt;
  } else {
    const auto now =
        std::chrono::high_resolution_clock::now().time_since_epoch().count();
    cfg.seed = static_cast<std::uint64_t>(now);
    spdlog::debug("No seed provided; using time-based seed: {}", *cfg.seed);
  }

  // ---- run app ----
  fairlanes::GrandCentral grand_central{cfg};
  grand_central.create_initial_accounts();
  grand_central.main_loop();

  return 0;
}
