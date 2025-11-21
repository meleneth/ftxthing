#include "fairlanes/ecs/fwd.hpp"
#include <string>

#include "fairlanes/context/app_ctx.hpp"
#include "party_business.hpp"

namespace fairlanes::ecs::components {
using fairlanes::context::AppCtx;

PartyBusiness::PartyBusiness(AppCtx &context, std::string initial)
    : doing(initial) {
  (void)context;
  // spdlog::debug("PartyBusiness ctor: registry={}, doing={}",
  //             fmt::ptr(&context.registry()), doing);
}

} // namespace fairlanes::ecs::components