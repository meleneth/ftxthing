#pragma once
#include <entt/entt.hpp>

#include "app/app_context.hpp"

namespace fairlanes {
class Party {
public:
  Party(AppContext &ctx);
  entt::entity create_member(std::string name);

private:
  std::vector<entt::entity> members;
  entt::registry &reg_;
};
} // namespace fairlanes