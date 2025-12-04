#pragma once

#include "combat_events.hpp"
#include <eventpp/eventdispatcher.h>

namespace fairlanes::combat {

// Immediate, non-queued bus for combat events.
using CombatBus =
    eventpp::EventDispatcher<CombatEventId, void(const CombatEvent &),
                             CombatPolicies>;

} // namespace fairlanes::combat
