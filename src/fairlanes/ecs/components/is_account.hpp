#pragma once
#include <memory>
#include <string>


namespace fairlanes {
class AppContext;
class FancyLog;
} // namespace fairlanes

namespace fairlanes::ecs::components {
// Marks an entity as an Account
struct IsAccount {
  std::string account_name_;
  std::shared_ptr<fairlanes::FancyLog> log_;
  IsAccount(fairlanes::AppContext &context, std::string name);
};

} // namespace fairlanes::ecs::components
