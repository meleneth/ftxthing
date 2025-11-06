#include "fairlanes/ecs/fwd.hpp"
#include <string>

#include "app/app_context.hpp"
#include "party_business.hpp"

namespace fairlanes::ecs::components {
using fairlanes::AppContext;

PartyBusiness::PartyBusiness(AppContext &context, std::string initial)
    : doing(initial) {
  (void)context;
  // spdlog::debug("PartyBusiness ctor: registry={}, doing={}",
  //             fmt::ptr(&context.registry()), doing);
}

} // namespace fairlanes::ecs::components