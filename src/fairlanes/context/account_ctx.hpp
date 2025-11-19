#pragma once
#include <entt/entt.hpp>

#include "app/move_only.hpp"
#include "fairlanes/widgets/fancy_log.hpp"

namespace fairlanes::context {

struct AccountCtx {
  entt::registry *reg_{};
  entt::entity self_{};
  fairlanes::widgets::FancyLog log_;

  AccountCtx() = default;

  AccountCtx(entt::registry *r, entt::entity self) : reg_(r), self_(self) {}

  MARK_CLASS_MOVEONLY(AccountCtx);
};

} // namespace fairlanes::context
