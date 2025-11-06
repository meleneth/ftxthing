#pragma once
#include "fairlanes/ecs/fwd.hpp"
#include <string>

namespace fairlanes {
class AppContext;
}

namespace fairlanes::ecs::components {
using fairlanes::AppContext;

struct PartyBusiness {
  std::string doing;

  PartyBusiness(AppContext &context, std::string initial);
};

} // namespace fairlanes::ecs::components