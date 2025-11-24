#include "is_account.hpp"
#include "fairlanes/context/account_ctx.hpp"
#include "fairlanes/context/app_ctx.hpp"
#include "fairlanes/widgets/fancy_log.hpp"

namespace fairlanes::ecs::components {

IsAccount::IsAccount(fairlanes::context::AppCtx &context, std::string name,
                     entt::entity account)
    : account_name_(name), ctx_(fairlanes::context::AccountCtx{
                               context.reg_, context.rng_, account}) {
  (void)context;
  // spdlog::debug("IsAccount ctor: registry={}",
  // fmt::ptr(&context.registry()));
}

} // namespace fairlanes::ecs::components
