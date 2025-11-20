#pragma once
#include <entt/entt.hpp>

#include "app/move_only.hpp"
#include "fairlanes/widgets/fancy_log.hpp"
#include "systems/random_hub.hpp"

namespace fairlanes::context {

struct EncounterCtx {
  entt::registry *reg_{};
  fairlanes::widgets::FancyLog *log_;
  entt::entity self_{};
  entt::entity party_{};

  EncounterCtx() = default;

  EncounterCtx(entt::registry *r, fairlanes::widgets::FancyLog *log,
               entt::entity self, entt::entity party)
      : reg_(r), log_(log), self_(self), party_(party) {}

  /* MARK_CLASS_MOVEONLY(EncounterCtx); */
};

} // namespace fairlanes::context
