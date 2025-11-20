#include "party_member.hpp"

#include <entt/entt.hpp>
#include <ranges>
#include <string>

#include "fairlanes/ecs/fwd.hpp"

namespace fairlanes::ecs::components {

PartyMember::PartyMember(entt::entity party) : party_(party) {}

} // namespace fairlanes::ecs::components
