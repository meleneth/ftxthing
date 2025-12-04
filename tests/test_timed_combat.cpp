// timed_combat_fsm_tests.cpp

#include <catch2/catch_test_macros.hpp>

#include "fairlanes/fsm/timed_combat.hpp"

using namespace fairlanes::fsm::timed_combat;

TEST_CASE("TimedCombatMachine starts in Charging") {
  TimedCombatData data;
  actions act{data};
  guards grd{data};

  sml::sm<TimedCombatSm> fsm{data, act, grd};

  using namespace boost::sml;
  REQUIRE(fsm.is(state<Charging>));
}

TEST_CASE("TimedCombatMachine accumulates charge on Tick") {
  TimedCombatData data;
  data.charge_max = 100.0f;
  data.charge_per_ms = 1.0f; // 1 unit per ms for easy math.

  actions act{data};
  guards grd{data};

  sml::sm<TimedCombatSm> fsm{data, act, grd};

  fsm.process_event(Tick{10});
  REQUIRE_THAT(data.charge_current, Catch::Matchers::WithinRel(10, 0.001));

  fsm.process_event(Tick{20});
  REQUIRE_THAT(data.charge_current, Catch::Matchers::WithinRel(30, 0.001));
}

TEST_CASE("RequestSkill while Charging goes to Queued") {
  TimedCombatData data;
  data.charge_max = 100.0f;
  data.charge_per_ms = 1.0f;
  data.charge_current = 100.0f; // pretend already full

  actions act{data};
  guards grd{data};

  sml::sm<TimedCombatSm> fsm{data, act, grd};

  fsm.process_event(RequestSkill{42});
  using namespace boost::sml;
  REQUIRE(fsm.is(state<Queued>));
  REQUIRE(data.queued_skill == 42);
}

TEST_CASE("Queued -> UsingSkill on RequestSkill") {
  TimedCombatData data;
  data.charge_current = 100.0f;
  data.queued_skill = 7;

  actions act{data};
  guards grd{data};

  sml::sm<TimedCombatSm> fsm{data, act, grd};

  using namespace boost::sml;
  // Force start state
  REQUIRE(fsm.is(state<Charging>));
  fsm.process_event(RequestSkill{7}); // Charging -> Queued
  REQUIRE(fsm.is(state<Queued>));

  fsm.process_event(RequestSkill{7}); // Queued -> UsingSkill
  REQUIRE(fsm.is(state<UsingSkill>));
  REQUIRE(data.current_skill == 7);
  REQUIRE(data.queued_skill == -1);
}

TEST_CASE("UsingSkill -> GCD -> Charging on SkillFinished") {
  TimedCombatData data;
  data.charge_max = 100.0f;
  data.charge_per_ms = 1.0f;
  data.charge_current = 100.0f;

  actions act{data};
  guards grd{data};

  sml::sm<TimedCombatSm> fsm{data, act, grd};

  using namespace boost::sml;

  // Charging -> Queued
  fsm.process_event(RequestSkill{99});
  REQUIRE(fsm.is(state<Queued>));

  // Queued -> UsingSkill
  fsm.process_event(RequestSkill{99});
  REQUIRE(fsm.is(state<UsingSkill>));

  // UsingSkill -> GCD
  fsm.process_event(SkillFinished{});
  REQUIRE(fsm.is(state<GCD>));
  // REQUIRE(data.current_skill == -1);
  REQUIRE_THAT(data.charge_current, Catch::Matchers::WithinRel(0, 0.001));

  // GCD -> Charging (second SkillFinished for now)
  fsm.process_event(SkillFinished{});
  REQUIRE(fsm.is(state<Charging>));
}

TEST_CASE("StopCombat moves to Stopped and clears skills") {
  TimedCombatData data;
  data.charge_current = 100.0f;
  data.queued_skill = 1;
  data.current_skill = 2;

  actions act{data};
  guards grd{data};

  sml::sm<TimedCombatSm> fsm{data, act, grd};

  using namespace boost::sml;

  fsm.process_event(StopCombat{});
  REQUIRE(fsm.is(state<Stopped>));
  REQUIRE(data.queued_skill == -1);
  REQUIRE(data.current_skill == -1);

  fsm.process_event(ResumeCombat{});
  REQUIRE(fsm.is(state<Charging>));
}
