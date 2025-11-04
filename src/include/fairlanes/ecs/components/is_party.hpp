#pragma once
#include <boost/sml.hpp>
#include <string>

#include "systems/log.hpp"

#include "fairlanes/ecs/fwd.hpp"
#include "fairlanes/fsm/party_loop.hpp"
#include "fairlanes/fsm/party_loop_ctx.hpp"

namespace fairlanes::ecs::components {
namespace sml = boost::sml;

// Marks an entity as a Party (party itself is an entity)
using ::fairlanes::fsm::PartyLoop;
using ::fairlanes::fsm::PartyLoopCtx;

struct IsParty {
  PartyLoopCtx ctx_;
  sml::sm<PartyLoop> sm_;
  entt::entity account_;
  std::string name_;
  int level_ = 1;

  IsParty(AppContext &context, entt::entity party, std::string name,
          entt::entity account)
      : ctx_{&context.registry(), party, nullptr}, sm_{PartyLoop{}, ctx_},
        account_{account}, name_{name} {
    // spdlog::debug("IsParty ctor: registry={}",
    // fmt::ptr(&context.registry()));
  }

  void next() {
    using fairlanes::fsm::NextEvent;
    sm_.process_event(NextEvent{});
  }
};
} // namespace fairlanes::ecs::components
