// context.hpp
#pragma once
#include <entt/entt.hpp>
namespace ftxui {
class ScreenInteractive;
}

namespace fairlanes {
class ConsoleOverlay;
class RandomHub;
class Router;
class Party;

class AppContext {
public:
  // accessors only (could be methods if you prefer)
  ConsoleOverlay &console() noexcept { return console_; }
  entt::registry &registry() noexcept { return registry_; }
  RandomHub &rng() noexcept { return rng_; }
  Router &router() noexcept { return router_; }
  ftxui::ScreenInteractive &screen() noexcept { return screen_; }

  Party *current_party() const noexcept { return current_party_; }
  void set_current_party(Party *p) noexcept { current_party_ = p; }

  // non-copyable, non-movable
  AppContext(const AppContext &) = delete;
  AppContext &operator=(const AppContext &) = delete;
  AppContext(AppContext &&) = delete;
  AppContext &operator=(AppContext &&) = delete;

private:
  friend class GrandCentral; // only GC can build it
  AppContext(ConsoleOverlay &c, entt::registry &r, RandomHub &g, Router &t,
             ftxui::ScreenInteractive &s)
      : console_(c), registry_(r), rng_(g), router_(t), screen_(s) {}

  ConsoleOverlay &console_;
  entt::registry &registry_;
  RandomHub &rng_;
  Router &router_;
  ftxui::ScreenInteractive &screen_;
  Party *current_party_ = nullptr;
};
} // namespace fairlanes
