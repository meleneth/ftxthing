#pragma once
#include "fairlanes/ecs/fwd.hpp"
#include <string>

namespace fairlanes::ecs::components {

struct PartyBusiness {
  std::string doing;

  PartyBusiness(AppContext &context, std::string initial) : doing(initial) {
    (void)context;
    // spdlog::debug("PartyBusiness ctor: registry={}, doing={}",
    //             fmt::ptr(&context.registry()), doing);
  }
};

} // namespace fairlanes::ecs::components