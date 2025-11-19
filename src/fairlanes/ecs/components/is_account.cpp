#include "is_account.hpp"
#include "fairlanes/context/account_ctx.hpp"
#include "fairlanes/context/app_ctx.hpp"
#include "fairlanes/widgets/fancy_log.hpp"

namespace fairlanes::ecs::components {

IsAccount::IsAccount(fairlanes::context::AccountCtx &ctx, entt::entity self,
                     std::string name)
    : self_(self), account_name_{std::move(name)}, ctx_{std::move(ctx)} {}

} // namespace fairlanes::ecs::components
