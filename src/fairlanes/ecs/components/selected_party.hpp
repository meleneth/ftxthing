#pragma once
#include <memory>
#include <string>
#include <vector>

#include <entt/entt.hpp>

#include "fairlanes/ecs/components/party_member.hpp"

namespace fairlanes::context {
struct AppCtx;
} // namespace fairlanes::context

namespace fairlanes::ecs::components {

// Marks an entity as an Account
struct SelectedParty {
  std::uint8_t dummy_{0};

  SelectedParty(fairlanes::context::AppCtx &context);
};

} // namespace fairlanes::ecs::components
