#pragma once
/// TurnTimer: per-combatant timing state for ATB-style combat.

#include <cstdint>

namespace fairlanes::ecs::components {

struct TurnTimer {
  float gauge_ = 0.0f; // 0..1 normalized, or "units to act"
  float speed_ = 1.0f; // units per second or per tick

  enum class State {
    Charging, // collecting time
    Queued,   // in encounter FIFO, waiting to act
    Acting,   // currently active_combatant_ for encounter
    Disabled  // dead / stunned / etc.
  } state_ = State::Charging;
};

} // namespace fairlanes::ecs::components
