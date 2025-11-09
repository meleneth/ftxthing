#pragma once
#include <string>
namespace fairlanes {
class AppContext;
class FancyLog;
} // namespace fairlanes
namespace fairlanes::ecs::components {
// Marks an entity as an Account
struct IsAccount {
  std::string account_name_;
  std::unique_ptr<fairlanes::FancyLog> log_;
  IsAccount(fairlanes::AppContext &context, std::string name)
      : account_name_(name) {
    (void)context;
    // spdlog::debug("IsAccount ctor: registry={}",
    // fmt::ptr(&context.registry()));
  }
};
} // namespace fairlanes::ecs::components
