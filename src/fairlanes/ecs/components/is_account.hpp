#pragma once
#include <memory>
#include <string>
#include <vector>

#include <entt/entt.hpp>

namespace fairlanes {
class AppContext;
} // namespace fairlanes
namespace fairlanes::widgets {
class FancyLog;
}
namespace fairlanes::fsm {
struct PartyLoopCtx;
}

namespace fairlanes::ecs::components {

using fairlanes::widgets::FancyLog;
// Marks an entity as an Account
struct IsAccount {
  std::string account_name_;
  std::shared_ptr<fairlanes::widgets::FancyLog> log_;
  std::vector<entt::entity> parties_;

  IsAccount(fairlanes::AppContext &context, std::string name);
};

} // namespace fairlanes::ecs::components
