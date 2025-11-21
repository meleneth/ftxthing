#include "selected_character.hpp"
#include "fairlanes/context/app_ctx.hpp"
#include "fairlanes/widgets/fancy_log.hpp"

namespace fairlanes::ecs::components {

SelectedCharacter::SelectedCharacter(fairlanes::context::AppCtx &context) {
  (void)context;
  // spdlog::debug("SelectedCharacter ctor: registry={}",
  // fmt::ptr(&context.registry()));
}

} // namespace fairlanes::ecs::components
