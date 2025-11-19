// app_ctx.hpp
#pragma once
#include <entt/entt.hpp>

#include "app/move_only.hpp"
#include "fairlanes/context/account_ctx.hpp"
#include "fairlanes/widgets/fancy_log.hpp"

namespace fairlanes::context {
class RandomHub;
using widgets::FancyLog;

struct AppCtx {
  entt::registry reg_;
  RandomHub *rng_{};
  FancyLog log_; // owned by value

  AppCtx() = default;

  AppCtx(RandomHub &rng) : rng_{&rng} {}

  AccountCtx account_context(entt::entity account) {
    return AccountCtx{&reg_, account};
  }

  MARK_CLASS_MOVEONLY(AppCtx);
};

} // namespace fairlanes::context
