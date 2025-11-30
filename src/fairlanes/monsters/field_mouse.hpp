#pragma once

#include "entities/entity_builder.hpp"
#include "fairlanes/monsters/monster_kind.hpp"

namespace fairlanes::monster {

class FieldMouse {
public:
  static void apply(EntityBuilder &b); // defined in .cpp
};

void register_field_mouse();

} // namespace fairlanes::monster
