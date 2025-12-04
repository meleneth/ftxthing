// timed_combat_fsm.hpp
#pragma once
#include <cstdint>

#include <boost/sml.hpp>

#include "systems/log.hpp"

namespace fairlanes::fsm::timed_combat {

namespace sml = boost::sml;

// --- States (struct tags so MSVC is happy) ---

struct Charging {};
struct UsingSkill {};
struct Casting {};
struct GCD {};
struct Queued {};
struct Stunned {};
struct Frozen {};
struct Stopped {};

// --- Events ---

// Advance the internal timer / charge bar.
struct Tick {
  std::int32_t ms;
};

// A skill was requested (e.g., player pressed button,
// or AI picked something).
struct RequestSkill {
  // For now, just an int; you can swap for your skill ID type later.
  int skill_id;
};

// The currently executing skill has finished.
struct SkillFinished {};

// External control events:
struct ApplyStun {};
struct ClearStun {};
struct ApplyFrozen {};
struct ClearFrozen {};
struct StopCombat {};
struct ResumeCombat {};

// --- Per-combatant data ("context") ---

struct TimedCombatData {
  // Simple ATB-style charge bar.
  float charge_current{0.0f};
  float charge_max{100.0f};   // threshold to become "ready"
  float charge_per_ms{0.05f}; // how fast we gain charge

  // Skill being acted on / queued.
  int current_skill{-1};
  int queued_skill{-1};

  // Convenience helpers for tests / integration.
  bool is_ready() const { return charge_current >= charge_max; }
  void reset_charge() { charge_current = 0.0f; }

  void add_charge(std::int32_t ms) {
    charge_current += charge_per_ms * static_cast<float>(ms);
  }
};

// --- Actions & guards ---

struct actions {
  TimedCombatData &data;

  void accumulate_charge(const Tick &e) {
    spdlog::info("Added charge");
    data.add_charge(e.ms);
    clamp_charge(e);
  }

  void clamp_charge(const Tick &) {
    if (data.charge_current > data.charge_max) {
      data.charge_current = data.charge_max;
    }
  }

  void mark_queued(const RequestSkill &e) { data.queued_skill = e.skill_id; }

  void begin_skill(const RequestSkill &e) {
    data.current_skill = e.skill_id;
    data.queued_skill = -1;
  }

  void begin_skill_from_queue() {
    data.current_skill = data.queued_skill;
    data.queued_skill = -1;
  }

  void clear_skill(const SkillFinished &) { data.current_skill = -1; }

  void reset_charge(const SkillFinished &) { data.reset_charge(); }

  // For entering/exiting hard stop.
  void clear_queue_on_stop(const StopCombat &) {
    data.queued_skill = -1;
    data.current_skill = -1;
  }
};

struct guards {
  TimedCombatData &data;

  bool charge_full(const Tick &) const {
    return data.charge_current >= data.charge_max;
  }

  bool has_queued_skill(const RequestSkill &) const {
    return data.queued_skill != -1;
  }

  bool has_queued_skill() const { return data.queued_skill != -1; }
};

// --- The FSM itself ---

struct TimedCombatSm {
  auto operator()() const {
    using namespace sml;

    // Using dependency injection for data/actions/guards.
    return make_transition_table(
        // ------------------------------
        // Normal flow: Charging → Queued → UsingSkill → GCD → Charging
        // ------------------------------

        // Tick while Charging: accumulate charge, clamp,
        // and if full we *stay* in Charging but can be transitioned externally
        // (or you can auto-jump to Queued here if you like).
        *state<Charging> + event<Tick> / (&actions::accumulate_charge),

        // When player/AI requests a skill and we're still Charging,
        // we go to Queued if we’re already full — otherwise we stay in Charging
        // but record queued skill.
        state<Charging> + event<RequestSkill> / &actions::mark_queued =
            state<Queued>,

        // Queued: we are ready and have a specific skill queued.
        // In this minimal version, going to UsingSkill is explicit:
        state<Queued> + event<RequestSkill> / &actions::begin_skill =
            state<UsingSkill>,

        // UsingSkill → GCD when skill finishes.
        state<UsingSkill> + event<SkillFinished> / (&actions::clear_skill,
                                                    &actions::reset_charge) =
            state<GCD>,

        // GCD is a brief lockout where charge does not accumulate.
        // For now, a Tick while in GCD does nothing but you could model GCD as
        // a timer.
        state<GCD> + event<Tick> = state<GCD>,

        // After some external condition, we can go back to Charging;
        // in this minimal version, we just say SkillFinished also ends GCD.
        state<GCD> + event<SkillFinished> = state<Charging>,

        // ------------------------------
        // Control / CC states
        // ------------------------------

        // Global stop: any state → Stopped.
        // You can expand with more transitions as needed.
        state<Charging> + event<StopCombat> / &actions::clear_queue_on_stop =
            state<Stopped>,
        state<Queued> + event<StopCombat> / &actions::clear_queue_on_stop =
            state<Stopped>,
        state<UsingSkill> + event<StopCombat> / &actions::clear_queue_on_stop =
            state<Stopped>,
        state<GCD> + event<StopCombat> / &actions::clear_queue_on_stop =
            state<Stopped>,

        // Resume from Stopped back to Charging.
        state<Stopped> + event<ResumeCombat> = state<Charging>,

        // Stuns/Frozen are just placeholders to give you types;
        // transitions can be filled later.
        state<Charging> + event<ApplyStun> = state<Stunned>,
        state<Stunned> + event<ClearStun> = state<Charging>,
        state<Charging> + event<ApplyFrozen> = state<Frozen>,
        state<Frozen> + event<ClearFrozen> = state<Charging>);
  }
};

} // namespace fairlanes::fsm::timed_combat
