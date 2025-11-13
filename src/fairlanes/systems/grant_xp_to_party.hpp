#pragma once
#include <entt/entt.hpp>

namespace fairlanes::ecs::components {
struct PartyMember;
struct TrackXp;
} // namespace fairlanes::ecs::components
namespace fairlanes::fsm {
class PartyLoopCtx;
}
namespace fairlanes::systems {
using fairlanes::fsm::PartyLoopCtx;

class GrantXPToParty {
public:
  static void commit(PartyLoopCtx &ctx, entt::entity party, int amount);
};

} // namespace fairlanes::systems
