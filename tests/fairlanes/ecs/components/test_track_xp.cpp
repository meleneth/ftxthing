// tests/test_track_xp.cpp
#include <catch2/catch_test_macros.hpp>
#include <entt/entt.hpp>

#include "app/app_config.hpp"
#include "app/grand_central.hpp"
#include "fairlanes/ecs/components/track_xp.hpp"

using fairlanes::ecs::components::TrackXP;

TEST_CASE("TrackXP basic leveling", "[ecs][xp]") {
  fairlanes::AppConfig app_config;
  fairlanes::GrandCentral gc{app_config};
  auto &reg = gc.registry();
  spdlog::info("Created GrandCentral");

  // Minimal entities to build an EntityCtx:
  auto account = gc.create_account("xp_test_account");
  auto entity = reg.create();

  // Build ctx the same way the rest of the codebase does
  auto account_ctx = gc.ctx_.account_context(account);
  spdlog::info("account_ctx created");

  // Attach TrackXP starting at 0 xp
  auto &xp = reg.emplace<fairlanes::ecs::components::TrackXP>(
      entity, account_ctx.entity_context(entity), 0);
  entt::handle handle{reg, entity};

  SECTION("Starts at level 1 with 0 xp") {
    REQUIRE(xp.level_ == 1);
    REQUIRE(xp.xp_ == 0);
    REQUIRE(xp.next_level_at == xp.xp_for_level(2));
  }

  SECTION("Adding xp levels up correctly") {
    // Add just enough to reach next level
    spdlog::info("calling add_xp");

    xp.add_xp(handle, xp.next_level_at);
    spdlog::info("after add_xp");

    // We don’t assert an exact curve here, just that it leveled
    REQUIRE(xp.level_ > 1);
    REQUIRE(xp.xp_ >= xp.xp_for_level(2));

    // Add a large amount that should cause multiple level-ups
    xp.add_xp(handle, 5000);
    REQUIRE(xp.level_ > 2);
    REQUIRE(xp.xp_ >= xp.xp_for_level(xp.level_));
  }

  SECTION("xp_for_level() curve is quadratic") {
    constexpr int base = TrackXP::BASE_XP_VALUE;

    // Assuming xp_for_level(N) = base * N * (N + 1) / 2
    REQUIRE(xp.xp_for_level(1) == base * 1 * 2 / 2);
    REQUIRE(xp.xp_for_level(2) == base * 2 * 3 / 2);
    REQUIRE(xp.xp_for_level(10) == base * 10 * 11 / 2);
  }
}
