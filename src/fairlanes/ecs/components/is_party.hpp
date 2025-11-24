#pragma once
#include <boost/sml.hpp>
#include <string>

#include "fairlanes/context/entity_ctx.hpp"
#include "fairlanes/ecs/fwd.hpp"
#include "fairlanes/fsm/party_loop.hpp"
#include "systems/log.hpp"

namespace fairlanes::ecs::components {
namespace sml = boost::sml;

// Marks an entity as a Party (party itself is an entity)
using fairlanes::fsm::PartyLoop;

struct IsParty {
  fairlanes::context::EntityCtx ctx_;
  sml::sm<PartyLoop> sm_;
  entt::entity account_;
  std::string name_;
  int level_ = 1;
  std::vector<entt::entity> party_members_;

  IsParty(EntityCtx context, std::string name, entt::entity account);

  void next();
  bool needs_town();
};
} // namespace fairlanes::ecs::components
