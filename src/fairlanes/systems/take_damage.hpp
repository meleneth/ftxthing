#pragma once
#include <entt/entt.hpp>
namespace fairlanes::context {
struct AttackCtx;
}
namespace fairlanes::systems {

class TakeDamage {
public:
  static void commit(fairlanes::context::AttackCtx &ctx);
};

} // namespace fairlanes::systems