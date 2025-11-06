#pragma once
#include <string>
namespace fairlanes {
class AppContext;
}
namespace fairlanes::ecs::components {
using fairlanes::AppContext;

// Marks an entity as an Account
struct IsAccount {
  std::string account_name_;
  IsAccount(AppContext &context, std::string name) : account_name_(name) {
    (void)context;
    // spdlog::debug("IsAccount ctor: registry={}",
    // fmt::ptr(&context.registry()));
  }
};
} // namespace fairlanes::ecs::components
