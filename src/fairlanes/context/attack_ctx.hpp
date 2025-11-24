#pragma once
#include <entt/entt.hpp>

#include "app/move_only.hpp"
#include "fairlanes/concepts/damage.hpp"
#include "fairlanes/context/entity_ctx.hpp"
#include "fairlanes/widgets/fancy_log.hpp"
#include "systems/random_hub.hpp"

namespace fairlanes::context {

struct AttackCtx {
  entt::registry &reg_;
  fairlanes::widgets::FancyLog &log_;
  fairlanes::RandomHub &rng_;
  entt::entity attacker_;
  entt::entity defender_;
  fairlanes::concepts::Damage damage_;

  AttackCtx(entt::registry &reg, fairlanes::widgets::FancyLog &log,
            fairlanes::RandomHub &rng, entt::entity attacker,
            entt::entity defender)
      : reg_(reg), log_(log), rng_(rng), attacker_(attacker),
        defender_(defender) {}

  fairlanes::context::EntityCtx entity_context(entt::entity e) {
    return fairlanes::context::EntityCtx{reg_, log_, rng_, e};
  }

  static AttackCtx make_attack(EntityCtx &ctx, entt::entity attacker,
                               entt::entity defender) {
    return AttackCtx{ctx.reg_, ctx.log_, ctx.rng_, attacker, defender};
  }

  MARK_CLASS_MOVEONLY(AttackCtx);
};

} // namespace fairlanes::context