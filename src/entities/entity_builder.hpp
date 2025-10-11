#pragma once

#include <entt/entt.hpp>

#include"entities/component_builder.hpp"

class EntityBuilder {
  entt::registry& reg_;
  entt::entity e_{entt::null};

public:
  explicit EntityBuilder(entt::registry& r) : reg_(r) { e_ = reg_.create(); }

  template <typename C>
  EntityBuilder& with(C c) {
    reg_.emplace_or_replace<C>(e_, std::move(c));
    return *this;
  }

  // Defaults for any C with a ComponentBuilder<C>
  template <typename C>
  EntityBuilder& with_default() {
    reg_.emplace_or_replace<C>(e_, ComponentBuilder<C>::defaults());
    return *this;
  }

  entt::entity build() const { return e_; }
};
