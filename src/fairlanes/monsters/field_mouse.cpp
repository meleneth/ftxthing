#include "field_mouse.hpp"

#include <algorithm>

#include "entities/component_builder.hpp"
#include "entities/entity_builder.hpp"
#include "fairlanes/ecs/components/stats.hpp"
#include "fairlanes/monsters/monster_registry.hpp"

namespace fairlanes::monster {

using fairlanes::ecs::components::Stats;
void FieldMouse::apply(EntityBuilder &b) {
  // Ensure Stats exists (either defaulted elsewhere or create fresh)
  auto &s = b.ctx().reg_.get<Stats>(b.entity());
  s.name_ = "Field Mouse";
  s.hp_ = 5;
  s.max_hp_ = 5;
  s.mp_ = 0;

  // Optional tags
  // auto &t = reg.get_or_emplace<Tags>(b.entity());
  // t.values = {"mouse", "precious"};
}

void register_field_mouse() {
  register_monster(fairlanes::monster::MonsterKind::FieldMouse,
                   [](EntityBuilder &b) { FieldMouse::apply(b); });
}

} // namespace fairlanes::monster
