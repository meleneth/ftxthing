#include "party_member.hpp"

#include <entt/entt.hpp>
#include <ranges>
#include <string>

#include "fairlanes/ecs/fwd.hpp"

namespace fairlanes::ecs::components {

PartyMember::PartyMember(fairlanes::context::EntityCtx context,
                         std::string name, entt::entity party)
    : party_(party), ctx_(std::move(context)) {
  (void)context;
  (void)name;
}

} // namespace fairlanes::ecs::components
