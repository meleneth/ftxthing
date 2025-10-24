#pragma once
#include <entt/entt.hpp>

#include "app/app_context.hpp"

namespace fairlanes {
class FancyLog;

class Party {
public:
  Party(AppContext &ctx);
  entt::entity create_member(std::string name);

private:
  std::vector<entt::entity> members;
  entt::registry &reg_;
  FancyLog &log_;
  RandomHub &rng_;
};
} // namespace fairlanes