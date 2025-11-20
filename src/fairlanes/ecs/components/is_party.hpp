#pragma once
#include <boost/sml.hpp>
#include <string>

#include "app/registry_facade.hpp"
#include "fairlanes/ecs/fwd.hpp"
#include "fairlanes/fsm/party_loop.hpp"
#include "fairlanes/fsm/party_loop_ctx.hpp"
#include "systems/log.hpp"

namespace fairlanes::ecs::components {
struct PartyMember;
namespace sml = boost::sml;

// Marks an entity as a Party (party itself is an entity)
using fairlanes::fsm::PartyLoop;
using fairlanes::fsm::PartyLoopCtx;

struct IsParty : BE_REGISTRY(IsParty) {
  PartyLoopCtx ctx_;
  sml::sm<PartyLoop> sm_;
  entt::entity account_;
  entt::entity self_;
  std::string name_;
  int level_ = 1;
  std::vector<entt::entity> party_members_;

  IsParty(fairlanes::fsm::PartyLoopCtx &context, entt::entity party,
          std::string name, entt::entity account);

  void next();
  bool needs_town();
  entt::entity create_member(std::string name);

  // Call `fn(entt::handle)` for each member of `party_e`
  template <typename PM = PartyMember, typename Fn>
  inline void for_each_member(Fn &&fn) {
    auto view = ctx_.reg_->view<PM>();         // entities with PartyMember
    for (auto e : view) {                      // iterate entities
      if (view.get(e).party_ == ctx_.party_) { // match party
        fn(entt::handle{ctx_.reg_, e});        // yield handle
      }
    }
  }
};
} // namespace fairlanes::ecs::components
