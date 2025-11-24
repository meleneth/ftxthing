#include "fairlanes/ecs/fwd.hpp"
#include <string>

#include "party_business.hpp"

namespace fairlanes::ecs::components {

PartyBusiness::PartyBusiness(std::string initial) : doing(initial) {

  // spdlog::debug("PartyBusiness ctor: registry={}, doing={}",
  //             fmt::ptr(&context.registry()), doing);
}

} // namespace fairlanes::ecs::components
