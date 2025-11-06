// tests/test_is_account.cpp
#include <catch2/catch_test_macros.hpp>

#include "app/app_config.hpp"
#include "app/grand_central.hpp"

TEST_CASE("GrandCentral basic usage") {
  fairlanes::AppConfig app_config;
  SECTION("can instantiate") { fairlanes::GrandCentral gc{app_config}; }
}
