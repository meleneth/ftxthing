#pragma once
#include <entt/entt.hpp>

namespace fairlanes::context {
class EntityCtx;
}
namespace fairlanes::systems {

class GrantXPToParty {
public:
  static void commit(fairlanes::context::EntityCtx party, int amount);
};

} // namespace fairlanes::systems
