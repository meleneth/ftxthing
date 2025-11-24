#pragma once
#include <entt/entt.hpp>
#include <vector>
namespace fairlanes::context {
class EntityCtx;
}

namespace fairlanes::concepts {

class EncounterBuilder {
public:
  static void thump_it_out(fairlanes::context::EntityCtx &ctx);
};

} // namespace fairlanes::concepts