// tests/test_entity_builder.cpp
#include <catch2/catch_test_macros.hpp>
#include <entt/entt.hpp>
#include <nlohmann/json.hpp>

#include "entities/component_builder.hpp" // ComponentBuilder<Stats/Tags>
#include "entities/entity_builder.hpp" // EntityBuilder with with_default<T>()
#include "fairlanes/skills/thump.hpp"

TEST_CASE("Skill: Thump", "[entity][builder]") {
  entt::registry reg;

  SECTION("Default stats for reference") {
    auto e = EntityBuilder{reg}.with_default<Stats>().build();
    const auto &s = reg.get<Stats>(e);
    REQUIRE(s.hp_ == 10);
    REQUIRE(s.mp_ == 0);
  }

  SECTION("Thump does damage") {
    using fairlanes::skills::Thump;
    Thump in_the_night;
    auto attacker = EntityBuilder{reg}.with_default<Stats>().build();
    auto defender = EntityBuilder{reg}.with_default<Stats>().build();

    in_the_night.thump(reg, attacker, defender);
    const auto &s = reg.get<Stats>(defender);
    REQUIRE(s.hp_ != 10);
  }
}
