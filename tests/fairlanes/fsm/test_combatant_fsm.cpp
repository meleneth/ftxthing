#include <catch2/catch_test_macros.hpp>
#include <entt/entt.hpp>

#include "fairlanes/ecs/components/encounter.hpp"
#include "fairlanes/ecs/components/stats.hpp"
#include "fairlanes/ecs/components/turn_timer.hpp"
#include "fairlanes/fsm/combatant_fsm.hpp"

using fairlanes::combat::BeginActing;
using fairlanes::combat::FinishActing;
using fairlanes::combat::Kill;
using fairlanes::combat::Tick;
using fairlanes::ecs::components::CombatantFSM;
using fairlanes::ecs::components::Encounter;
using fairlanes::ecs::components::TurnTimer;

namespace {

struct CombatantFixture {
  entt::registry reg;
  entt::entity encounter_e{entt::null};
  entt::entity combatant_e{entt::null};

  Encounter *encounter{nullptr};
  TurnTimer *timer{nullptr};
  fairlanes::ecs::components::Stats *stats{nullptr};
  CombatantFSM *fsm_comp{nullptr};

  CombatantFixture() {
    // Create entities
    encounter_e = reg.create();
    combatant_e = reg.create();

    // Attach components
    encounter = &reg.emplace<Encounter>(encounter_e);

    timer = &reg.emplace<TurnTimer>(combatant_e);
    timer->gauge_ = 0.0f;
    timer->speed_ = 1.0f;
    timer->state_ = TurnTimer::State::Charging;

    stats = &reg.emplace<fairlanes::ecs::components::Stats>(combatant_e);
    stats->hp_ = 10;
    stats->max_hp_ = 10;

    // Build the FSM context and component
    fairlanes::combat::CombatantCtx ctx{
        .reg = reg,
        .encounter = encounter_e,
        .self = combatant_e,
    };

    fsm_comp = &reg.emplace<CombatantFSM>(combatant_e, ctx);
  }

  auto &fsm() { return fsm_comp->sm; }
};

} // namespace

TEST_CASE("CombatantFSM charges gauge while Charging",
          "[combat][fsm][timing]") {
  CombatantFixture fx;

  REQUIRE(fx.timer->state_ == TurnTimer::State::Charging);
  REQUIRE(fx.timer->gauge_ == Approx(0.0f));

  // First three ticks: should stay in Charging and accumulate
  fx.fsm().process_event(Tick{0.25f});
  REQUIRE(fx.timer->state_ == TurnTimer::State::Charging);
  REQUIRE(fx.timer->gauge_ == Approx(0.25f));

  fx.fsm().process_event(Tick{0.25f});
  REQUIRE(fx.timer->state_ == TurnTimer::State::Charging);
  REQUIRE(fx.timer->gauge_ == Approx(0.50f));

  fx.fsm().process_event(Tick{0.25f});
  REQUIRE(fx.timer->state_ == TurnTimer::State::Charging);
  REQUIRE(fx.timer->gauge_ == Approx(0.75f));
}

TEST_CASE("CombatantFSM enqueues ReadyToAct exactly once when gauge fills",
          "[combat][fsm][timing]") {
  CombatantFixture fx;

  // Bring gauge close to 1.0
  fx.fsm().process_event(Tick{0.75f});
  REQUIRE(fx.timer->state_ == TurnTimer::State::Charging);
  REQUIRE(fx.timer->gauge_ == Approx(0.75f));

  // This Tick should trip gauge_full, transition to Queued, and enqueue
  // ReadyToAct
  fx.fsm().process_event(Tick{0.25f});

  REQUIRE(fx.timer->state_ == TurnTimer::State::Queued);
  REQUIRE(fx.timer->gauge_ == Approx(1.0f));

  int ready_count = 0;
  fx.encounter->event_queue_.process(
      [&](CombatEventId id, const void *payload) {
        if (id == CombatEventId::ReadyToAct) {
          auto ev = *static_cast<const ReadyToActEvent *>(payload);
          REQUIRE(ev.combatant == fx.combatant_e);
          ++ready_count;
        }
      });

  REQUIRE(ready_count == 1);

  // Further ticks in Queued state should NOT enqueue more events or change
  // gauge.
  fx.fsm().process_event(Tick{1.0f}); // big dt, should be ignored

  REQUIRE(fx.timer->state_ == TurnTimer::State::Queued);
  REQUIRE(fx.timer->gauge_ == Approx(1.0f));

  int ready_count_after = 0;
  fx.encounter->event_queue_.process(
      [&](CombatEventId id, const void *payload) {
        if (id == CombatEventId::ReadyToAct) {
          (void)payload;
          ++ready_count_after;
        }
      });

  REQUIRE(ready_count_after == 0);
}

TEST_CASE("CombatantFSM BeginActing and FinishActing transitions",
          "[combat][fsm][timing]") {
  CombatantFixture fx;

  // Force into Queued state (skip event queue check here)
  fx.fsm().process_event(Tick{1.0f});
  REQUIRE(fx.timer->state_ == TurnTimer::State::Queued);
  REQUIRE(fx.timer->gauge_ == Approx(1.0f));

  // Encounter selects this combatant as active
  fx.fsm().process_event(BeginActing{});
  REQUIRE(fx.timer->state_ == TurnTimer::State::Acting);
  REQUIRE(fx.timer->gauge_ == Approx(0.0f));

  // While Acting, Tick should not change gauge
  fx.fsm().process_event(Tick{0.5f});
  REQUIRE(fx.timer->state_ == TurnTimer::State::Acting);
  REQUIRE(fx.timer->gauge_ == Approx(0.0f));

  // Animation finishes
  fx.fsm().process_event(FinishActing{});
  REQUIRE(fx.timer->state_ == TurnTimer::State::Charging);
  REQUIRE(fx.timer->gauge_ == Approx(0.0f));
}

TEST_CASE(
    "CombatantFSM Kill disables from any state and prevents further charging",
    "[combat][fsm][timing]") {
  CombatantFixture fx;

  SECTION("Kill from Charging") {
    fx.timer->gauge_ = 0.3f;
    fx.timer->state_ = TurnTimer::State::Charging;

    fx.fsm().process_event(Kill{});

    REQUIRE(fx.timer->state_ == TurnTimer::State::Disabled);

    float before = fx.timer->gauge_;
    fx.fsm().process_event(Tick{1.0f});
    REQUIRE(fx.timer->gauge_ == Approx(before));
  }

  SECTION("Kill from Queued") {
    // Force to Queued
    fx.fsm().process_event(Tick{1.0f});
    REQUIRE(fx.timer->state_ == TurnTimer::State::Queued);

    fx.fsm().process_event(Kill{});
    REQUIRE(fx.timer->state_ == TurnTimer::State::Disabled);

    float before = fx.timer->gauge_;
    fx.fsm().process_event(Tick{1.0f});
    REQUIRE(fx.timer->gauge_ == Approx(before));
  }

  SECTION("Kill from Acting") {
    // Force to Acting
    fx.fsm().process_event(Tick{1.0f}); // -> Queued
    fx.fsm().process_event(BeginActing{});
    REQUIRE(fx.timer->state_ == TurnTimer::State::Acting);

    fx.fsm().process_event(Kill{});
    REQUIRE(fx.timer->state_ == TurnTimer::State::Disabled);

    float before = fx.timer->gauge_;
    fx.fsm().process_event(Tick{1.0f});
    REQUIRE(fx.timer->gauge_ == Approx(before));
  }
}

TEST_CASE("CombatantFSM does not charge when dead (hp <= 0)",
          "[combat][fsm][timing]") {
  CombatantFixture fx;

  // Simulate dead stats
  fx.stats->hp_ = 0;

  fx.timer->gauge_ = 0.0f;
  fx.timer->state_ = TurnTimer::State::Charging;

  fx.fsm().process_event(Tick{1.0f});
  REQUIRE(fx.timer->gauge_ == Approx(0.0f));
  REQUIRE(fx.timer->state_ == TurnTimer::State::Charging);
}
