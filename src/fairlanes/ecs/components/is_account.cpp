#include "is_account.hpp"
#include "fairlanes/context/account_ctx.hpp"
#include "fairlanes/context/app_ctx.hpp"
#include "fairlanes/widgets/fancy_log.hpp"

namespace fairlanes::ecs::components {

IsAccount::IsAccount(fairlanes::context::AccountCtx ctx, std::string name)
    : account_name_{name}, ctx_{ctx} {}

} // namespace fairlanes::ecs::components
