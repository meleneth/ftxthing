#pragma once

#include <entt/entt.hpp>
#include <utility>

#include "entities/component_builder.hpp"
#include "fairlanes/context/entity_ctx.hpp"
#include "fairlanes/ecs/components/stats.hpp"
#include "fairlanes/ecs/components/track_xp.hpp"
#include "fairlanes/monsters/monster_kind.hpp"
#include "fairlanes/monsters/monster_registry.hpp"

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

  template <typename C> EntityBuilder &with_default() {
    ctx_.reg_.emplace_or_replace<C>(e_, ComponentBuilder<C>::defaults(ctx_));
    return *this;
  }

  // ---- New: accessors so archetype functions can poke internals ----
  fairlanes::context::EntityCtx &ctx() { return ctx_; }
  const fairlanes::context::EntityCtx &ctx() const { return ctx_; }

  entt::entity entity() const { return e_; }
  // ------------------------------------------------------------------

  entt::entity build() const { return e_; }

  EntityBuilder &with_monster_defaults() {
    using fairlanes::ecs::components::Stats;
    using fairlanes::ecs::components::Tags;
    using fairlanes::ecs::components::TrackXP;

    with_default<Stats>();
    with_default<Tags>();
    with_default<TrackXP>();
    // Add any other “every monster has this” components here.

    return *this;
  }

  EntityBuilder &monster(fairlanes::monster::MonsterKind kind) {
    using fairlanes::monster::monster_registry;
    with_monster_defaults();
    auto &reg = monster_registry();
    auto it = reg.find(kind);
    if (it == reg.end()) {
      // For now, crash loudly if you forgot to register.
      throw std::runtime_error("MonsterKind not registered");
    }

    it->second(*this);
    return *this;
  }
};
