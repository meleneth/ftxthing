// tests/test_entity_builder.cpp
#include <catch2/catch_test_macros.hpp>
#include <entt/entt.hpp>
#include <nlohmann/json.hpp>

#include "fairlanes/concepts/damage.hpp"

TEST_CASE("Damage") {
  auto x = fairlanes::concepts::Damage{.lightning = 100};
  auto y = fairlanes::concepts::Damage{.fire = 100};

  x += y;

  REQUIRE(x.fire == 100);
  REQUIRE(x.lightning == 100);

  REQUIRE(y.fire == 100);
  REQUIRE(y.lightning == 0);
}
