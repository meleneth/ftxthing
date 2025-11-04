#pragma once
#include "fairlanes/ecs/fwd.hpp"
#include <string>

namespace fairlanes::ecs::components {

// Marks an entity as an Account
struct IsAccount {
  std::string account_name;
};

} // namespace fairlanes::ecs::components
