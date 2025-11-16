#include "selected_party.hpp"
#include "app/app_context.hpp"
#include "fairlanes/widgets/fancy_log.hpp"

namespace fairlanes::ecs::components {

SelectedParty::SelectedParty(fairlanes::AppContext &context) {
  (void)context;
  // spdlog::debug("SelectedParty ctor: registry={}",
  // fmt::ptr(&context.registry()));
}

// for_each_entity(reg, [&](entt::entity e, auto &pos, auto &vel) {
//  do your ECS thing
//});

} // namespace fairlanes::ecs::components
