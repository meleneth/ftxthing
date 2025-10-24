#pragma once
#include <entt/entt.hpp>

#include "account.hpp"
#include "app/app_config.hpp"
#include "entities/party.hpp"
#include "systems/random_hub.hpp"
#include "widgets/fancy_log.hpp"
#include "widgets/root_component.hpp"

namespace fairlanes {
class GrandCentral {
public:
  GrandCentral(const AppConfig &cfg);
  void main_loop();
  Account &add_account(std::string name);
  Account *active_account();
  void switch_account(std::size_t idx);

  // Map F1..F8 → accounts[0..7]
  bool handle_event(const ftxui::Event &e);
  void sim_tick(float dt);

  entt::registry &registry() { return reg_; }

private:
  entt::registry reg_;
  std::vector<std::unique_ptr<Account>> accounts_;
  std::size_t active_idx_ = 0;
  ftxui::Component root_component_;
  std::shared_ptr<FancyLog> console_;
  std::shared_ptr<RandomHub> random_;
  entt::registry registry_;
};
} // namespace fairlanes