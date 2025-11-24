#pragma once
#include <memory>
#include <string>
#include <vector>

#include <entt/entt.hpp>

#include "fairlanes/context/account_ctx.hpp"

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
};

} // namespace fairlanes::ecs::components
