#pragma once
#include <entt/entt.hpp>
#include <spdlog/spdlog.h>

#include "systems/random_hub.hpp"
#include "widgets/fancy_log.hpp"

namespace fairlanes::fsm {

struct PartyLoopCtx {
  entt::registry *reg_{nullptr};
  entt::entity party_{entt::null};
  std::shared_ptr<FancyLog> log_;
  RandomHub &rng;
};

} // namespace fairlanes::fsm
