// tests/test_entity_builder.cpp
#include <catch2/catch_test_macros.hpp>
#include <entt/entt.hpp>
#include <nlohmann/json.hpp>

#include "entities/component_builder.hpp" // ComponentBuilder<Stats/Tags>
#include "entities/entity_builder.hpp" // EntityBuilder with with_default<T>()

using json = nlohmann::json;

struct Stats; // forward decls only if headers need it
struct Tags;

TEST_CASE("EntityBuilder + ComponentBuilder basics", "[entity][builder]") {
  entt::registry reg;

  SECTION("Stats defaults are installed") {
    auto e = EntityBuilder{reg}.with_default<Stats>().build();
    const auto &s = reg.get<Stats>(e);
    REQUIRE(s.hp_ == 10);
    REQUIRE(s.mp_ == 0);
  }

  SECTION("Stats JSON overrides apply on top of defaults") {
    auto e = EntityBuilder{reg}.with_default<Stats>().build();
    auto &s = reg.get<Stats>(e);

    json j = R"({ "hp": 25, "mp": 3 })"_json;
    ComponentBuilder<Stats>::apply(s, j);

    REQUIRE(s.hp_ == 25);
    REQUIRE(s.mp_ == 3);
  }

  SECTION("Tags default + JSON array assignment") {
    auto e = EntityBuilder{reg}.with_default<Tags>().build();
    auto &t = reg.get<Tags>(e);

    json j = R"({ "values": ["dps", "glass"] })"_json;
    ComponentBuilder<Tags>::apply(t, j);

    REQUIRE(t.values.size() == 2);
    REQUIRE(t.values[0] == "dps");
    REQUIRE(t.values[1] == "glass");
  }

  SECTION("Preset/trait composition via a lambda") {
    auto e =
        EntityBuilder{reg}.with_default<Stats>().with_default<Tags>().build();

    // a "trait": GlassCannon
    auto glass_cannon = [&](entt::entity x) {
      auto &s = reg.get<Stats>(x);
      s.hp_ = 1;
      s.mp_ = 50;

      auto &t = reg.get<Tags>(x);
      t.values = {"glass", "dps"};
    };

    glass_cannon(e);

    const auto &s = reg.get<Stats>(e);
    const auto &t = reg.get<Tags>(e);
    REQUIRE(s.hp_ == 1);
    REQUIRE(s.mp_ == 50);
    REQUIRE(t.values.size() == 2);
  }
}
