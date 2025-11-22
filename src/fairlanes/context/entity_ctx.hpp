#pragma once
#include <entt/entt.hpp>

#include "app/move_only.hpp"
#include "fairlanes/widgets/fancy_log.hpp"
#include "systems/random_hub.hpp"

namespace fairlanes::context {

struct EntityCtx {
  entt::registry &reg_;
  fairlanes::widgets::FancyLog &log_;
  fairlanes::RandomHub &rng_;
  entt::entity self_;

  EntityCtx(entt::registry &reg, fairlanes::widgets::FancyLog &log,
            fairlanes::RandomHub &rng, entt::entity self)
      : reg_(reg), log_(log), rng_(rng), self_(self) {}
  EntityCtx entity_context(entt::entity e) {
    return EntityCtx{reg_, log_, rng_, e};
  }

  MARK_CLASS_MOVEONLY(EntityCtx);
};

} // namespace fairlanes::context