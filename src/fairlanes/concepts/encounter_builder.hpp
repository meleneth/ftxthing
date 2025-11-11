#pragma once
#include <entt/entt.hpp>
#include <vector>
namespace fairlanes {
class AppContext;
}

namespace fairlanes::concepts {
class Encounter;

class EncounterBuilder {
public:
  static Encounter thump_it_out(fairlanes::AppContext &ctx, entt::entity party);
};

} // namespace fairlanes::concepts