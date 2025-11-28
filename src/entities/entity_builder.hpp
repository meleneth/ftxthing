#pragma once

#include <entt/entt.hpp>

#include "entities/component_builder.hpp"
#include "fairlanes/context/entity_ctx.hpp"
#include "fairlanes/ecs/components/track_xp.hpp"
class EntityBuilder {
  fairlanes::context::EntityCtx ctx_;
  entt::entity e_{entt::null};

public:
  explicit EntityBuilder(fairlanes::context::EntityCtx context)
      : ctx_(context), e_(ctx_.reg_.create()) {}

  template <typename C> EntityBuilder &with(C c) {
    ctx_.reg_.emplace_or_replace<C>(e_, std::move(c));
    return *this;
  }

  // Defaults for any C with a ComponentBuilder<C>
  template <typename C> EntityBuilder &with_default() {
    ctx_.reg_.emplace_or_replace<C>(e_, ComponentBuilder<C>::defaults(ctx_));
    return *this;
  }

  entt::entity build() const { return e_; }
};
