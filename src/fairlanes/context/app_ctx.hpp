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
  std::shared_ptr<FancyLog> log_;

  AppCtx() = default;

  AppCtx(entt::registry *reg, RandomHub *rng)
      : reg_(reg), rng_{rng}, log_(std::make_shared<FancyLog>()) {}

  AccountCtx account_context(entt::entity account) {
    return AccountCtx{reg_, rng_, account};
  }
  EntityCtx entity_context(entt::entity e) {
    return EntityCtx{reg_, log_.get(), rng_, e};
  }
  /* MA RK_CLASS_MOVEONLY(AppCtx); */
};

} // namespace fairlanes::context
