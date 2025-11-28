// tests/test_entity_builder.cpp
#include <memory>

#include <catch2/catch_test_macros.hpp>
#include <entt/entt.hpp>
#include <nlohmann/json.hpp>

#include "entities/component_builder.hpp"
#include "entities/entity_builder.hpp"
#include "fairlanes/context/attack_ctx.hpp"
#include "fairlanes/ecs/components/stats.hpp" // <-- needed
#include "fairlanes/skills/thump.hpp"
#include "fairlanes/widgets/fancy_log.hpp"

TEST_CASE("Skill: Thump", "[entity][builder]") {
  using fairlanes::ecs::components::Stats;
  using fairlanes::skills::Thump;

  entt::registry reg;
  fairlanes::widgets::FancyLog log;
  entt::entity some_entity = reg.create();
  fairlanes::RandomHub rng;

  fairlanes::context::EntityCtx ctx{reg, log, rng, some_entity};

  SECTION("Default stats for reference") {
    auto e = EntityBuilder{ctx}.with_default<Stats>().build();
    const auto &s = reg.get<Stats>(e);
    REQUIRE(s.hp_ == 10);
    REQUIRE(s.mp_ == 0);
  }

  SECTION("Thump does damage (deterministic RNG)") {
    // One registry for everything:
    auto attacker = EntityBuilder{ctx}.with_default<Stats>().build();
    auto defender = EntityBuilder{ctx}.with_default<Stats>().build();

    // Create a party entity (if ctx expects one)
    auto party_e = reg.create();

    // Log + RNG
    fairlanes::widgets::FancyLog log;
    fairlanes::RandomHub rng{/*seed=*/12345}; // make deterministic if supported

    Thump thumper;

    const int hp_before = reg.get<Stats>(defender).hp_;
    bool damaged = false;
    for (int i = 0; i < 8 && !damaged; ++i) {
      thumper.thump(
          fairlanes::context::AttackCtx{reg, log, rng, attacker, defender});
      damaged = (reg.get<Stats>(defender).hp_ < hp_before);
    }

    REQUIRE(damaged); // defender HP should have dropped at least once
  }
}
