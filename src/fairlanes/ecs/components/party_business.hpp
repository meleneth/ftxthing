#pragma once
#include "fairlanes/ecs/fwd.hpp"
#include <string>

namespace fairlanes::context {
class AppCtx;
}

namespace fairlanes::ecs::components {
using fairlanes::context::AppCtx;

struct PartyBusiness {
  std::string doing;

  PartyBusiness(AppCtx &context, std::string initial);
};

} // namespace fairlanes::ecs::components