// combat_events.hpp
#pragma once

#include <entt/entt.hpp>

namespace fairlanes::combat {

enum class CombatEventId {
  ApplyDamage,
  Death,
  BuffApplied,
  BuffExpired,
  // add more as needed
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

enum class ReadyEventId {
  CombatantReady,
};

struct ReadyEvent {
  ReadyEventId type{ReadyEventId::CombatantReady};
  entt::entity actor{entt::null};
};

struct ReadyPolicies {
  static ReadyEventId getEvent(const ReadyEvent &e) { return e.type; }
};

} // namespace fairlanes::combat
