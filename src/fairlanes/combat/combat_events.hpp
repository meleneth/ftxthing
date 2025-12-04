#pragma once

#include <entt/entt.hpp>

namespace fairlanes::combat {

enum class CombatEventId {
  ApplyDamage,
  Death,
  // add more later: GrantXP, BuffApplied, etc.
};

struct CombatEvent {
  CombatEventId type;
  entt::entity source{entt::null};
  entt::entity target{entt::null};
  int amount{0};
};

struct CombatPolicies {
  static CombatEventId getEvent(const CombatEvent &e) { return e.type; }
};

} // namespace fairlanes::combat
