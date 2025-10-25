#pragma once
#include <entt/entt.hpp>
#include <vector>

namespace fairlanes {
class Party;

class Encounter {
public:
  Encounter(Party &party);
  void tick(float dt);

private:
  Party &party_;
  std::vector<entt::entity> enemies_;
};
} // namespace fairlanes