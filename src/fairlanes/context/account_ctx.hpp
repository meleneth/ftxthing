#pragma once
#include <entt/entt.hpp>

#include "app/move_only.hpp"
#include "fairlanes/widgets/fancy_log.hpp"

namespace fairlanes::context {

struct AccountCtx {
  entt::registry *reg_;
  fairlanes::widgets::FancyLog *log_;
  entt::entity self_;

  AccountCtx() = default;

  AccountCtx(entt::registry *r, fairlanes::widgets::FancyLog *log,
             entt::entity self)
      : reg_(r), log_(log), self_(self) {}

  // MARK_CLASS_MOVEONLY(AccountCtx);
};

} // namespace fairlanes::context
