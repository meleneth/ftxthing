#pragma once
#include <entt/entt.hpp>
namespace fairlanes::concepts {
struct Damage;
}
namespace fairlanes::context {
class EntityCtx;
}
namespace fairlanes::systems {

class TakeDamage {
public:
  static void commit(entt::entity attacker, fairlanes::context::EntityCtx ctx,
                     fairlanes::concepts::Damage damage);
};

} // namespace fairlanes::systems
