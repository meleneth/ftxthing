// tests/test_is_account.cpp
#include "app/app_config.hpp"
#include "app/grand_central.hpp"
#include "fairlanes/ecs/components/is_account.hpp"
#include <catch2/catch_test_macros.hpp>
#include <entt/entt.hpp>

using fairlanes::ecs::components::IsAccount;

TEST_CASE("IsAccount basic usage", "[ecs][account]") {

  SECTION("emplace with name") {
    fairlanes::AppConfig app_config;
    fairlanes::GrandCentral gc{app_config};
    auto e = gc.registry().create();

    gc.registry().emplace<IsAccount>(e, gc.app_context(), "Acme");
    auto &acct = gc.registry().get<IsAccount>(e);
    REQUIRE(acct.account_name_ == "Acme");
  }

  SECTION("default then assign") {
    fairlanes::AppConfig app_config;
    fairlanes::GrandCentral gc{app_config};
    auto e = gc.registry().create();
    gc.registry().emplace<IsAccount>(e, gc.app_context(), "some value");

    auto &acct = gc.registry().get<IsAccount>(e);
    acct.account_name_ = "Umbrella";
    REQUIRE(acct.account_name_ == "Umbrella");
  }
}
