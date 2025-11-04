#pragma once
#include <entt/entt.hpp>

#include "app/app_config.hpp"
#include "app/app_context.hpp"

#include "systems/random_hub.hpp"
#include "widgets/fancy_log.hpp"
#include "widgets/root_component.hpp"

namespace fairlanes {
class GrandCentral {
public:
  GrandCentral(const AppConfig &cfg);
  void main_loop();
  entt::entity create_account(AppContext &ctx, std::string name);
  entt::entity create_party_in_account(AppContext &ctx, std::string name,
                                       entt::entity account);
  entt::entity create_member_in_party(AppContext &ctx, std::string name,
                                      entt::entity party);

  void switch_account(std::size_t idx);

  // Map F1..F8 → accounts[0..7]
  bool handle_event(const ftxui::Event &e);
  inline void tick_party_fsms(float dt);

  entt::registry &registry() { return reg_; }

private:
  entt::registry reg_;
  std::size_t active_idx_ = 0;
  ftxui::Component root_component_;
  std::shared_ptr<FancyLog> console_;
  std::shared_ptr<RandomHub> random_;
};
} // namespace fairlanes