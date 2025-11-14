#include "selected_character.hpp"
#include "app/app_context.hpp"
#include "fairlanes/widgets/fancy_log.hpp"

namespace fairlanes::ecs::components {

SelectedCharacter::SelectedCharacter(fairlanes::AppContext &context) {
  (void)context;
  // spdlog::debug("SelectedCharacter ctor: registry={}",
  // fmt::ptr(&context.registry()));
}

} // namespace fairlanes::ecs::components
