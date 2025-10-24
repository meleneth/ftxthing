#pragma once
#include <cstdint>
#include <optional>
#include <string>

namespace fairlanes {

struct AppConfig {
  // Logging / mode
  bool debug = false;
  bool fullscreen = true;
  std::optional<std::string>
      log_path; // if set, write logs here (also keep console)

  // RNG
  std::optional<std::uint64_t>
      seed; // if not provided, main() will fill with time-based seed
  std::uint64_t stream = 0; // PCG stream selector
};

} // namespace fairlanes
