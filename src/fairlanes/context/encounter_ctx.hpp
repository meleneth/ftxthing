#pragma once
#include <entt/entt.hpp>

#include "app/move_only.hpp"
#include "fairlanes/widgets/fancy_log.hpp"
#include "systems/random_hub.hpp"

namespace fairlanes::context {

struct EncounterCtx {
  entt::registry *reg_{};
  fairlanes::widgets::FancyLog *log_;
  fairlanes::RandomHub *rng_;
  entt::entity self_{};

  EncounterCtx(entt::registry *r, fairlanes::widgets::FancyLog *log,
               fairlanes::RandomHub *rng, entt::entity self)
      : reg_(r), log_(log), rng_(rng), self_(self) {}

  /* MARK_CLASS_MOVEONLY(EncounterCtx); */
};

} // namespace fairlanes::context
