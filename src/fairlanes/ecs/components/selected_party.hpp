#pragma once
#include <memory>
#include <string>
#include <vector>

#include <entt/entt.hpp>

#include "fairlanes/ecs/components/party_member.hpp"

namespace fairlanes::context {
class AppCtx;
} // namespace fairlanes::context
namespace fairlanes::widgets {
class FancyLog;
}
namespace fairlanes::fsm {
struct PartyLoopCtx;
}

namespace fairlanes::ecs::components {

using fairlanes::widgets::FancyLog;

// Marks an entity as an Account
struct SelectedParty {
  SelectedParty(fairlanes::context::AppCtx &context);
  std::vector<entt::entity> party_members_;

  template <class F>
  void for_each_party_member(entt::registry &reg, entt::entity party, F &&f) {
    auto view = reg.view<fairlanes::ecs::components::PartyMember>();
    for (auto entity : view) {
      const auto &pm = view.get<PartyMember>(entity);
      if (pm.party_ != party) {
        continue;
      }
      auto needle =
          std::find(party_members_.begin(), party_members_.end(), entity);

      if (needle == party_members_.end()) {
        party_members_.push_back(entity);
      }
    }

    for (auto party_member_entity : party_members_) {
      f(party_member_entity);
    }
  };
};

} // namespace fairlanes::ecs::components
