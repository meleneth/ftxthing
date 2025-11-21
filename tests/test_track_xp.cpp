#include <catch2/catch_test_macros.hpp>
#include <entt/entt.hpp>

#include "app/app_config.hpp"
#include "app/grand_central.hpp"
#include "fairlanes/context/entity_ctx.hpp"
#include "fairlanes/ecs/components/track_xp.hpp"

using fairlanes::ecs::components::TrackXP;

TEST_CASE("TrackXP basic leveling", "[ecs][xp]") {
  fairlanes::AppConfig app_config;
  fairlanes::GrandCentral gc{app_config};

  auto e = gc.ctx_.reg_->create();
  fairlanes::context::EntityCtx entity_ctx{gc.ctx_.reg_, gc.ctx_.log_.get(),
                                           gc.ctx_.rng_, e};
  auto &xp = gc.emplace<TrackXP>(e, entity_ctx, 0);
  entt::handle handle{*gc.ctx_.reg_, e};

  SECTION("Starts at level 1 with 0 xp") {
    REQUIRE(xp.level_ == 1);
    REQUIRE(xp.xp_ == 0);
    REQUIRE(xp.next_level_at == xp.xp_for_level(2));
  }

  SECTION("Adding xp levels up correctly") {
    // Add just enough to reach next level
    xp.add_xp(xp.next_level_at);
    REQUIRE(xp.level_ > 1);
    REQUIRE(xp.xp_ >= xp.xp_for_level(2));

    // Add a large amount that should cause multiple level-ups
    xp.add_xp(5000);
    REQUIRE(xp.level_ > 2);
    REQUIRE(xp.xp_ >= xp.xp_for_level(xp.level_));
  }

  SECTION("xp_for_level() curve is quadratic") {
    constexpr int base = TrackXP::BASE_XP_VALUE;
    REQUIRE(xp.xp_for_level(1) == base * 1 * 2 / 2);
    REQUIRE(xp.xp_for_level(2) == base * 2 * 3 / 2);
    REQUIRE(xp.xp_for_level(10) == base * 10 * 11 / 2);
  }
}
