#pragma once
#include <entt/entt.hpp>
#include <vector>

#include "fairlanes/context/entity_ctx.hpp"

namespace fairlanes::concepts {

struct EncounterBuilder {
  fairlanes::context::EntityCtx ctx_;
  EncounterBuilder(fairlanes::context::EntityCtx ctx);

  void thump_it_out();

  void add_field_mouse();

  void add_to_enemy_team(entt::entity entity);
};

} // namespace fairlanes::concepts
