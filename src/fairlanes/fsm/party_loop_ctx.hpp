#pragma once
#include <memory>

#include <entt/entt.hpp>
#include <spdlog/spdlog.h>

#include "systems/random_hub.hpp"

namespace fairlanes::widgets {
class FancyLog;
}
namespace fairlanes::fsm {

struct PartyLoopCtx {
  entt::registry *reg_{nullptr};
  entt::entity party_{entt::null};
  fairlanes::widgets::FancyLog *log_;
  RandomHub &rng;
};

} // namespace fairlanes::fsm
