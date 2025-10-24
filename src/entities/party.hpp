#pragma once
#include <chrono>
#include <entt/entt.hpp>

#include "app/app_context.hpp"

namespace fairlanes {
class FancyLog;

class Party {
public:
  Party(AppContext &ctx, std::string label);
  entt::entity create_member(AppContext &ctx, std::string name);
  void tick(float dt);

private:
  std::vector<entt::entity> members;
  entt::registry &reg_;
  FancyLog &log_;
  RandomHub &rng_;
  std::string label_;
};
} // namespace fairlanes