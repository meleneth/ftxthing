#pragma once
#include <entt/entt.hpp>

#include "app/move_only.hpp"
#include "fairlanes/widgets/fancy_log.hpp"

namespace fairlanes::context {

struct EntityCtx {
  entt::registry *reg_{};
  fairlanes::widgets::FancyLog *log_;
  entt::entity self_{};

  EntityCtx() = default;

  EntityCtx(entt::registry *r, fairlanes::widgets::FancyLog *log,
            entt::entity self)
      : reg_(r), log_(log), self_(self) {}

  MARK_CLASS_MOVEONLY(EntityCtx);
};

} // namespace fairlanes::context
