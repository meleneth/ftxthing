// tests/test_entity_builder.cpp
#include <catch2/catch_test_macros.hpp>
#include <entt/entt.hpp>
#include <nlohmann/json.hpp>

#include "entities/component_builder.hpp"
#include "entities/entity_builder.hpp"
#include "fairlanes/ecs/components/stats.hpp" // <-- needed
#include "fairlanes/fsm/party_loop_ctx.hpp"
#include "fairlanes/skills/thump.hpp"
#include "widgets/fancy_log.hpp"

TEST_CASE("Skill: Thump", "[entity][builder]") {
  using fairlanes::ecs::components::Stats;
  using fairlanes::skills::Thump;

  entt::registry reg;

  SECTION("Default stats for reference") {
    auto e = EntityBuilder{reg}.with_default<Stats>().build();
    const auto &s = reg.get<Stats>(e);
    REQUIRE(s.hp_ == 10);
    REQUIRE(s.mp_ == 0);
  }

  SECTION("Thump does damage (deterministic RNG)") {
    // One registry for everything:
    auto attacker = EntityBuilder{reg}.with_default<Stats>().build();
    auto defender = EntityBuilder{reg}.with_default<Stats>().build();

    // Create a party entity (if ctx expects one)
    auto party_e = reg.create();

    // Log + RNG
    fairlanes::FancyLog log{};
    fairlanes::RandomHub rng{/*seed=*/12345}; // make deterministic if supported

    fairlanes::fsm::PartyLoopCtx ctx{&reg, party_e, log, rng};

    Thump thumper;

    const int hp_before = reg.get<Stats>(defender).hp_;
    bool damaged = false;
    for (int i = 0; i < 8 && !damaged; ++i) {
      thumper.thump(ctx, attacker, defender);
      damaged = (reg.get<Stats>(defender).hp_ < hp_before);
    }

    REQUIRE(damaged); // defender HP should have dropped at least once
  }
}
