#pragma once
#include <boost/sml.hpp>
#include <string>

#include "fairlanes/ecs/fwd.hpp"
#include "fairlanes/fsm/party_loop.hpp"
#include "fairlanes/fsm/party_loop_ctx.hpp"
#include "systems/log.hpp"

namespace fairlanes::ecs::components {
namespace sml = boost::sml;

// Marks an entity as a Party (party itself is an entity)
using fairlanes::fsm::PartyLoop;
using fairlanes::fsm::PartyLoopCtx;

// Member belongs to a Party (attach to player/member entities)
struct PartyMember {
  fairlanes::ecs::Entity party_{};

  PartyMember(AppContext &context, std::string name, entt::entity party)
      : party_(party) {
    (void)context;
    // spdlog::debug("PartyMember ctor: registry={}, name={}",
    //             fmt::ptr(&context.registry()), name);
    (void)name;
  }
};

} // namespace fairlanes::ecs::components
