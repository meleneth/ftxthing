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
    auto e = gc.ctx_.reg_->create();

    gc.emplace<IsAccount>(e, gc.ctx_.account_context(e), "Acme");
    auto &acct = gc.ctx_.reg_->get<IsAccount>(e);
    REQUIRE(acct.account_name_ == "Acme");
  }

  SECTION("default then assign") {
    fairlanes::AppConfig app_config;
    fairlanes::GrandCentral gc{app_config};
    auto e = gc.ctx_.reg_->create();
    fairlanes::widgets::FancyLog log;
    fairlanes::context::AccountCtx account_context{gc.ctx_.reg_, gc.ctx_.rng_,
                                                   e};
    gc.emplace<IsAccount>(e, account_context, "some value");

    auto &acct = gc.get<IsAccount>(e);
    REQUIRE(acct.account_name_ == "some value");

    acct.account_name_ = "Umbrella";
    REQUIRE(acct.account_name_ == "Umbrella");
  }
}
