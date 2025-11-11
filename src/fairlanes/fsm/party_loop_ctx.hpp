#pragma once
#include <entt/entt.hpp>
#include <spdlog/spdlog.h>

namespace fairlanes::fsm {

struct PartyLoopCtx {
  entt::registry *reg_{nullptr};
  entt::entity party_{entt::null};
  spdlog::logger *log_{nullptr};
};

} // namespace fairlanes::fsm
