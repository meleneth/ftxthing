#pragma once
#include <entt/entt.hpp>

template <typename Tag>
void set_unique_tag(entt::registry &reg, entt::entity e) {
  for (auto existing : reg.view<Tag>())
    reg.remove<Tag>(existing);

  reg.emplace_or_replace<Tag>(e);
}
