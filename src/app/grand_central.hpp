#pragma once
#include <entt/entt.hpp>
#include <ftxui/component/component.hpp>
#include <ftxui/component/event.hpp>

#include "app/app_config.hpp"
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
  fairlanes::context::AppCtx ctx_;

  GrandCentral(const AppConfig &cfg);
  void main_loop();
  entt::entity create_account(std::string name);
  entt::entity create_party_in_account(std::string name, entt::entity account);
  entt::entity create_member_in_party(std::string name, entt::entity party);

  void switch_account(std::size_t idx);
  void create_initial_accounts();

  // Map F1..F8 → accounts[0..7]
  bool handle_event(const ftxui::Event &e);
  inline void tick_party_fsms(float dt);

  RootComponent *root_component();
  entt::entity get_account(int id);

  // Call `fn(entt::handle)` for each member of `accounts`
  template <typename Fn> inline void for_each_account(Fn &&fn) {
    for (auto account_id : account_ids) {
      fn(entt::handle{ctx_.reg_, account_id}); // yield handle
    }
  }

  template <typename T> T &get(entt::entity e) { return ctx_.reg_.get<T>(e); }

  template <typename Component, typename... Args>
  Component &emplace(entt::entity e, Args &&...args) {
    return ctx_.reg_.emplace<Component>(e, std::forward<Args>(args)...);
  }

private:
  std::size_t active_idx_ = 0;
  ftxui::Component root_component_;
  std::vector<entt::entity> account_ids;
  entt::entity selected_account_{entt::null};
  entt::entity selected_party_{entt::null};
  entt::entity selected_character_{entt::null};
  uint64_t seed_;
  RandomHub random_;
};
} // namespace fairlanes