#pragma once
#include <memory>
#include <string>
#include <vector>

#include <entt/entt.hpp>

#include "fairlanes/context/account_ctx.hpp"
#include "fairlanes/ecs/components/is_party.hpp"
#include "fairlanes/widgets/fancy_log.hpp"

namespace fairlanes::fsm {
struct PartyLoopCtx;
}

namespace fairlanes::ecs::components {

using fairlanes::widgets::FancyLog;
// Marks an entity as an Account
struct IsAccount {
  entt::entity self_;

  std::string account_name_;
  std::vector<entt::entity> parties_;
  fairlanes::context::AccountCtx ctx_;

  IsAccount(fairlanes::context::AccountCtx &context, entt::entity self,
            std::string name);

  // Call `fn(entt::handle)` for each member of `party_e`
  template <typename IP = fairlanes::ecs::components::IsParty, typename Fn>
  inline void for_each_party(Fn &&fn) {
    auto view = ctx_.reg_->view<IP>();          // entities with PartyMember
    for (auto e : view) {                       // iterate entities
      if (view.get(e).account_ == ctx_.self_) { // match account
        fn(entt::handle{ctx_.reg_, e});         // yield handle
      }
    }
  }
};

} // namespace fairlanes::ecs::components
