#pragma once
#include <entt/entt.hpp>
#include <ftxui/component/component.hpp>
#include <ftxui/component/event.hpp>

#include "app/app_config.hpp"
#include "app/app_context.hpp"
#include "fairlanes/context/app_ctx.hpp"

#include "systems/random_hub.hpp"

namespace fairlanes::widgets {
class RootComponent;
class FancyLog;
} // namespace fairlanes::widgets
namespace fairlanes {
using fairlanes::widgets::FancyLog;
using fairlanes::widgets::RootComponent;
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
  void create_initial_accounts();

  AppContext &app_context();

  // Map F1..F8 → accounts[0..7]
  bool handle_event(const ftxui::Event &e);
  inline void tick_party_fsms(float dt);

  RootComponent *root_component();
  entt::registry &registry() { return reg_; }
  entt::entity get_account(int id);

private:
  entt::registry reg_;
  std::size_t active_idx_ = 0;
  std::shared_ptr<FancyLog> console_;
  ftxui::Component root_component_;
  std::vector<entt::entity> account_ids;
  entt::entity selected_account_{entt::null};
  entt::entity selected_party_{entt::null};
  entt::entity selected_character_{entt::null};
  uint64_t seed_;
  std::shared_ptr<RandomHub> random_;
  AppContext app_context_;
  fairlanes::context::AppCtx ctx_;
};
} // namespace fairlanes