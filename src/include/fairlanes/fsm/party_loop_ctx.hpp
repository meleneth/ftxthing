#pragma once
#include <boost/sml.hpp>
#include <entt/entt.hpp>
#include <spdlog/spdlog.h>

namespace fairlanes::fsm {
namespace sml = boost::sml;

struct PartyLoopCtx {
  entt::registry *reg{nullptr};
  entt::entity party{entt::null};
  spdlog::logger *log{nullptr};
};

} // namespace fairlanes::fsm
