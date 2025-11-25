#pragma once
#include <memory>
#include <string>
#include <vector>

#include <entt/entt.hpp>

#include "fairlanes/context/account_ctx.hpp"
#include "fairlanes/ecs/components/is_party.hpp"

namespace fairlanes::context {
struct AppCtx;
} // namespace fairlanes::context

namespace fairlanes::widgets {
class FancyLog;
}

namespace fairlanes::ecs::components {

using fairlanes::widgets::FancyLog;
// Marks an entity as an Account
struct IsAccount {
  std::string account_name_;
  std::vector<entt::entity> parties_;
  fairlanes::context::AccountCtx ctx_;

  IsAccount(fairlanes::context::AppCtx &ctx, std::string name,
            entt::entity account);

  // Call `fn(entt::handle)` for each member of this party
  template <typename IP = fairlanes::ecs::components::IsParty, typename Fn>
  inline void for_each_party(Fn &&fn) {
    auto &reg = ctx_.reg_;

    for (auto e : parties_) {
      // Skip stale entries
      /*
      if (!reg.valid(e)) {
        continue;
      }*/

      // Skip if the entity doesn't have the expected component
      /*if (!reg.all_of<PM>(e)) {
        continue;
      }*/

      fn(entt::handle{reg, e});
    }
  }
};

} // namespace fairlanes::ecs::components
