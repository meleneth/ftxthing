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
  entt::registry reg;
  fairlanes::RandomHub rng;
  fairlanes::context::AppCtx app_ctx{reg, rng};
  auto account = reg.create();
  auto account_ctx = app_ctx.account_context(account);

  SECTION("only moveable") {
    static_assert(
        !std::is_copy_constructible_v<fairlanes::context::AccountCtx>);
    static_assert(!std::is_copy_assignable_v<fairlanes::context::AccountCtx>);
    static_assert(!std::is_copy_constructible_v<fairlanes::context::AppCtx>);
    static_assert(!std::is_copy_assignable_v<fairlanes::context::AppCtx>);
    //  static_assert(std::is_copy_constructible_v<fairlanes::context::EntityCtx>);
    static_assert(std::is_copy_assignable_v<fairlanes::context::EntityCtx>);
  }
  SECTION("#account_context") {
    REQUIRE(account_ctx.self_ == account);
    REQUIRE(&account_ctx.reg_ == &reg);
    auto other_account_ctx = app_ctx.account_context(account);
    REQUIRE(account_ctx.log_.get() != other_account_ctx.log_.get());
  }
  SECTION("#entity_context") {
    auto entity = reg.create();
    auto entity_ctx = account_ctx.entity_context(entity);
    REQUIRE(account_ctx.self_ == account);
    REQUIRE(&account_ctx.reg_ == &reg);
  }
}
