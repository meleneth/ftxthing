#include "is_account.hpp"
#include "app/app_context.hpp"
#include "fairlanes/context/account_ctx.hpp"
#include "fairlanes/widgets/fancy_log.hpp"

namespace fairlanes::ecs::components {

IsAccount::IsAccount(fairlanes::AppContext &context, std::string name,
                     entt::entity account)
    : account_name_(name), log_(std::make_shared<FancyLog>()),
      ctx_(fairlanes::context::AccountCtx{context.registry(), context.rng(),
                                          account}) {
  (void)context;
  // spdlog::debug("IsAccount ctor: registry={}",
  // fmt::ptr(&context.registry()));
}

} // namespace fairlanes::ecs::components
