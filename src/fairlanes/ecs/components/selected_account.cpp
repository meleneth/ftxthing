#include "selected_account.hpp"
#include "fairlanes/context/app_ctx.hpp"
#include "fairlanes/widgets/fancy_log.hpp"

namespace fairlanes::ecs::components {

SelectedAccount::SelectedAccount(fairlanes::context::AppCtx &context) {
  (void)context;
  // spdlog::debug("SelectedAccount ctor: registry={}",
  // fmt::ptr(&context.registry()));
}

} // namespace fairlanes::ecs::components
