#include "selected_account.hpp"
#include "app/app_context.hpp"
#include "fairlanes/widgets/fancy_log.hpp"

namespace fairlanes::ecs::components {

SelectedAccount::SelectedAccount(fairlanes::AppContext &context) {
  (void)context;
  // spdlog::debug("SelectedAccount ctor: registry={}",
  // fmt::ptr(&context.registry()));
}

} // namespace fairlanes::ecs::components
