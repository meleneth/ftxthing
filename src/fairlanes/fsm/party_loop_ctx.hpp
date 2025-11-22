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
  entt::registry *reg_;
  fairlanes::widgets::FancyLog *log_;
  RandomHub &rng;
  entt::entity party_{entt::null};

  PartyLoopCtx(entt::registry *reg, fairlanes::widgets::FancyLog *log,
               fairlanes::RandomHub &rng_, entt::entity party)
      : reg_(reg), log_(log), rng(rng_), party_(party) {}
};

} // namespace fairlanes::fsm
