// app_ctx.hpp
#pragma once
#include <entt/entt.hpp>

#include "app/move_only.hpp"
#include "fairlanes/context/account_ctx.hpp"
#include "fairlanes/widgets/fancy_log.hpp"
#include "systems/random_hub.hpp"

namespace fairlanes::context {
using widgets::FancyLog;

struct AppCtx {
  entt::registry *reg_;
  fairlanes::RandomHub *rng_;
  FancyLog log_; // owned by value

  AppCtx() = default;

  AppCtx(entt::registry *reg, RandomHub *rng) : reg_(reg), rng_{rng} {}

  AccountCtx account_context(entt::entity account) {
    return AccountCtx{reg_, &log, account};
  }

  // MARK_CLASS_MOVEONLY(AppCtx);
};

} // namespace fairlanes::context
