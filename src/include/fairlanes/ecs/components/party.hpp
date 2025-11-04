#pragma once
#include <boost/sml.hpp>
#include <string>

#include "fairlanes/ecs/fwd.hpp"
#include "fairlanes/fsm/party_loop.hpp"

namespace fairlanes::ecs::components {
namespace sml = boost::sml;

// Marks an entity as a Party (party itself is an entity)
using fairlanes::fsm::PartyLoop;
struct IsParty {
  sml::sm<PartyLoop> sm_;
  entt::entity account_;
  std::string name_;
  int level_ = 1;

  // Construct with name + account
  IsParty(std::string name, entt::entity account)
      : sm_(PartyLoop{}), account_(account), name_(name) {}

  void next() {
    using fairlanes::fsm::NextEvent;
    sm_.process_event(NextEvent{});
  }
};

// Member belongs to a Party (attach to player/member entities)
struct PartyMember {
  fairlanes::ecs::Entity party{};
};

} // namespace fairlanes::ecs::components
