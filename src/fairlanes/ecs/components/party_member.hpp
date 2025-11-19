#pragma once
#include <entt/entt.hpp>
#include <ranges>
#include <string>

#include "fairlanes/ecs/fwd.hpp"
namespace fairlanes::context {
struct AppCtx;
}

namespace fairlanes::ecs::components {

struct PartyMember {
  fairlanes::ecs::Entity party_{entt::null};

  PartyMember(fairlanes::context::AppCtx &context, std::string /*name*/,
              entt::entity party);
};

// Call `fn(entt::handle)` for each member of `party_e`
template <typename PM = PartyMember, typename Fn>
inline void for_each_member(entt::registry &reg, entt::entity party_e,
                            Fn &&fn) {
  auto view = reg.view<PM>();            // entities with PartyMember
  for (auto e : view) {                  // iterate entities
    if (view.get(e).party_ == party_e) { // match party
      fn(entt::handle{reg, e});          // yield handle
    }
  }
}

} // namespace fairlanes::ecs::components
