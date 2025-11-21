#pragma once
#include <entt/entt.hpp>

#include "app/move_only.hpp"
#include "fairlanes/widgets/fancy_log.hpp"
#include "systems/random_hub.hpp"

namespace fairlanes::context {

struct EntityCtx {
  entt::registry *reg_;
  fairlanes::widgets::FancyLog *log_;
  fairlanes::RandomHub *rng_;
  entt::entity self_;

  EntityCtx entity_context(entt::entity e) {
    return EntityCtx{reg_, log_, rng_, e};
  }

  /* M ARK_CLASS_MOVEONLY(EntityCtx); */
};

} // namespace fairlanes::context
