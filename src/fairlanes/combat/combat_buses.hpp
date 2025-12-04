// combat_buses.hpp
#pragma once

#include <eventpp/eventdispatcher.h>
#include <eventpp/eventqueue.h>

#include "combat_events.hpp"

namespace fairlanes::combat {

using CombatBus =
    eventpp::EventDispatcher<CombatEventId, void(const CombatEvent &),
                             CombatPolicies>;

// This is the "ready_combatants" long-span queue
using ReadyCombatantsQueue =
    eventpp::EventQueue<ReadyEventId, void(const ReadyEvent &), ReadyPolicies>;

} // namespace fairlanes::combat
