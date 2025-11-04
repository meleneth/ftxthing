#pragma once
#include "fairlanes/ecs/fwd.hpp"
#include "fairlanes/fsm/party_loop.hpp"
#include "fairlanes/fsm/party_loop_ctx.hpp"
#include "systems/log.hpp"

#include <spdlog/spdlog.h>
#include <string>

namespace fairlanes::ecs::components {
using fairlanes::fsm::PartyLoopCtx;
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
