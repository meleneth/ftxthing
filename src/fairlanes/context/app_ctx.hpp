#pragma once
#include <memory>

#include <entt/entt.hpp>

#include "app/move_only.hpp"
#include "fairlanes/context/account_ctx.hpp"
#include "fairlanes/widgets/fancy_log.hpp"
#include "systems/random_hub.hpp"

namespace fairlanes::context {

struct AppCtx {
  entt::registry &reg_;
  fairlanes::RandomHub &rng_;
  std::unique_ptr<fairlanes::widgets::FancyLog> log_;

  AppCtx(entt::registry &reg, fairlanes::RandomHub &rng)
      : reg_(reg), rng_(rng),
        log_(std::make_unique<fairlanes::widgets::FancyLog>()) {}

  fairlanes::context::AccountCtx account_context(entt::entity account) const {
    return AccountCtx{reg_, rng_, account};
  }

  fairlanes::context::EntityCtx entity_context(entt::entity e) const {
    return EntityCtx{reg_, *log_.get(), rng_, e};
  }

  MARK_CLASS_MOVEONLY(AppCtx);
};

} // namespace fairlanes::context
