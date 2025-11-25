#pragma once
#include <boost/sml.hpp>
#include <string>

#include "app/registry_facade.hpp"
#include "fairlanes/context/entity_ctx.hpp"
#include "fairlanes/ecs/fwd.hpp"
#include "fairlanes/fsm/party_loop.hpp"
#include "systems/log.hpp"

namespace fairlanes::ecs::components {
struct PartyMember;
namespace sml = boost::sml;

// Marks an entity as a Party (party itself is an entity)
using fairlanes::context::EntityCtx;
using fairlanes::fsm::PartyLoop;

struct IsParty : BE_REGISTRY(IsParty) {
  EntityCtx ctx_;
  sml::sm<PartyLoop> sm_;
  entt::entity account_;
  entt::entity self_;
  std::string name_;
  int level_ = 1;
  std::vector<entt::entity> party_members_;

  IsParty(fairlanes::context::EntityCtx ctx, std::string name,
          entt::entity account);

  void next();
  bool needs_town();
  entt::entity create_member(std::string name);

  // Call `fn(entt::handle)` for each member of this party
  template <typename PM = PartyMember, typename Fn>
  inline void for_each_member(Fn &&fn) {
    auto &reg = ctx_.reg_;

    for (auto e : party_members_) {
      // Skip stale entries
      /*
      if (!reg.valid(e)) {
        continue;
      }*/

      // Skip if the entity doesn't have the expected component
      /*if (!reg.all_of<PM>(e)) {
        continue;
      }*/

      fn(entt::handle{reg, e});
    }
  }
};
} // namespace fairlanes::ecs::components
