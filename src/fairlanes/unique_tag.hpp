#pragma once
#include <entt/entt.hpp>
#include <utility> // for std::forward

template <typename Tag, typename... Args>
void set_unique_tag(entt::registry *reg, entt::entity e, Args &&...args) {
  // Remove Tag from everyone else
  auto view = reg->view<Tag>();
  for (auto existing : view) {
    if (existing != e) {
      reg->remove<Tag>(existing);
    }
  }

  // Set (or reset) Tag on the chosen entity with forwarded args
  reg->emplace_or_replace<Tag>(e, std::forward<Args>(args)...);
}
