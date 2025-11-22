// tests/test_is_account.cpp
#include "app/app_config.hpp"
#include "app/grand_central.hpp"
#include "fairlanes/context/app_ctx.hpp"
#include "fairlanes/ecs/components/is_account.hpp"
#include "systems/random_hub.hpp"
#include <catch2/catch_test_macros.hpp>
#include <entt/entt.hpp>

using fairlanes::ecs::components::IsAccount;

TEST_CASE("AppCtx basic usage", "[ecs][account]") {

  SECTION("ctor") {
    entt::registry reg;
    fairlanes::RandomHub rng;
    fairlanes::context::AppCtx app_ctx{reg, rng};

    // REQUIRE(app_ctx != nullptr);
  }
}
