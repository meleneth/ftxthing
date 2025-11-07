#pragma once
#include <boost/sml.hpp>
#include <entt/entt.hpp>
#include <spdlog/spdlog.h>

namespace fairlanes::fsm {
namespace sml = boost::sml;

struct PartyLoopCtx {
  entt::registry *reg_{nullptr};
  entt::entity party_{entt::null};
  spdlog::logger *log_{nullptr};
};

} // namespace fairlanes::fsm
