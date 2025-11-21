#pragma once
#include <entt/entt.hpp>
#include <memory>

#include "app/move_only.hpp"
#include "fairlanes/context/entity_ctx.hpp"
#include "fairlanes/fsm/party_loop_ctx.hpp"
#include "fairlanes/widgets/fancy_log.hpp"

namespace fairlanes::context {

struct AccountCtx {
  entt::registry *reg_;
  std::shared_ptr<fairlanes::widgets::FancyLog> log_;
  RandomHub *rng_;
  entt::entity self_;

  AccountCtx() = default;

  AccountCtx(entt::registry *r, RandomHub *rng, entt::entity self)
      : reg_(r), log_(std::make_shared<fairlanes::widgets::FancyLog>()),
        rng_(rng), self_(self) {}

  EntityCtx entity_context(entt::entity e) {
    return EntityCtx{reg_, log_.get(), rng_, e};
  }

  fairlanes::fsm::PartyLoopCtx party_loop_context(entt::entity e) {
    return fairlanes::fsm::PartyLoopCtx{reg_, e, log_.get(), rng_};
  }
  /* MA RK_CLASS_MOVEONLY(AccountCtx); */
};

} // namespace fairlanes::context
