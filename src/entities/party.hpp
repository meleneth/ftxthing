#pragma once
#include <entt/entt.hpp>

namespace fairlanes {
class Party {
public:
  Party();
  void add_member(std::string name);

private:
  std::vector<entt::entity> members;
};
} // namespace fairlanes