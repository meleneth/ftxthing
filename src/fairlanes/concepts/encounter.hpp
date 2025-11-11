#pragma once
#include <entt/entt.hpp>
#include <vector>

namespace fairlanes {
class AppContext;
}

namespace fairlanes::concepts {

class Encounter {
public:
  Encounter(fairlanes::AppContext &ctx, entt::entity party);
  void tick(float dt);

private:
  entt::entity &party_;
  std::vector<entt::entity> enemies_;
};
} // namespace fairlanes::concepts