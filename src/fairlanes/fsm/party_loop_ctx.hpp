#pragma once
#include <memory>

#include <entt/entt.hpp>
#include <spdlog/spdlog.h>

#include "fairlanes/context/entity_ctx.hpp"
#include "systems/random_hub.hpp"

namespace fairlanes::widgets {
class FancyLog;
}
namespace fairlanes::fsm {

struct PartyLoopCtx {
  entt::registry *reg_{nullptr};
  entt::entity party_;
  fairlanes::widgets::FancyLog *log_;
  fairlanes::RandomHub *rng_;

  fairlanes::context::EntityCtx entity_context(entt::entity e) {
    return fairlanes::context::EntityCtx{reg_, log_, rng_, e};
  }
};

} // namespace fairlanes::fsm
