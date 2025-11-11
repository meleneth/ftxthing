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

struct IsParty {
  PartyLoopCtx ctx_;
  sml::sm<PartyLoop> sm_;
  entt::entity account_;
  std::string name_;
  int level_ = 1;

  IsParty(AppContext &context, entt::entity party, std::string name,
          entt::entity account);

  void next();
};
} // namespace fairlanes::ecs::components
