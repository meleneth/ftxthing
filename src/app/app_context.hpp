// context.hpp
#pragma once
#include <entt/entt.hpp>
namespace fairlanes::widgets {
class FancyLog;
}
namespace fairlanes {
class RandomHub;
class GrandCentral;
using fairlanes::widgets::FancyLog;

class AppContext {
public:
  // accessors
  std::shared_ptr<FancyLog> log() noexcept { return log_; }
  const std::shared_ptr<FancyLog> log() const noexcept { return log_; }

  entt::registry &registry() noexcept { return registry_; }
  const entt::registry &registry() const noexcept { return registry_; }

  RandomHub &rng() noexcept { return rng_; }
  const RandomHub &rng() const noexcept { return rng_; }

  // non-copyable, non-movable
  AppContext(const AppContext &) = delete;
  AppContext &operator=(const AppContext &) = delete;
  AppContext(AppContext &&) = delete;
  AppContext &operator=(AppContext &&) = delete;

private:
  friend class GrandCentral; // only GC can build it
  AppContext(std::shared_ptr<FancyLog> log, entt::registry &r, RandomHub &g)
      : log_(log), registry_(r), rng_(g) {}

  std::shared_ptr<FancyLog> log_;
  entt::registry &registry_;
  RandomHub &rng_;
};
} // namespace fairlanes
