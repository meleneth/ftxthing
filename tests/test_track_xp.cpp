#include <catch2/catch_test_macros.hpp>
#include <entt/entt.hpp>

#include "fairlanes/ecs/components/track_xp.hpp"

using fairlanes::ecs::components::TrackXP;

TEST_CASE("TrackXP basic leveling", "[ecs][xp]") {
  entt::registry reg;
  auto e = reg.create();
  auto &xp = reg.emplace<TrackXP>(e);

  SECTION("Starts at level 1 with 0 xp") {
    REQUIRE(xp.level == 1);
    REQUIRE(xp.xp == 0);
    REQUIRE(xp.next_level_at == TrackXP::xp_for_level(2));
  }

  SECTION("Adding xp levels up correctly") {
    // Add just enough to reach next level
    xp.add_xp(xp.next_level_at);
    REQUIRE(xp.level > 1);
    REQUIRE(xp.xp >= TrackXP::xp_for_level(2));

    // Add a large amount that should cause multiple level-ups
    xp.add_xp(5000);
    REQUIRE(xp.level > 2);
    REQUIRE(xp.xp >= TrackXP::xp_for_level(xp.level));
  }

  SECTION("xp_for_level() curve is quadratic") {
    constexpr int base = TrackXP::BASE_XP_VALUE;
    REQUIRE(TrackXP::xp_for_level(1) == base * 1 * 2 / 2);
    REQUIRE(TrackXP::xp_for_level(2) == base * 2 * 3 / 2);
    REQUIRE(TrackXP::xp_for_level(10) == base * 10 * 11 / 2);
  }
}
