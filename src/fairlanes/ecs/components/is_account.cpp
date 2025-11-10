#include "is_account.hpp"
#include "app/app_context.hpp"
#include "widgets/fancy_log.hpp"

namespace fairlanes::ecs::components {

IsAccount::IsAccount(fairlanes::AppContext &context, std::string name)
    : account_name_(name), log_(std::make_shared<FancyLog>()) {
  (void)context;
  // spdlog::debug("IsAccount ctor: registry={}",
  // fmt::ptr(&context.registry()));
}

} // namespace fairlanes::ecs::components
